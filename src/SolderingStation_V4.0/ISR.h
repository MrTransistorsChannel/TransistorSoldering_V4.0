ISR(PCINT1_vect) { //Обработчик прерывания кнопки включения
  if (!digitalRead(PWR_BTN)) {
    PCMSK1 = 0b00000000;
  }
}
