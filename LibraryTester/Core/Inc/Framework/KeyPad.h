/*
 * KeyPad_INT.h
 *
 *  Created on: Feb 1, 2020
 *      Author: Michel Keijzers
 */

#ifndef SRC_KEYPAD_INT_H_
#define SRC_KEYPAD_INT_H_

#include <stdint.h>

#include <Framework/SysTickSubscribers.h>

#include "Gpio.h"


typedef void (*KEY_PAD_CALLBACK_FUNCTION_PTR)(char key);

/**
 * According https://github.com/nimaltd/KeyPad/blob/master/KeyPad.c
 * but rows and columns swapped
 */

enum EState
{
   Idle,
   DebouncingPressed,
   WaitForFirstHold,
   WaitForNextHold,
   DebouncingReleased
};


class KeyPad : ISysTickSubscriber
{
public:
   static const uint8_t MAX_NR_OF_KEY_PAD_ROWS = 4;
   static const uint8_t MAX_NR_OF_KEY_PAD_COLUMNS = 4;

   KeyPad(uint8_t nrOfRows, uint8_t nrOfColumns, const char* keys, const GpioId rows[], const GpioId columns[],
         KEY_PAD_CALLBACK_FUNCTION_PTR callbackFunction,
         uint8_t pollTime, uint16_t firstHoldTime, uint16_t nextHoldTime,
         uint8_t debounceTime, uint8_t sysTickSubscriberIndex);

   virtual ~KeyPad();

   void Init();

   /* override */ void OnTick();

   char Scan();
private:

   int8_t GetLowColumn();

   void SetRows();

   uint8_t     _nrOfRows;
   uint8_t     _nrOfColumns;

   char* _keys;

   GpioId _rows[MAX_NR_OF_KEY_PAD_ROWS];
   GpioId _columns[MAX_NR_OF_KEY_PAD_COLUMNS];

   KEY_PAD_CALLBACK_FUNCTION_PTR _callbackFunction;

   uint16_t _firstHoldTime; // Time before hold functionality starts
   uint16_t _nextHoldTime; // Time between each hold trigger
   uint16_t _debounceTime;

   uint8_t  _sysTickSubscriberIndex;

   EState _state;
   uint32_t _timer; // FOr keeping track for debouncing and hold states
   char _previousKey;
};

#endif /* SRC_KEYPAD_INT_H_ */
