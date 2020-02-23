/*
 * SysTickSubscribers.h
 *
 *  Created on: Feb 8, 2020
 *      Author: Michel Keijzers
 */


#ifndef ISYSTICK_SUBSCRIBER_H_
#define ISYSTICK_SUBSCRIBER_H_

class ISysTickSubscriber
{
public:
   ISysTickSubscriber() {}
   virtual ~ISysTickSubscriber() {}
   virtual void OnTick() = 0;
};

#endif /* ISYSTICK_SUBSCRIBER_H_ */
