#include "stm32f10x.h"
#include "Lcd_Driver.h"
#include "main.h"
#include "GUI.h"

void LCD_GPIO_Init(void)
{

	GPIO_InitTypeDef  GPIO_InitStructure;
	      
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB ,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_10| GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
      
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA ,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4| GPIO_Pin_5| GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* Default mode */
	LCD_CS_SET;
	LCD_SCL_CLR;
	LCD_RS_SET;
	LCD_LED_SET;
	
}

void  SPI_WriteData(u8 Data)
{
	uint8_t  i=0;
	for(i=0; i<8; i++)
	{
		if(Data & 0x80)
		{
			LCD_SDA_SET; 
    }
		else 
		{
			LCD_SDA_CLR;
		}
		
    LCD_SCL_SET;
		LCD_SCL_CLR;
		
    Data <<= 1; 
	}
}


void Lcd_WriteIndex(u8 Index)
{
   LCD_CS_CLR;
	
   LCD_RS_CLR;
	
	 SPI_WriteData(Index);
	
   LCD_CS_SET;
}


void Lcd_WriteData(u8 Data)
{
   LCD_CS_CLR;
	
   LCD_RS_SET;
	
   SPI_WriteData(Data);
	
   LCD_CS_SET; 
}

void LCD_WriteData_16Bit(u16 Data)
{
   LCD_CS_CLR;
	
   LCD_RS_SET;
	
	 SPI_WriteData(Data>>8); 
	
	 SPI_WriteData(Data); 	
	
   LCD_CS_SET; 
}

void Lcd_WriteReg(u8 Index,u8 Data)
{
	Lcd_WriteIndex(Index);
  Lcd_WriteData(Data);
}

void Lcd_Reset(void)
{
	LCD_RST_CLR;
	delay_ms(100);
	
	LCD_RST_SET;
	delay_ms(120);
}

//LCD Init For 1.44Inch LCD Panel with ST7789.
void Lcd_Init(void)
{	
	LCD_GPIO_Init();
	
	Lcd_Reset(); //Reset before LCD Init.

	/* Software Reset */
	Lcd_WriteIndex(0x01);
	delay_ms(120);
	
	/* Sleep out */
	Lcd_WriteIndex(0x11);
	delay_ms (120);
	
	/* Memory Data Access Control */
	Lcd_WriteIndex(0x36);
	Lcd_WriteData(0x00);
	
	/* 16 Bits RGB565 */
	Lcd_WriteIndex(0x3A);
	Lcd_WriteData(0x55);
	
	/* Porch Set */
	Lcd_WriteIndex(0xB2);
	Lcd_WriteData(0x0C);
	Lcd_WriteData(0x0C);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x33);
	Lcd_WriteData(0x33);
	
	/* Gate Control */
  Lcd_WriteIndex(0xB7);
  Lcd_WriteData(0x35);
	
	/* VCOM set */
	Lcd_WriteIndex(0xBB);
	Lcd_WriteData(0x19);
	
  /* LCM Control */
  Lcd_WriteIndex(0xC0);
  Lcd_WriteData(0x2C);
		
	/* VDV and VRH Enable */
	Lcd_WriteIndex(0xC2);
	Lcd_WriteData(0x01);
		
	/* VRH Set */
	Lcd_WriteIndex(0xC3);
	Lcd_WriteData(0x12);
	
	/* VDV Set */
	Lcd_WriteIndex(0xC4);
	Lcd_WriteData(0x20);
		
	/* Frame Rate Contro */
	Lcd_WriteIndex(0xC6); 
	Lcd_WriteData(0x0F);
	
	/* Power Control 1 */
	Lcd_WriteIndex(0xD0); 
	Lcd_WriteData(0xA4);
	Lcd_WriteData(0xA1);

	/* Power Control 2	*/
	/*default*/
	
	/* Positive Voltage Gamma Control */
	Lcd_WriteIndex(0xE0);
	Lcd_WriteData(0xD0); 
	Lcd_WriteData(0x04); 
	Lcd_WriteData(0x0D); 
	Lcd_WriteData(0x11);
	Lcd_WriteData(0x13); 
	Lcd_WriteData(0x2B); 
	Lcd_WriteData(0x3F); 
	Lcd_WriteData(0x54);
	Lcd_WriteData(0x4C); 
	Lcd_WriteData(0x18); 
	Lcd_WriteData(0x0D); 
	Lcd_WriteData(0x0B);
	Lcd_WriteData(0x1F); 
	Lcd_WriteData(0x23);
	
	/* Negative Gamma Control */
	Lcd_WriteIndex(0xE1);
	Lcd_WriteData(0xD0); 
	Lcd_WriteData(0x04); 
	Lcd_WriteData(0x0C); 
	Lcd_WriteData(0x11); 
	Lcd_WriteData(0x13);
	Lcd_WriteData(0x2C); 
	Lcd_WriteData(0x3F); 
	Lcd_WriteData(0x44); 
	Lcd_WriteData(0x51);
	Lcd_WriteData(0x2F); 
	Lcd_WriteData(0x1F); 
	Lcd_WriteData(0x1F); 
	Lcd_WriteData(0x20);
	Lcd_WriteData(0x23); 

	/* Display Inversion on */
	Lcd_WriteIndex(0x21); 
	
	/* Display on	*/
	Lcd_WriteIndex(0x29);
	delay_ms(100);
}

void Lcd_BeginWrite(void)
{
	LCD_CS_CLR;
	LCD_RS_SET;
}

void Lcd_EndWrite(void)
{
	LCD_CS_SET;
}

void Lcd_SetRegion(u16 x_start,u16 y_start,u16 x_end,u16 y_end)
{	
	
	
	/* Column Address Set */
	Lcd_WriteIndex(0x2A);
	Lcd_WriteData(0x00);
	Lcd_WriteData(x_start);
	
	Lcd_WriteData(0x00);
	Lcd_WriteData(x_end);

	/* Row Address Set */
	Lcd_WriteIndex(0x2B);
	Lcd_WriteData(0x00);
	Lcd_WriteData(y_start);
	
	Lcd_WriteData(0x00);
	Lcd_WriteData(y_end);
	
	/* Memory Write */
	Lcd_WriteIndex(0x2C);

}


void Lcd_SetXY(u16 x, u16 y)
{
  Lcd_SetRegion(x, y, x, y);
}


void Gui_DrawPoint(u16 x, u16 y, u16 Data)
{

	Lcd_SetXY(x, y);
	
	LCD_WriteData_16Bit(Data);

}    

void Lcd_WriteColor(u16 color)
{
	SPI_WriteData(color >>8);
	SPI_WriteData(color);
	
}

void Lcd_Clear(uint16_t color)
{
    uint32_t i;

    Lcd_SetRegion(
        0,
        0,
        X_MAX_PIXEL - 1,
        Y_MAX_PIXEL - 1);

    Lcd_BeginWrite();

    for(i = 0;
        i < (X_MAX_PIXEL * Y_MAX_PIXEL);
        i++)
    {
        Lcd_WriteColor(color);
    }

    Lcd_EndWrite();
}
