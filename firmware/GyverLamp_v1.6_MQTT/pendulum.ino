// -------------------------pendulum-------------------------
void prismataRoutine() {

  if (modes[currentMode].scale > 100) modes[currentMode].scale = 100;

  EVERY_N_MILLIS(33) {
    hue++;
  }

  FastLED.clear();

  if ((modes[currentMode].scale >= 0) && (modes[currentMode].scale < 20)) { 
     cPalette = RainbowColors_p;
  } else if ((modes[currentMode].scale >= 20) && (modes[currentMode].scale < 40)) { 
     cPalette  =  PartyColors_p;
  } else if ((modes[currentMode].scale >= 40) && (modes[currentMode].scale < 60)) {
     cPalette  =  CloudColors_p;
  } else if ((modes[currentMode].scale >= 60) && (modes[currentMode].scale < 80)) {
     cPalette  =  LavaColors_p;
  } else if ((modes[currentMode].scale >= 80) && (modes[currentMode].scale <= 100)) {
     cPalette = ForestColors_p;
  }

  //blur2d(leds, WIDTH, HEIGHT, 1); 
  //dimAll(255U - modes[currentMode].scale % 11U);
  for (int x = 0; x < WIDTH; x++)
  {
    uint8_t beat = (GET_MILLIS() * (accum88(x + 1)) * 28 * modes[currentMode].speed) >> 17;
    uint8_t y = scale8(sin8(beat), HEIGHT-1);
    drawPixelXY(x, y, ColorFromPalette(cPalette , x * 7 + hue));
  }
}
