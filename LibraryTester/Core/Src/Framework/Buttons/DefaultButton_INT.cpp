/*
 * Button_INT.cpp
 *
 *  Created on: Feb 1, 2020
 *      Author: michel
 */

#include <Framework/Buttons/DefaultButton_INT.h>
#include <Framework/ISysTickSubscriber.h>
#include <Framework/SysTickSubscribers.h>


DefaultButton_INT::DefaultButton_INT(
   GpioId gpio, BUTTON_CALLBACK_FUNCTION_PTR callbackFunction,
   uint16_t holdDelayTime, uint16_t holdStepTime,
   uint8_t sysTickSubscriberIndex, uint8_t debounceTime)
:  BaseButton(gpio, callbackFunction, holdDelayTime, holdStepTime, debounceTime, sysTickSubscriberIndex)
{
}


DefaultButton_INT::~DefaultButton_INT()
{
}


/* override */ void DefaultButton_INT::OnButtonPressed()
{
   (*_callbackFunction)(false);
}


/* override */ void DefaultButton_INT::OnButtonReleased()
{
   // No action required
}


/* override */ void DefaultButton_INT::OnButtonHold()
{
   (*_callbackFunction)(true);
}
