/*
 * BaseButton.cpp
 *
 *  Created on: Feb 9, 2020
 *      Author: miche
 */

#include <assert.h>

#include <Framework/Buttons/BaseButton.h>
#include <Framework/SysTickSubscribers.h>


BaseButton::BaseButton(GpioId gpio, BUTTON_CALLBACK_FUNCTION_PTR callbackFunction, uint16_t firstHoldTime, uint16_t nextHoldTime, uint8_t debounceTime, uint8_t sysTickSubscriberIndex)
: _gpio(gpio),
  _state(Released),
  _callbackFunction(callbackFunction),
  _firstHoldTime(firstHoldTime),
  _nextHoldTime(nextHoldTime),
  _debounceTime(debounceTime),
  _sysTickSubscriberIndex(sysTickSubscriberIndex)
{
   SysTickSubscribers::SetSubscriber(_sysTickSubscriberIndex, this);

   if (HAL_GPIO_ReadPin(_gpio.port, _gpio.pin) == GPIO_PIN_SET)
   {
      _state = WaitForFirstHold;
      SysTickSubscribers::SetInterval(_sysTickSubscriberIndex, _firstHoldTime);
   }
}


BaseButton::~BaseButton()
{
   // No action required
}


void BaseButton::CheckTrigger(uint16_t pin)
{
   if (pin == _gpio.pin)
   {
      switch (_state)
      {
      case EState::Released:
         if (HAL_GPIO_ReadPin(_gpio.port, _gpio.pin) == GPIO_PIN_SET)
         {
            OnButtonPressed();
            SysTickSubscribers::SetInterval(_sysTickSubscriberIndex, _debounceTime);
            _state = PressedDebouncing;
         }
         break;

      case EState::PressedDebouncing: // Fall through
      case EState::ReleasedDebouncing:
         // Ignore press/release, but reset debounce timer.
         SysTickSubscribers::SetInterval(_sysTickSubscriberIndex, _debounceTime);
         break;

      case EState::WaitForFirstHold: // Fall through
      case EState::WaitForNextHold:
         if (HAL_GPIO_ReadPin(_gpio.port, _gpio.pin) == GPIO_PIN_SET)
         {
            // Should not happen since button is already pressed
            OnButtonPressed();
            SysTickSubscribers::SetInterval(_sysTickSubscriberIndex, _debounceTime);
            _state = PressedDebouncing;
         }
         else
         {
            OnButtonReleased();
            SysTickSubscribers::SetInterval(_sysTickSubscriberIndex, _debounceTime);
            _state = ReleasedDebouncing;
         }
         break;

      default:
         assert(false);
      }
   }
}


void BaseButton::OnTick()
{
   switch (_state)
   {
   case EState::Released:
      assert(false);
      break;

   case EState::PressedDebouncing:
      if (HAL_GPIO_ReadPin(_gpio.port, _gpio.pin) == GPIO_PIN_SET)
      {
         SysTickSubscribers::SetInterval(_sysTickSubscriberIndex, _firstHoldTime);
         _state = WaitForFirstHold;;
      }
      else
      {
         // Should not happen
         SysTickSubscribers::SetInterval(_sysTickSubscriberIndex, _debounceTime);
         _state = ReleasedDebouncing;
      }
      break;

   case EState::WaitForFirstHold:
      SysTickSubscribers::SetInterval(_sysTickSubscriberIndex, _nextHoldTime);
      _state = WaitForNextHold;
      OnButtonHold();
      break;

   case EState::WaitForNextHold:
      OnButtonHold();
      break;

   case EState::ReleasedDebouncing:
      if (HAL_GPIO_ReadPin(_gpio.port, _gpio.pin) == GPIO_PIN_SET)
      {
         // Should not happen
         SysTickSubscribers::SetInterval(_sysTickSubscriberIndex, _debounceTime);
         _state = PressedDebouncing;
      }
      else
      {
         SysTickSubscribers::SetInterval(_sysTickSubscriberIndex, 0);
         _state = Released;
      }
      break;

   default:
      assert(false);
   }
}
