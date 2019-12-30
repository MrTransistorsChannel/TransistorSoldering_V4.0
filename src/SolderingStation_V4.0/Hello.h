#define NUM_PAGES               2               //Количество страниц приветствия
#define HELLO_PAGE_TIME         1500            //Время показа одной страницы (в миллисекундах)

/*
 * Страницы задаются в виде массива из 2х строк по 16 символов
 * 
 * Пример одной страницы:
 * 
 * {"################",
 *  "################"},
 *  
 * Пример 2-х страниц:
 * 
 * {"################",
 *  "################"},
 * {"################",
 *  "################"},
 */

String helloPages[NUM_PAGES][2] = {
  {"ПАЯЛЬНАЯ СТАНЦИЯ",
   "      V4.0      "},
  {"       by",
   "  MrTransistor  "}
};