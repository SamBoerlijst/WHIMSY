#include <SD.h>
const int chipSelect = 10;

void setup () {
 Serial.begin(9600);
 //check for SD presence
 if (SD.begin(chipSelect))
    {
    Serial.println("SD card is present & ready");
    } 
    else
    {
    Serial.println("SD card missing or failure");
    while(1);  //wait here forever
    }
 
 // Check to see if the file exists: 
 if (SD.exists("LOG.txt")) {
    // example.txt exists
    Serial.println("LOG.txt available");
 }
 else {
    // example.txt doesn't exist
    Serial.println("LOG.txt not available");
    if (SD.exists("LOG.txt")) {}
    else {
      File dataFile = SD.open("LOG.txt", FILE_WRITE);
      dataFile.print("Temperature_*C");
      dataFile.print("\t");
      dataFile.print("Pressure_hPa");
      dataFile.print("\t");
      dataFile.print("Humidity_%");
      dataFile.print("\t");
      dataFile.print("Gas_KOhms");
      dataFile.print("\t");
      dataFile.print("Approx._Altitude_m");
      dataFile.print("\t");
      dataFile.print("Wind_speed_m/s");
      dataFile.print("\t");
      dataFile.print("wind_direction_cardinal");
      dataFile.println();
      dataFile.close();
    } 
    Serial.println("file created");
  }}

void loop () {
  //none
  }
