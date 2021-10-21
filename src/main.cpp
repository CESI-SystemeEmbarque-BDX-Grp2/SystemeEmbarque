#include <Arduino.h>
#include <tempSensor.h>
#include <gps.h>
#include <ihm.h>
#include <horloge.h>
#include <config.h>
#include <sdCard.h>
#include "EEPROM.h"

#define RED_BUTTON 18
#define GREEN_BUTTON 19
#define LIGHT_SENSOR A0
// Interval as a button is considered to be pressed as a long time
#define THRESHOLD 3000

String mode = "";
String lastMode = "";

CapteurTemperature capteurTemperature;
CapteurGPS capteurGPS;
Horloge horloge;
Parameters settings;

void redButtonInterrupt()
{
    static unsigned long temps = 0;

    if (temps == 0)
    {
        // Lorsque le bouton est appuyé :
        temps = millis();
    }
    else
    {
        if ((millis() - temps) < 10)
        {
            // Si un rebond est détécté sur le bouton
        }
        else
        {
            if ((millis() - temps) > 5000)
            {
                //instruction appuie long
                Serial.println("Appui long !");
                if (mode == "STANDARD" || mode == "ECO")
                {
                    lastMode = mode;
                    mode = "MAINTENANCE";
                }
                else if (mode == "MAINTENANCE")
                {
                    mode = lastMode;
                    lastMode = "MAINTENANCE";
                }
                else if (mode == "CONFIG")
                {
                    mode = "STANDARD";
                }
                temps = 0;
                hmiDisplayMode(&mode);
            }
            else
            {
                //instruction appuie court
                Serial.println("Appui court !");
                temps = 0;
            }
        }
    }
}

void greenButtonInterrupt()
{
    static unsigned long temps = 0;

    if (temps == 0)
    {
        // Lorsque le bouton est appuyé :
        temps = millis();
    }
    else
    {
        if ((millis() - temps) < 10)
        {
            // Si un rebond est détécté sur le bouton
        }
        else
        {
            if ((millis() - temps) > 5000)
            {
                //instruction appuie long
                Serial.println("Appui long !");
                if (mode == "STANDARD")
                {
                    mode = "ECO";
                    lastMode = "STANDARD";
                }
                else if (mode == "ECO")
                {
                    lastMode = "ECO";
                    mode = "STANDARD";
                }
                temps = 0;
                hmiDisplayMode(&mode);
            }
            else
            {
                //instruction appuie court
                Serial.println("Appui court !");
                temps = 0;
            }
        }
    }
}

void standardMode()
{
    // Serial.println("DEBUG : Standard mode");
    // Millis fucntion return an unsigned long
    static unsigned long waitTimestart = millis();

    int waitTime = (settings.LOG_INTERVAL * 60 * 1000);

    if (mode == "ECO")
    {
        waitTime *= 2;
    }
    //
    if ((millis() - waitTimestart) > waitTime)
    {
        String data = "";

        // First we start by setting the date :
        data += getTime();
        data += ";";

        //Secondly we add the temperature, pressure and humidity information :
        getInformationFromTempSensor(&capteurTemperature);
        unsigned long counter = millis();
        // Looking if receiving data from the sensor
        while (isnan(capteurTemperature.temperature) || isnan(capteurTemperature.pressure) || isnan(capteurTemperature.humidity))
        {
            if (millis() - counter > settings.TIMEOUT)
            {
                if (capteurTemperature.error == 2)
                {
                    hmiDisplayErrorWithTempSensor();
                }
                else
                {
                    capteurTemperature.error += 1;
                }
            }
        }

        // Adding data to the string
        int lightValue = analogRead(LIGHT_SENSOR);
        lightValue = map(lightValue, 0, 800, 0, 100);
        data += String(lightValue);
        data += "%;";
        data += String(capteurTemperature.temperature, 2);
        data += "°C;";
        data += String(capteurTemperature.pressure, 2);
        data += "Pa;";
        data += String(capteurTemperature.humidity, 2);
        data += "%;";

        if (mode != "ECO")
        {
            getPosition(&capteurGPS);
            counter = millis();
            // Adding GPS data
            while (!capteurGPS.gpsData)
            {
                if (millis() - counter > settings.TIMEOUT)
                {
                    if (capteurGPS.error == 2)
                    {
                        hmiDisplayErrorWithGPS();
                    }
                    else
                    {
                        capteurGPS.error += 1;
                    }
                }
            }

            data += capteurGPS.gpsData;
        }
        else
        {
            if (capteurGPS.mesure)
            {
                getPosition(&capteurGPS);
                counter = millis();
                // Adding GPS data
                while (!capteurGPS.gpsData)
                {
                    if (millis() - counter > settings.TIMEOUT)
                    {
                        if (capteurGPS.error == 2)
                        {
                            hmiDisplayErrorWithGPS();
                        }
                        else
                        {
                            capteurGPS.error += 1;
                        }
                    }
                }
                data += capteurGPS.gpsData;
            }
            capteurGPS.mesure = !capteurGPS.mesure;
        }

        Serial.println(data);

        setNow(&horloge);
        writeData(data, &horloge, &settings);

        // Reset timer if we dont want a loop :)
        waitTimestart = millis();
    }
}

void resetSerial()
{
    while (Serial.available())
    {
        Serial.parseInt();
    }
}

void configMode()
{
    hmiDisplayMode(&mode);
    Serial.println("Veuillez choisir le paramètre à modifier");
    Serial.println("0 : LOG_INTERVAL");
    Serial.println("1 : FILE_MAX_SIZE");
    Serial.println("2 : RESET");
    Serial.println("3 : VERSION");
    Serial.println("4 : TIMEOUT");
    Serial.println("5 : LUMIN");
    Serial.println("6 : TEMP_AIR");
    Serial.println("7 : HYGR");
    Serial.println("8 : PRESSURE");
    Serial.println("9 : DATE_TIME");

    while (Serial.available() <= 0)
    {
    }

    int var = Serial.parseInt();
    Serial.println(var);

    Serial.print("Vous voulez changer : ");

    if (var == 0)
    {
        Serial.println("LOG_INTERVAL");
        Serial.println("Quel intervalle entre deux mesures voulez vous mettre");
        resetSerial();
        while (Serial.available() <= 0)
        {
        }
        settings.LOG_INTERVAL = Serial.parseInt();
        EEPROM.put(0, settings.LOG_INTERVAL);
        Serial.print("Le nouvel intervalle est de : ");
        Serial.println(settings.LOG_INTERVAL);
    }
    else if (var == 1)
    {
        Serial.println("FILE_MAX_SIZE");
        Serial.println("Quel taille de fichier maximum voulez vous mettre");
        resetSerial();
        while (Serial.available() <= 0)
        {
        }
        settings.FILE_MAX_SIZE = Serial.parseInt();
        EEPROM.put(2, settings.FILE_MAX_SIZE);
        Serial.print("La nouvelle taille de fichier max est de : ");
        Serial.println(settings.FILE_MAX_SIZE);
    }
    else if (var == 2)
    {
        Serial.println("RESET");
        reset();
    }
    else if (var == 3)
    {
        Serial.println("VERSION");
    }
    else if (var == 4)
    {
        Serial.println("TIMEOUT");
        Serial.println("A partir de quelle durée en secondes voulez vous abandonner l'acquisition des données");
        resetSerial();
        while (Serial.available() <= 0)
        {
        }
        settings.TIMEOUT = Serial.parseInt();
        EEPROM.put(4, settings.TIMEOUT);
        Serial.print("Le nouveau timeout est de : ");
        Serial.println(settings.TIMEOUT);
    }
    else if (var == 5)
    {
        Serial.println("LUMIN");
        Serial.println("Quel LUMIN voulez vous modifier :");
        Serial.println("1 : LUMIN_LOW");
        Serial.println("2 : LUMIN_HIGH");
        resetSerial();
        while (Serial.available() <= 0)
        {
        }
        int lumin_type = Serial.parseInt();
        Serial.print("Vous avez sélectionné : ");
        if (lumin_type == 1)
        {
            Serial.println("LUMIN_LOW");
            Serial.println("A quelle valeur voulez vous paramétrer LUMIN_LOW?");
            resetSerial();
            while (Serial.available() <= 0)
            {
            }
            settings.LUMIN_LOW = Serial.parseInt();
            EEPROM.put(7, settings.LUMIN_LOW);
            Serial.print("La nouvelle valeur de LUMIN_LOW est de : ");
            Serial.println(settings.LUMIN_LOW);
        }
        if (lumin_type == 2)
        {
            Serial.println("LUMIN_HIGH");
            Serial.println("A quelle valeur voulez vous paramétrer LUMIN_HIGH?");
            resetSerial();
            while (Serial.available() <= 0)
            {
            }
            settings.LUMIN_HIGH = Serial.parseInt();
            EEPROM.put(9, settings.LUMIN_HIGH);
            Serial.print("La nouvelle valeur de LUMIN_HIGH est de : ");
            Serial.println(settings.LUMIN_HIGH);
        }
    }
    else
    {
        Serial.println("Pas encore implémenté");
    }
}

void checkMode()
{
    hmiDisplayMode(&mode);
    // Serial.println("*******************************************************************************************************************");
    // Serial.println("██████╗ ██╗    ██╗    ███╗   ███╗ █████╗ ██╗███╗   ██╗████████╗███████╗███╗   ██╗ █████╗ ███╗   ██╗ ██████╗███████╗");
    // Serial.println("╚════██╗██║    ██║    ████╗ ████║██╔══██╗██║████╗  ██║╚══██╔══╝██╔════╝████╗  ██║██╔══██╗████╗  ██║██╔════╝██╔════╝");
    // Serial.println(" █████╔╝██║ █╗ ██║    ██╔████╔██║███████║██║██╔██╗ ██║   ██║   █████╗  ██╔██╗ ██║███████║██╔██╗ ██║██║     █████╗  ");
    // Serial.println(" ╚═══██╗██║███╗██║    ██║╚██╔╝██║██╔══██║██║██║╚██╗██║   ██║   ██╔══╝  ██║╚██╗██║██╔══██║██║╚██╗██║██║     ██╔══╝  ");
    // Serial.println("██████╔╝╚███╔███╔╝    ██║ ╚═╝ ██║██║  ██║██║██║ ╚████║   ██║   ███████╗██║ ╚████║██║  ██║██║ ╚████║╚██████╗███████╗");
    // Serial.println("╚═════╝  ╚══╝╚══╝     ╚═╝     ╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═══╝   ╚═╝   ╚══════╝╚═╝  ╚═══╝╚═╝  ╚═╝╚═╝  ╚═══╝ ╚═════╝╚══════╝");
    // Serial.println("*******************************************************************************************************************");

    Serial.println("DEBUG : Maintenance");

    Serial.println(getTime());
    getInformationFromTempSensor(&capteurTemperature);
    Serial.print("  - Temperature : ");
    Serial.print(capteurTemperature.temperature);
    Serial.println("°C");

    Serial.print("  - Pression : ");
    Serial.print(capteurTemperature.pressure);
    Serial.println("Pa");

    Serial.print("  - Humidité : ");
    Serial.print(capteurTemperature.humidity);
    Serial.println("%");

    Serial.print("  - Luminosité : ");
    int luminosity = analogRead(LIGHT_SENSOR);
    Serial.print(luminosity);
    Serial.print(" / ");
    luminosity = map(luminosity, 0, 800, 0, 100);
    Serial.print(luminosity);
    Serial.println("%");

    Serial.print("  - Position : ");
    getPosition(&capteurGPS);
    Serial.println(capteurGPS.gpsData);
    delay(1000);
    if (mode == "STANDARD")
    {
        loop();
    }
}

void setup()
{
    Serial.begin(9600);

    pinMode(RED_BUTTON, INPUT_PULLUP);
    pinMode(GREEN_BUTTON, INPUT_PULLUP);

    // Initialization of all the components
    setupLED();
    setupLCD();
    setupVM335Sensor();
    setupGPS();
    setupClock(2021, 10, 20, 3, 13, 34, 0);

    setNow(&horloge);
    setupSD(&horloge);

    initParameters(&settings);

    if (!digitalRead(RED_BUTTON))
    {
        mode = "CONFIG";
        configMode();
    }
    else
    {
        mode = "STANDARD";
    }

    lastMode = mode;

    // Add interruption for the two buttons
    attachInterrupt(digitalPinToInterrupt(RED_BUTTON), redButtonInterrupt, CHANGE);
    attachInterrupt(digitalPinToInterrupt(GREEN_BUTTON), greenButtonInterrupt, CHANGE);

    // Show that the system is ready with the ihm
    hmiDisplayMode(&mode);
}

void loop()
{
    if (mode == "STANDARD" || mode == "ECO")
    {
        standardMode();
    }
    else if (mode == "MAINTENANCE")
    {
        checkMode();
    }
    else if (mode == "CONFIG")
    {
        configMode();
    }
}
