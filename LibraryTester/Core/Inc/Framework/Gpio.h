/*
 * Gpio.h
 *
 *  Created on: Feb 2, 2020
 *      Author: Michel Keijzers
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "stm32f1xx_hal.h"

typedef struct
{
   GPIO_TypeDef* port;
   uint16_t      pin;
} GpioId;


// Functions are too costly (parameter passing).
#define IS_GPIO_PIN_RESET(port, pin) (((port)->IDR) & (pin)) == GPIO_PIN_RESET
#define IS_GPIO_PIN_SET(port, pin) (((port)->IDR) & (pin)) != GPIO_PIN_RESET
#define RESET_GPIO_PIN(port, pin) (port)->BSRR = (pin);
#define SET_GPIO_PIN(port, pin) (port)->BSRR = ((uint32_t) (pin)) << 16u

#endif /* GPIO_H_ */
