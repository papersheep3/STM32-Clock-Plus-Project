/**
  ******************************************************************************
  * @file    GUI.h
  * @brief   GUI 图形接口层
  ******************************************************************************
  */

#ifndef __GUI_H
#define __GUI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "LCD_Drive.h"
#include "Font.h"

void GUI_DrawPoint(uint16_t x,
                   uint16_t y,
                   uint16_t color);

void GUI_DrawLine(uint16_t x1,
                  uint16_t y1,
                  uint16_t x2,
                  uint16_t y2,
                  uint16_t color);

void GUI_DrawRect(uint16_t x,
                  uint16_t y,
                  uint16_t w,
                  uint16_t h,
                  uint16_t color);

void GUI_FillRect(uint16_t x,
                  uint16_t y,
                  uint16_t w,
                  uint16_t h,
                  uint16_t color);

void GUI_DrawCircle(uint16_t x0,
                    uint16_t y0,
                    uint16_t r,
                    uint16_t color);

void GUI_ShowChar(uint16_t x,
                  uint16_t y,
                  char chr,
                  sFONT *font,
                  uint16_t color,
                  uint16_t bgcolor);

void GUI_ShowString(uint16_t x,
                    uint16_t y,
                    char *str,
                    sFONT *font,
                    uint16_t color,
                    uint16_t bgcolor);

void GUI_ShowChinese(uint16_t x,
                     uint16_t y,
                     char *gbk,
                     uint16_t color,
                     uint16_t bgcolor);

void GUI_ShowImage(const unsigned char *p, uint8_t c, uint8_t k, uint8_t x, uint8_t y);

										 
#ifdef __cplusplus
}
#endif

#endif
