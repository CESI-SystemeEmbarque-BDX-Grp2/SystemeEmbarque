#include <Arduino.h>
#include <SPI.h>
#include <horloge.h>
#include "SdFat.h"
#include <Wire.h>
#include <ihm.h>
#include "config.h"

SdFat sd;
File file;
int i = 0;
char fileNameC[15];
String tempName;

void setupSD(Horloge *clock)
{
    sd.begin(4, SD_SCK_HZ(4));

    if (sd.card()->errorCode())
    {
        hmiDisplayErrorWithSD("SD access err.");
    }

    tempName += String(clock->year);
    tempName += String(clock->month);
    tempName += String(clock->day, DEC);
    tempName += "_";
    tempName += i;
    tempName += ".csv";
    tempName.toCharArray(fileNameC, sizeof(fileNameC));

    if (file.open(fileNameC, O_WRONLY | O_CREAT))
    {
        Serial.println("Fichier ouvert !");
    }
    else
    {
        Serial.println("Error oppening file");
    }
}

void writeData(String data, Horloge *clock, Parameters *settings)
{
    Serial.println("**********************");

    if (file.size() < settings->FILE_MAX_SIZE)
    {
        if (file.size() == 0)
        {
            file.println("Date; Heure; Luminosite; Temperature; Pression; Humidite; Position");
            file.flush();
        }
        else
        {
            file.println(data);
            file.flush();
        }
    }
    else
    {
        while (!file.rename(fileNameC))
        {
            i++;
            tempName = "";
            tempName += String(clock->year);
            tempName += String(clock->month);
            tempName += String(clock->day, DEC);
            tempName += "_";
            tempName += i;
            tempName += ".csv";
            tempName.toCharArray(fileNameC, 15);
            Serial.println(tempName);
        }
        Serial.println("File renamed");
        file.close();
        setupSD(clock);
        writeData(data, clock, settings);
    }
}