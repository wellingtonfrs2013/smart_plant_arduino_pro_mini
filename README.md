# 🌱 Tamagochi Smart Plant

Planta virtual estilo Tamagotchi com monitoramento real de ambiente. Exibe emoções animadas no display OLED conforme as condições da planta — temperatura, umidade do ar, umidade do solo e horário.

## Hardware

| Componente | Descrição |
|---|---|
| Arduino Pro Mini | ATmega328P, 5V, 16MHz |
| OLED SSD1306 128x64 | Display via I2C |
| DS3231 | RTC via I2C |
| DHT22 | Sensor de temperatura e umidade do ar |
| Capacitive Soil Sensor v2 | Sensor de umidade do solo (analógico) |
| Touch Sensor | Botão capacitivo para navegar entre telas |

## Pinout

| Pino | Componente |
|---|---|
| D2 | DHT22 (dados) |
| D4 | Touch sensor |
| A0 | Sensor de solo capacitivo v2 |
| A4 (SDA) | OLED + DS3231 |
| A5 (SCL) | OLED + DS3231 |

## Telas

Navegue entre as três telas tocando no sensor touch:

1. **Emoticon** — animação da planta com emoção atual
2. **Data/Hora** — hora, data e dia da semana (RTC DS3231)
3. **Sensores** — temperatura, umidade do ar e umidade do solo

## Emoções

| Emoção | Condição |
|---|---|
| 😊 Feliz | Todas as condições normais |
| 😴 Dormindo | Após 20:30 |
| 😢 Solo seco | Umidade do solo < 35% |
| 🥶 Frio | Temperatura < 18°C |
| 🥵 Calor | Temperatura > 30°C |

## Dependências

```ini
adafruit/RTClib@^2.1.4
marlinfirmware/U8glib-HAL@^0.5.5
adafruit/DHT sensor library@^1.4.7
```

## Estrutura do Projeto

```
SmartPlantProMini/
├── include/
│   ├── SmartPlantApp.h       # Classe principal da aplicação
│   ├── SmartPlantConfig.h    # Constantes e configurações
│   ├── SmartPlantDisplay.h   # Classe de renderização OLED
│   └── SmartPlantTypes.h     # Tipos compartilhados (enums, structs)
└── src/
    ├── main.cpp              # Entry point Arduino
    ├── SmartPlantApp.cpp     # Lógica principal
    └── SmartPlantDisplay.cpp # Desenho no display
```

## Configuração

Todas as configurações estão em `include/SmartPlantConfig.h`:

```cpp
kColdTemperature  = 18    // °C — limiar de frio
kHotTemperature   = 30    // °C — limiar de calor
kDrySoilThreshold = 35    // % — solo considerado seco
kSleepHour        = 20    // hora de dormir
kSleepMinute      = 30    // minuto de dormir
```

### Calibração do sensor de solo

Com o sensor conectado ao A0, ajuste os valores conforme sua leitura:

```cpp
kSoilDryValue = 519   // ADC lido com sensor no ar
kSoilWetValue = 310   // ADC lido com sensor na água
```

### Sincronizar o RTC

Se o horário ficar errado após uma nova gravação, force a sincronização:

1. Altere `kForceTimeSync = true` em `SmartPlantConfig.h`
2. Grave o firmware (o RTC será ajustado para a hora de compilação)
3. Altere de volta para `kForceTimeSync = false` e grave novamente

## Build & Upload

```bash
# Compilar
pio run

# Gravar
pio run --target upload
```
