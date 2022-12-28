#ifndef POUCH_HEATER_h
 #define POUCH_HEATER_h

 #define POUCH_HEATER_1 0
 #define POUCH_HEATER_2 1

 #define POUCH_TEMP_1 0 
 #define POUCH_TEMP_2 1


 #define POUCH_THERMISTOR_1_ON  true
 #define POUCH_THERMISTOR_2_ON  true

 extern int16_t pouchHeater1_set_temp;
 extern int16_t pouchHeater2_set_temp;

 extern int8_t pouchHeater1_current_temp;
 extern int8_t pouchHeater2_current_temp;

 extern bool pouchHeater1_on;
 extern bool pouchHeater2_on;

 extern bool isHeating;

 void pouchHeater_Init();
 void manage_pouchHeater();
 float analog2temp(uint8_t);

#endif