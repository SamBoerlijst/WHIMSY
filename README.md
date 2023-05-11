# WHIMSY
An Arduino-based data-logger for meteorological data including: air pressure/altitude, gas, humidity, temperature, wind direction, wind speed. Data is collected with a specified interval, starting from the moment the device is powered and saving the measurements to a csv file on a SD card. 

## Parts
- Arduino nano
- SEN 08942 wind adenometer sensor[^1]
- SEN 08942 wind specometer sensor[^1]
- BME 280 or 680 weather sensor[^2]
- SD card reader
- SD card (up to 2GB FAT16)
- 10k resistor
- microUSB-USB connector
[^1]: Argent datasystems; Weather Sensor Assembly p/n 80422; https://www.argentdata.com/catalog/product_info.php?products_id=145
[^2]: Adafruit board using Bosch BME 260 or 680 gass sensor; https://www.bosch-sensortec.com/products/environmental-sensors/humidity-sensors-bme280/  or https://www.bosch-sensortec.com/products/environmental-sensors/gas-sensors/bme680/; 

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
* Wind direction (tertiary intercardinal directions[^3])
[^3]: "N", "NNE", "NE"...

## Specifications:
### Communication protocol: SPI
### Libraries: math, Wire, SD, SPI, LowPower, Adafruit_Sensor and Adafruit_BME680
### Battery-life: 1 week on a 20.000mAh powerbank; 0.019A/h
