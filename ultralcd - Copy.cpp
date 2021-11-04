#include <avr/pgmspace.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <math.h>

#include "ultralcd.h"
#include "stepper.h"
#include "pins.h"
#include "config.h"

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

char jpconv[64]; //english to japanese conversion

int8_t incrementalMode = 0;
int8_t temp_incrementalMode = 0;
int8_t heaterTypeSection = 0; 

void lcdProgMemprint(const char *str)
{
  char ch=pgm_read_byte(str);
  while(ch)
  {
    lcd.print(ch);
    ch=pgm_read_byte(++str);
  }
}
#define lcdprintPGM(x) lcdProgMemprint(PSTR(x))

uint8_t arrow[8] = {B00000, B00100, B00110 ,B11111, B00110, B00100, B00000, B00000};
uint8_t back[8]  = {B00100, B01110 ,B11111, B00100, B11100, B00000, B00000, B00000};
uint8_t down[8]  = {B00000, B00000 ,B00000, B00000, B00000, B10001, B01010, B00100};

#define ARROW 0
#define BACK  1
#define DOWN  2


uint8_t buttonState = 0;
uint8_t last_buttonState = 0;
uint8_t buttonPressed = 0;

uint32_t blocking_enc;
uint32_t lcd_next_update_millis;
uint32_t lcd_cursor_update_millis;
int8_t encoderpos; 
uint32_t encoderPosition;
uint32_t lastEncoderPosition = 1;
uint8_t lastEncoderBits;

volatile uint8_t buttons; //Contains the bits of the currently pressed buttons.
uint32_t previous_millis_cursor = 0;

uint8_t buttonPressedCount= 0;

int8_t index = 0;
int8_t lastIndex = 0;

//rotaryIncrement    
int8_t rotaryIncrement= 0;
int8_t last_rotaryIncrement = 0;

int8_t currentPage = 0;

MAIN_MENU menuNumber; //main manu
uint8_t mainMenuItem = (HEATER_CUTTER - CONTROL); //last menu item - fast menu item 

CONTROL_MENU ControlMenuItem; 
uint8_t totalControlMenuItem = (HOME_OFFSET - CONTROL_MENU_BACK);

HEATER_CUTTER_MENU cuttingMenuItem;
uint8_t totalCuttingMenuItem = (HEATER_CUTTING_DELAY - HEATER_CUTTER_MENU_BACK);

uint8_t heaterTypeSection_Counter = 0;
uint8_t incrementalMode_Counter = 0;

float temp_homingOffset = 0;


bool statusFlag = true;
bool mainMenuFlag = true;
bool controlMenuFlag =false;
bool heaterCutterMenuFlag =false;

bool manuallySelectManiMenu = false;
bool manuallySelectControlMenu = false;
bool manuallySelectHeaterCutterMenu = false;

void rotaryEncoder_Init()
{
  pinMode(BUZZER_IO, OUTPUT);
  pinMode(BTN_EN1, INPUT_PULLUP);
  pinMode(BTN_EN2, INPUT_PULLUP);
  pinMode(BTN_ENC, INPUT_PULLUP);

  rotary_buttons_update();
  encoderpos = 0;
}

void beeper(uint8_t numOfBeep, uint8_t beepPattern)
{
  uint16_t beepDelay;
  
  if(beepPattern == SHORT_BEEP)  beepDelay = 10;
  if(beepPattern == LONG_BEEP )  beepDelay = 300;

  for (int i = 0; i < numOfBeep; i++)
  {
    digitalWrite(BUZZER_IO, HIGH);
    delay(beepDelay);
    digitalWrite(BUZZER_IO, LOW);
    delay(beepDelay);
  }
}


void lcd_Init()
{
  lcd.begin(LCD_WIDTH, LCD_HEIGHT);
  lcd.createChar(ARROW, arrow);   //Create the arrow symbol
  lcd.createChar(BACK, back);     //Create the back symbol
  lcd.createChar(DOWN, down );    //Create the back symbol
  lcd.home();
  lcd.clear();
}

void lcd_clear()
{
  lcd.home();
  lcd.clear();
}


void lcd_update()
{
  if (lcd_cursor_update_millis < millis())
  {
    if (abs(encoderpos) >= ENCODER_PULSES_PER_STEP)
    {
      encoderPosition += encoderpos / ENCODER_PULSES_PER_STEP;
      index = encoderPosition / ENCODER_STEPS_PER_MENU_ITEM;

      if(index > lastIndex) 
        rotaryIncrement ++;
      else if(index < lastIndex) 
        rotaryIncrement --;  
      //Serial.println(rotaryIncrement);

      encoderpos = 0;
      lastIndex = index;
    }
    
    menu_update();
    lcd_cursor_update_millis = millis() + 100;  
  }
  isButtonPressed(); //check if button pressed por not
}

bool isButtonPressed()
{
  if(BTN_CLICKED) 
    buttonPressed = true;
  return buttonPressed;
}

void buttonReleased()
{
  buttonPressed = false;
}


void rotary_buttons_update()
{
  uint8_t newbutton=0;
  if(digitalRead(BTN_EN1)==0)  newbutton|=EN_A;
  if(digitalRead(BTN_EN2)==0)  newbutton|=EN_B;
  
  uint8_t buttonState = 0;
  buttonState = !digitalRead(BTN_ENC);
  if((blocking_enc<millis()) && (last_buttonState != buttonState))  
  {
    if(last_buttonState) 
    {
      newbutton |= EN_C; 
      last_buttonState = 0;
      blocking_enc = millis() + blocktime;
    }
    else
      last_buttonState = 1;
  }
  buttons = newbutton;

  // if((blocking_enc<millis()) && (digitalRead(BTN_ENC)==0))  
  //   newbutton |= EN_C;
  // buttons = newbutton;
  // // Serial.println(buttons,BIN);

  
  #ifdef REVERSE_ENCODER_DIRECTION
    #define ENCODER_DIFF_CW  (encoderpos--)
    #define ENCODER_DIFF_CCW (encoderpos++)
  #else
    #define ENCODER_DIFF_CW  (encoderpos++)
    #define ENCODER_DIFF_CCW (encoderpos--)  
  #endif
  
  #define ENCODER_SPIN(_E1, _E2) switch (lastEncoderBits) { case _E1: ENCODER_DIFF_CW; break; case _E2: ENCODER_DIFF_CCW; }
    
 //manage encoder rotation
  uint8_t enc=0;
  if (buttons & EN_A) enc |= B01;
  if (buttons & EN_B) enc |= B10;
  if(enc != lastEncoderBits)
  {
    switch (enc) 
    {
        case encrot0: ENCODER_SPIN(encrot3, encrot1); break;
        case encrot1: ENCODER_SPIN(encrot0, encrot2); break;
        case encrot2: ENCODER_SPIN(encrot1, encrot3); break;
        case encrot3: ENCODER_SPIN(encrot2, encrot0); break;
    }
  }
  lastEncoderBits = enc;
}


void menu_update()
{
  /*------------------cursor selection------------------*/
  if(rotaryIncrement != last_rotaryIncrement &&  millis() - previous_millis_cursor >= LCD_CURSOR_UPDATE_INTERVAL)
  {
    if(rotaryIncrement > last_rotaryIncrement)
    {
      previous_millis_cursor=millis();
      currentPage++;

      if(mainMenuFlag && currentPage >= mainMenuItem)
        currentPage = mainMenuItem;
      if(controlMenuFlag && currentPage >= totalControlMenuItem)
        currentPage = totalControlMenuItem;
      if(heaterCutterMenuFlag && currentPage >= totalCuttingMenuItem)
        currentPage = totalCuttingMenuItem;
    }

    else if(rotaryIncrement < last_rotaryIncrement)
    {
      previous_millis_cursor=millis();
      currentPage--;

      if (currentPage <= 0)
      currentPage = 0;
    }

    // -------------debug---------------
    if(DEBUGGING)
    {
      Serial.print("RotaryIncrement: ");
      Serial.print(rotaryIncrement    );
      Serial.print("    CurrentPage: ");
      Serial.println(currentPage);
    }
  }
  /*-----------------------end--------------------------*/

  /*------------------menu selection--------------------*/
  if (mainMenuFlag) //main manu selection          
  {
    if(manuallySelectManiMenu) //if inside the menu, do not turn rotaryIncrement    
    {
      currentPage = menuNumber;
      manuallySelectManiMenu = false;
      manuallySelectControlMenu = false;
      manuallySelectHeaterCutterMenu = false;
    }

    menuNumber = (MAIN_MENU)currentPage;
    mainMenu();
  }

  else if (controlMenuFlag) //submenu selection 
  {
    if(manuallySelectControlMenu)// if inside the submenu, do not turn rotaryIncrement    
    {
      currentPage = ControlMenuItem;
      manuallySelectManiMenu = false;
      manuallySelectControlMenu = false;
      manuallySelectHeaterCutterMenu = false;
    }

    ControlMenuItem = (CONTROL_MENU)currentPage;
    controlMenu();
  }

  else if (heaterCutterMenuFlag) //submenu1 selection 
  {
    if(manuallySelectHeaterCutterMenu)// if inside the submenu, do not turn rotaryIncrement    
    {
      currentPage = cuttingMenuItem;
      manuallySelectManiMenu = false;
      manuallySelectControlMenu = false;
      manuallySelectHeaterCutterMenu = false;
    }
    
    cuttingMenuItem = (HEATER_CUTTER_MENU)currentPage;
    cuttingMenu();
  }
  /*-----------------------end--------------------------*/
  last_rotaryIncrement = rotaryIncrement;
  lastEncoderPosition = encoderPosition;
}


void mainMenu()
{
  switch(menuNumber)
  { 
    case CONTROL:
    { 
      printStr(jp("       [ﾒﾆｭｰ]      "), 0, 0);    //MENU
      printChar(ARROW, 0, 1);
      printStr(jp("ｺﾝﾄﾛｰﾙ             "));          //Control
      printStr(jp(" ﾋｰﾀｰｶｯﾄ            "), 0, 2);   //Heater Cutter
      printStr(jp("                    "), 0, 3);
      
      if(buttonPressed) // go to submenu
      {
        buttonReleased();
        beeper(1,SHORT_BEEP);
        mainMenuFlag = false;
        controlMenuFlag = true;  
        heaterCutterMenuFlag = false;  
        manuallySelectControlMenu = true;
        ControlMenuItem = (CONTROL_MENU) INITIALIZE;
      }
    }
    break;

    case HEATER_CUTTER:
    {
      printStr(jp("       [ﾒﾆｭｰ]      "), 0, 0);    //MENU
      printStr(jp(" ｺﾝﾄﾛｰﾙ            "), 0, 1);    //Control
      printChar(ARROW, 0, 2);
      printStr(jp("ﾋｰﾀｰｶｯﾄ            "));          //Heater Cutter
      printStr(jp("                    "), 0, 3);
      if(buttonPressed)
      {
        buttonReleased();
        beeper(1,SHORT_BEEP);
        mainMenuFlag = false;
        controlMenuFlag = false;  
        heaterCutterMenuFlag = true; 
        manuallySelectHeaterCutterMenu = true;
        cuttingMenuItem = (HEATER_CUTTER_MENU) HEATER_CUTTING;
      }
    }
    break;
  }
}

void controlMenu()
{
  switch(ControlMenuItem)
  { 
    case CONTROL_MENU_BACK:
    {
      printChar(BACK, 0, 0);
      printStr(jp( "     [ｺﾝﾄﾛｰﾙ]      "));           //Control
      printStr(jp(" ｼｮｷｶ               "), 0, 1);    //Initialize
      printStr(jp(" ﾋｰﾀｰｾﾝﾀｸ           "), 0, 2);       //Heater Selection 1.6mm or 2.4mm
      printStr(jp(" ｶｯﾀｰｶｲﾃﾝｶｲｼ        "), 0, 3);     //Cutter Rotation
      printChar(DOWN, 19, 3);

      if(buttonPressed)
      {  
        buttonReleased();
        beeper(1,SHORT_BEEP);
        mainMenuFlag= true;
        controlMenuFlag = false;
        heaterCutterMenuFlag = false;
        manuallySelectManiMenu = true;
        menuNumber = (MAIN_MENU)HEATER_CUTTER; 
      }

    }
    break;
    
    case INITIALIZE:
    {
      printStr(jp("      [ｺﾝﾄﾛｰﾙ]      "), 0, 0);      //Control
      printChar(ARROW, 0, 1);                         
      printStr(jp( "ｼｮｷｶ               "));            //Initialize
      printStr(jp(" ﾋｰﾀｰｾﾝﾀｸ           "), 0, 2);       //Heater Selection 1.6mm or 2.4mm
      printStr(jp(" ｶｯﾀｰｶｲﾃﾝｶｲｼ        "), 0, 3);       //Cutter Rotation
      printChar(DOWN, 19, 3);

      if(buttonPressed)
      {  
        buttonReleased();
        beeper(1,SHORT_BEEP);

        lcd_clear();
        printStr(jp(">ｶｯﾀｰｹﾞﾝﾃﾝｼｭﾄｸ...    "), 0, 0);
        printStr(jp("                    "), 0, 1);	
        printStr(jp("                    "), 0, 2);
        printStr(jp("｢ﾋﾟｰ｣ﾄﾅﾙﾏﾃﾞﾏｯﾃｸﾀﾞｻｲ"), 0, 3);

        motor_home();

        mainMenuFlag= false;
        controlMenuFlag = true;
        heaterCutterMenuFlag = false;
        manuallySelectControlMenu= true;
        ControlMenuItem = (CONTROL_MENU)CUTTER_ROTATION; 
      }
    }
    break;

    case HEATER_SELECTION:
    {
      uint8_t select = 0;
      if (buttonPressedCount == 1) 
      {
        select = 1;
        manuallySelectControlMenu = true;
      }
      
      if(select == 0)
      {
        printStr(jp("      [ｺﾝﾄﾛｰﾙ]     "), 0, 0);      //Control              
        printStr(jp(" ｼｮｷｶ              "), 0, 1);      //Initialize
        printChar(ARROW, 0, 2);   
        printStr(jp("ﾋｰﾀｰｾﾝﾀｸ           "));            //Heater Selection 1.6mm or 2.4mm
        printStr(jp(" ｶｯﾀｰｶｲﾃﾝｶｲｼ        "), 0, 3);      //Cutter Rotation
        printChar(DOWN, 19, 3);
      }
      else
      {
        
        if(encoderPosition > lastEncoderPosition && heaterTypeSection_Counter < 4)   { heaterTypeSection_Counter +=1; }
        if(encoderPosition < lastEncoderPosition && heaterTypeSection_Counter > 0)   { heaterTypeSection_Counter -=1; }
       
        if(heaterTypeSection_Counter%2 == 0) /*get only even number. Even numbers are divisible by 2 */
        {
          heaterTypeSection = heaterTypeSection_Counter;
          switch (heaterTypeSection)
          {
            case 0: // back 
              {
                printChar(BACK, 0, 0);
                printStr(jp( "     ﾋｰﾀｰｾﾝﾀｸ       "));
                printStr(jp(" ｼｰｽﾋｰﾀｰ: 1.6[mm]   "), 0, 1);
                printStr(jp(" ｼｰｽﾋｰﾀｰ: 2.4[mm]   "), 0, 2);
                printStr(jp("                    "), 0, 3);
              }
              break;
            
            case 2: // Heater 1.6mm  [case:2 instead of case:1 (slow down the rotary knob)]
              {
                printStr(jp("      ﾋｰﾀｰｾﾝﾀｸ       "), 0, 0);
                printChar(ARROW, 0, 1);
                printStr(jp( "ｼｰｽﾋｰﾀｰ: 1.6[mm]   "));
                printStr(jp(" ｼｰｽﾋｰﾀｰ: 2.4[mm]   "), 0, 2);

                if(heaterTypeSelect == HEATER_1_6MM)//1.6mm
                  printStr(jp("                    "), 0, 3);
                if(heaterTypeSelect == HEATER_2_4MM)//2.4mm
                  printStr(jp("*ﾋｰﾀｰﾁｮｳｾｲﾋﾂﾖｳｶﾞｱﾘﾏｽ"), 0, 3);
               
              }
              break;
            
            case 4: // Heater 2.4mm [case:2 instead of case:2 (slow down the rotary knob)]
              {
                printStr(jp("      ﾋｰﾀｰｾﾝﾀｸ       "), 0, 0);
                printStr(jp(" ｼｰｽﾋｰﾀｰ: 1.6[mm]   "), 0, 1);
                printChar(ARROW, 0, 2);
                printStr(jp( "ｼｰｽﾋｰﾀｰ: 2.4[mm]   "));

                if(heaterTypeSelect == HEATER_1_6MM)//1.6mm
                  printStr(jp("*ﾋｰﾀｰﾁｮｳｾｲﾋﾂﾖｳｶﾞｱﾘﾏｽ"), 0, 3);
                if(heaterTypeSelect == HEATER_2_4MM)//2.4mm
                  printStr(jp("                    "), 0, 3);
              }
              break;
          }
        }
        //Serial.println(heaterTypeSection);
      }

      if(buttonPressed)
      {  
        buttonReleased();
        beeper(1,SHORT_BEEP);
        buttonPressedCount++;

        if(buttonPressedCount >= 2)
        {
          buttonPressedCount = 0;
          mainMenuFlag= false;
          controlMenuFlag = true;
          heaterCutterMenuFlag = false;
          manuallySelectControlMenu= true;
          if(heaterTypeSection != 0) // 2 for 1.6mm, 4 for 2.4mm
          { 
            motor_home(); // initializing cutter
            update_eeprom = true;
            heaterTypeSection_Counter = 0;
            heaterTypeSelect = heaterTypeSection; //update eeprom 
            ControlMenuItem = (CONTROL_MENU)CUTTER_ROTATION;  
          }       
          else
            ControlMenuItem = (CONTROL_MENU)HEATER_SELECTION;
      
          heaterTypeSection = heaterTypeSelect;
          lcd_clear();
        }
      }

    }
    break;

    case CUTTER_ROTATION: 
    {
      uint8_t select = 0;
      if (buttonPressedCount == 1) 
      {
        select = 1;
        digitalWrite(SPINDLE_SW,HIGH);
        manuallySelectControlMenu = true;
      }

      if(select == 0)
      {
        printStr(jp("      [ｺﾝﾄﾛｰﾙ]      "), 0, 0);      //Control    
        printStr(jp(" ｼｮｷｶ               "), 0, 1);      //Initialize
        printStr(jp(" ﾋｰﾀｰｾﾝﾀｸ           "), 0, 2);       //Heater Selection 1.6mm or 2.4mm
        printChar(ARROW, 0, 3);
        printStr(jp( "ｶｯﾀｰｶｲﾃﾝｶｲｼ        "));             //Cutter Rotation
        printChar(DOWN, 19, 3);
      }
      else
      {
        printStr(jp(">ｶｲﾃﾝﾁｭｳ             "), 0, 0);     //Cutter rotating
        printStr(jp("                    "), 0, 1);	
        printStr(jp("ﾄﾒﾏｽｶ?              "), 0, 2);      //Want to stop
        printChar(ARROW, 0, 3);
        printStr(jp("ﾊｲ                  "));            //Yes
      }
      
      if(buttonPressed)
      {  
        buttonReleased();
        beeper(1,SHORT_BEEP);
        buttonPressedCount++;

        if(buttonPressedCount >= 2)
        {
          buttonPressedCount = 0;
          digitalWrite(SPINDLE_SW,LOW);
          mainMenuFlag= false;
          controlMenuFlag = true;
          heaterCutterMenuFlag = false;
          manuallySelectControlMenu= true;
          ControlMenuItem = (CONTROL_MENU)MOTOR_INCREMENT; 
          lcd_clear();
        }
      }
    }  
    break;

    case MOTOR_INCREMENT:
    {
      uint8_t select = 0;
      if (buttonPressedCount == 1) 
      {
        select = 1;
        manuallySelectControlMenu = true;
      }

      if(select == 0)
      {
        printStr(jp(" ｼｮｷｶ              ^"), 0, 0);      //Initialize
        printStr(jp(" ﾋｰﾀｰｾﾝﾀｸ           "), 0, 1);       //Heater Selection 1.6mm or 2.4mm
        printStr(jp(" ｶｯﾀｰｶｲﾃﾝｶｲｼ        "), 0, 2);       //Cutter Rotation
        printChar(ARROW, 0, 3);
        printStr(jp("ﾓｰﾀｰｲﾄﾞｳﾘｮｳ         "));             //motor movement amount   
        printChar(DOWN, 19, 3);
      }

      else
      {
        if(encoderPosition > lastEncoderPosition && incrementalMode_Counter < 6)   { incrementalMode_Counter ++; }
        if(encoderPosition < lastEncoderPosition && incrementalMode_Counter > 0)   { incrementalMode_Counter --; }

        if(incrementalMode_Counter %2 == 0) /*get only even number.Even numbers are divisible by 2 */
        {
          incrementalMode = incrementalMode_Counter;
          switch(incrementalMode)
          {
            case 0:
              {
                printChar(BACK, 0, 0);                          //back
                printStr(jp( "  ﾓｰﾀｰｲﾄﾞｳﾘｮｳｾﾝﾀｸ   "));                 
                printStr(jp(" ｲﾄﾞｳﾘｮｳ: 0.01[mm]  "), 0, 1);       
                printStr(jp(" ｲﾄﾞｳﾘｮｳ: 0.10[mm]  "), 0, 2);
                printStr(jp(" ｲﾄﾞｳﾘｮｳ: 1.00[mm]  "), 0, 3);
              }
              break;
            
            case 2: //[case:2 instead of case:1 (slow down the rotary knob)]
              {
                printStr(jp("   ﾓｰﾀｰｲﾄﾞｳﾘｮｳｾﾝﾀｸ   "), 0, 0);     //select motor movement amount selection
                printChar(ARROW, 0, 1);
                printStr(jp( "ｲﾄﾞｳﾘｮｳ: 0.01[mm]  "));
                printStr(jp(" ｲﾄﾞｳﾘｮｳ: 0.10[mm]  "), 0, 2);
                printStr(jp(" ｲﾄﾞｳﾘｮｳ: 1.00[mm]  "), 0, 3);
              }
              break;
            
            case 4: //[case:4 instead of case:2 (slow down the rotary knob)]
              {
                printStr(jp("   ﾓｰﾀｰｲﾄﾞｳﾘｮｳｾﾝﾀｸ   "), 0, 0);     //select motor movement amount selection
                printStr(jp(" ｲﾄﾞｳﾘｮｳ: 0.01[mm]  "), 0, 1);
                printChar(ARROW, 0, 2);
                printStr(jp( "ｲﾄﾞｳﾘｮｳ: 0.10[mm]  "));
                printStr(jp(" ｲﾄﾞｳﾘｮｳ: 1.00[mm]  "), 0, 3);
              }
              break;
            
            case 6: //[case:6 instead of case:3 (slow down the rotary knob)]
              {
                printStr(jp("   ﾓｰﾀｰｲﾄﾞｳﾘｮｳｾﾝﾀｸ   "), 0, 0);     //select motor movement amount selection
                printStr(jp(" ｲﾄﾞｳﾘｮｳ: 0.01[mm]  "), 0, 1);
                printStr(jp(" ｲﾄﾞｳﾘｮｳ: 0.10[mm]  "), 0, 2);
                printChar(ARROW, 0, 3);
                printStr(jp( "ｲﾄﾞｳﾘｮｳ: 1.00[mm]  "));
              }
              break;
          }
        }
      }

      if(buttonPressed)
      {  
        buttonReleased();
        beeper(1,SHORT_BEEP);
        buttonPressedCount++;

        if(buttonPressedCount >= 2)
        {
          buttonPressedCount = 0;
          mainMenuFlag= false;
          controlMenuFlag = true;
          heaterCutterMenuFlag = false;
          manuallySelectControlMenu= true;
          if(incrementalMode != 0)
          {
            incrementalMode_Counter = 0;
            temp_incrementalMode = incrementalMode;
            ControlMenuItem = (CONTROL_MENU)HOME_OFFSET;
          }
          else
            ControlMenuItem = (CONTROL_MENU)MOTOR_INCREMENT;  
          incrementalMode = temp_incrementalMode;
          lcd_clear();
        }
      }
    }
    break;

    case HOME_OFFSET:
    {
      uint8_t select = 0;
      if(buttonPressedCount == 1) 
      {
        select = 1;
        manuallySelectControlMenu = true;
      }

      if(select == 0)
      {
        printStr(jp(" ﾋｰﾀｰｾﾝﾀｸ          ^"), 0, 0);       //Heater Selection 1.6mm or 2.4mm
        printStr(jp(" ｶｯﾀｰｶｲﾃﾝｶｲｼ        "), 0, 1);       //Cutter Rotation
        printStr(jp(" ﾓｰﾀｰｲﾄﾞｳﾘｮｳ        "), 0, 2);       //motor movement amount   
        printChar(ARROW, 0, 3);
        printStr(jp( "ｹﾞﾝﾃﾝｲﾁﾁｮｳｾｲ       "));             //change cutter soft home   
      }
      else
      {
        printStr(jp(">ｹﾞﾝﾃﾝｲﾁﾁｮｳｾｲ (mm)  "), 0, 0);
        printStr(jp(" ｹﾞﾝｻﾞｲﾁ:      mm  "), 0, 1);
        printFloat(previousHomingOffset, 10, 1);

        printStr(jp(" ｾｯﾃｲﾁ  :      mm  "), 0, 2);
        printFloat(temp_homingOffset, 10, 2);
        printStr(jp("*ｱﾀｲｶﾞｵｵｷｲﾄﾁｶｸﾅﾘﾏｽ  "), 0, 3);
      }

      float increment = 0.00;
      switch(incrementalMode)
      {
        case 0:
        case 2:
          increment = 0.01;
          break;
        case 4:
          increment = 0.10;
          break;
        case 6:
          increment = 1.00;
          break;
      }

      if(encoderPosition > lastEncoderPosition)   { temp_homingOffset += increment; }
      if(encoderPosition < lastEncoderPosition)   { temp_homingOffset -= increment; }

      if(temp_homingOffset<0)   temp_homingOffset = 0;
      if(temp_homingOffset>5)   temp_homingOffset = 5;


      if(buttonPressed)
      {  
        buttonReleased();
        beeper(1,SHORT_BEEP);
        buttonPressedCount++;
            
        if(buttonPressedCount == 2)
        {
          buttonPressedCount = 0;
          homingOffset = temp_homingOffset;
          previousHomingOffset = homingOffset;
          update_eeprom = true;
	        
          motor_home();

          mainMenuFlag= false;
          controlMenuFlag = true;
          heaterCutterMenuFlag = false;
          manuallySelectControlMenu= true;
          ControlMenuItem = (CONTROL_MENU)HOME_OFFSET; 
          lcd_clear();
        }
        temp_homingOffset = previousHomingOffset;
      }
    }
    break;
  }
}

void cuttingMenu()
{
  switch (cuttingMenuItem)
  {
    case HEATER_CUTTER_MENU_BACK:
    {
      printChar(BACK, 0, 0);
      printStr(jp( "      ﾋｰﾀｰｶｯﾄ       "));
      if(heaterTypeSection == HEATER_1_6MM)//1.6mm
        printStr(jp(" ｶｯﾄｶｲｼ:[1.6mm]    "), 0, 1);
      if(heaterTypeSection == HEATER_2_4MM)//2.4mm
        printStr(jp(" ｶｯﾄｶｲｼ:[2.4mm]    "), 0, 1);
      printStr(jp(" ｶｯﾄｼﾞｶﾝ            "), 0, 2);
      printStr("                    ", 0, 3);

      if(buttonPressed)
      {  
        buttonReleased();
        beeper(1,SHORT_BEEP);
        mainMenuFlag= true;
        controlMenuFlag = false;
        heaterCutterMenuFlag = false;

        manuallySelectManiMenu = true;
        menuNumber = (MAIN_MENU)CONTROL; 
      }
    }
    break;
    
    case HEATER_CUTTING:
    {
      printStr(jp( "       ﾋｰﾀｰｶｯﾄ     "), 0, 0);
      printChar(ARROW, 0, 1);
      if(heaterTypeSection == HEATER_1_6MM)//1.6mm
        printStr(jp("ｶｯﾄｶｲｼ:[1.6mm]     "));
      if(heaterTypeSection == HEATER_2_4MM)//2.4mm
        printStr(jp("ｶｯﾄｶｲｼ:[2.4mm]     "));
      printStr(jp(" ｶｯﾄｼﾞｶﾝ            "), 0, 2);
      printStr("                    ", 0, 3);

      if(buttonPressed)
      {  
        buttonReleased();
        lcd_clear();
        beeper(1,SHORT_BEEP);

        printStr(jp(">ｶｯﾀｰｶｲﾃﾝﾁｭｳ...    "), 0, 0);     
        digitalWrite(SPINDLE_SW, HIGH); //turn on cutter motor
        delay(500); // delay for relax

        if(heaterTypeSection == HEATER_1_6MM)//1.6mm
          printStr(jp(">1.6mmﾋｰﾀｰｶｯﾄｶｲｼ   "), 0, 1);   
        if(heaterTypeSection == HEATER_2_4MM)//2.4mm
          printStr(jp(">2.4mmﾋｰﾀｰｶｯﾄｶｲｼ   "), 0, 1); 

        printStr(jp("*｢ﾋﾟｰ｣ﾄﾅﾙﾏﾃﾞﾏｯﾃｸﾀﾞｻｲ"), 0, 3);

        delay(500); // delay for relax

        speedSet(CUTTING_SPEED);
        goDestination(CW, X_Motor, CUTTING_TRAVEL_DISTANCE);
        while(isMotorBusy()); //wait here until motor reach its desire position
        beeper(1,LONG_BEEP);

        printStr(jp(">ｼｰｽﾋｰﾀｰｦﾏﾜｼﾃｸﾀﾞｻｲ "), 0, 0);

        int8_t delayTime = cutting_delay;
        while(delayTime >= 0)
        {
          printStr(jp(" ﾄﾞｳｻﾃｲｼﾏﾃﾞｱﾄ:  sec"), 0, 1);
          printInt(delayTime, 14, 1);
          waitingTime(1); // 1sec delay
          delayTime--;
        }

        //turn off cutter motor
        digitalWrite(SPINDLE_SW, LOW); 
        beeper(2,LONG_BEEP);

        lcd_clear();        
        printStr(jp(">ｶｯﾀｰｶﾞﾊﾅﾚﾃｲﾏｽ... "), 0, 0);

        goDestination(CCW, X_Motor, CUTTING_TRAVEL_DISTANCE);
        while(isMotorBusy()); //wait here for motor reach its desire position

        mainMenuFlag= false;
        controlMenuFlag = false;
        heaterCutterMenuFlag = true;
        manuallySelectHeaterCutterMenu = true;
        cuttingMenuItem = (HEATER_CUTTER_MENU) HEATER_CUTTING;

        lcd_clear();
        printStr(jp(">ｼｭｳﾘｮｳｼﾏｼﾀ｡     "), 0, 0);
        beeper(1,LONG_BEEP);
        delay(500);
      }
    }
    break;

    case HEATER_CUTTING_DELAY:
    {
      uint8_t select = 0;
      if (buttonPressedCount == 1) 
      {
        select = 1;
        manuallySelectHeaterCutterMenu = true;
      }
      
      if(select==0)
      {
        printStr(jp( "       ﾋｰﾀｰｶｯﾄ     "), 0, 0);
        if(heaterTypeSection == HEATER_1_6MM)//1.6mm
          printStr(jp(" ｶｯﾄｶｲｼ:[1.6mm]    "), 0, 1);
        if(heaterTypeSection == HEATER_2_4MM)//2.4mm
          printStr(jp(" ｶｯﾄｶｲｼ:[2.4mm]    "), 0, 1);
        printChar(ARROW, 0, 2);
        printStr(jp("ｶｯﾄｼﾞｶﾝ             "));
        printStr("                    ", 0, 3);
      }
      else
      {
        printStr(jp(">ｶｯﾀｰﾄﾞｳｻｼﾞｶﾝ:  sec"), 0, 0);
        printInt(cutting_delay, 14, 0);
        printStr("                    ", 0, 1);
        printStr("                    ", 0, 2);
        printStr("                    ", 0, 3);
        
        if(encoderPosition > lastEncoderPosition)   { cutting_delay += 1; }
        if(encoderPosition < lastEncoderPosition)   { cutting_delay -= 1; }

        if(cutting_delay<1)    cutting_delay = 1;
        if(cutting_delay>10)   cutting_delay = 10;
      }

      if(buttonPressed)
      {  
        buttonReleased();
        beeper(1,SHORT_BEEP);
        buttonPressedCount++;

        if (buttonPressedCount >= 2)
        {
          buttonPressedCount = 0;
          update_eeprom = true;

          mainMenuFlag= false;
          controlMenuFlag = false;
          heaterCutterMenuFlag = true;
          manuallySelectHeaterCutterMenu = true;
          cuttingMenuItem = (HEATER_CUTTER_MENU) HEATER_CUTTING;
          lcd.clear();
        }
      }
    }
    break;
  }
}

/*
	Prints text to display
	desc > Will wrap to second display line
		   if text is bigger than 20 chars.
		   Doesn't work for text with more
		   than 32 chars (lcd resets).
	text: String > Text to print
	index: int > Index to print text at
	line: int > Line to print text at
*/
void printStr(String text, uint8_t index, uint8_t line) 
{
	if(text.length() > LCD_WIDTH) 
  {
		lcd.setCursor(0, 0);
		lcd.print(text);
		lcd.setCursor(0, 1);
		lcd.print(text.substring(LCD_WIDTH + 1, text.length()));
	}
	else 
  {
		lcd.setCursor(index, line);
		lcd.print(text);
	}
}

void printChar(uint8_t c, uint8_t index, uint8_t line)
{
  lcd.setCursor(index, line);
  lcd.write(uint8_t(c));
}

void printStr(String text)
{
  lcd.print(text);
}

void printInt(uint16_t value, uint8_t index, uint8_t line)
{
  lcd.setCursor(index, line);
  lcd.print(value);
}

void printFloat(float value, uint8_t index, uint8_t line)
{
  lcd.setCursor(index, line);
  lcd.print(value, 2);
}

/*
* custom delay function
*/
void waitingTime(float s)
{
	int16_t ms = s*1000;
	while (0 < ms)
	{
		delay(1); //1ms delay
		--ms;
	}
}

/*
* This code converts half-width Katakana from Unicode
* https://programresource.net/2020/03/09/3113.html
* https://synapse.kyoto/lib/MGLCD/CharacterCode/page001.html
*/
char* jp(const char *text) 
{
  char *cur = &jpconv[0];
  const char *chk = text;
  while (*chk != 0) 
  {
    if ((*chk & 0xFF) == 0xEF && (*(chk+1) & 0xBC) == 0xBC) 
    {
      *cur = (*++chk & 0x02) << 5;
      *cur++ = *++chk + *cur;
    } 
    else
      *cur++ = *chk;
    chk++;
  }
  *cur = 0;
  return jpconv;
}
 
// -------------------------------------- End --------------------------------------