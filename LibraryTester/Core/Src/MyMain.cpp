#include <Framework/Buttons/CounterButton_INT.h>
#include <Framework/Buttons/DefaultButton_INT.h>
#include <Framework/Buttons/LongPressButton_INT.h>
#include <Framework/Buttons/MomentaryButton_INT.h>
#include <Framework/Buttons/ToggleButton_INT.h>
#include <Framework/KeyPad.h>
#include <Framework/KeyPad.h>
#include <Framework/LcdDisplay.h>
#include <Framework/ShiftRegister.h>
#include "Framework/SysTickSubscribers.h"

#include "Main.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "stm32f1xx_hal.h"

extern I2C_HandleTypeDef hi2c1;
extern SPI_HandleTypeDef hspi2;

void ProcessDefaultButtonPressed(bool hold);
void ProcessLongPressButton(bool longPress);
void ProcessMomentaryButton(bool onOffState);
void ProcessToggleButton(bool onOffState);
void ProcessCounterButton(uint8_t currentValue);
void ProcessCounterDiceButton(uint8_t currentValue);
void ProcessKeyPad(char key);
void UpdateLcd();

const uint8_t NR_OF_SYS_TICK_SUBSCRIBERS = 6;
// 0: KeyPad_INT 1: Default Button 2: Toggle Button, 3: Counter Button, 4: Long Press Button, 5: LCD Display

SysTickSubscribers _sysTickSubscibers(NR_OF_SYS_TICK_SUBSCRIBERS);

DefaultButton_INT   _defaultButton     (               { GPIO_PUSH_BUTTON_1_GPIO_Port, GPIO_PUSH_BUTTON_1_Pin }, &ProcessDefaultButtonPressed    ,  500,    1, 1, 50);
ToggleButton_INT    _toggleButton      (               { GPIO_PUSH_BUTTON_2_GPIO_Port, GPIO_PUSH_BUTTON_2_Pin }, &ProcessToggleButton            ,          2, 50);
CounterButton_INT   _counterButton     (0, 10,  2, 50, { GPIO_PUSH_BUTTON_3_GPIO_Port, GPIO_PUSH_BUTTON_3_Pin }, &ProcessCounterButton           , 1000,  100, 3, 50);

//MomentaryButton_INT _momentaryButton     (               { GPIO_PUSH_BUTTON_4_GPIO_Port, GPIO_PUSH_BUTTON_4_Pin }, &ProcessMomentaryButton       ,                                        3, 50);
//CounterButton_INT     _counterDiceButton (6, -1, -1, 1 , { GPIO_PUSH_BUTTON_4_GPIO_Port, GPIO_PUSH_BUTTON_4_Pin }, &ProcessCounterDiceButton       , 1000, 100,                            3, 50);
LongPressButton_INT _longPressButton     (               { GPIO_PUSH_BUTTON_4_GPIO_Port, GPIO_PUSH_BUTTON_4_Pin }, 1000, &ProcessLongPressButton   ,                                        4, 50);

const GpioId _keyPadRows[]    = { { GPIO_KEYPAD_ROW_1_GPIO_Port, GPIO_KEYPAD_ROW_1_Pin },
                                { GPIO_KEYPAD_ROW_2_GPIO_Port, GPIO_KEYPAD_ROW_2_Pin },
                                { GPIO_KEYPAD_ROW_3_GPIO_Port, GPIO_KEYPAD_ROW_3_Pin },
                                { GPIO_KEYPAD_ROW_4_GPIO_Port, GPIO_KEYPAD_ROW_4_Pin } };
const GpioId _keyPadColumns[] = { { GPIO_KEYPAD_COLUMN_1_GPIO_Port, GPIO_KEYPAD_COLUMN_1_Pin },
                                { GPIO_KEYPAD_COLUMN_2_GPIO_Port, GPIO_KEYPAD_COLUMN_2_Pin },
                                { GPIO_KEYPAD_COLUMN_3_GPIO_Port, GPIO_KEYPAD_COLUMN_3_Pin },
                                { GPIO_KEYPAD_COLUMN_4_GPIO_Port, GPIO_KEYPAD_COLUMN_4_Pin } };

KeyPad _keyPad(4, 4, "123A456B789C*0#D",_keyPadRows, _keyPadColumns, &ProcessKeyPad, 10, 500, 300, 20, 0);


uint16_t _values[8] = { 0, 0, 0, 0, 0, 0, 0, 0 }; // button 1 + hold, button 2 + hold etc for display.
char     _symbols[8] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' }; // Symbols for display

//012345678901235
// 123-126 126-126   button 1 value + hold . button 2 + hold
// 163-164 613-164   button 3 value + hold . button 4 + hold

char _lcdLine0[17];
char _lcdLine1[17];


LcdDisplay       _lcdDisplay   (&hi2c1, 0x27, &UpdateLcd, 99, 5); // Refresh every 99 ms, not 100 ms (than fractions are changing when displayed items have a period of % 100 ms == 0
bool _lcdIsDirty = false;

ShiftRegister    _shiftRegister(&hspi2, GPIO_LATCH_GPIO_Port, GPIO_LATCH_Pin);

uint8_t _dataToShift[4] = { 0x00, 0xf0, 0x0f, 0xff };


void UpdateLcdLine0()
{
   snprintf(_lcdLine0, 17, "%3d%c%3d%c%3d%c%3d%c", _values[0], _symbols[0], _values[1], _symbols[1],
                                                   _values[2], _symbols[2], _values[3], _symbols[3]);
   _lcdDisplay.SetLine(0, _lcdLine0);
}


void UpdateLcdLine1()
{
   snprintf(_lcdLine1, 17, "%3d%c%3d%c%3d%c%3d%c", _values[4], _symbols[4], _values[5], _symbols[5],
                                                   _values[6], _symbols[6], _values[7], _symbols[7]);
   _lcdDisplay.SetLine(1, _lcdLine1);
}



void UpdateLcd()
{
   if (_lcdIsDirty)
   {
      UpdateLcdLine0();
      UpdateLcdLine1();
      _lcdIsDirty = false;
   }
}


void ProcessDefaultButtonPressed(bool hold)
{
   if (hold)
   {
      _values[1] = (_values[1] + 1) % 1000;
   }
   else
   {
      _values[0] = (_values[0] + 1) % 1000;
   }

   _lcdIsDirty = true;
}


void ProcessLongPressButton(bool longPress)
{
   uint8_t pos = longPress ? 7 : 6;
   _values[pos] = (_values[pos] + 1) % 1000;
   _lcdIsDirty = true;
}



void ProcessMomentaryButton(bool onOffState)
{
   _symbols[6] = onOffState ? 'X' : ' ';
   _values[6] = (_values[6] + 1) % 1000;
   _lcdIsDirty = true;
}


void ProcessToggleButton(bool onOffState)
{
   _symbols[2] = onOffState ? 'X' : ' ';
   _values[2] = (_values[2] + 1) % 1000;
   _lcdIsDirty = true;
}


void ProcessCounterButton(uint8_t currentValue)
{
   _values[4] = currentValue;
   _lcdIsDirty = true;
}


void ProcessCounterDiceButton(uint8_t currentValue)
{
   _values[6] = currentValue;
   _lcdIsDirty = true;
}


void ProcessKeyPad(char key)
{
   static uint8_t item = 0;

   _values[item] = key;

   for (int index = 0; index < 8; index++)
   {
      _symbols[index] = ' ';
   }
   _symbols[item] = '.';

   _lcdIsDirty = true;

   item = (item + 1) % 8;
}

void MyInit()
{

   //HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000U);
   //HAL_NVIC_SetPriority(SysTick_IRQn, TICK_INT_PRIORITY, 0U);

    //_lcdDisplay.I2C_Scan();
    _lcdDisplay.Init();

    // set address to 0x00
    _lcdDisplay.SetLine(0, "-"); // First char ignored
    _lcdDisplay.SetLine(0, "-LIBRARY TESTER-");
    _lcdDisplay.SetLine(1, "----- v1.0 -----");

    // Keypad init.
    _keyPad.Init();
}


int MyMain(void)
{
   // Code for testing keypad Scan speed
   uint32_t start = SysTickSubscribers::GetTick();
   for (uint32_t i = 0; i < 10000; i++)
   {
      char key = _keyPad.Scan();
   }
   uint32_t end= SysTickSubscribers::GetTick();
   char key = _keyPad.Scan();


   /*
   uint16_t key = _keyPad.Scan();
   if (key != 0)
   {
      char line2[17];
      sprintf(line2, "Key pressed: %c  ", _keyPad.GetChar(key));
      _lcdDisplay.SetLine(1, line2);
   }

   */

   /*
   for (uint8_t n = 0; n < sizeof(_dataToShift); n++)
   {
	  //LCD_SendCommand(LCD_ADDR, 0b10000000); // Line 1, address to 0
	  //LCD_SendString(LCD_ADDR, "Cx Txx Txx Cx");
	  //LCD_SendCommand(LCD_ADDR, 0b11000000); // Line 2, address to 40
	  //LCD_SendString(LCD_ADDR, "Cap 1: 10 uF");

	  _shiftRegister.ShiftOut(_dataToShift + n, 1);
	  HAL_Delay(100);
   }
   */

   return 0;
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
   // Keys
   _defaultButton.CheckTrigger(GPIO_Pin);
   _toggleButton.CheckTrigger(GPIO_Pin);
   _counterButton.CheckTrigger(GPIO_Pin);

   //_momentaryButton.CheckTrigger(GPIO_Pin);
   //_counterDiceButton.CheckTrigger(GPIO_Pin);
   _longPressButton.CheckTrigger(GPIO_Pin);
}



void HAL_Delay(uint32_t milliseconds)
{
   /* Initially clear flag */
   (void) SysTick->CTRL;
   while (milliseconds != 0)
   {
      /* COUNTFLAG returns 1 if timer counted to 0 since the last flag read */
      milliseconds -= (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) >> SysTick_CTRL_COUNTFLAG_Pos;
   }
}
