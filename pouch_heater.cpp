#include <Arduino.h>
#include "Marlin.h"
#include "serial.h"
#include "pins_RAMPS_13.h"

#include "pouch_heater.h"
#include "VL6180X_sensor.h"
#include "peristaltic_pump.h"
#include "thermistortables.h"


#define PGM_RD_W(x)   (short)pgm_read_word(&x)

int16_t pouchHeater1_set_temp = 25;
int8_t pouchHeater1_current_temp;

int16_t pouchHeater2_set_temp = 25;
int8_t pouchHeater2_current_temp;

bool pouchHeater1_on = false;
bool pouchHeater2_on = false;

bool isHeating = false;


unsigned long previous_millis_heat=0;
unsigned long wait_heat_time = 100;    //we only need to check the heater 10 times a second


void pouchHeater_Init()
{
  // init heater
  if(POUCH_HEATER_1_PIN > -1) pinMode(POUCH_HEATER_1_PIN, OUTPUT);
  if(POUCH_HEATER_2_PIN > -1) pinMode(POUCH_HEATER_2_PIN, OUTPUT);

  // init temparature sensor
  if(POUCH_TEMP_1_PIN > -1) pinMode(POUCH_TEMP_1_PIN, INPUT);
  if(POUCH_TEMP_2_PIN > -1) pinMode(POUCH_TEMP_2_PIN, INPUT);

  digitalWrite(POUCH_HEATER_1_PIN, LOW); //turn off
  digitalWrite(POUCH_HEATER_2_PIN, LOW); //turn off
}



void manage_pouchHeater()
{
  if( (millis()-previous_millis_heat) >  wait_heat_time )    //we only need to check the heater 10 times a second  
  {              
    previous_millis_heat = millis();

    pouchHeater1_current_temp = analog2temp(POUCH_TEMP_1);
    if (POUCH_HEATER_1_PIN > -1) 
    {
      if(pouchHeater1_on)
      {
        if((pouchHeater1_current_temp >= pouchHeater1_set_temp)) 
        {
          digitalWrite(POUCH_HEATER_1_PIN,LOW); //turn off
          isHeating = false;
        }  
        else 
          digitalWrite(POUCH_HEATER_1_PIN,HIGH); //turn on
      }
      else
        digitalWrite(POUCH_HEATER_1_PIN,LOW); //turn off
    }

    pouchHeater2_current_temp = analog2temp(POUCH_TEMP_2);
    if (POUCH_HEATER_2_PIN > -1) 
    {
      if(pouchHeater2_on)
      {
        if((pouchHeater2_current_temp >= pouchHeater2_set_temp)) 
        {
          digitalWrite(POUCH_HEATER_2_PIN,LOW);  //turn off
          isHeating = false;
        }
        else 
          digitalWrite(POUCH_HEATER_2_PIN,HIGH); //turn on
      }
      else
        digitalWrite(POUCH_HEATER_2_PIN,LOW);  //turn off
    }
  }
}


float analog2temp(uint8_t sensorNumber) {
  uint8_t e = sensorNumber;
  int raw  =  0;
  float celsius = 0;
  int pin; 

  switch (sensorNumber)
  {
    case POUCH_TEMP_1:
      pin = POUCH_TEMP_1_PIN;
     break;
  
    case POUCH_TEMP_2:
      pin = POUCH_TEMP_2_PIN;
     break;
  }

  for(int j=1;j<=OVERSAMPLENR;j++){
    raw += analogRead(pin);
  }

  if (temptable_11[e] != NULL) {
    float celsius = 0;
    uint8_t i;
    short(*tt)[][2] = (short(*)[][2])(temptable_11[e]);
    int value = (int)temptable_11[e];
    for (i = 1; i < value; i++) {
      if (PGM_RD_W((*tt)[i][0]) > raw) {
        celsius = PGM_RD_W((*tt)[i - 1][1]) +
                  (raw - PGM_RD_W((*tt)[i - 1][0])) *
                  (float)(PGM_RD_W((*tt)[i][1]) - PGM_RD_W((*tt)[i - 1][1])) /
                  (float)(PGM_RD_W((*tt)[i][0]) - PGM_RD_W((*tt)[i - 1][0]));
        return celsius;
      }
    }

    // Overflow: Set to last value in the table   
    if (i == (sizeof(temptable_11) / sizeof(temptable_11[0])) - 1){ 
      celsius = PGM_RD_W((*tt)[i - 1][1]);
    }
  }
  else {
    celsius = NAN;
  }

  return celsius;
}
