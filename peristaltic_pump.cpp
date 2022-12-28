//#include <Arduino.h>
//#include <avr/interrupt.h>

#include "Marlin.h"
#include "configuration_store.h"

#include "pins_RAMPS_13.h"
#include "VL6180X_sensor.h"
#include "peristaltic_pump.h"


#define maxPWM_TopValue 8000 // 1KHz
#define minPWM_TopValue 50  

int16_t pump_speed =0;

int16_t pump1Speed =50;
int16_t pump2Speed =50;

bool pump1_On =0;
bool pump2_On =0;

bool manuallyPump1_On =0;
bool manuallyPump2_On =0;

int16_t pump1LastSpeed =0;
int16_t pump2LastSpeed =0;

uint16_t sensor1_current_distance;
uint16_t sensor2_current_distance;

unsigned long previous_millis_pump=0;
unsigned long wait_update_time = 1000;    //we only need to check the heater 1 times a second


void pumpSpeed_Init()
{
  cli(); // stop interrupts

  /* init pwm pin as output*/
  SET_OUTPUT(PUMP_1_ENA_PIN);  //pin D5 OCR3A PWM
  SET_OUTPUT(PUMP_2_ENA_PIN);  //pin D6 OCR4A PWM


  /*-----------------use to make PWM signal for the controlling duty cycle of the mosfet---------------------*/
  /* ------------------PUMP1(PUMP_1_ENA_PIN)------------------ */
  TCCR3A = 0; // clear TCCR3A register to 0
  TCCR3B = 0; // clear TCCR3B
  TCNT3  = 0; // initialize counter value to 0

  TCCR3A |= (1 << WGM31);  // PWM, Phase correct, Top ICR
  TCCR3B |= (1 << CS30) | (1 << WGM33); // Prescaler 1
  
  //freq = FCPU/(2*prescaler*ICR_TOP)
  //1000Hz = 16000000Hz/(2*1*8000))
  //10000Hz = 16000000Hz/(2*1*800))
  ICR3 = maxPWM_TopValue;  // max 1.0 KHz, value 8000

  OCR3A = 0; //set inital value 0
  /*-----------------------------------------------------------*/

  /* ------------------PUMP2(PUMP_2_ENA_PIN)------------------ */
  TCCR4A = 0; // clear TCCR4A register to 0
  TCCR4B = 0; // clear TCCR4B
  TCNT4  = 0; // initialize counter value to 0

  TCCR4A |= (1 << WGM41);  // PWM, Phase correct, Top ICR
  TCCR4B |= (1 << CS40) | (1 << WGM43); // Prescaler 1
  
  //freq = FCPU/(2*prescaler*ICR_TOP)
  //1000Hz = 16000000Hz/(2*1*8000))
  //10000Hz = 16000000Hz/(2*1*800))
  ICR4 = maxPWM_TopValue;  // max 1.0 KHz, value 8000

  OCR4A = 0; //set inital value 0
  /*-----------------------------------------------------------*/

  sei(); // enable global interrupts
} 


void set_pumpSpeed(int8_t whichPump, int8_t speed)
{
  //map motor speed (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  pump_speed = (long)(speed - 0) * (long)(7998 - 0) / (long)(100 - 0) + 0; //7998 is max pwm value instend of 8000
  //Serial.println(pump_speed);
  switch (whichPump)
  {
    case PUMP_1:
      OCR3A = pump_speed; //pwm update
      pump1Speed = speed;
     break;
    case PUMP_2:
      OCR4A = pump_speed; //pwm update
      pump2Speed = speed;
     break;
  }
}

void pumpOn(uint8_t whichPump)
{
  switch (whichPump)
  {
    case PUMP_1:
      PUMP_1_PWM_ON;
      pump1_On = true;
     break;
    case PUMP_2:
      PUMP_2_PWM_ON;
      pump2_On = true;
     break;
  }
}

void pumpOff(uint8_t whichPump)
{
  switch (whichPump)
  {
    case PUMP_1:
      PUMP_1_PWM_OFF;
      pump1_On = false;
     break;
    case PUMP_2:
      PUMP_2_PWM_OFF;
      pump2_On = false;
     break;
  }
}


void updatePumpSpeed()
{
  if(pump1LastSpeed != pump1Speed)
  {
    set_pumpSpeed(PUMP_1,pump1Speed);
    pump1LastSpeed = pump1Speed;
  }

  if(pump2LastSpeed != pump2Speed)
  {
    set_pumpSpeed(PUMP_2,pump2Speed);
    pump2LastSpeed = pump2Speed;
  }
}


void manage_pump()
{
  if((millis()-previous_millis_pump) > wait_update_time) //check every 1 times in a secound
  {
    previous_millis_pump = millis();
    updatePumpSpeed();

    //pump 1 control
    sensor1_current_distance = distance(NOZZLE_1_SENSOR);
    if (sensor1_current_distance >= (uint16_t) sensorThreshold)
    {
      if(pump1_On || manuallyPump1_On)
       pumpOn(PUMP_1);
      else
       pumpOff(PUMP_1);
    }
    else
     pumpOff(PUMP_1);

    //pump 2 control
    sensor2_current_distance = distance(NOZZLE_2_SENSOR);
    if(sensor2_current_distance >= (uint16_t) sensorThreshold)
    {
      if(pump2_On || manuallyPump2_On)
       pumpOn(PUMP_2);
      else
       pumpOff(PUMP_2);
    }
    else
     pumpOff(PUMP_2);
  }
}