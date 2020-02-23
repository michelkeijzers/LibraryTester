/*
 * ShiftRegister.cpp
 *
 *  Created on: Jan 30, 2020
 *      Author: michel
 */

#include <Framework/ShiftRegister.h>

ShiftRegister::ShiftRegister(SPI_HandleTypeDef* hSpi, GPIO_TypeDef* latchPort, uint16_t latchPin)
: _hSpi(hSpi),
  _latchPort(latchPort),
  _latchPin(latchPin)
{
}

ShiftRegister::~ShiftRegister()
{
}


void ShiftRegister::ShiftOut(uint8_t* data, uint16_t size)
{
	// Latch pin LOW
	HAL_GPIO_WritePin(_latchPort, _latchPin, GPIO_PIN_RESET);

	// Write data with SPI.
	HAL_StatusTypeDef res = HAL_SPI_Transmit(_hSpi, data, size, 1);
	if (res != HAL_OK)
	{
		while(1);
	}

	// Once data is ready, set latch pin to HIGH
	HAL_GPIO_WritePin(_latchPort, _latchPin, GPIO_PIN_SET);
}
