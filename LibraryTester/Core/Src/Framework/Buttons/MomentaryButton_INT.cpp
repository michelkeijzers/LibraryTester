/*
 * Button_INT.cpp
 *
 *  Created on: Feb 1, 2020
 *      Author: michel
 */

#include <Framework/Buttons/MomentaryButton_INT.h>
#include <Framework/ISysTickSubscriber.h>
#include <Framework/SysTickSubscribers.h>


MomentaryButton_INT::MomentaryButton_INT(GpioId gpio, MOMENTARY_BUTTON_CALLBACK_FUNCTION_PTR callbackFunction,
   uint8_t sysTickSubscriberIndex, uint8_t debounceTime)
:  BaseButton(gpio, callbackFunction, 0, 0, debounceTime, sysTickSubscriberIndex)
{
}


MomentaryButton_INT::~MomentaryButton_INT()
{
}


/* override */ void MomentaryButton_INT::OnButtonPressed()
{
   (*_callbackFunction)(true);
}


/* override */ void MomentaryButton_INT::OnButtonReleased()
{
   (*_callbackFunction)(false);
}



/* override */ void MomentaryButton_INT::OnButtonHold()
{
   // No actions required
}
