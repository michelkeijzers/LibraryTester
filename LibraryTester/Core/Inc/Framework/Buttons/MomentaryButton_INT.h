/*
 * ToggleButton_INT.h
 *
 *  Created on: Feb 1, 2020
 *      Author: Michel Keijzers
 */

#ifndef MOMENTARY_BUTTON_INT_H_
#define MOMENTARY_BUTTON_INT_H_

#include <Framework/Buttons/BaseButton.h>
#include <Framework/Gpio.h>
#include <Framework/SysTickSubscribers.h>

typedef void (*MOMENTARY_BUTTON_CALLBACK_FUNCTION_PTR)(bool buttonState);

class MomentaryButton_INT : public BaseButton
{
public:
   MomentaryButton_INT(GpioId pin, MOMENTARY_BUTTON_CALLBACK_FUNCTION_PTR callbackFunction,
     uint8_t sysTickSubscriberIndex, uint8_t period);

	virtual ~MomentaryButton_INT();

   /* override */ void OnButtonPressed();
   /* override */ void OnButtonReleased();
   /* override */ void OnButtonHold();
};

#endif /* MOMENTARY_BUTTON_INT_H_ */
