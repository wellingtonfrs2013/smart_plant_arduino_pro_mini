#pragma once

namespace SmartPlantConfig
{
constexpr bool kEnableEmoticonTest = false;

// Defina como true para forçar a sincronização do RTC na próxima gravação.
// Após gravar, altere para false e grave novamente para travar o horário correto.
constexpr bool kForceTimeSync = false;

constexpr int kTouchPin = 4;
constexpr int kDhtPin = 2;
constexpr int kSoilPin = A0;

// Calibração do sensor capacitivo v2 (ajuste conforme seu sensor):
// Leitura no ar (seco) e submerso em água (úmido)
constexpr int kSoilDryValue  = 519;  // ADC em ar seco (calibrado)
constexpr int kSoilWetValue  = 310;  // ADC em água (pendente calibração)

constexpr int kColdTemperature = 18;
constexpr int kHotTemperature = 30;
constexpr int kDrySoilThreshold = 35;
constexpr int kSleepHour = 20;
constexpr int kSleepMinute = 30;

constexpr int kDefaultSoil = 60;
constexpr unsigned long kSensorIntervalMs = 2000;
constexpr unsigned long kTouchDebounceMs = 500;
constexpr unsigned long kFrameDelayMs = 50;
constexpr unsigned long kAnimationFrameMs = 400;
constexpr unsigned long kBlinkFrameMs = 250;
constexpr unsigned long kTestModeStepMs = 5000;
constexpr int kScreenCount = 3;
}