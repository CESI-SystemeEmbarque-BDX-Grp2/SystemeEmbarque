#include <EEPROM.h>
#include <Arduino.h>

/**
    Nom	            Type	Taille 	Adresse	    Valeur défaut
    LOG_INTERVALL	int	        2	    0	        10
    FILE_MAX_SIZE	int	        2	    2	        4096
    TIMEOUT	        int	        2	    4	        30
    LUMIN	        bool	    1	    6	        1
    LUMIN_LOW	    int	        2	    7	        255
    LUMIN_HIGH	    int	        2	    9	        768
    TEMP_AIR	    bool	    1	    11      	1
    MIN_TEMP_AIR	int	        2	    12      	-10
    MAX_TEMP_ AIR	int	        2	    14	        60
    HYGR	        bool    	1	    16	        1
    HYGR_MINT	    int	        2	    17	        0
    HYGR_MAXT	    int	        2	    19      	50
    PRESSURE	    bool    	1	    21      	1
    PRESSURE_MIN	int	        2	    22      	850
    PRESSURE_MAX	int	        2	    24      	1080

**/

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

void initParameters(Parameters *params)
{
    EEPROM.get(0, params->LOG_INTERVAL);
    EEPROM.get(2, params->FILE_MAX_SIZE);
    EEPROM.get(4, params->TIMEOUT);
    EEPROM.get(6, params->LUMIN);
    EEPROM.get(7, params->LUMIN_LOW);
    EEPROM.get(9, params->LUMIN_HIGH);
    EEPROM.get(11, params->TEMP_AIR);
    EEPROM.get(12, params->MIN_TEMP_AIR);
    EEPROM.get(14, params->MAX_TEMP_AIR);
    EEPROM.get(16, params->HYGR);
    EEPROM.get(17, params->HYGR_MINT);
    EEPROM.get(19, params->HYGR_MAXT);
    EEPROM.get(21, params->PRESSURE);
    EEPROM.get(22, params->PRESSURE_MIN);
    EEPROM.get(24, params->PRESSURE_MAX);
}

/*
void showCurrentSettings()
{
    Serial.println("Nom	            Type       Taille 	  Adresse	        Valeur défaut");
    Serial.println("LOG_INTERVALL     int           2	    0	                  10");
    Serial.println("FILE_MAX_SIZE	  int           2	    2	                4096");
    Serial.println("TIMEOUT	          int           2	    4	                  30");
    Serial.println("LUMIN	          bool	        1       6	                   1");
    Serial.println("LUMIN_LOW	      int           2	    7	                 255");
    Serial.println("LUMIN_HIGH	      int           2	    9	                 768");
    Serial.println("TEMP_AIR	      bool	        1	    11      	           1");
    Serial.println("MIN_TEMP_AIR	  int           2	    12      	         -10");
    Serial.println("MAX_TEMP_ AIR	  int           2	    14	                  60");
    Serial.println("HYGR	          bool    	    1	    16	                   1");
    Serial.println("HYGR_MINT	      int           2	    17	                   0");
    Serial.println("HYGR_MAXT         int           2	    19       	          50");
    Serial.println("PRESSURE          bool    	    1	    21      	           1");
    Serial.println("PRESSURE_MIN      int           2	    22      	         850");
    Serial.println("PRESSURE_MAX      int           2	    24      	        1080");
}
*/

/**
 * Function to reset parameters to default values
 * @return Return if reset is successful
 **/
bool reset()
{
    EEPROM.put(0, 10);
    EEPROM.put(2, 2000);
    EEPROM.put(4, 30);
    EEPROM.put(6, 1);
    EEPROM.put(7, 255);
    EEPROM.put(9, 768);
    EEPROM.put(11, 1);
    EEPROM.put(12, -10);
    EEPROM.put(14, 60);
    EEPROM.put(16, 1);
    EEPROM.put(17, 0);
    EEPROM.put(19, 50);
    EEPROM.put(21, 1);
    EEPROM.put(22, 850);
    EEPROM.put(24, 1080);
    return true;
}
