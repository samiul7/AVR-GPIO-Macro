#ifndef PERISTALTIC_PUMP_h
 #define PERISTALTIC_PUMP_h
 
 #define PUMP_1 0
 #define PUMP_2 1
 
 #define PUMP_1_PWM_ON          TCCR3A  |=  (1 << COM3A1) // PWM, NON-INVERTING
 #define PUMP_1_PWM_OFF         TCCR3A  &= ~(1 << COM3A1) // Disable PWM

 #define PUMP_2_PWM_ON          TCCR4A  |=  (1 << COM4A1) // PWM, NON-INVERTING
 #define PUMP_2_PWM_OFF         TCCR4A  &= ~(1 << COM4A1) // Disable PWM

 #define IS_PUMP_1_PWM_ON       ((TCCR3A & (1 << COM3A1)) >> 7)
 #define IS_PUMP_2_PWM_ON       ((TCCR4A & (1 << COM4A1)) >> 7)


 extern bool pump1_On;
 extern bool pump2_On;

 extern bool manuallyPump1_On;
 extern bool manuallyPump2_On;

 extern int16_t pump1Speed;
 extern int16_t pump2Speed;

 extern uint16_t sensor1_current_distance;
 extern uint16_t sensor2_current_distance;
 

 void pumpSpeed_Init();
 void set_pumpSpeed(int8_t whichPump, int8_t speed);

 void pumpOn(uint8_t whichPump);
 void pumpOff(uint8_t whichPump);
 void updatePumpSpeed();
 void manage_pump();
#endif