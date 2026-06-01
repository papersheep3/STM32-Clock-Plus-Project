#include "stm32f10x.h"                  // Device header

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "esp_usart.h"
#include "esp_at.h"
#include "main.h"

#define RX_BUFFER_SIZE 2048

#define RX_RESULT_OK	  0
#define RX_RESULT_ERROR 1
#define RX_RESULT_FAIL  2

static uint32_t rxdata[RX_BUFFER_SIZE];
static uint32_t rxlen;
static bool rxready;
static uint8_t rxresult;

static void on_usart_receive(uint8_t data)
{
	/* 没有数据请求，不接受数据 */
	if(!rxready)
	{
		return;
	}
	
	/* 接收数据，防止缓冲区溢出 */
	if(rxlen < RX_BUFFER_SIZE)
	{
		rxdata[rxlen++] = data;
	}
	else
	{
		rxready = false;
		rxresult = RX_RESULT_FAIL;
		return;
	}
	
	/* 数据接收完毕判断 */
	if(data == '\n')
	{
		/*  判断换行是否为'\r\n' */
		if(rxlen >= 2 && rxdata[rxlen-2] == '\r')
		{
			/* 收到OK */
			if(rxlen >= 4 && rxdata[rxlen-4] == 'O' && rxdata[rxlen-3] == 'K')
			{
				rxready = false;
				rxresult = RX_RESULT_OK;
			}
			/* 收到ERROR */
			else if(rxlen >= 7 && rxdata[rxlen-7] == 'E' && rxdata[rxlen-6] == 'R'
							&& rxdata[rxlen-5] == 'R' && rxdata[rxlen-4] == 'O'
							&& rxdata[rxlen-3] == 'R')
			{
				rxready = false;
				rxresult = RX_RESULT_ERROR;
			}
		}
	}
	
}

bool esp_at_init(void)
{
	rxready = false;
	esp_usart_init();
	
	esp_usart_receive_register(on_usart_receive);
	
	return esp_at_reset();
}
bool esp_at_command(const char *cmd, const char **rsp, uint32_t *length, uint32_t timeout)
{
	rxlen = 0;
	rxready = true;
	rxresult = RX_RESULT_FAIL;
	
	esp_usart_write_string(cmd);
	esp_usart_write_string("\r\n");
	
	while(rxready && timeout--)
	{
		delay_ms(1);
	}
	rxready = false;
	
	if(rsp)	/*  防止传入NULL */
	{
		*rsp = (const char*)rxdata;
	}
	if(length)
	{
		*length = rxlen;
	}
	
	return rxresult == RX_RESULT_OK;
}


bool esp_at_senddata(const uint8_t *data, uint32_t length)
{
	esp_usart_write_data((uint8_t *)data, length);
	
	return true;
}

bool esp_at_reset(void)
{
	/* 复位esp32 */
	if(!esp_at_command("AT+RESTORE", NULL, NULL, 1000))
	{
		return false;
	}
	delay_ms(2000);
	
	/* 关闭回显 */
	if(!esp_at_command("ATE0", NULL, NULL, 1000))
	{
		return false;
	}
	
	/* 关闭存储 */
	if(!esp_at_command("AT+SYSSTORE=0", NULL, NULL, 1000))
	{
		return false;
	}
	
	return true;
}

bool esp_at_wifi_init(void)
{
	/* 设置esp32为Station模式 */
	if(!esp_at_command("AT+CWMODE=1", NULL, NULL, 1000))
	{
		return false;
	}
	
	return true;
}

bool esp_at_wifi_connect(const char *ssid, const char *pwd)
{
	char cmd[64];
	
	snprintf(cmd, sizeof(cmd), "AT+CWJAP=\"%s\",\"%s\"", ssid, pwd);
	if(!esp_at_command(cmd, NULL, NULL, 10000))
	{
		return false;
	}
	
	return true;
}

bool esp_at_http_get(const char *url, const char **rsp, uint32_t *length, uint32_t timeout)
{
	char cmd[128];
	
	snprintf(cmd, sizeof(cmd), "AT+HTTPCGET=\"%s\"", url);
	if(!esp_at_command(cmd, rsp, length, 10000))
	{
		return false;
	}
	
	return true;
}

bool esp_at_stnp_init(void)
{
	/* 设置为sntp模式 */
	if(!esp_at_command("AT+SNTP=1,\"cn.ntp.org.cn\"", NULL, NULL, 1000))
	{
		return false;
	}
	
	/* 查询sntp时间 */
	if(!esp_at_command("AT+CIPSNTPTIME?", NULL, NULL, 1000))
	{
		return false;
	}
	
	return true;
}

bool esp_at_time_get(uint32_t *timestamp)
{
	const char* rsp;
	uint32_t length;
	
	if(!esp_at_command("AT+SYSTIMESTAMP?", &rsp, (uint32_t *)length, 1000))
	{
		return false;
	}
	
	char *sts = strstr(rsp, "+SYSTIMESTAMP:");
	sts += strlen("+SYSTIMESTAMP:");
	
	*timestamp = atoi(rsp);
	
	return true;
}
