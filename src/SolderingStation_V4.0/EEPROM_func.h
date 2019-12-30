void get_settings() {//ФУНКЦИЯ ЗАПРОСА НАСТРОЕК ИЗ ПАМЯТИ
  preset_temp = EEPROM.readInt(0);
  shift = EEPROM.readInt(2);
  real100 = EEPROM.readInt(4);
  real400 = EEPROM.readInt(6);
  P = EEPROM.readFloat(8);
  I = EEPROM.readFloat(12);
  D = EEPROM.readFloat(16);
  min_temp = EEPROM.readInt(20);
  max_temp = EEPROM.readInt(22);
}

void save_preset() {//ФУНКЦИЯ СОХРАНЕНИЯ ПРЕДУСТАНОВКИ ТЕМПЕРАТУРЫ
  EEPROM.writeInt(0, preset_temp);
}
void save_correction() {//ФУНКЦИЯ СОХРАНЕНИЯ КОРРЕКТИРОВКИ ТЕМПЕРАТУРЫ
  EEPROM.writeInt(2, shift);
}

void save_cal() {//ФУНКЦИЯ СОХРАНЕНИЯ КАЛИБРОВОЧНЫХ ЗНАЧЕНИЙ
  EEPROM.writeInt(4, real100);
  EEPROM.writeInt(6, real400);
}

void save_PID() {//ФУНКЦИЯ СОХРАНЕНИЯ КОЭФФИЦИЕНТОВ ПИД-РЕГУЛЯТОРА
  EEPROM.writeFloat(8, P);
  EEPROM.writeFloat(12, I);
  EEPROM.writeFloat(16, D);
}

void save_maxmin() {//ФУНКЦИЯ СОХРАНЕНИЯ ГРАНИЦ ТЕМПЕРАТУРНОГО ДИАПАЗОНА
  EEPROM.writeInt(20, min_temp);
  EEPROM.writeInt(22, max_temp);
}

void save_defaults() {//ФУНКЦИЯ ОТКАТА НА НАСТРОЙКИ ПО УМОЛЧАНИЮ
  EEPROM.writeInt(0, DEFAULT_PRESET);
  EEPROM.writeInt(2, 0);
  EEPROM.writeInt(4, 100);
  EEPROM.writeInt(6, 400);
  EEPROM.writeFloat(8, DEFAULT_P);
  EEPROM.writeFloat(12, DEFAULT_I);
  EEPROM.writeFloat(16, DEFAULT_D);
  EEPROM.writeInt(20, ABS_MIN);
  EEPROM.writeInt(22, ABS_MAX);
  get_settings();
}

void check_version() {//ФУНКЦИЯ ПРОВЕРКИ КОДА ВЕРСИИ ПРОШИВКИ
  if (_VERSION_ != EEPROM.read(0x3FF)) {
    EEPROM.write(0x3FF, _VERSION_);
    save_defaults();
  }
}
