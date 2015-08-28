# PoolMonitor_Particle

Reused my previous code from https://github.com/mv740/PoolMonitor but on a [particle board](https://store.particle.io/?product=particle-photon)

##important
include these two library in your app 
- "OneWire/OneWire.h" : https://github.com/Hotaman/OneWireSpark
- "Adafruit_ST7735/Adafruit_ST7735.h" : https://github.com/adafruit/Adafruit-ST7735-Library

## description
- read the temperature from a DS18B20 sensor
- ouput the value on a 1.8 ST7335 LCD
- publish the value on the particule cloud
  - you can fetch this value using get request :   [GET DOCUMENTATION ](https://docs.particle.io/reference/firmware/photon/#spark-variable-)

## Connections  
| TFT LCD      | Particle  |
|:------------:|:---------:|
| VCC          |   3.3V    |
| GND          |   GND     |
| SCL          |   A3      |
| SDA          |   A5      |
| PS/DC        |   D6       |
| RES          |   D2       |
| CS           |   A3      |

| BS18B20     | Ardruino  |
|:----------:|:---------:|
| Red        | 3.3V      |
| Black      | GND       |
| White      | D0         |

**4.7K resistor between white and red**
