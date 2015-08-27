# PoolMonitor_Particle

Reused my previous code from https://github.com/mv740/PoolMonitor but on a particule board

##important
include these two library in your app 
- "OneWire/OneWire.h" : https://github.com/Hotaman/OneWireSpark
- "Adafruit_ST7735/Adafruit_ST7735.h" : https://github.com/adafruit/Adafruit-ST7735-Library

## description
- read the temperature from a DS18B20 sensor
- ouput the value on a 1.8 ST7335 LCD
- publish the value on the particule cloud
  - you can fetch this value using get request :   [GET DOCUMENTATION ](https://docs.particle.io/reference/firmware/photon/#spark-variable-)
