#pragma once

#include <DHT.h>
#include <RTClib.h>
#include <U8glib-HAL.h>

#include "SmartPlantDisplay.h"

class SmartPlantApp
{
public:
  SmartPlantApp();

  void setup();
  void loop();

private:
  void updateTestMode();
  void updateSensors(unsigned long currentMillis);
  void updateTouchInput();
  void renderCurrentScreen();
  AnimationState buildAnimationState() const;
  PlantMood resolveMood(const DateTime &now) const;
  ScreenId currentScreen() const;

  U8GLIB_SSD1306_128X64 display;
  DHT dht;
  RTC_DS3231 rtc;
  SmartPlantDisplay renderer;
  AppState state;
};