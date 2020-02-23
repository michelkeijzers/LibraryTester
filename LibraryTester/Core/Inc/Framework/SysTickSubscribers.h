/*
 * SysTickSubscribers.h
 *
 *  Created on: Feb 8, 2020
 *      Author: Michel Keijzers
 */

#ifndef SYSTICK_SUBSCRIBERS_H_
#define SYSTICK_SUBSCRIBERS_H_

#include <stdint.h>

#include "MyMain.h"
#include "ISysTickSubscriber.h"


extern "C"
{
   void SysTickSubscribersOnTick(uint32_t tickValue);
}


struct SysTickSubscriberData
{
   uint16_t             offset_ms;
   uint16_t             period_ms;
   ISysTickSubscriber*  sysTickSubscriber;
};



class SysTickSubscribers
{
public:
   SysTickSubscribers(uint8_t nrOfSubscribers);
   virtual ~SysTickSubscribers();

   static void SetSubscriber(uint8_t subscriberIndex, ISysTickSubscriber* subscriber);

   static uint16_t GetInterval(uint8_t subscriberIndex);
   static void SetInterval(uint8_t subscriberIndex, uint16_t period_ms);

   static void OnTick(uint32_t tickValue);

   static uint32_t GetTick();

private:
   static SysTickSubscriberData* _data; // Dynamic array

   static uint8_t _nrOfSubscribers;

   static uint32_t _tickValue;
};

extern SysTickSubscribers sysTicksubscribers;


#endif /* SYSTICK_SUBSCRIBERS_H_ */

