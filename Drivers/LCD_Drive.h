/**
  ******************************************************************************
  * @file    LCD_Drive.h
  * @brief   ST7789 LCD 底层驱动头文件
  * @author  ChatGPT
  * @date    2026
  *
  * 适用硬件：
  *   MCU  : STM32F103C8T6
  *   LCD  : ST7789 240x240 TFT
  *   接口 : SPI1 硬件 SPI
  *
  ******************************************************************************
  */

#ifndef __LCD_DRIVE_H
#define __LCD_DRIVE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f10x.h"

/* LCD 分辨率 */
#define LCD_WIDTH      240
#define LCD_HEIGHT     240

/* RGB565 常用颜色 */
#define WHITE          0xFFFF
#define BLACK          0x0000
#define BLUE           0x001F
#define RED            0xF800
#define GREEN          0x07E0
#define CYAN           0x7FFF
#define YELLOW         0xFFE0
#define MAGENTA        0xF81F
#define GRAY           0x8430

/*---------------- GPIO 引脚定义 ----------------*/

/* SPI1:
 * SCK  -> PA5
 * MOSI -> PA7
 */

/* CS */
#define LCD_CS_PORT        GPIOA
#define LCD_CS_PIN         GPIO_Pin_4

/* DC */
#define LCD_DC_PORT        GPIOB
#define LCD_DC_PIN         GPIO_Pin_1

/* RST */
#define LCD_RST_PORT       GPIOB
#define LCD_RST_PIN        GPIO_Pin_0

/* BLK */
#define LCD_BLK_PORT       GPIOB
#define LCD_BLK_PIN        GPIO_Pin_10

/* GPIO 操作宏 */
#define LCD_CS_LOW()       GPIO_ResetBits(LCD_CS_PORT, LCD_CS_PIN)
#define LCD_CS_HIGH()      GPIO_SetBits(LCD_CS_PORT, LCD_CS_PIN)

#define LCD_DC_LOW()       GPIO_ResetBits(LCD_DC_PORT, LCD_DC_PIN)
#define LCD_DC_HIGH()      GPIO_SetBits(LCD_DC_PORT, LCD_DC_PIN)

#define LCD_RST_LOW()      GPIO_ResetBits(LCD_RST_PORT, LCD_RST_PIN)
#define LCD_RST_HIGH()     GPIO_SetBits(LCD_RST_PORT, LCD_RST_PIN)

#define LCD_BLK_ON()       GPIO_SetBits(LCD_BLK_PORT, LCD_BLK_PIN)
#define LCD_BLK_OFF()      GPIO_ResetBits(LCD_BLK_PORT, LCD_BLK_PIN)

/* 显示方向 */
typedef enum
{
    LCD_DIR_0 = 0,
    LCD_DIR_90,
    LCD_DIR_180,
    LCD_DIR_270
}LCD_Direction_t;

/* 函数声明 */
void LCD_Init(void);

void LCD_WriteCommand(uint8_t cmd);
void LCD_WriteData8(uint8_t data);
void LCD_WriteData16(uint16_t data);

void LCD_SetAddress(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye);

void LCD_Clear(uint16_t color);

void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color);

void LCD_SetDirection(LCD_Direction_t dir);

void LCD_BackLight(uint8_t state);

void LCD_Fill(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye, uint16_t color);

#ifdef __cplusplus
}
#endif

#endif
