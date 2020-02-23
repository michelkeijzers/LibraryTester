/*
 * CounterButton_INT.h
 *
 *  Created on: Feb 1, 2020
 *      Author: Michel Keijzers
 */

#ifndef COUNTER_BUTTON_INT_H_
#define COUNTER_BUTTON_INT_H_

#include <Framework/Gpio.h>
#include <Framework/SysTickSubscribers.h>
#include <Framework/Buttons/BaseButton.h>

typedef void (*COUNTER_BUTTON_CALLBACK_FUNCTION_PTR)(uint8_t value);

class CounterButton_INT : public BaseButton
{
public:
   CounterButton_INT(uint16_t startValue, int16_t stepValue, int16_t holdValue, uint16_t endValue,
         GpioId gpio, COUNTER_BUTTON_CALLBACK_FUNCTION_PTR callbackFunction, uint16_t holdDelayTime, uint16_t holdStepTime,
         uint8_t sysTickSubscriberIndex, uint8_t debounceTime);

	virtual ~CounterButton_INT();

private:
   /* override */ void OnButtonPressed();
   /* override */ void OnButtonReleased();
   /* override */ void OnButtonHold();

	void UpdateValue(int16_t deltaValue);


private:
	uint16_t    _startValue;
	int16_t     _stepValue;
	int16_t     _holdValue;
	uint16_t    _endValue;
	uint16_t    _currentValue;

	COUNTER_BUTTON_CALLBACK_FUNCTION_PTR _callbackFunction;

   void UpdateValue();
};

#endif /* COUNTER_BUTTON_INT_H_ */
