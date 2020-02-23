/*
 * ShiftRegister.h
 *
 *  Created on: Jan 30, 2020
 *      Author: Michel Keijzers
 */

#ifndef SHIFT_REGISTER_H_
#define SHIFT_REGISTER_H_

#include "stm32f1xx_hal.h"

class ShiftRegister
{
public:
	ShiftRegister(SPI_HandleTypeDef* hSpi, GPIO_TypeDef* latchPort, uint16_t latchPin);
	virtual ~ShiftRegister();

	void ShiftOut(uint8_t* data, uint16_t size);

private:
	SPI_HandleTypeDef* _hSpi;

	GPIO_TypeDef*      _latchPort;
	uint16_t           _latchPin;
};

#endif /* SHIFT_REGISTER_H_ */
