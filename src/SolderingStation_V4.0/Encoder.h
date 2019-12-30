byte stp, prev_stp;
char encStepped;
int code = 500;

typedef enum {
  FULL_STEP = 0,
  QUART_STEP
} enc_alg;

char readEnc(enc_alg alg) {
  /**************ОБНОВЛЕННЫЙ И ОБЫЧНЫЙ АЛГОРИТМЫ СЧИТЫВАНИЯ ЭНКОДЕРА**************/
  encStepped = 0;// обнуление выходного значения
  prev_stp = stp;
  stp = (!digitalRead(ENC_A) << 1) | !digitalRead(ENC_B);
  if (stp == 0) {
    code = 500;
  } // вправо - 00 01 11 10 00 (0 1 3 2 0), влево - 00 10 11 01 00 (0 2 3 1 0)//
  if (prev_stp != stp) {
    if (stp == 1) {
      code = code - 4;
    }
    if (stp == 2) {
      code = code + 3;
    }
    if (stp == 3) {
      code = code * 2;
    }

    if (alg == QUART_STEP) {
      if (code == 496 || code == 992 || code == 995) {
        encStepped = 'R';///////////если повернули вправо = 'R'
      }
      if (code == 503 || code == 1006 || code == 1002) {
        encStepped = 'L';//////////если влево = 'L'
      }
    }
    else if (alg == FULL_STEP) {
      if (code == 995) {
        encStepped = 'R';///////////если повернули вправо = 'R'
      }
      if (code == 1002) {
        encStepped = 'L';//////////если влево = 'L'
      }
    }
  }
  /*******************************************************************************/
  return encStepped;//возвращаем значение
}
