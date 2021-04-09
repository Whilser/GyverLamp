bool sw = false;

void pattern_1()  {

  EVERY_N_MILLIS(modes[currentMode].speed) {
    FastLED.clear(); sw = !sw;

    if (sw) {
      for (int x = 1; x < round(WIDTH/2); x++) {
        for (int y = 0; y < HEIGHT; y++) {
          drawPixelXY(x, y, CRGB::Red);
        }
      }
     } else {

     for (int x = round(WIDTH/2); x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
          drawPixelXY(x, y, CRGB::Blue);
        }
      } 
     }
  }
}

void pattern_2()  {

  EVERY_N_MILLIS(modes[currentMode].speed) {
    FastLED.clear(); sw = !sw;

    if (sw) {
      for (int x = 1; x < WIDTH; x++) {
        for (int y = 0; y <  round(HEIGHT/2); y++) {
          drawPixelXY(x, y, CRGB::Red);
        }
      }      
     } else {

     for (int x = 1; x < WIDTH; x++) {
        for (int y = round(HEIGHT/2); y < HEIGHT; y++) {
          drawPixelXY(x, y, CRGB::Blue);
        }
      } 
    }
  }
}

void pattern_3() {

  EVERY_N_MILLIS(modes[currentMode].speed) {
    FastLED.clear(); 
    sw = !sw;

    for (int x = 1; x < WIDTH; x++) {
      drawPixelXY(x, round(HEIGHT/2), CRGB::White);
    }

    if (sw) {
      for (int x = 1; x < WIDTH; x++) {
        for (int y = 0; y <=  round(HEIGHT/3); y++) {
          drawPixelXY(x, y, CRGB::Red);
        }
      }
     } else {

     for (int x = 1; x < WIDTH; x++) {
        for (int y = round(HEIGHT*2/3); y < HEIGHT; y++) {
          drawPixelXY(x, y, CRGB::Blue);
        }
      }
    }  
  }
}

void pattern_4() {

  EVERY_N_MILLIS(modes[currentMode].speed) {
    FastLED.clear(); sw = !sw;

    if (sw) {
      for (int x = 1; x < WIDTH; x++) {
        for (int y = 0; y <  HEIGHT; y++) {
          drawPixelXY(x, y, CRGB::Red);
        }
      }
     } else {

     for (int x = 1; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
          drawPixelXY(x, y, CRGB::Blue);
        }
      } 
    }
  }  
}

int p5 = 0; int pp5, ppp5 = 0;
void pattern_5() {

    EVERY_N_MILLIS(modes[currentMode].speed) {
      p5++; if (p5 >= WIDTH) p5 = 0;

      FastLED.clear(); 

      for (int x = 1; x < round(WIDTH/4) ; x++) {
        for (int y = 1; y <  HEIGHT; y++) {

          pp5 = x+p5; if (pp5 > WIDTH) pp5 = pp5 - WIDTH;
          ppp5 = x+p5+round(WIDTH/2); if (ppp5 > WIDTH) ppp5 = ppp5 - WIDTH;

          drawPixelXY(pp5, y, CRGB::Red);
          drawPixelXY(ppp5, y, CRGB::Blue);
        }
      }
    }
}

void policeStroboRoutine() {

  if ((modes[currentMode].scale >= 0) && (modes[currentMode].scale < 20)) pattern_1();
  if ((modes[currentMode].scale >= 20) && (modes[currentMode].scale < 40)) pattern_2();
  if ((modes[currentMode].scale >= 40) && (modes[currentMode].scale < 60)) pattern_3();
  if ((modes[currentMode].scale >= 60) && (modes[currentMode].scale < 80)) pattern_4();
  if (modes[currentMode].scale >= 80) pattern_5();

}
