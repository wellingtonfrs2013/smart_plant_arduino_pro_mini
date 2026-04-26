#pragma once

#include <RTClib.h>
#include <U8glib-HAL.h>

#include "SmartPlantTypes.h"

class SmartPlantDisplay
{
public:
  explicit SmartPlantDisplay(U8GLIB_SSD1306_128X64 &display);

  void render(ScreenId screenId,
              const SensorReadings &sensorReadings,
              const DateTime &now,
              PlantMood mood,
              const AnimationState &animationState);

private:
  void drawEmoticonScreen(PlantMood mood, const AnimationState &animationState);
  void drawDateTimeScreen(const DateTime &now);
  void drawSensorScreen(const SensorReadings &sensorReadings);
  void drawPlantBaseAnimated(int centerX, int centerY, int frame);
  void drawBlinkingEyes(int centerX, int faceY, bool blink);

  U8GLIB_SSD1306_128X64 &display;
};