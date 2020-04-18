// процедура dimAll использована готовая из эффекта  ометы
// если компилятор скетча жалуется на повторы, эти строчки нужно удалить
void dimAll(uint8_t value) {
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    leds[i].nscale8(value); //fadeToBlackBy
  }
}


// --------------------------- эффект спирали ----------------------
/*
 * Aurora: https://github.com/pixelmatix/aurora
 * https://github.com/pixelmatix/aurora/blob/sm3.0-64x64/PatternSpiro.h
 * Copyright (c) 2014 Jason Coon
 * Неполная адаптация SottNick
 */
    byte spirotheta1 = 0;
    byte spirotheta2 = 0;
    byte spirohueoffset = 0;

    const uint8_t spiroradiusx = WIDTH / 4;
    const uint8_t spiroradiusy = HEIGHT / 4;
    
    const uint8_t spirocenterX = WIDTH / 2;
    const uint8_t spirocenterY = HEIGHT / 2;
    
    const uint8_t spirominx = spirocenterX - spiroradiusx;
    const uint8_t spiromaxx = spirocenterX + spiroradiusx + 1;
    const uint8_t spirominy = spirocenterY - spiroradiusy;
    const uint8_t spiromaxy = spirocenterY + spiroradiusy + 1;

    uint8_t spirocount = 1;
    uint8_t spirooffset = 256 / spirocount;
    boolean spiroincrement = false;

    boolean spirohandledChange = false;

uint8_t mapsin8(uint8_t theta, uint8_t lowest = 0, uint8_t highest = 255) {
  uint8_t beatsin = sin8(theta);
  uint8_t rangewidth = highest - lowest;
  uint8_t scaledbeat = scale8(beatsin, rangewidth);
  uint8_t result = lowest + scaledbeat;
  return result;
}

uint8_t mapcos8(uint8_t theta, uint8_t lowest = 0, uint8_t highest = 255) {
  uint8_t beatcos = cos8(theta);
  uint8_t rangewidth = highest - lowest;
  uint8_t scaledbeat = scale8(beatcos, rangewidth);
  uint8_t result = lowest + scaledbeat;
  return result;
}
  
void spiroRoutine() {
      dimAll(250);

      boolean change = false;
      
      for (int i = 0; i < spirocount; i++) {
        uint8_t x = mapsin8(spirotheta1 + i * spirooffset, spirominx, spiromaxx);
        uint8_t y = mapcos8(spirotheta1 + i * spirooffset, spirominy, spiromaxy);

        uint8_t x2 = mapsin8(spirotheta2 + i * spirooffset, x - spiroradiusx, x + spiroradiusx);
        uint8_t y2 = mapcos8(spirotheta2 + i * spirooffset, y - spiroradiusy, y + spiroradiusy);

        CRGB color = ColorFromPalette( PartyColors_p, (spirohueoffset + i * spirooffset)     , 128U); // вообще-то палитра должна постоянно меняться, но до адаптации этого руки уже не дошли

      if (x2<WIDTH && y2<HEIGHT) // добавил проверки. не знаю, почему эффект подвисает без них
        leds[getPixelNumber(x2, y2)] += color;
        
        if((x2 == spirocenterX && y2 == spirocenterY) ||
           (x2 == spirocenterX && y2 == spirocenterY)) change = true;
      }

      spirotheta2 += 2;

      EVERY_N_MILLIS(12) {
        spirotheta1 += 1;
      }

      EVERY_N_MILLIS(75) {
        if (change && !spirohandledChange) {
          spirohandledChange = true;
          
          if (spirocount >= WIDTH || spirocount == 1) spiroincrement = !spiroincrement;

          if (spiroincrement) {
            if(spirocount >= 4)
              spirocount *= 2;
            else
              spirocount += 1;
          }
          else {
            if(spirocount > 4)
              spirocount /= 2;
            else
              spirocount -= 1;
          }

          spirooffset = 256 / spirocount;
        }
        
        if(!change) spirohandledChange = false;
      }

      EVERY_N_MILLIS(33) {
        spirohueoffset += 1;
      }
}
