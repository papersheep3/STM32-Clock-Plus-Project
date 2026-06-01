#ifndef ___LCD__DRIVER_H
#define ___LCD__DRIVER_H

#define X_MAX_PIXEL   240
#define Y_MAX_PIXEL   240

#define RED  	0xF800
#define GREEN	0x07E0
#define BLUE 	0x001F
#define WHITE	0xFFFF
#define BLACK	0x0000
#define YELLOW  0xFFE0
#define GRAY0   0xEF7D   	
#define GRAY1   0x8410      
#define GRAY2   0x4208      




#define LCD_CTRLA   	  	GPIOA		
#define LCD_CTRLB   	  	GPIOB		



#define LCD_SCL        	GPIO_Pin_5	//  PA5   <->    SCK
#define LCD_SDA        	GPIO_Pin_7	//  PA7   <->    SDA
#define LCD_CS        	GPIO_Pin_4  //  PA4   <->    CS

#define LCD_LED        	GPIO_Pin_10  //  PB10  <->      LEDA BL
#define LCD_RS         	GPIO_Pin_1	//   PB1   <->      RS	 DC
#define LCD_RST     	  GPIO_Pin_0	//   PB0   <->      RES	 RST

//#define LCD_CS_SET(x) LCD_CTRL->ODR=(LCD_CTRL->ODR&~LCD_CS)|(x ? LCD_CS:0)


#define	LCD_SCL_SET  	LCD_CTRLA->BSRR=LCD_SCL    
#define	LCD_SDA_SET  	LCD_CTRLA->BSRR=LCD_SDA   
#define	LCD_CS_SET  	LCD_CTRLA->BSRR=LCD_CS  

    
#define	LCD_LED_SET  	LCD_CTRLB->BSRR=LCD_LED   
#define	LCD_RS_SET  	LCD_CTRLB->BSRR=LCD_RS 
#define	LCD_RST_SET  	LCD_CTRLB->BSRR=LCD_RST 

#define	LCD_SCL_CLR  	LCD_CTRLA->BRR=LCD_SCL  
#define	LCD_SDA_CLR  	LCD_CTRLA->BRR=LCD_SDA 
#define	LCD_CS_CLR  	LCD_CTRLA->BRR=LCD_CS 
    
#define	LCD_LED_CLR  	LCD_CTRLB->BRR=LCD_LED 
#define	LCD_RST_CLR  	LCD_CTRLB->BRR=LCD_RST
#define	LCD_RS_CLR  	LCD_CTRLB->BRR=LCD_RS 

#define LCD_DATAOUT(x) LCD_DATA->ODR=x; 
#define LCD_DATAIN     LCD_DATA->IDR;   

#define LCD_WR_DATA(data){\
LCD_RS_SET;\
LCD_CS_CLR;\
LCD_DATAOUT(data);\
LCD_WR_CLR;\
LCD_WR_SET;\
LCD_CS_SET;\
} 



void LCD_GPIO_Init(void);
void Lcd_WriteIndex(u8 Index);
void Lcd_WriteData(u8 Data);
void Lcd_WriteReg(u8 Index,u8 Data);
u16 Lcd_ReadReg(u8 LCD_Reg);
void Lcd_Reset(void);
void Lcd_Init(void);
void Lcd_SetXY(u16 x,u16 y);
void Gui_DrawPoint(u16 x,u16 y,u16 Data);
unsigned int Lcd_ReadPoint(u16 x,u16 y);
void Lcd_SetRegion(u16 x_start,u16 y_start,u16 x_end,u16 y_end);
void LCD_WriteData_16Bit(u16 Data);
void Lcd_BeginWrite(void);
void Lcd_EndWrite(void);
void Lcd_WriteColor(u16 color);
void Lcd_Clear(uint16_t color);
void Lcd_Test(void);



#endif
