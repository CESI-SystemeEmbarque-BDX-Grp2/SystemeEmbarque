typedef struct
{
    String gpsData;
    bool mesure = true;
    int error = 0;
} CapteurGPS;

void getPosition(CapteurGPS *capteurGPS);
void setupGPS();
