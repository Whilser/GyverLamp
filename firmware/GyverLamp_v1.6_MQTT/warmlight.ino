void warmLightRoutine()
{
  uint8_t centerY = max((uint8_t)round(HEIGHT / 2.0F) - 1, 0);
  uint8_t bottomOffset = (uint8_t)(!(HEIGHT & 1) && (HEIGHT > 1));
  if (modes[currentMode].scale<17) modes[currentMode].scale = 17;
  for (int16_t y = centerY; y >= 0; y--)
  {
    CRGB color = CHSV(
                   45U,
                   map(modes[currentMode].speed, 0U, 255U, 0U, 170U),
                   y == centerY
                   ? 255U
                 : (modes[currentMode].scale / 100.0F) > ((centerY + 1.0F) - (y + 1.0F)) / (centerY + 1.0F) ? 255U : 0U);
    for (uint8_t x = 0U; x < WIDTH; x++)
    {
      drawPixelXY(x, y, color);
      drawPixelXY(x, max((uint8_t)(HEIGHT - 1U) - (y + 1U) + bottomOffset, 0U), color);
    }
  }
}
