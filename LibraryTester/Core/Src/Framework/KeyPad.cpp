/*
 * KeyPad_INT.cpp
 *
 *  Created on: Feb 1, 2020
 *      Author: michel
 *
 * If pollTime = 10 ms, do not use a debounce speed of 10 ms (use 20 ms)
 */


#include <assert.h>
#include <Framework/KeyPad.h>
#include <Framework/Gpio.h>
#include <stdlib.h>
#include <string.h>


KeyPad::KeyPad(uint8_t nrOfRows, uint8_t nrOfColumns, const char* keys, const GpioId rows[], const GpioId columns[],
      KEY_PAD_CALLBACK_FUNCTION_PTR callbackFunction,
      uint8_t pollTime, uint16_t firstHoldTime, uint16_t nextHoldTime,
      uint8_t debounceTime, uint8_t sysTickSubscriberIndex)
: _nrOfRows(nrOfRows),
  _nrOfColumns(nrOfColumns),
  _callbackFunction(callbackFunction),
  _firstHoldTime(firstHoldTime),
  _nextHoldTime(nextHoldTime),
  _debounceTime(debounceTime),
  _sysTickSubscriberIndex(sysTickSubscriberIndex),
  _state(Idle),
  _timer(0),
  _previousKey(' ')
{
   SysTickSubscribers::SetSubscriber(_sysTickSubscriberIndex, this);
   SysTickSubscribers::SetInterval(_sysTickSubscriberIndex, pollTime);

   assert (nrOfRows <= MAX_NR_OF_KEY_PAD_ROWS);
   assert (nrOfColumns <= MAX_NR_OF_KEY_PAD_COLUMNS);

   for (uint8_t row = 0; row < _nrOfRows; row++)
   {
      _rows[row] = rows[row];
   }

   for (uint8_t column = 0; column < _nrOfColumns; column++)
   {
      _columns[column] = columns[column];
   }

   _keys = (char*) malloc(_nrOfRows * _nrOfColumns);
   if (_keys != NULL)
   {
      strncpy(_keys, keys, _nrOfRows * _nrOfColumns);
   }
   else
   {
      assert(false);
   }
}


KeyPad::~KeyPad()
{
   // No functionality needed
}


void KeyPad::Init()
{
   SetRows();
   for (uint8_t row = 0; row < _nrOfRows; row++)
   {
      HAL_GPIO_WritePin(_rows[row].port, _rows[row].pin, GPIO_PIN_RESET);
      //RESET_GPIO_PIN(_rows[row].port, _rows[row].pin);
   }
}


/**
 * Hold state before main state, because main state affects hold state
 */
/* override */ void KeyPad::OnTick()
{
   switch (_state)
   {
   case Idle:                   // Fall through
   {
      char key = Scan();
      if (key != ' ')
      {
         _previousKey = key;
         _timer = SysTickSubscribers::GetTick();
         _state = DebouncingPressed;
      }
      break;
   }

   case DebouncingPressed:
   {
      char key = Scan();
      if (key == ' ')
      {
         _timer = SysTickSubscribers::GetTick();
         _state = DebouncingReleased;
      }
      else if (key != _previousKey)
      {
         _previousKey = key;
         _timer = SysTickSubscribers::GetTick();
      }
      else if (SysTickSubscribers::GetTick() >= _timer + _debounceTime)
      {
         (*_callbackFunction)(key);
         _timer = SysTickSubscribers::GetTick();
         _state = WaitForFirstHold;
      }
      break;
   }

   case DebouncingReleased:
   {
      char key = Scan();

      if (SysTickSubscribers::GetTick() >= _timer + _debounceTime)
      {
         if (key == ' ')
         {
            _state = Idle;
         }
         else
         {
            _previousKey = key;
            _timer = SysTickSubscribers::GetTick();
            _state = DebouncingPressed;
         }
      }
      break;
   }

   case WaitForFirstHold:
   {
      char key = Scan();
      if (key == ' ')
      {
         _timer = SysTickSubscribers::GetTick();
         _state = DebouncingReleased;
      }
      else if (key != _previousKey)
      {
         _previousKey = key;
         _timer = SysTickSubscribers::GetTick();
         _state = DebouncingPressed;
      }
      else if (SysTickSubscribers::GetTick() >= _timer + _firstHoldTime)
      {
         (*_callbackFunction)(key);
         _timer = SysTickSubscribers::GetTick();
         _state = WaitForNextHold;
      }
      break;
   }

   case WaitForNextHold:
   {
      char key = Scan();
      if (key == ' ')
      {
         _timer = SysTickSubscribers::GetTick();
         _state = DebouncingReleased;
      }
      else if (key != _previousKey)
      {
         _previousKey = key;
         _timer = SysTickSubscribers::GetTick();
         _state = DebouncingPressed;
      }
      else if (SysTickSubscribers::GetTick() >= _timer + _nextHoldTime)
      {
         (*_callbackFunction)(key);
         _timer = SysTickSubscribers::GetTick();
      }
      break;
   }

   default:
      assert(false);
      break;
   }
}


/**
 * The execution time of this function is 21.1 us on a 72 MHz STM32F103C8T6
 * It can be improved slightly by copying GetLowColumn and SetRows inside this function.
 */
char KeyPad::Scan()
{
   char pressedChar = ' ';

   SetRows();

   for (uint8_t row = 0; row < _nrOfRows; row++)
   {
      HAL_GPIO_WritePin(_rows[row].port, _rows[row].pin, GPIO_PIN_RESET);
      int8_t columnLow = GetLowColumn();
      //HAL_GPIO_WritePin(_rows[row].port, _rows[row].pin, GPIO_PIN_SET);
      SET_GPIO_PIN(_rows[row].port, _rows[row].pin);

      if (columnLow != -1)
      {
         pressedChar = _keys[row * _nrOfRows + columnLow];
         break;
      }
   }

   SetRows();

   return pressedChar;
}



int8_t KeyPad::GetLowColumn()
{
   for (uint8_t column = 0; column < _nrOfColumns; column++)
   {
      //if (HAL_GPIO_ReadPin(_columns[column].port, _columns[column].pin) == GPIO_PIN_RESET)
      if (IS_GPIO_PIN_RESET(_columns[column].port, _columns[column].pin))
      {
         return column;
      }
   }

   return -1;
}


void KeyPad::SetRows()
{
   for (uint8_t row = 0; row < _nrOfRows; row++)
   {
      HAL_GPIO_WritePin(_rows[row].port, _rows[row].pin, GPIO_PIN_SET);
      //SET_GPIO_PIN(_rows[row].port, _rows[row].pin);
      //GPIOx->BSRR = (uint32_t)GPIO_Pin << 16u;
   }
}
