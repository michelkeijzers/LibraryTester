/*
 * LcdDisplay.cpp
 *
 *  Created on: Jan 30, 2020
 *      Author: miche
 */

#include <Framework/LcdDisplay.h>
#include <Framework/SysTickSubscribers.h>

#include "stm32f1xx_hal.h"

// #define LCD_ADDR (0x27 << 1)

#define PIN_RS    (1 << 0)
#define PIN_EN    (1 << 2)
#define BACKLIGHT (1 << 3)

#define LCD_DELAY_MS 5



LcdDisplay::LcdDisplay(I2C_HandleTypeDef* hI2c, uint8_t i2cChannel, UPDATE_LCD_FUNCTION_PTR callbackFunction, uint16_t refreshTime, uint8_t sysTickSubscriberIndex)
: _hI2c(hI2c),
  _i2cChannel(i2cChannel << 1),
  _callbackFunction(callbackFunction)
{
   SysTickSubscribers::SetSubscriber(sysTickSubscriberIndex, this);
   SysTickSubscribers::SetInterval(sysTickSubscriberIndex, refreshTime);
}


LcdDisplay::~LcdDisplay()
{
}


void LcdDisplay::SetLine(uint8_t line, const char* text)
{
	// set address to 0x40 for second line;
   SendCommand(0b10000000 + 0x40 * line);
   SendString(text);
}


void LcdDisplay::I2C_Scan()
{
    //char info[] = "Scanning I2C bus...\r\n";
    //HAL_UART_Transmit(&huart2, (uint8_t*)info, strlen(info), HAL_MAX_DELAY);

    HAL_StatusTypeDef res;
    for (uint8_t i = 0; i < 128; i++) {
        res = HAL_I2C_IsDeviceReady(_hI2c, i << 1, 1, 10);
        if(res == HAL_OK)
        {
        	while(true) {}; // Set a breakpoint here to check the channel
        }
    }

    //HAL_UART_Transmit(&huart2, (uint8_t*)"\r\n", 2, HAL_MAX_DELAY);
}


void LcdDisplay::Init()
{
    // 4-bit mode, 2 lines, 5x7 format
    SendCommand(0b00110000);
    // display & cursor home (keep this!)
    SendCommand(0b00000010);
    // display on, right shift, underline off, blink off
    SendCommand(0b00001100);
    // clear display (optional here)
    SendCommand(0b00000001);
}


HAL_StatusTypeDef LcdDisplay::SendInternal(uint8_t data, uint8_t flags)
{
    HAL_StatusTypeDef res;
    for(;;) {
        res = HAL_I2C_IsDeviceReady(_hI2c, _i2cChannel, 1, HAL_MAX_DELAY);
        if(res == HAL_OK)
            break;
    }

    uint8_t up = data & 0xF0;
    uint8_t lo = (data << 4) & 0xF0;

    uint8_t data_arr[4];
    data_arr[0] = up|flags|BACKLIGHT|PIN_EN;
    data_arr[1] = up|flags|BACKLIGHT;
    data_arr[2] = lo|flags|BACKLIGHT|PIN_EN;
    data_arr[3] = lo|flags|BACKLIGHT;

    //res = HAL_I2C_Master_Transmit_IT(_hI2c, _i2cChannel, data_arr, sizeof(data_arr));
    res = HAL_I2C_Master_Transmit(_hI2c, _i2cChannel, data_arr, sizeof(data_arr), HAL_MAX_DELAY);
    //HAL_Delay(LCD_DELAY_MS);
    return res;
}


void LcdDisplay::SendCommand(uint8_t cmd)
{
    SendInternal(cmd, 0);
}


void LcdDisplay::SendData(uint8_t data)
{
    SendInternal(data, PIN_RS);
}


void LcdDisplay::SendString(const char *str)
{
    while(*str)
    {
        SendData((uint8_t)(*str));
        str++;
    }
}


void LcdDisplay::OnTick()
{

   (*_callbackFunction)();
}
