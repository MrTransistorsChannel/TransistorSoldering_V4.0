//ОПЯТЬ ЖЕ МНОГО КОДА, ТРУДНО ВСЕ ЗАКОММЕНТИРОВАТЬ, ЕСЛИ ИНТЕРЕСНО, МОЖЕТЕ МНЕ НАПИСАТЬ: https://vk.com/zvukooperator_horeografia25tm

int scroll_point = 0, scroll_page = 0;
bool refresh_flg = true;

String names[6] = {
  "+/- t°C",
  "Калибровкa",
  "Настройка PID",
  "Мин и макс t°C",
  "Сброс",
  "Выход"
};

void temp_shift() {
  lcd.clear();
  lcd.print("Корректировка:");
  lcd.setCursor(8, 1);
  lcd.print("°C");
  while (digitalRead(ENC_SW)) {
    char c = readEnc(FULL_STEP);
    if (c != 0) refresh_flg = true;
    if (c == 'R') shift++;
    if (c == 'L') shift--;
    shift = constrain(shift, -50, 50);
    if (refresh_flg) {
      refresh_flg = false;
      clearChars(5, 1, 3);
      lcd.setCursor(5, 1);
      lcd.print(shift);
    }
  }
  save_correction();
}

void temp_cal() {
  lcd.clear();
  lcd.print("Нагрев жала...");
  lcd.setCursor(4, 1);
  lcd.print("/100у.е.");
  while (temp < 100 || temp > 100) {
    temp = analogRead(TC_IN);
    if (temp != prev_temp) {
      clearChars(1, 1, 3);
      lcd.setCursor(1, 1);
      lcd.print(temp);
      digitalWrite(HT_PIN, temp < 100 ? 1 : 0);
      prev_temp = temp;
    }
  }
  lcd.clear();
  lcd.print("Реальная темп-ра");
  lcd.setCursor(8, 1);
  lcd.print("°C");
  while (digitalRead(ENC_SW)) {
    temp = analogRead(TC_IN);
    digitalWrite(HT_PIN, temp < 100 ? 1 : 0);
    char c = readEnc(FULL_STEP);
    if (c != 0) refresh_flg = true;
    if (c == 'R') real100++;
    if (c == 'L') real100--;
    real100 = constrain(real100, ABS_MIN, ABS_MAX);
    if (refresh_flg) {
      refresh_flg = false;
      clearChars(5, 1, 3);
      lcd.setCursor(5, 1);
      lcd.print(real100);
    }
  }
  lcd.clear();
  lcd.print("Нагрев жала...");
  lcd.setCursor(4, 1);
  lcd.print("/400у.е.");
  while (temp < 400 || temp > 400) {
    temp = analogRead(TC_IN);
    if (temp != prev_temp) {
      clearChars(1, 1, 3);
      lcd.setCursor(1, 1);
      lcd.print(temp);
      digitalWrite(HT_PIN, temp < 400 ? 1 : 0);
      prev_temp = temp;
    }
  }
  refresh_flg = true;
  lcd.clear();
  lcd.print("Реальная темп-ра");
  lcd.setCursor(8, 1);
  lcd.print("°C");
  while (digitalRead(ENC_SW)) {
    temp = analogRead(TC_IN);
    digitalWrite(HT_PIN, temp < 400 ? 1 : 0);
    char c = readEnc(FULL_STEP);
    if (c != 0) refresh_flg = true;
    if (c == 'R') real400++;
    if (c == 'L') real400--;
    real400 = constrain(real400, ABS_MIN, ABS_MAX);
    if (refresh_flg) {
      refresh_flg = false;
      clearChars(5, 1, 3);
      lcd.setCursor(5, 1);
      lcd.print(real400);
    }
  }
  save_cal();

}

void PID_setup() {
  refresh_flg = true;
  lcd.clear();
  lcd.print("Коэффициент P:");
  while (digitalRead(ENC_SW)) {
    char c = readEnc(QUART_STEP);
    if (c != 0) refresh_flg = true;
    if (c == 'R') P += 0.01;
    if (c == 'L') P -= 0.01;
    P = constrain(P, 0, 20);
    if (refresh_flg) {
      refresh_flg = false;
      clearChars(5, 1, 5);
      lcd.setCursor(5, 1);
      lcd.print(P);
    }
  }
  refresh_flg = true;
  lcd.clear();
  lcd.print("Коэффициент I:");
  while (!digitalRead(ENC_SW));
  while (digitalRead(ENC_SW)) {
    char c = readEnc(QUART_STEP);
    if (c != 0) refresh_flg = true;
    if (c == 'R') I += 0.01;
    if (c == 'L') I -= 0.01;
    I = constrain(I, 0, 20);
    if (refresh_flg) {
      refresh_flg = false;
      clearChars(5, 1, 5);
      lcd.setCursor(5, 1);
      lcd.print(I);
    }
  }
  refresh_flg = true;
  lcd.clear();
  lcd.print("Коэффициент D:");
  while (!digitalRead(ENC_SW));
  while (digitalRead(ENC_SW)) {
    char c = readEnc(QUART_STEP);
    if (c != 0) refresh_flg = true;
    if (c == 'R') D += 0.01;
    if (c == 'L') D -= 0.01;
    D = constrain(D, 0, 20);
    if (refresh_flg) {
      refresh_flg = false;
      clearChars(5, 1, 5);
      lcd.setCursor(5, 1);
      lcd.print(D);
    }
  }
  save_PID();
}

void endpoint_setup() {
  refresh_flg = true;
  lcd.clear();
  lcd.print("Мин. темп-ра:");
  lcd.setCursor(8, 1);
  lcd.print("°C");
  while (!digitalRead(ENC_SW));
  while (digitalRead(ENC_SW)) {
    char c = readEnc(QUART_STEP);
    if (c != 0) refresh_flg = true;
    if (c == 'R') min_temp++;
    if (c == 'L') min_temp--;
    min_temp = constrain(min_temp, ABS_MIN, ABS_MAX);
    if (refresh_flg) {
      refresh_flg = false;
      clearChars(5, 1, 3);
      lcd.setCursor(5, 1);
      lcd.print(min_temp);
    }
  }
  refresh_flg = true;
  lcd.clear();
  lcd.print("Макс. темп-ра:");
  lcd.setCursor(8, 1);
  lcd.print("°C");
  while (!digitalRead(ENC_SW));
  while (digitalRead(ENC_SW)) {
    char c = readEnc(QUART_STEP);
    if (c != 0) refresh_flg = true;
    if (c == 'R') max_temp++;
    if (c == 'L') max_temp--;
    max_temp = constrain(max_temp, ABS_MIN, ABS_MAX);
    if (refresh_flg) {
      refresh_flg = false;
      clearChars(5, 1, 3);
      lcd.setCursor(5, 1);
      lcd.print(max_temp);
    }
  }
  save_maxmin();
}

void set_defaults() {
  static bool yesFlg;
  yesFlg = false;
  refresh_flg = true;
  lcd.clear();
  lcd.print("Сброс настроек?");
  lcd.setCursor(4, 1);
  lcd.print("НЕТ    ДА");
  while (!digitalRead(ENC_SW));
  while (digitalRead(ENC_SW)) {
    char c = readEnc(FULL_STEP);
    if (c != 0) refresh_flg = true;
    if (c == 'R') yesFlg = true;
    if (c == 'L') yesFlg = false;
    if (refresh_flg) {
      refresh_flg = false;
      clearChars(3, 1, 1);
      clearChars(10, 1, 1);
      if (!yesFlg) {
        lcd.setCursor(3, 1);
        lcd.print(">");
      }
      else {
        lcd.setCursor(10, 1);
        lcd.print(">");
      }
    }
  }
  if (yesFlg) {
    save_defaults();
  }

}

void menu() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(">");
  lcd.setCursor(1, 0);
  lcd.print(names[0]);
  lcd.setCursor(1, 1);
  lcd.print(names[1]);
  while (!digitalRead(ENC_SW));
  prev_preset = 0;
  scroll_page = 0;
  scroll_point = 0;
  static bool exit_flg;
  exit_flg = false;
  while (!exit_flg) {
    char c = readEnc(FULL_STEP);
    if (c == 'R') {
      scroll_point++;
      refresh_flg = true;
    }
    if (c == 'L') {
      scroll_point--;
      refresh_flg = true;
    }
    if (scroll_point > 1) {
      scroll_page++;
      scroll_point = 1;
    }
    if (scroll_point < 0) {
      scroll_page--;
      scroll_point = 0;
    }
    scroll_page = constrain(scroll_page, 0, 4);
    if (refresh_flg) {
      refresh_flg = false;
      lcd.clear();
      lcd.setCursor(0, scroll_point);
      lcd.print(">");
      lcd.setCursor(1, 0);
      lcd.print(names[scroll_page]);
      lcd.setCursor(1, 1);
      lcd.print(names[scroll_page + 1]);
    }
    if (!digitalRead(ENC_SW)) {
      while (!digitalRead(ENC_SW));
      switch (scroll_point + scroll_page) {
        case 0:
          lcd.clear();
          refresh_flg = true;
          temp_shift();
          break;
        case 1:
          lcd.clear();
          refresh_flg = true;
          temp_cal();
          break;
        case 2:
          lcd.clear();
          refresh_flg = true;
          PID_setup();
          break;
        case 3:
          lcd.clear();
          refresh_flg = true;
          endpoint_setup();
          break;
        case 4:
          lcd.clear();
          refresh_flg = true;
          set_defaults();
          break;
        case 5:
          showTempNames();
          exit_flg = true;
          break;
      }
      refresh_flg = true;
      while (!digitalRead(ENC_SW));
    }
  }
}
