void PatternIncrementalDrift2() {
  const int CENTER_X = WIDTH / 2;
  const int CENTER_Y = HEIGHT / 2;
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
  uint8_t dim = beatsin8(2, 170, 250);
  dimAll(dim);
//  FastLED.clear();
  for (uint8_t i = 0; i < WIDTH; i++)
  {
    CRGB color;
    uint8_t x = 0;
    uint8_t y = 0;

    if (i < CENTER_X) {
      x = beatcos8((i + 1) * 2, i, WIDTH - i);
      y = beatsin8((i + 1) * 2, i, HEIGHT - i);
      color = ColorFromPalette(cPalette ,i * 14);
    }
    else 
    {
      x = beatsin8((WIDTH  - i) * 2, WIDTH - i, i + 1);
      y = beatcos8((HEIGHT - i) * 2, HEIGHT - i, i + 1);
      color = ColorFromPalette(cPalette ,(31 - i) * 14);
    }
    drawPixelXY(x, y, color);
  }
}
