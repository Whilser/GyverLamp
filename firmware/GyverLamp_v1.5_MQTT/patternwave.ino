    byte patterwavethetaUpdate = 0;
    byte patterwavethetaUpdateFrequency = 0;
    byte patterwavetheta = 0;

    byte patterwavehueUpdate = 0;
    byte patterwavehueUpdateFrequency = 0;
    byte patterwavehue = 0;

    byte patterwaverotation = 0;


    uint8_t patterwavemaxX = WIDTH - 1;
    uint8_t patterwavemaxY = HEIGHT - 1;

    uint8_t patterwavewaveCount = 1;  // 1 or 2

    
void patternWave() {
  int n = 0;
  int m = 0;
        for (int x = 0; x < WIDTH; x++) {
           n = quadwave8(x * 2 + patterwavetheta) / (( modes[currentMode].scale / WIDTH ) + 4);
           m = quadwave8((32 - x) * 2 + patterwavetheta) / (( modes[currentMode].scale / WIDTH ) + 4);


           drawPixelXY(x, n, ColorFromPalette(RainbowColors_p , x + patterwavehue));
           drawPixelXY(x, m, ColorFromPalette(RainbowColors_p , x + patterwavehue));


           if (patterwavewaveCount == 2)
              drawPixelXY(x, patterwavemaxY - n, ColorFromPalette(RainbowColors_p , x + patterwavehue));
        }
        dimAll(((modes[currentMode].scale ) * 1.28 ) + 128);

        if (patterwavethetaUpdate >= patterwavethetaUpdateFrequency) {
            patterwavethetaUpdate = 0;
            patterwavetheta++;
        }
        else {
            patterwavethetaUpdate++;
        }

        if (patterwavehueUpdate >= patterwavehueUpdateFrequency) {
            patterwavehueUpdate = 0;
            patterwavehue++;
        }
        else {
            patterwavehueUpdate++;
        }

}
