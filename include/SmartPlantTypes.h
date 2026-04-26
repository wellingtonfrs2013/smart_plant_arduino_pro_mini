#pragma once

#include <Arduino.h>

enum class ScreenId : uint8_t
{
  Emoticon = 0,
  DateTime = 1,
  Sensor = 2,
};

enum class PlantMood : uint8_t
{
  Happy = 0,
  Night = 1,
  DrySoil = 2,
  Cold = 3,
  Hot = 4,
};

struct SensorReadings
{
  int temperature = 0;
  int humidity = 0;
  int soil = 60;
};

struct AnimationState
{
  int frame = 0;
  bool blink = false;
};

struct AppState
{
  SensorReadings sensors;
  uint8_t currentScreenIndex = 0;
  int touchState = LOW;
  int testMode = 0;
  unsigned long previousSensorMillis = 0;
};