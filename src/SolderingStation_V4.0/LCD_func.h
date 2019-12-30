void showHello() {//ФУНКЦИЯ ОТОБРАЖЕНИЯ ПРИВЕТСТВЕННОГО СООБЩЕНИЯ
  for (int i = 0; i < NUM_PAGES; i++) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(helloPages[i][0]);
    lcd.setCursor(0, 1);
    lcd.print(helloPages[i][1]);
    delay(HELLO_PAGE_TIME);
  }
}

void showTempNames() {//ФУНКЦИЯ ОТОБРАЖЕНИЯ ПОДПИСЕЙ ТЕМПЕРАТУР
  lcd.clear();
  lcd.print(L"Уст. t°C: ");
  lcd.setCursor(0, 1);
  lcd.print(L"Текущ. t°C: ");
}

void clearChars(int startPosX, int startPosY, int numOf) {//ФУНКЦИЯ СТИРАНИЯ N СИМВОЛОВ
  lcd.setCursor(startPosX, startPosY);// Ставим курсор на начальные координаты,
  for (int i = 0; i < numOf; i++) { // повторяем numOf раз
    lcd.print(" ");// рисование пробела
  }
}
