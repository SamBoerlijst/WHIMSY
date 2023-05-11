/**
 * Author: Sam Boerlijst
 * Date: 2-5-2021
 * Name: OHpact weather station
 *
 * provides functions to read out the sensors of a SEN 08942 "Wettermesseinheit":
 * - Wind Direction: analog sensor with 16 cardinal direction steps
 * - Wind Speed:     Interrupt counter
 *
 * provides functions to read out the sensors of a BME680:
 * - temperature
 * - barometer
 * - humidity
 * - gas
 * - altitude
 *
 * Check your controller's spec which pins have ADC or interrupt capabilities!
 *
 * Arduino nano pinout:
 * SD reader (GND, MISO, SC, MOSI, CS, 5V, 3V, GND | GND, D12, D13, D11, D10, 5V, NA, GND)
 * BME680 (SD, GND, SCK, SDI, VCC, CS | NA, GND, A5, A4, 5V, NA)
 * spedometer (1, 2, 3, 4 | GND, NA, NA, D3)
 * anemometer (1, 4 | A2, GND)
 * 
 * SD up to 2GB FAT16
 */

#include <math.h>
#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <LowPower.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10
#define Anemo A2
#define Spedometer 3
#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C

int speedCounter;
bool currentSpeedReading = LOW;
bool lastSpeedReading = LOW;
float Direction;
float metersPerSec;
//run every 2 seconds to initiate SD and sensors
long interval = 2000/2;

unsigned long lastResetSpeed = millis();
unsigned long previousMillis=0;
const int chipSelect = 10;
const unsigned int pinDirection, pinSpeed, directionOffset;
const unsigned int directionVolt[16]   = {320, 410, 450, 620, 900, 1190, 1400, 1980, 2250, 2930, 3080, 3430, 3840, 4040, 4620, 4780};
const unsigned int directionCardinal[16] = {113, 68,  90,  158, 135, 203,  180,  23,   45,   248,  225,  338,  0,    292,  315,  270 };
const unsigned int directionDegree[16] = {"0", "22.5", "45", "67.5", "90", "112.5", "135", "157.5", "180", "202.5", "225", "247.5", "270", "292.5", "315", "337.5"
};

void WEATHER_speedInterruptHandler () {
    currentSpeedReading = digitalRead(3);
    if (lastSpeedReading == LOW && currentSpeedReading == HIGH){
        speedCounter++;}
    lastSpeedReading = currentSpeedReading;
}

void setup () {
    Serial.begin(9600);
    while (!Serial);
    Serial.println("BME280 test");
    unsigned status;

    status = bme.begin();  
    // You can also pass in a Wire library object like &Wire2
    // status = bme.begin(0x76, &Wire2)
    if (!status) {
//      Serial.println("Could not find a valid BME280 sensor, check wiring!");
      while (1);
    }
    
    //Check for SD presence
    initializeSD();
    //Set up oversampling and filter initialization
    pinMode(Anemo, INPUT);
    pinMode(Spedometer, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(3), WEATHER_speedInterruptHandler, CHANGE); //reset after hour

    getWindDir();
    getWindSpeed();
    // Create logfile if non existant 
    createLOG();

    //set clockspeed multiplier to half to save power
    CLKPR = 0x80;
    CLKPR = 0x02;
}

void loop(){
    unsigned long currentMillis = millis(); //get snapshot of current time
    if ((unsigned long)(currentMillis - previousMillis) >= interval){
    getWindSpeed();
    getWindDir();
    WriteLOG();
    //set interval to 1 hour divided by clock speed 3600000/2
    interval = 3600000/2;
    previousMillis = currentMillis;  //set snapshot as offset to track time until next event
}}

void getWindDir () {
    unsigned int mvolt = analogRead(A2);
    mvolt = map(mvolt, 0, 1023, 0, 5000);  //map to range 0-5000 to make use of voltage values as defined in data sheet

    //search for correct index
    unsigned char i;
    for (i = 0; i < 16; i++) {
        if (mvolt <= directionVolt[i]) break;
    }
    i--;  //the correct reference is actually lower than the reference value found.
    Direction =  ((directionCardinal[i] + 0) % 360);
}

void getWindSpeed () {
    unsigned long Now = millis();
    double secondsPassed = (Now - lastResetSpeed) / 1000;  //overflows after 49 days
    //one impulse per second equals 2.4 km/h -> 0.6666 m/s
    metersPerSec = (2 * speedCounter) / (3 * max(secondsPassed, 1.0));
    speedCounter = 0;
    lastResetSpeed = Now;
}

void initializeSD(){
  //check for SD presence
  if (SD.begin(chipSelect))
    {
//    Serial.println("SD card is present & ready");
  } 
  else
    {
//    Serial.println("SD card missing or failure");
    while(1);  //wait here forever
  }
}

void createLOG(){
    //create file and banner
    if (SD.exists("LOG.txt")) {
      // example.txt exists
//      Serial.println("LOG.txt available");
    }
    else {
       //example.txt doesn't exist
//    Serial.println("LOG.txt not available");
    if (SD.exists("LOG.txt")) {}
    else {
      File dataFile = SD.open("LOG.txt", FILE_WRITE);
      dataFile.print("Temperature_*C");
      dataFile.print("\t");
      dataFile.print("Pressure_hPa");
      dataFile.print("\t");
      dataFile.print("Humidity_%");
      dataFile.print("\t");
      dataFile.print("Approx._Altitude_m");
      dataFile.print("\t");
      dataFile.print("Wind_speed_m/s");
      dataFile.print("\t");
      dataFile.print("wind_direction_cardinal");
      dataFile.println();
      dataFile.close();
    } 
//    Serial.println("file created");
  }
}

void WriteLOG(){
    File dataFile = SD.open("LOG.txt", FILE_WRITE);
    if (dataFile) {
      dataFile.print(bme.readTemperature());
      dataFile.print("\t");
      dataFile.print(bme.readPressure() / 100.0F);
      dataFile.print("\t");
      dataFile.print(bme.readHumidity());
      dataFile.print("\t");
      dataFile.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
      dataFile.print("\t");
      dataFile.print(metersPerSec);
      dataFile.print("\t");
      dataFile.print(Direction);
      dataFile.println();
      dataFile.close();
     }
     else {
//      Serial.println("LOG.txt not available");
     }
}
