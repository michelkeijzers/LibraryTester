/*
 * SysTickSubscribers.cpp
 *
 *  Created on: Feb 8, 2020
 *      Author: miche
 */

#include "Framework/SysTickSubscribers.h"

#include "stdlib.h"

SysTickSubscriberData*  SysTickSubscribers::_data = 0;

uint8_t                 SysTickSubscribers::_nrOfSubscribers = 0;

uint32_t                SysTickSubscribers::_tickValue = 0;


SysTickSubscribers::SysTickSubscribers(uint8_t nrOfSubscribers)
{
   _nrOfSubscribers = nrOfSubscribers;

   _data = (SysTickSubscriberData*) malloc(_nrOfSubscribers * sizeof(SysTickSubscriberData));

   for (int subscriberIndex = 0; subscriberIndex < _nrOfSubscribers; subscriberIndex++)
   {
      _data[subscriberIndex].offset_ms = 0;
      _data[subscriberIndex].period_ms = 0;
      _data[subscriberIndex].sysTickSubscriber = NULL;
   }
}


SysTickSubscribers::~SysTickSubscribers()
{
}


/* static */ void SysTickSubscribers::SetSubscriber(uint8_t subscriberIndex, ISysTickSubscriber* subscriber)
{
   _data[subscriberIndex].sysTickSubscriber = subscriber;
   SetInterval(subscriberIndex, 0);
}


/* static */ uint16_t SysTickSubscribers::GetInterval(uint8_t subscriberIndex)
{
   return _data[subscriberIndex].period_ms;
}


/* static */ void SysTickSubscribers::SetInterval(uint8_t subscriberIndex, uint16_t period_ms)
{
   _data[subscriberIndex].offset_ms = period_ms == 0 ? 0 : _tickValue % period_ms;
   _data[subscriberIndex].period_ms = period_ms;
}


/* static */ uint32_t SysTickSubscribers::GetTick()
{
   return _tickValue;
}


/* static */ void SysTickSubscribers::OnTick(uint32_t tickValue)
{
   _tickValue = tickValue;

   for (uint8_t subscriber = 0; subscriber < _nrOfSubscribers; subscriber++)
   {
      if ((_data[subscriber].period_ms > 0) &&
          (((tickValue - _data[subscriber].offset_ms) % _data[subscriber].period_ms) == 0))
      {
         _data[subscriber].sysTickSubscriber->OnTick();
      }
   }
}


void SysTickSubscribersOnTick(uint32_t tickValue)
{
   SysTickSubscribers::OnTick(tickValue);
}
