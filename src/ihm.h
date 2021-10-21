void setupLED();
void setupLCD();

void hmiDisplayMode(String *mode);

/* Declaration of all possible errors */

/*
 * Turn on the led switching between red and green light
 * Show en error message on the LCD
*/
void hmiDisplayErrorWithTempSensor();
void hmiDisplayErrorWithGPS();
void hmiDisplayErrorWithSD(String error);