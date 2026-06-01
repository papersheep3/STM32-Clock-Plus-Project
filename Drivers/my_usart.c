#include "stm32f10x.h"                  // Device header
#include "my_usart.h"
#include "string.h"

static myusart_receive_callback_t myusart_receive_callback;
static myusart_send_finish_callback_t myusart_send_finish_callback;

void myusart_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = DMA1_Channel4_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);
	
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = 9600;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_Even;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	
	
	USART_Init(USART1, &USART_InitStruct);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	USART_Cmd(USART1, ENABLE);

	
}

void myusart_SendArray_async(uint16_t *Array, uint8_t len)
{
	DMA_InitTypeDef DMA_InitStruct;
	DMA_InitStruct.DMA_BufferSize = len;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStruct.DMA_M2M = DMA_M2M_Enable;
	DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)Array;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Disable;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;
	
	
	DMA_Init(DMA1_Channel4, &DMA_InitStruct);	
	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);
	DMA_Cmd(DMA1_Channel4, ENABLE);
	
}

void myusart_SendByte(uint16_t data)
{
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) != SET);
	USART_SendData(USART1, data);
}

void myusart_SendArray(uint16_t *Array, uint8_t len)
{
	int i;
	for(i=0; i<len; i++)
	{
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) != SET);
		USART_SendData(USART1, Array[i]);
	}
}

void myusart_SendString(char* str)
{
	myusart_SendArray((uint16_t *)str, strlen(str));
}

void myusart_receive_register(myusart_receive_callback_t callback)
{
	myusart_receive_callback = callback;
}

void myusart_send_finish_register(myusart_send_finish_callback_t callback)
{
	myusart_send_finish_callback = callback;
}

void USART1_IRQHandler(void)
{
	uint16_t data;
	if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET)
	{
		data = USART_ReceiveData(USART1);
		
		if(myusart_receive_callback)
		{
			myusart_receive_callback(data);
		}
	}
}

void DMA1_Channel4_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC4) == SET)
	{
		if(myusart_send_finish_callback)
		{
			myusart_send_finish_callback();
		}
		DMA_ClearITPendingBit(DMA1_IT_TC4);
	}
}
