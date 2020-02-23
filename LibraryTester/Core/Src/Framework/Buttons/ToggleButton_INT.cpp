/*
 * ToggleButton_INT.cpp
 *
 *  Created on: Feb 1, 2020
 *      Author: michel
 */

#include <Framework/Buttons/ToggleButton_INT.h>
#include <Framework/ISysTickSubscriber.h>
#include <Framework/SysTickSubscribers.h>


ToggleButton_INT::ToggleButton_INT(GpioId gpio, BUTTON_CALLBACK_FUNCTION_PTR callbackFunction,
   uint8_t sysTickSubscriberIndex, uint8_t debounceTime)
:  BaseButton(gpio, callbackFunction, 0, 0, debounceTime, sysTickSubscriberIndex),
   _onOffState(false)
{
}

ToggleButton_INT::~ToggleButton_INT()
{
}


/* override */ void ToggleButton_INT::OnButtonPressed()
{
   _onOffState = !_onOffState;
   (*_callbackFunction)(_onOffState);
}



/* override */ void ToggleButton_INT::OnButtonReleased()
{
   // No actions required
}


/* override */ void ToggleButton_INT::OnButtonHold()
{
   // No actions required
}
