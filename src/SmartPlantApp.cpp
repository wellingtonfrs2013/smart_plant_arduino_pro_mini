#include "SmartPlantApp.h"

#include <Arduino.h>

#include "SmartPlantConfig.h"

namespace
{
constexpr uint8_t DHT_TYPE = DHT22;
}

SmartPlantApp::SmartPlantApp()
    : display(U8G_I2C_OPT_NONE),
      dht(SmartPlantConfig::kDhtPin, DHT_TYPE),
      renderer(display)
{
  state.sensors.soil = SmartPlantConfig::kDefaultSoil;
}

void SmartPlantApp::setup()
{
  Serial.begin(9600);
  pinMode(SmartPlantConfig::kTouchPin, INPUT);
  dht.begin();

  if (!rtc.begin())
  {
    Serial.println("Couldn't find RTC");
    while (1)
    {
    }
  }

  if (SmartPlantConfig::kForceTimeSync || rtc.lostPower())
  {
    Serial.println(SmartPlantConfig::kForceTimeSync ? "Forcando sincronizacao do RTC..." : "RTC sem energia, ajustando horario...");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  delay(2000);
}

void SmartPlantApp::loop()
{
  updateTestMode();

  const unsigned long currentMillis = millis();
  updateSensors(currentMillis);
  updateTouchInput();
  renderCurrentScreen();

  delay(SmartPlantConfig::kFrameDelayMs);
}

void SmartPlantApp::updateTestMode()
{
  if (!SmartPlantConfig::kEnableEmoticonTest)
  {
    return;
  }

  state.testMode = (millis() / SmartPlantConfig::kTestModeStepMs) % 5;
}

void SmartPlantApp::updateSensors(unsigned long currentMillis)
{
  if (currentMillis - state.previousSensorMillis < SmartPlantConfig::kSensorIntervalMs)
  {
    return;
  }

  state.sensors.temperature = dht.readTemperature();
  state.sensors.humidity = dht.readHumidity();

  const int rawSoil = analogRead(SmartPlantConfig::kSoilPin);
  const int soilPercent = map(rawSoil,
                              SmartPlantConfig::kSoilDryValue,
                              SmartPlantConfig::kSoilWetValue,
                              0, 100);
  state.sensors.soil = constrain(soilPercent, 0, 100);

  state.previousSensorMillis = currentMillis;
}

void SmartPlantApp::updateTouchInput()
{
  state.touchState = digitalRead(SmartPlantConfig::kTouchPin);

  if (state.touchState != HIGH)
  {
    return;
  }

  state.currentScreenIndex++;

  if (state.currentScreenIndex >= SmartPlantConfig::kScreenCount)
  {
    state.currentScreenIndex = 0;
  }

  delay(SmartPlantConfig::kTouchDebounceMs);
}

void SmartPlantApp::renderCurrentScreen()
{
  const DateTime now = rtc.now();
  const PlantMood mood = resolveMood(now);
  const AnimationState animationState = buildAnimationState();

  display.firstPage();
  do
  {
    renderer.render(currentScreen(), state.sensors, now, mood, animationState);
  } while (display.nextPage());
}

AnimationState SmartPlantApp::buildAnimationState() const
{
  AnimationState animationState;
  animationState.frame = (millis() / SmartPlantConfig::kAnimationFrameMs) % 2;
  animationState.blink = (millis() / SmartPlantConfig::kBlinkFrameMs) % 12 == 0;
  return animationState;
}

PlantMood SmartPlantApp::resolveMood(const DateTime &now) const
{
  bool isNight = now.hour() > SmartPlantConfig::kSleepHour ||
                 (now.hour() == SmartPlantConfig::kSleepHour && now.minute() >= SmartPlantConfig::kSleepMinute);
  bool isDrySoil = state.sensors.soil < SmartPlantConfig::kDrySoilThreshold;
  bool isCold = state.sensors.temperature < SmartPlantConfig::kColdTemperature;
  bool isHot = state.sensors.temperature > SmartPlantConfig::kHotTemperature;

  if (SmartPlantConfig::kEnableEmoticonTest)
  {
    isNight = false;
    isDrySoil = false;
    isCold = false;
    isHot = false;

    if (state.testMode == 1)
      isNight = true;
    if (state.testMode == 2)
      isDrySoil = true;
    if (state.testMode == 3)
      isCold = true;
    if (state.testMode == 4)
      isHot = true;
  }

  if (isNight)
  {
    return PlantMood::Night;
  }

  if (isDrySoil)
  {
    return PlantMood::DrySoil;
  }

  if (isCold)
  {
    return PlantMood::Cold;
  }

  if (isHot)
  {
    return PlantMood::Hot;
  }

  return PlantMood::Happy;
}

ScreenId SmartPlantApp::currentScreen() const
{
  return static_cast<ScreenId>(state.currentScreenIndex);
}