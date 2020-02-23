/*
 * ToggleButton_INT.h
 *
 *  Created on: Feb 1, 2020
 *      Author: Michel Keijzers
 */

#ifndef LONG_PRESS_BUTTON_INT_H_
#define LONG_PRESS_BUTTON_INT_H_

#include <Framework/Buttons/BaseButton.h>
#include <Framework/Gpio.h>
#include <Framework/SysTickSubscribers.h>


/**
 * Use (first) hold time for long press functionality
 */
class LongPressButton_INT : public BaseButton
{
public:
   LongPressButton_INT(
    GpioId gpio,
    uint16_t longPressTime, BUTTON_CALLBACK_FUNCTION_PTR longPressCallbackFunction,
    uint8_t sysTickSubscriberIndex, uint8_t debounceTime);

   virtual ~LongPressButton_INT();

protected:

   /* override */ void OnButtonPressed();
   /* override */ void OnButtonReleased();
   /* override */ void OnButtonHold();

private:
};

#endif /* LONG_PRESS_BUTTON_INT_H_ */
