#include <ChainableLED.h>
#include <LiquidCrystal.h>

/*
 *  Declaration of the led object
 * @param PORT
 * @param PORT
 * @param LED_NUMBER : Number of led connected
 */
ChainableLED led(7, 8, 1);

/*
 *  Declaration of the led object
 * @param PORTS Connections ports of the LCD
 */
LiquidCrystal lcd(22, 23, 27, 26, 25, 24);

/*
 * Initialization of the led object
 */
void setupLED()
{
    led.init();
}

/*
 * Initialization of the LCD screen
 */
void setupLCD()
{
    lcd.begin(16, 2);
    lcd.setCursor(5, 0);
    lcd.print("**3W**");
    lcd.setCursor(0, 1);
    lcd.print("Initialization...");
}

/*
 * Turn on the led on green color
 * Show a message that the program is ready
*/
void hmiDisplayMode(String *mode)
{
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print("**3W**");
    lcd.setCursor(0, 1);
    lcd.print((*mode));
    if ((*mode) == "STANDARD")
    {
        led.setColorRGB(0, 0, 255, 0);
    }
    else if ((*mode) == "CONFIG")
    {

        led.setColorRGB(0, 255, 255, 0);
    }
    else if ((*mode) == "ECO")
    {
        led.setColorRGB(0, 0, 0, 255);
    }
    else if ((*mode) == "MAINTENANCE")
    {
        led.setColorRGB(0, 255, 0.489 * 255, 0);
    }
}

/*
 * Turn on the led switching between red and green light
 * Show en error message on the LCD
*/
void hmiDisplayErrorWithTempSensor()
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Err: TempSensor");
    lcd.setCursor(1, 1);
    lcd.print("Can't access");
    led.setColorRGB(0, 0, 255, 0);
    delay(500);
    led.setColorRGB(0, 255, 0, 0);
    delay(500);
}

void hmiDisplayErrorWithGPS()
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Err: GPS");
    lcd.setCursor(1, 1);
    lcd.print("Timeout !");
    led.setColorRGB(0, 255, 0, 0);
    delay(500);
    led.setColorRGB(0, 255, 255, 0);
    delay(500);
}

void hmiDisplayErrorWithSD(String error)
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Err: SD");
    lcd.setCursor(1, 1);
    lcd.print(error);
    while (1)
    {
        led.setColorRGB(0, 255, 255, 255);
        delay(2 / 3);
        led.setColorRGB(0, 255, 0, 0);
        delay(1 / 3);
    }
}