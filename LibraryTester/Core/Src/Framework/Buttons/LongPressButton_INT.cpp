/*
 * Button_INT.cpp
 *
 *  Created on: Feb 1, 2020
 *      Author: michel
 */

#include <Framework/Buttons/LongPressButton_INT.h>
#include <Framework/ISysTickSubscriber.h>
#include <Framework/SysTickSubscribers.h>


LongPressButton_INT::LongPressButton_INT(
 GpioId gpio,
 uint16_t longPressTime, BUTTON_CALLBACK_FUNCTION_PTR longPressCallbackFunction,
 uint8_t sysTickSubscriberIndex, uint8_t debounceTime)
: BaseButton(gpio, longPressCallbackFunction, longPressTime - debounceTime, 0, debounceTime, sysTickSubscriberIndex)
{
}


LongPressButton_INT::~LongPressButton_INT()
{
}


/* virtual */ void LongPressButton_INT::OnButtonPressed()
{
   // No action required
}


/* virtual */ void LongPressButton_INT::OnButtonReleased()
{
   (*_callbackFunction)(false);
}


/* virtual */ void LongPressButton_INT::OnButtonHold()
{
   (*_callbackFunction)(true);
   SysTickSubscribers::SetInterval(_sysTickSubscriberIndex, 0);
   _state = PressedDebouncing;
}
