#include <BME280I2C.h>
#include <Arduino.h>
#include <Wire.h>
#include <ihm.h>

BME280I2C bme;
BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
BME280::PresUnit presUnit(BME280::PresUnit_Pa);

typedef struct
{
    float temperature;
    float pressure;
    float humidity;
    int error = 0;
} CapteurTemperature;

void setupVM335Sensor()
{
    Wire.begin();
    while (!bme.begin())
    {
        Serial.println("Sensor could not be initialized");
        hmiDisplayErrorWithTempSensor();
    }

    switch (bme.chipModel())
    {
    case BME280::ChipModel_BME280:
        Serial.println("Found BME280 sensor! Success.");
        break;
    case BME280::ChipModel_BMP280:
        Serial.println("Found BMP280 sensor! No Humidity available.");
        break;
    default:
        Serial.println("Found UNKNOWN sensor! Error!");
    }
}

void getInformationFromTempSensor(CapteurTemperature *sensor)
{
    bme.read((*sensor).pressure, (*sensor).temperature, (*sensor).humidity, tempUnit, presUnit);
}
