#ifndef ___MY_USART_H
#define ___MY_USART_H

typedef void (*myusart_receive_callback_t)(uint16_t data);
typedef void (*myusart_send_finish_callback_t)(void);

void myusart_Init(void);
void myusart_SendByte(uint16_t data);
void myusart_SendArray(uint16_t *Array, uint8_t len);
void myusart_SendString(char* str);
void myusart_SendArray_async(uint16_t *Array, uint8_t len);


void myusart_receive_register(myusart_receive_callback_t callback);
void myusart_send_finish_register(myusart_send_finish_callback_t callback);

#endif
