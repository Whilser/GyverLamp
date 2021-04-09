
void setCurrentPallete(){
      if (modes[currentMode].scale > 100) modes[currentMode].scale = 100; // чтобы не было проблем при прошивке без очистки памяти
      curPalette = palette_arr[(int)((float)modes[currentMode].scale/100*((sizeof(palette_arr)/sizeof(TProgmemRGBPalette16 *))-1U))];
}

// ------------------------------ ЭФФЕКТ МЕРЦАНИЕ ----------------------
// (c) SottNick

#define TWINKLES_SPEEDS 4     // всего 4 варианта скоростей мерцания
#define TWINKLES_MULTIPLIER 6 // слишком медленно, если на самой медленной просто по единичке добавлять

void twinklesRoutine() {
    if (loadingFlag)
    {
      loadingFlag = false;
      setCurrentPallete();
      hue = 0U;
      for (uint32_t idx=0; idx < NUM_LEDS; idx++) {
        if (random8(modes[currentMode].scale % 11U) == 0){
          ledsbuff[idx].r = random8();                          // оттенок пикселя
          ledsbuff[idx].g = random8(1, TWINKLES_SPEEDS * 2 +1); // скорость и направление (нарастает 1-4 или угасает 5-8)
          ledsbuff[idx].b = random8();                          // яркость
        }
        else
          ledsbuff[idx] = 0;                                    // всё выкл
      }
    }
    for (uint32_t idx=0; idx < NUM_LEDS; idx++) {
      if (ledsbuff[idx].b == 0){
        if (random8(modes[currentMode].scale % 11U) == 0 && hue > 0) {  // если пиксель ещё не горит, зажигаем каждый ХЗй
          ledsbuff[idx].r = random8();                          // оттенок пикселя
          ledsbuff[idx].g = random8(1, TWINKLES_SPEEDS +1);     // скорость и направление (нарастает 1-4, но не угасает 5-8)
          ledsbuff[idx].b = ledsbuff[idx].g;                    // яркость
          hue--; // уменьшаем количество погасших пикселей
        }
      }
      else if (ledsbuff[idx].g <= TWINKLES_SPEEDS){             // если нарастание яркости
        if (ledsbuff[idx].b > 255U - ledsbuff[idx].g - TWINKLES_MULTIPLIER){            // если досигнут максимум
          ledsbuff[idx].b = 255U;
          ledsbuff[idx].g = ledsbuff[idx].g + TWINKLES_SPEEDS;
        }
        else
          ledsbuff[idx].b = ledsbuff[idx].b + ledsbuff[idx].g + TWINKLES_MULTIPLIER;
      }
      else {                                                    // если угасание яркости
        if (ledsbuff[idx].b <= ledsbuff[idx].g - TWINKLES_SPEEDS + TWINKLES_MULTIPLIER){// если досигнут минимум
          ledsbuff[idx].b = 0;                                  // всё выкл
          hue++; // считаем количество погасших пикселей
        }
        else
          ledsbuff[idx].b = ledsbuff[idx].b - ledsbuff[idx].g + TWINKLES_SPEEDS - TWINKLES_MULTIPLIER;
      }
      if (ledsbuff[idx].b == 0)
        leds[idx] = 0U;
      else
        leds[idx] = ColorFromPalette(*curPalette, ledsbuff[idx].r, ledsbuff[idx].b);
    }
}
