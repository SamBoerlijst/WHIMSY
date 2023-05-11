# WHIMSY
An Arduino-based data-logger for meteorological data including: air pressure/altitude, gas, humidity, temperature, wind direction, wind speed. Data is collected with a specified interval, starting from the moment the device is powered and saving the measurements to a csv file on a SD card. 

## Parts
- Arduino nano
- SEN 08942 wind adenometer sensor
- SEN 08942 wind specometer sensor
- BME 680 weather sensor
- SD card reader
- SD card (up to 2GB FAT16)
- microUSB-USB connector

## Schematic
### pinout
_Check your controller's spec which pins have ADC or interrupt capabilities!_
* SD reader (GND, MISO, SC, MOSI, CS, 5V, 3V, GND | GND, D12, D13, D11, D10, 5V, NA, GND)
* BME680 (SD, GND, SCK, SDI, VCC, CS | NA, GND, A5, A4, 5V, NA)
* spedometer (1, 2, 3, 4 | GND, NA, NA, D3)
* anemometer (1, 4 | A2, GND)


## Output
* Temperature (*C)
* Pressure (hPa)
* Humidity(%)
* Gas (kOhms)
* Approximate altitude(m)
* Wind speed (m/s)
* Wind direction (tertiary intercardinal directions[^1])
[^1]: "N", "NNE", "NE"...

## Specifications:
### Communication protocol: SPI
### Libraries: math, Wire, SD, SPI, LowPower, Adafruit_Sensor and Adafruit_BME680
### Battery-life: 1 week on a 20.000mAh powerbank
