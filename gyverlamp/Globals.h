/*
  Скетч к проекту "Многофункциональный RGB светильник"
  Страница проекта (схемы, описания): https://alexgyver.ru/GyverLamp/
  Исходники на GitHub: https://github.com/AlexGyver/GyverLamp/
  Нравится, как написан код? Поддержи автора! https://alexgyver.ru/support_alex/
  Автор: AlexGyver, AlexGyver Technologies, 2019
  https://AlexGyver.ru/

  клон версии от whisler
  https://github.com/Whisler/GyverLamp

  адаптация под сборку на platformio
  https://github.com/vortigont/GyverLamp
*/

#ifndef GLOBALS_H
#define GLOBALS_H

// Sketch configuration
#include "config.h"

#include <Arduino.h>
// Libs


// ========= Defines

// Enable Serial.print aliases if _DEBUG_ is defined
#ifdef _DEBUG_
  #define _SP(a) Serial.print(a);
  #define _SPLN(a) Serial.println(a);
  #define _SPF(a,b) Serial.printf(a,b);
  #define _SPTO(a) a;
#else
  #define _SP(a)
  #define _SPLN(a)
  #define _SPF(a,b)
  #define _SPTO(a)
#endif

/*
class Globals {
public:
    // public methods
    Globals();
    Globals(const Globals& orig);
    virtual ~Globals();
}
*/
#endif
