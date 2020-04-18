CHSV dawnColor;

/*
    timeset - callback вызывается при установке времени
*/
void timeset(){
  tickerAlarm.once_scheduled(1,  checkDawn);  // trigger Dawn checker
}

/*
 *  textScroller - callback function that scrolls text over canvas
 * @param void
 */
void textScroller() {
  fill_solid(leds, NUM_LEDS, dawnColor);
  fillString(gettimeStr(), CRGB::Black, false);
  FastLED.show();
}

String gettimeStr() {
    char timeStr[6];
    now = time(nullptr);
    sprintf(timeStr, "%u:%02u", localtime(&now)->tm_hour, localtime(&now)->tm_min);
    return String(timeStr);
}

void checkDawn() {
  now = time(nullptr);
  const tm* tm = localtime(&now);
  byte thisDay = tm->tm_wday;
  if (thisDay == 0) thisDay = 7;  // воскресенье это 0
  thisDay--;
  uint16_t thisTime = tm->tm_hour * 60 + tm->tm_min;   // число минут с начала суток

  // проверка рассвета
  if (alarm[thisDay].state &&                                       // день будильника
      thisTime >= (alarm[thisDay].time - dawnOffsets[dawnMode]) &&  // позже начала
      thisTime < (alarm[thisDay].time + DAWN_TIMEOUT) ) {           // раньше конца + минута

      // секунд от начала рассвета
      int16_t dawnPosition = (thisTime + dawnOffsets[dawnMode] - alarm[thisDay].time)*60 + tm->tm_sec;
      _SP("dawnPosition: "); _SPLN(dawnPosition);

      // manual override, next check after dawn ends
      if (manualOff) {
        alarmreSchedlue((dawnOffsets[dawnMode] + DAWN_TIMEOUT)*60 - dawnPosition);
        return;
      }

      // вычисляем цвет рассвета на текущую секунду
      dawnColor = CHSV(map(dawnPosition, 1, 60*dawnOffsets[dawnMode], 10, 35),
                       map(dawnPosition, 1, 60*dawnOffsets[dawnMode], 255, 170),
                       map(dawnPosition, 1, 60*dawnOffsets[dawnMode], DAWN_MIN_BRIGHT, DAWN_MAX_BRIGHT));
      if (! tickerScroller.active()) {
        tickerScroller.attach_ms_scheduled(TIMER_SCROLLER, textScroller);
        FastLED.setBrightness(255);
        dawnFlag = true;
      }
      tickerAlarm.once_scheduled(1,  checkDawn);    // tick every second while dawn
  } else {
    alarmreSchedlue(60 - tm->tm_sec); // recheck at next 00 seconds
  }
}

/*
 *  alarmreSchedlue - disable dawn light and reschedule dawnchecer
 * @param uint16_t timeout - reschedule timeout (seconds)
 */
void alarmreSchedlue(uint16_t timeout) {
  dawnFlag = false;
  manualOff = false;
  if (tickerScroller.active()) {
    tickerScroller.detach();
    FastLED.setBrightness(modes[currentMode].brightness);
    FastLED.clear();
    FastLED.show();
  }
  tickerAlarm.once_scheduled(timeout,  checkDawn);  // arm Dawn checker
}
