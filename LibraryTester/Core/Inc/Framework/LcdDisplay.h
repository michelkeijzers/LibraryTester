/*
 * LcdDisplay.h
 *
 *  Created on: Jan 30, 2020
 *      Author: Michel Keijzers
 */

#ifndef LCD_DISPLAY_H_
#define LCD_DISPLAY_H_

#include "stm32f1xx_hal.h"

#include "Framework/ISysTickSubscriber.h"

typedef void (*UPDATE_LCD_FUNCTION_PTR)();

class LcdDisplay : ISysTickSubscriber
{
public:
	LcdDisplay(I2C_HandleTypeDef* hI2c, uint8_t i2cChannel, UPDATE_LCD_FUNCTION_PTR callbackFunction, uint16_t refreshTime, uint8_t sysTickSubscriberIndex);
	virtual ~LcdDisplay();

	void I2C_Scan();
	void Init();

	void SetLine(uint8_t line, const char* text);

private:
	HAL_StatusTypeDef SendInternal(uint8_t data, uint8_t flags);
	void SendCommand(uint8_t cmd);
	void SendData(uint8_t data);
	void SendString(const char *str);

	void OnTick();

private:
	I2C_HandleTypeDef* _hI2c;
	uint8_t _i2cChannel;

	UPDATE_LCD_FUNCTION_PTR _callbackFunction;
};

#endif /* LCD_DISPLAY_H_ */
