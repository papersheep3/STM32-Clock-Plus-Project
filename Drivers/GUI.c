/**
  ******************************************************************************
  * @file    GUI.c
  * @brief   GUI 图形接口实现
  ******************************************************************************
  */

#include "GUI.h"
#include <string.h>

void GUI_DrawPoint(uint16_t x, uint16_t y, uint16_t color)
{
    LCD_DrawPoint(x, y, color);
}

/**
  * @brief Bresenham 画线算法
  */
void GUI_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    int dx, dy;
    int sx, sy;
    int err, e2;

    dx = (x2 > x1) ? (x2 - x1) : (x1 - x2);
    dy = (y2 > y1) ? (y2 - y1) : (y1 - y2);

    sx = (x1 < x2) ? 1 : -1;
    sy = (y1 < y2) ? 1 : -1;

    err = dx - dy;

    while(1)
    {
        GUI_DrawPoint(x1, y1, color);

        if(x1 == x2 && y1 == y2)
            break;

        e2 = err << 1;

        if(e2 > -dy)
        {
            err -= dy;
            x1 += sx;
        }

        if(e2 < dx)
        {
            err += dx;
            y1 += sy;
        }
    }
}

/**
  * @brief 画矩形
  */
void GUI_DrawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
    GUI_DrawLine(x, y, x + w, y, color);
    GUI_DrawLine(x, y, x, y + h, color);
    GUI_DrawLine(x + w, y, x + w, y + h, color);
    GUI_DrawLine(x, y + h, x + w, y + h, color);
}

/**
  * @brief 填充矩形
  */
void GUI_FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
    LCD_Fill(x,
             y,
             x + w - 1,
             y + h - 1,
             color);
}

/**
  * @brief 中点画圆法
  */
void GUI_DrawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color)
{
    int x = 0;
    int y = r;
    int d = 1 - r;

    while(x <= y)
    {
        GUI_DrawPoint(x0 + x, y0 + y, color);
        GUI_DrawPoint(x0 - x, y0 + y, color);
        GUI_DrawPoint(x0 + x, y0 - y, color);
        GUI_DrawPoint(x0 - x, y0 - y, color);

        GUI_DrawPoint(x0 + y, y0 + x, color);
        GUI_DrawPoint(x0 - y, y0 + x, color);
        GUI_DrawPoint(x0 + y, y0 - x, color);
        GUI_DrawPoint(x0 - y, y0 - x, color);

        if(d < 0)
        {
            d += 2 * x + 3;
        }
        else
        {
            d += 2 * (x - y) + 5;
            y--;
        }

        x++;
    }
}

/**
  * @brief 显示 ASCII 字符
  */
void GUI_ShowChar(uint16_t x, uint16_t y, char chr, sFONT *font, uint16_t color, uint16_t bgcolor)
{
    uint8_t i, j;
    uint8_t temp;
    uint16_t bytes_per_line;

    chr -= 32;

    bytes_per_line = (font->Width + 7) / 8;

    for(i = 0; i < font->Height; i++)
    {
        for(j = 0; j < font->Width; j++)
        {
            temp = font->table[chr* font->Height *bytes_per_line + i*bytes_per_line + j/8];

            if(temp & (0x80 >> (j % 8)))
            {
                GUI_DrawPoint(x + j, y + i, color);
            }
            else
            {
                GUI_DrawPoint(x + j,y + i,bgcolor);
            }
        }
    }
}

/**
  * @brief 显示字符串
  */
void GUI_ShowString(uint16_t x, uint16_t y, char *str, sFONT *font, uint16_t color, uint16_t bgcolor)
{
    while(*str)
    {
        GUI_ShowChar(x, y, *str, font, color, bgcolor);

        x += font->Width;

        str++;
    }
}

/**
  * @brief 显示 GBK 汉字
  */
void GUI_ShowChinese(uint16_t x, uint16_t y, char *gbk, uint16_t color, uint16_t bgcolor)
{
    uint8_t i, j;
    uint8_t temp;

    for(i = 0; i < FONT_GBK_NUM; i++)
    {
        if(gbk[0] == tfont16[i].Index[0] &&
           gbk[1] == tfont16[i].Index[1])
        {
            for(j = 0; j < 32; j++)
            {
                temp = tfont16[i].Msk[j];

                uint8_t bit;

                for(bit = 0; bit < 8; bit++)
                {
                    if(temp & (0x80 >> bit))
                    {
                        GUI_DrawPoint(x + ((j % 2) * 8 + bit),y + (j / 2),color);
                    }
                    else
                    {
                        GUI_DrawPoint(x + ((j % 2) * 8 + bit),y + (j / 2),bgcolor);
                    }
                }
            }

            return;
        }
    }
}

void GUI_ShowImage(const unsigned char *p, uint8_t c, uint8_t k, uint8_t x, uint8_t y)
{
		uint32_t i, picL, picH;
		LCD_SetAddress(x, y, x+(c-1), y+(k-1));
    for(i=0; i<c*k; i++)
	  {	
			picL = *(p + i*2);
			picH = *(p + i*2+1);				
			LCD_WriteData16(picH<<8|picL);  						
	  }		
}
