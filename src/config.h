bool reset();

typedef struct
{
    int LOG_INTERVAL;
    int FILE_MAX_SIZE;
    int TIMEOUT;
    bool LUMIN;
    int LUMIN_LOW;
    int LUMIN_HIGH;
    bool TEMP_AIR;
    int MIN_TEMP_AIR;
    int MAX_TEMP_AIR;
    bool HYGR;
    int HYGR_MINT;
    int HYGR_MAXT;
    bool PRESSURE;
    int PRESSURE_MIN;
    int PRESSURE_MAX;
} Parameters;

void initParameters(Parameters *params);
void showCurrentSettings();

void setLogInterval(String *param, Parameters *settings);
void setFileMaxSize(String *param, Parameters *settings);
void setTimeout(String *param, Parameters *settings);
void setLUMIN(String *param, Parameters *settings);
void setLUMINLow(String *param, Parameters *settings);