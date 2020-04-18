uint32_t effTimer;
bool stop_eff = false;

void effectsTick() {
  if (dawnFlag || stop_eff) return;

  if (millis() - effTimer >= ((currentMode < 5 || currentMode > 13) ? modes[currentMode].speed : 50) ) {
      effTimer = millis();
      switch (currentMode) {
        case 0: sparklesRoutine();
          break;
        case 1: fireRoutine();
          break;
        case 2: rainbowVertical();
          break;
        case 3: rainbowHorizontal();
          break;
        case 4: colorsRoutine();
          break;
        case 5: madnessNoise();
          break;
        case 6: cloudNoise();
          break;
        case 7: lavaNoise();
          break;
        case 8: plasmaNoise();
          break;
        case 9: rainbowNoise();
          break;
        case 10: rainbowStripeNoise();
          break;
        case 11: zebraNoise();
          break;
        case 12: forestNoise();
          break;
        case 13: oceanNoise();
          break;
        case 14: colorRoutine();
          break;
        case 15: snowRoutine();
          break;
        case 16: matrixRoutine();
          break;
        case 17: lightersRoutine();
          break;
        case 18: aquaRoutine();
          break;
        case 19: starfallRoutine();
          break;
        case 20: lightBallsRoutine();
          break;
        case 21: spiroRoutine();
          break;
      }
      FastLED.show();
  }
}

void changePower() {
  if (ONflag && !stop_eff) return;

  if (ONflag) {
    // Включение
    stop_eff = false;
    int steps = 1 + round(modes[currentMode].brightness / 80);
    for (int i = 0; i <= modes[currentMode].brightness; i += steps) {
      FastLED.setBrightness(i);
      effectsTick();
      delay(2);
      FastLED.show();
    }
    FastLED.setBrightness(modes[currentMode].brightness);
    delay(2);
    FastLED.show();

  } else {
    // Выключение
    int steps = 1 + round(modes[currentMode].brightness / 40);
    for (int i = modes[currentMode].brightness; i >= 0; i -= steps) {
      FastLED.setBrightness(i);
      effectsTick();
      delay(2);
      FastLED.show();
    }

    stop_eff = true;
    FastLED.clear();
    delay(2);
    FastLED.show();
  }

  // записываем статус лампы в память
  EEPROM.write(420, ONflag); EEPROM.commit();

}
