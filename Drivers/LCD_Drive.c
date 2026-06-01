/**
  ******************************************************************************
  * @file    LCD_Drive.c
  * @brief   ST7789 LCD 底层驱动
  ******************************************************************************
  */

#include "LCD_Drive.h"
#include "main.h"

/**
  * @brief SPI1 初始化
  */
static void LCD_SPI_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef SPI_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1 | RCC_APB2Periph_GPIOB, ENABLE);

    /* SPI 引脚 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* 普通 GPIOA */
    GPIO_InitStructure.GPIO_Pin = LCD_CS_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
		
	  /* 普通 GPIOB */
	  GPIO_InitStructure.GPIO_Pin = LCD_DC_PIN | LCD_RST_PIN | LCD_BLK_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler =SPI_BaudRatePrescaler_2;	    /* 72MHz / 2 = 36MHz */
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitStructure);

    SPI_Cmd(SPI1, ENABLE);

		/* 初始化电平 */
    LCD_CS_HIGH();
		LCD_DC_HIGH();
		LCD_RST_HIGH();
		LCD_BLK_OFF();
}

/**
  * @brief SPI 发送 1 字节
  */
static void LCD_SPI_SendByte(uint8_t data)
{
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

    SPI_I2S_SendData(SPI1, data);

    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
}

/**
  * @brief 写命令
  */
void LCD_WriteCommand(uint8_t cmd)
{
    LCD_DC_LOW();
    LCD_CS_LOW();

    LCD_SPI_SendByte(cmd);

    LCD_CS_HIGH();
}

/**
  * @brief 写 8 位数据
  */
void LCD_WriteData8(uint8_t data)
{
    LCD_DC_HIGH();
    LCD_CS_LOW();

    LCD_SPI_SendByte(data);

    LCD_CS_HIGH();
}

/**
  * @brief 写 16 位数据
  */
void LCD_WriteData16(uint16_t data)
{
    LCD_DC_HIGH();
    LCD_CS_LOW();

    LCD_SPI_SendByte(data >> 8);
    LCD_SPI_SendByte(data);

    LCD_CS_HIGH();
}

/**
  * @brief 设置显示窗口
  */
void LCD_SetAddress(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye)
{
    LCD_WriteCommand(0x2A);

    LCD_WriteData16(xs);
    LCD_WriteData16(xe);

    LCD_WriteCommand(0x2B);

    LCD_WriteData16(ys);
    LCD_WriteData16(ye);

    LCD_WriteCommand(0x2C);
}

/**
  * @brief LCD 填充区域
  */
void LCD_Fill(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye, uint16_t color)
{
    uint32_t i;
    uint32_t total;

    total = (xe - xs + 1) * (ye - ys + 1);

    LCD_SetAddress(xs, ys, xe, ye);

    LCD_DC_HIGH();
    LCD_CS_LOW();

    for(i = 0; i < total; i++)
    {
        LCD_SPI_SendByte(color >> 8);
        LCD_SPI_SendByte(color);
    }

    LCD_CS_HIGH();
}

/**
  * @brief 清屏
  */
void LCD_Clear(uint16_t color)
{
    LCD_Fill(0, 0, 239, 239, color);
}

/**
  * @brief 画点
  */
void LCD_DrawPoint(uint16_t x,
                   uint16_t y,
                   uint16_t color)
{
    LCD_SetAddress(x, y, x, y);

    LCD_WriteData16(color);
}

/**
  * @brief 背光控制
  */
void LCD_BackLight(uint8_t state)
{
    if(state)
    {
        LCD_BLK_ON();
    }
    else
    {
        LCD_BLK_OFF();
    }
}

/**
  * @brief 设置显示方向
  */
void LCD_SetDirection(LCD_Direction_t dir)
{
	
    LCD_WriteCommand(0x36);

    switch(dir)
    {
        case LCD_DIR_0:
            LCD_WriteData8(0x00);
            break;

        case LCD_DIR_90:
            LCD_WriteData8(0x70);
            break;

        case LCD_DIR_180:
            LCD_WriteData8(0xC0);
            break;

        case LCD_DIR_270:
            LCD_WriteData8(0xA0);
            break;
    }
}

/**
  * @brief LCD 初始化
  */
void LCD_Init(void)
{
	
		/* 硬件SPI初始化 */
    LCD_SPI_Init();

    /* 硬复位 */
    LCD_RST_LOW();
    delay_ms(100);

    LCD_RST_HIGH();
    delay_ms(100);

    /* Sleep Out */
    LCD_WriteCommand(0x11);
    delay_ms(120);

    /* RGB565 */
    LCD_WriteCommand(0x3A);
    LCD_WriteData8(0x55);
	
		/* Display Inversion on */
		LCD_WriteCommand(0x21);

    /* Memory Access */
    LCD_SetDirection(LCD_DIR_0);

    /* Porch Setting */
    LCD_WriteCommand(0xB2);
    LCD_WriteData8(0x0C);
    LCD_WriteData8(0x0C);
    LCD_WriteData8(0x00);
    LCD_WriteData8(0x33);
    LCD_WriteData8(0x33);

    /* Gate Control */
    LCD_WriteCommand(0xB7);
    LCD_WriteData8(0x35);

    /* VCOM */
    LCD_WriteCommand(0xBB);
    LCD_WriteData8(0x19);

    /* LCM */
    LCD_WriteCommand(0xC0);
    LCD_WriteData8(0x2C);

    /* VDV VRH */
    LCD_WriteCommand(0xC2);
    LCD_WriteData8(0x01);

    /* VRH */
    LCD_WriteCommand(0xC3);
    LCD_WriteData8(0x12);

    /* VDV */
    LCD_WriteCommand(0xC4);
    LCD_WriteData8(0x20);

    LCD_WriteCommand(0xC6);
    LCD_WriteData8(0x0F);

    LCD_WriteCommand(0xD0);
    LCD_WriteData8(0xA4);
    LCD_WriteData8(0xA1);

    /* Positive Voltage Gamma */
    LCD_WriteCommand(0xE0);

    LCD_WriteData8(0xD0);
    LCD_WriteData8(0x04);
    LCD_WriteData8(0x0D);
    LCD_WriteData8(0x11);
    LCD_WriteData8(0x13);
    LCD_WriteData8(0x2B);
    LCD_WriteData8(0x3F);
    LCD_WriteData8(0x54);
    LCD_WriteData8(0x4C);
    LCD_WriteData8(0x18);
    LCD_WriteData8(0x0D);
    LCD_WriteData8(0x0B);
    LCD_WriteData8(0x1F);
    LCD_WriteData8(0x23);

    /* Negative Voltage Gamma */
    LCD_WriteCommand(0xE1);

    LCD_WriteData8(0xD0);
    LCD_WriteData8(0x04);
    LCD_WriteData8(0x0C);
    LCD_WriteData8(0x11);
    LCD_WriteData8(0x13);
    LCD_WriteData8(0x2C);
    LCD_WriteData8(0x3F);
    LCD_WriteData8(0x44);
    LCD_WriteData8(0x51);
    LCD_WriteData8(0x2F);
    LCD_WriteData8(0x1F);
    LCD_WriteData8(0x1F);
    LCD_WriteData8(0x20);
    LCD_WriteData8(0x23);

    /* Display ON */
    LCD_WriteCommand(0x29);

    LCD_BackLight(1);

    LCD_Clear(RED);
}
