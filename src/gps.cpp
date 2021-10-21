#include <SoftwareSerial.h>
#include <Arduino.h>

SoftwareSerial SoftSerial(10, 11);

typedef struct
{
    String gpsData;
    bool mesure = true;
    int error = 0;
} CapteurGPS;

void setupGPS()
{
    SoftSerial.begin(9600);
}

void getPosition(CapteurGPS *capteurGPS)
{
    bool t;
    // GPS Reading
    capteurGPS->gpsData = "";
    if (SoftSerial.available()) // if data is coming from software serial port ==> data is coming from SoftSerial GPS
    {
        t = true;
        while (t)
        {
            capteurGPS->gpsData = SoftSerial.readStringUntil('\n');
            if (capteurGPS->gpsData.startsWith("$GPGGA", 0))
            {
                t = false;
                break;
            }
        }
    }
}