/*
 * BaseButton.h
 *
 *  Created on: Feb 9, 2020
 *      Author: Michel Keijzers
 */

#ifndef BASE_BUTTON_H_
#define BASE_BUTTON_H_

#include "../ISysTickSubscriber.h"
#include "../Gpio.h"


class BaseButton : ISysTickSubscriber
{
public:
   typedef void (*BUTTON_CALLBACK_FUNCTION_PTR)(bool hold);

   enum EState
   {
      Released,
      PressedDebouncing,
      WaitForFirstHold,
      WaitForNextHold,
      ReleasedDebouncing
   };

   BaseButton(GpioId gpio, BUTTON_CALLBACK_FUNCTION_PTR callbackFunction, uint16_t firstHoldTime, uint16_t nextHoldTime, uint8_t debounceTime, uint8_t sysTickSubscriberIndex);

   virtual ~BaseButton();

   void CheckTrigger(uint16_t pin);

   virtual void OnTick();

   virtual void OnButtonPressed() = 0;

   virtual void OnButtonReleased() = 0;

   virtual void OnButtonHold() = 0;

protected:
   GpioId     _gpio;
   EState   _state;
   BUTTON_CALLBACK_FUNCTION_PTR _callbackFunction;
   uint16_t _firstHoldTime; // Time before hold functionality starts
   uint16_t _nextHoldTime; // Time between each hold trigger
   uint16_t _debounceTime;
   uint8_t  _sysTickSubscriberIndex;
};

#endif /* BASE_BUTTON_H_ */
