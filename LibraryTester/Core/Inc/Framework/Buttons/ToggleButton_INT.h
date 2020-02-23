/*
 * ToggleButton_INT.h
 *
 *  Created on: Feb 1, 2020
 *      Author: Michel Keijzers
 */

#ifndef TOGGLE_BUTTON_INT_H_
#define TOGGLE_BUTTON_INT_H_

#include <Framework/Buttons/BaseButton.h>
#include <Framework/Gpio.h>
#include <Framework/SysTickSubscribers.h>

/**
 * Callback function is misused: false: Off, true: On
 */
typedef void (*TOGGLE_BUTTON_CALLBACK_FUNCTION_PTR)(bool onOffState);

class ToggleButton_INT : public BaseButton
{
public:
   ToggleButton_INT(GpioId pin, BUTTON_CALLBACK_FUNCTION_PTR callbackFunction,
     uint8_t sysTickSubscriberIndex, uint8_t period);

	virtual ~ToggleButton_INT();

private:
   /* override */ void OnButtonPressed();
   /* override */ void OnButtonReleased();
   /* override */ void OnButtonHold();

private:
	bool _onOffState;
};

#endif /* TOGGLE_BUTTON_INT_H_ */
