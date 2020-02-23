/*
 * ToggleButton_INT.h
 *
 *  Created on: Feb 1, 2020
 *      Author: Michel Keijzers
 */

#ifndef DEFAULT_BUTTON_INT_H_
#define DEFAULT_BUTTON_INT_H_

#include <Framework/Buttons/BaseButton.h>
#include <Framework/Gpio.h>
#include <Framework/SysTickSubscribers.h>

class DefaultButton_INT : public BaseButton
{
public:
   DefaultButton_INT(GpioId gpio, BUTTON_CALLBACK_FUNCTION_PTR callbackFunction,
         uint16_t holdDelayTime, uint16_t holdStepTime,
         uint8_t sysTickSubscriberIndex, uint8_t debounceTime);

	virtual ~DefaultButton_INT();

private:
   /* override */ void OnButtonPressed();
   /* override */ void OnButtonReleased();
   /* override */ void OnButtonHold();

private:
};

#endif /* DEFAULT_BUTTON_INT_H_ */
