typedef struct
{
    float temperature;
    float pressure;
    float humidity;
    int error;
} CapteurTemperature;
/*
 * Function for sensor initialization
*/
void setupVM335Sensor();

/*
 * Function for getting temperature value
 * @param sensor : Structure for returning data values
*/
void getInformationFromTempSensor(CapteurTemperature *sensor);