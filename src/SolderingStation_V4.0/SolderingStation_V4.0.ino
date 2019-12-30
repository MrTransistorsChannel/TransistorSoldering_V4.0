/*************************Библиотеки*************************/
#include <EEPROMex.h>
#include <LowPower.h>
#include <LiquidCrystal_1602_RUS.h>
/************************************************************/
/****Код версии (для автосброса настроек при обновлении)*****/
#define _VERSION_   (byte)40
/************************************************************/
/***************************Файлы****************************/
#include "PID.h"
#include "DEFAULTS.h"
/************************************************************/

LiquidCrystal_1602_RUS    lcd(2, 3, 4, 5, 6, 7);          //Экран

/*****************Пользовательские настройки*****************/
#define ENC_HLD_TIME      1500                            //Время удержания кнопки энкодера для входа в меню
#define SAVE_DELAY        5000                            //Время после поворота энкодера, через которое сохраняется температура                            
#define PWR_BTN_TIME      1000                            //Время удержания кнопки питания
/************************************************************/
/****************************Пины****************************/
#define HT_PIN            A1
#define TC_IN             A0

#define LCD_BL            8
#define PWR_BTN           A2

#define ENC_WITH_GND

#define ENC_A             9
#define ENC_B             10
#define ENC_SW            11

#ifdef  ENC_WITH_GND
#define ENC_GND           12
#endif
/**************************Переменные************************/
uint64_t encHld_tm, encRot_tm, pwrBtn_tm;
uint16_t min_temp = ABS_MIN, max_temp = ABS_MAX, preset_temp, temp, prev_preset, prev_temp, real100 = 100, real400 = 400;
float P = DEFAULT_P, I = DEFAULT_I, D = DEFAULT_D;
int shift;
bool saved_flg = true;
/************************************************************/

PID pid(10);                                              //ПИД-регулятор с рабочим циклом 10 миллисекунд

/***************************Файлы****************************/
#include "Hello.h"
#include "EEPROM_func.h"
#include "LCD_func.h"
#include "Encoder.h"
#include "ISR.h"
#include "Menu.h"
/************************************************************/

void setup() {
  check_version();                                        //Проверка версии и сброс настроек при первой загрузке
  delay(100);
  get_settings();                                         //Запрос настроек из памяти

  pid.limitIntegral(-255, 255);                           //Ограничение интегрирующей составляющей регулятора

  init_pins();                                            //Инициализация пинов

  lcd.begin(16, 2);                                       //Инициализация и очистка экрана
  lcd.clear();

  enable_PCInt();                                         //Разрешение прерывания на кнопке включения

  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);    //Включение режима сна

  digitalWrite(LCD_BL, 1);                                //Включение подсветки экрана (сюда попадем если проснемся)

  showHello();                                            //Приветственное сообщение

  showTempNames();                                        //Подготовка экрана

  updateAllTimers();                                      //Сброс таймеров

  prev_stp = (!digitalRead(ENC_A) << 1) | !digitalRead(ENC_B);//Сброс шага энкодера

}

void loop() {//ОСНОВНОЙ ЦИКЛ, МНОГО КОДА, ТРУДНО ВСЕ ЗАКОММЕНТИРОВАТЬ
  if (digitalRead(PWR_BTN)) pwrBtn_tm = millis();
  if (millis() - pwrBtn_tm > PWR_BTN_TIME) {
    digitalWrite(HT_PIN, 0);
    lcd.clear();
    digitalWrite(LCD_BL, 0);
    while (!digitalRead(PWR_BTN));
    delay(100);
sleep:
    enable_PCInt();
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
    pwrBtn_tm = millis();
    while (!digitalRead(PWR_BTN)) {
      if (millis() - pwrBtn_tm >= PWR_BTN_TIME) break;
    }
    if (millis() - pwrBtn_tm < PWR_BTN_TIME) goto sleep;
    digitalWrite(LCD_BL, 1);
    showTempNames();
    updateAllTimers();
    prev_preset = 0;
    prev_stp = (!digitalRead(ENC_A) << 1) | !digitalRead(ENC_B);
  }
  if (digitalRead(ENC_SW)) encHld_tm = millis();
  if (analogRead(TC_IN) > 720) {
    lcd.clear();
    lcd.print(L"Нет паяльника!!!");
    while (analogRead(TC_IN) > 720);
    lcd.clear();
    showTempNames();
  }
  else {
    char encData = readEnc(QUART_STEP);
    if (encData == 'R') {
      if (!digitalRead(ENC_SW)) preset_temp += 10;
      else preset_temp++;
      encHld_tm = millis();
      encRot_tm = millis();
      saved_flg = false;
    }
    if (encData == 'L') {
      if (!digitalRead(ENC_SW)) preset_temp -= 10;
      else preset_temp--;
      encHld_tm = millis();
      encRot_tm = millis();
      saved_flg = false;
    }

    preset_temp = constrain(preset_temp, min_temp, max_temp);

    if (millis() - encRot_tm > SAVE_DELAY && !saved_flg) {
      save_preset();
      saved_flg = true;
    }

    temp = map(analogRead(TC_IN), 100, 400, real100, real400) - shift;

    if (abs(preset_temp - temp) > 10) {
      if (temp > preset_temp) {
        digitalWrite(HT_PIN, 0);
      }
      if (temp < preset_temp) {
        digitalWrite(HT_PIN, 1);
      }
    }
    else {
      if (pid.available()) analogWrite(HT_PIN, constrain(pid.calculatePID(P, I, D, preset_temp - temp), 0, 255));
    }

    if (millis() - encHld_tm > ENC_HLD_TIME) {
      lcd.clear();
      digitalWrite(HT_PIN, 0);
      prev_stp = (!digitalRead(ENC_A) << 1) | !digitalRead(ENC_B);
      menu();
      updateAllTimers();
      prev_stp = (!digitalRead(ENC_A) << 1) | !digitalRead(ENC_B);
    }

    if (preset_temp != prev_preset) {
      clearChars(10, 0, 3);
      lcd.setCursor(10, 0);
      lcd.print(preset_temp);
    }
    if (temp != prev_temp) {
      clearChars(12, 1, 3);
      lcd.setCursor(12, 0);
      if (temp > 50) { // и рисуем
        lcd.setCursor(12, 1);
        lcd.print(temp);//новую, если она больше 50 градусов
      }
      else { // иначе выводим "--"
        lcd.setCursor(12, 1);
        lcd.print("--");
      }
    }
  }
  prev_temp = temp;
  prev_preset = preset_temp;

}

void init_pins() {//ФУНКЦИЯ ИНИЦИАЛИЗАЦИИ ПИНОВ    
  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);
  pinMode(ENC_SW, INPUT_PULLUP);
#ifdef ENC_WITH_GND
  pinMode(ENC_GND, OUTPUT);
  digitalWrite(ENC_GND, 0);
#endif

  pinMode(PWR_BTN, INPUT_PULLUP);

  pinMode(LCD_BL, OUTPUT);

  pinMode(HT_PIN, OUTPUT);
}

void updateAllTimers() {//ФУНКЦИЯ СБРОСА ТАЙМЕРОВ
  pwrBtn_tm = millis();
  encHld_tm = millis();
  encRot_tm = millis();
}

void enable_PCInt() {//ФУНКЦИЯ РАЗРЕШЕНИЯ ПРЕРЫВАНИЯ
  PCICR = 0b00000010;// Разрешаем прерывания на PCINT линии 1 (аналоговые пины)
  PCMSK1 = 0b00000100;//Разрешаем прерывания на пине A2
}

void disable_PCInt() {//ФУНКЦИЯ ЗАПРЕЩЕНИЯ ПРЕРЫВАНИЯ
  PCICR = 0b00000010;// Запрещаем прерывания на аналоговых пинах до следующего сна
  PCMSK1 = 0b00000000;// Запрещаем прерывания на пине A2 до следующего сна
}
