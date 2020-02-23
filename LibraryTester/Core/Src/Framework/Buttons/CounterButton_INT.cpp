/*
 * CounterButton_INT.cpp
 *
 *  Created on: Feb 1, 2020
 *      Author: michel
 */

#include <Framework/Buttons/CounterButton_INT.h>
#include <Framework/ISysTickSubscriber.h>
#include <Framework/SysTickSubscribers.h>


CounterButton_INT::CounterButton_INT(uint16_t startValue, int16_t stepValue, int16_t holdValue, uint16_t endValue,
      GpioId gpio, COUNTER_BUTTON_CALLBACK_FUNCTION_PTR callbackFunction, uint16_t holdDelayTime, uint16_t holdStepTime,
      uint8_t sysTickSubscriberIndex, uint8_t debounceTime)
:  BaseButton(gpio, NULL, holdDelayTime, holdStepTime, debounceTime, sysTickSubscriberIndex),
   _startValue(startValue),
   _stepValue(stepValue),
   _holdValue(holdValue),
   _endValue(endValue),
   _currentValue(startValue),
  _callbackFunction(callbackFunction)
{
}


CounterButton_INT::~CounterButton_INT()
{
}


/* override */ void CounterButton_INT::OnButtonPressed()
{
   UpdateValue(_stepValue);
}


/* override */ void CounterButton_INT::OnButtonReleased()
{
   // No action required
}


/* override */ void CounterButton_INT::OnButtonHold()
{
   UpdateValue(_holdValue);
}


void CounterButton_INT::UpdateValue(int16_t deltaValue)
{
   _currentValue += deltaValue;
   if (((deltaValue >= 0) && (_currentValue > _endValue)) ||
       ((deltaValue <  0) && (_currentValue < _endValue)))
   {
      _currentValue = _startValue;
   }

   (*_callbackFunction)(_currentValue);
}
