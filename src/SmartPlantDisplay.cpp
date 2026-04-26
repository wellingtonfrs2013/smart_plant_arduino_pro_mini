#include "SmartPlantDisplay.h"

#include <string.h>

namespace
{
const char WEEKDAY_NAMES[7][14] = {
    "Sunday",
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday",
    "Saturday"};

const unsigned char ICON_CLOCK[] PROGMEM = {
    0x00, 0x00,
    0xe0, 0x07,
    0x18, 0x18,
    0x04, 0x20,
    0x02, 0x40,
    0x02, 0x40,
    0x82, 0x41,
    0x82, 0x41,
    0x82, 0x43,
    0x02, 0x40,
    0x02, 0x40,
    0x04, 0x20,
    0x18, 0x18,
    0xe0, 0x07,
    0x00, 0x00,
    0x00, 0x00};

const unsigned char ICON_CALENDAR[] PROGMEM = {
    0x00, 0x00,
    0xfc, 0x3f,
    0x04, 0x20,
    0x24, 0x24,
    0xfc, 0x3f,
    0x04, 0x20,
    0x94, 0x29,
    0x04, 0x20,
    0x54, 0x2a,
    0x04, 0x20,
    0x94, 0x29,
    0x04, 0x20,
    0xfc, 0x3f,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00};

const unsigned char ICON_TEMP[] PROGMEM = {
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x80, 0x01,
    0x80, 0x01,
    0x80, 0x01,
    0x80, 0x01,
    0x80, 0x01,
    0x80, 0x01,
    0xc0, 0x03,
    0xe0, 0x07,
    0x60, 0x06,
    0xe0, 0x07,
    0xc0, 0x03,
    0x00, 0x00};

const unsigned char ICON_HUMIDITY[] PROGMEM = {
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x80, 0x01,
    0xc0, 0x03,
    0xe0, 0x07,
    0xf0, 0x0f,
    0xf0, 0x0f,
    0xf0, 0x0f,
    0x70, 0x0e,
    0x30, 0x0c,
    0x18, 0x18,
    0x0c, 0x30,
    0x00, 0x00};

const unsigned char ICON_SOIL[] PROGMEM = {
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x80, 0x01,
    0xc0, 0x03,
    0x60, 0x06,
    0xc0, 0x03,
    0x80, 0x01,
    0xff, 0xff,
    0x24, 0x24,
    0x92, 0x49,
    0x49, 0x92,
    0x24, 0x24,
    0x00, 0x00};
}

SmartPlantDisplay::SmartPlantDisplay(U8GLIB_SSD1306_128X64 &display)
    : display(display)
{
}

void SmartPlantDisplay::render(ScreenId screenId,
                               const SensorReadings &sensorReadings,
                               const DateTime &now,
                               PlantMood mood,
                               const AnimationState &animationState)
{
  switch (screenId)
  {
  case ScreenId::Emoticon:
    drawEmoticonScreen(mood, animationState);
    break;
  case ScreenId::DateTime:
    drawDateTimeScreen(now);
    break;
  case ScreenId::Sensor:
    drawSensorScreen(sensorReadings);
    break;
  }
}

void SmartPlantDisplay::drawEmoticonScreen(PlantMood mood, const AnimationState &animationState)
{
  const int centerX = 64;
  const int centerY = 18;
  const int faceY = centerY;

  drawPlantBaseAnimated(centerX, centerY, animationState.frame);
  display.setFont(u8g_font_6x10);

  if (mood == PlantMood::Night)
  {
    display.drawLine(centerX - 8, faceY - 3, centerX - 3, faceY - 3);
    display.drawLine(centerX + 3, faceY - 3, centerX + 8, faceY - 3);
    display.drawLine(centerX - 5, faceY + 7, centerX + 5, faceY + 7);

    if (animationState.frame == 0)
    {
      display.drawStr(88, 18, "Z");
      display.drawStr(96, 10, "z");
    }
    else
    {
      display.drawStr(90, 16, "z");
      display.drawStr(98, 8, "Z");
    }
    return;
  }

  if (mood == PlantMood::DrySoil)
  {
    drawBlinkingEyes(centerX, faceY, animationState.blink);
    display.drawLine(centerX - 7, faceY + 8, centerX - 3, faceY + 5);
    display.drawLine(centerX - 3, faceY + 5, centerX + 3, faceY + 5);
    display.drawLine(centerX + 3, faceY + 5, centerX + 7, faceY + 8);

    if (animationState.frame == 0)
    {
      display.drawLine(91, 28, 88, 34);
      display.drawLine(91, 28, 94, 34);
      display.drawCircle(91, 36, 3);
    }
    return;
  }

  if (mood == PlantMood::Cold)
  {
    const int shakeOffset = animationState.frame == 0 ? 0 : 1;

    drawBlinkingEyes(centerX, faceY, animationState.blink);
    display.drawLine(centerX - 7, faceY + 7 + shakeOffset, centerX - 3, faceY + 5);
    display.drawLine(centerX - 3, faceY + 5, centerX, faceY + 7 + shakeOffset);
    display.drawLine(centerX, faceY + 7 + shakeOffset, centerX + 3, faceY + 5);
    display.drawLine(centerX + 3, faceY + 5, centerX + 7, faceY + 7 + shakeOffset);
    display.drawLine(centerX - 13, centerY + 29, centerX + 13, centerY + 29);
    display.drawLine(centerX - 2, centerY + 29, centerX - 2, centerY + 39);
    return;
  }

  if (mood == PlantMood::Hot)
  {
    const int sunRadius = animationState.frame == 0 ? 4 : 5;

    display.drawLine(centerX - 8, faceY - 4, centerX - 3, faceY - 3);
    display.drawLine(centerX + 3, faceY - 3, centerX + 8, faceY - 4);
    display.drawLine(centerX - 6, faceY + 7, centerX + 6, faceY + 7);

    if (animationState.frame == 0)
    {
      display.drawLine(90, 25, 87, 31);
      display.drawLine(90, 25, 93, 31);
      display.drawCircle(90, 33, 3);
    }

    display.drawCircle(103, 12, sunRadius);
    display.drawLine(103, 5, 103, 8);
    display.drawLine(103, 17, 103, 20);
    display.drawLine(96, 12, 99, 12);
    display.drawLine(107, 12, 110, 12);
    return;
  }

  drawBlinkingEyes(centerX, faceY, animationState.blink);
  display.drawLine(centerX - 7, faceY + 5, centerX - 3, faceY + 8);
  display.drawLine(centerX - 3, faceY + 8, centerX + 3, faceY + 8);
  display.drawLine(centerX + 3, faceY + 8, centerX + 7, faceY + 5);
}

void SmartPlantDisplay::drawDateTimeScreen(const DateTime &now)
{
  char timeText[9];
  char dateText[12];

  sprintf(timeText, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
  sprintf(dateText, "%02d/%02d/%02d", now.day(), now.month(), now.year() % 100);

  display.drawXBMP(0, 3, 16, 16, ICON_CLOCK);
  display.setFont(u8g_font_6x10);
  display.drawStr(20, 12, "Time:");

  display.setFont(u8g_font_7x13B);
  display.drawStr(58, 15, timeText);

  display.drawHLine(0, 24, 128);

  display.drawXBMP(0, 30, 16, 16, ICON_CALENDAR);
  display.setFont(u8g_font_6x10);
  display.drawStr(20, 39, "Date:");

  display.setFont(u8g_font_7x13B);
  display.drawStr(58, 42, dateText);

  display.setFont(u8g_font_7x13B);
  display.drawStr(20, 56, WEEKDAY_NAMES[now.dayOfTheWeek()]);
}

void SmartPlantDisplay::drawSensorScreen(const SensorReadings &sensorReadings)
{
  char temperatureText[8];
  char humidityText[8];
  char soilText[8];

  sprintf(temperatureText, "%d", sensorReadings.temperature);
  sprintf(humidityText, "%d%%", sensorReadings.humidity);
  sprintf(soilText, "%d%%", sensorReadings.soil);

  const int firstRowY = 13;
  const int secondRowY = 33;
  const int thirdRowY = 53;

  display.drawXBMP(0, 0, 16, 16, ICON_TEMP);
  display.setFont(u8g_font_6x10);
  display.drawStr(20, firstRowY, "Temperature:");

  display.setFont(u8g_font_7x13B);
  display.drawStr(100, firstRowY, temperatureText);

  const int temperatureValueX = 100 + (strlen(temperatureText) * 7);
  display.drawCircle(temperatureValueX + 3, firstRowY - 8, 2);
  display.drawStr(temperatureValueX + 8, firstRowY, "C");

  display.drawXBMP(0, 20, 16, 16, ICON_HUMIDITY);
  display.setFont(u8g_font_6x10);
  display.drawStr(20, secondRowY, "Air:");

  display.setFont(u8g_font_7x13B);
  display.drawStr(58, secondRowY, humidityText);

  display.drawXBMP(0, 40, 16, 16, ICON_SOIL);
  display.setFont(u8g_font_6x10);
  display.drawStr(20, thirdRowY, "Soil:");

  display.setFont(u8g_font_7x13B);
  display.drawStr(58, thirdRowY, soilText);
}

void SmartPlantDisplay::drawPlantBaseAnimated(int centerX, int centerY, int frame)
{
  const int sway = frame == 0 ? -1 : 1;

  display.drawCircle(centerX, centerY, 16);
  display.drawLine(centerX, centerY + 16, centerX, centerY + 31);

  display.drawLine(centerX, centerY + 22, centerX - 10 + sway, centerY + 16);
  display.drawLine(centerX - 10 + sway, centerY + 16, centerX - 20 + sway, centerY + 20);
  display.drawLine(centerX - 20 + sway, centerY + 20, centerX - 10 + sway, centerY + 25);
  display.drawLine(centerX - 10 + sway, centerY + 25, centerX, centerY + 22);
  display.drawLine(centerX - 3, centerY + 22, centerX - 14 + sway, centerY + 20);

  display.drawLine(centerX, centerY + 22, centerX + 10 - sway, centerY + 16);
  display.drawLine(centerX + 10 - sway, centerY + 16, centerX + 20 - sway, centerY + 20);
  display.drawLine(centerX + 20 - sway, centerY + 20, centerX + 10 - sway, centerY + 25);
  display.drawLine(centerX + 10 - sway, centerY + 25, centerX, centerY + 22);
  display.drawLine(centerX + 3, centerY + 22, centerX + 14 - sway, centerY + 20);

  display.drawLine(centerX - 18, centerY + 32, centerX + 18, centerY + 32);
  display.drawLine(centerX - 15, centerY + 32, centerX - 10, centerY + 44);
  display.drawLine(centerX + 15, centerY + 32, centerX + 10, centerY + 44);
  display.drawLine(centerX - 10, centerY + 44, centerX + 10, centerY + 44);
}

void SmartPlantDisplay::drawBlinkingEyes(int centerX, int faceY, bool blink)
{
  if (blink)
  {
    display.drawLine(centerX - 8, faceY - 3, centerX - 4, faceY - 3);
    display.drawLine(centerX + 4, faceY - 3, centerX + 8, faceY - 3);
    return;
  }

  display.drawDisc(centerX - 6, faceY - 3, 2);
  display.drawDisc(centerX + 6, faceY - 3, 2);
}