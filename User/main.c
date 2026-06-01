#include "stm32f10x.h"
//#include "LCD_Drive.h"
//#include "GUI.h"
//#include "Font.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "main.h"
#include "rtc.h"
#include "esp_usart.h"
#include "esp_at.h"
#include "weather.h"
#include "OLED.h"
#include "LCD_Drive.h"
#include "GUI.h"
#include "Font.h"
#include "Picture.h"

//static const char *wifi_ssid = "papersheep";
//static const char *wifi_pwd  = "chen0711";
//static const char *url 			 = "https://api.seniverse.com/v3/weather/now.json?key=SxE7s5Vrh1nZ6XtJy&location=guangzhou&language=en&unit=c";


int main(void)
{

	LCD_Init();
	
	delay_ms(200);
	
	GUI_ShowImage(Burning, 120, 120, 60, 60);
	
//	LCD_Clear(GRAY);
//	
//	GUI_FillRect(0,0,20,20,RED);
//	
//	GUI_FillRect(220,0,20,20,GREEN);
//	
//	GUI_FillRect(0,220,20,20,BLUE);
//	
//	GUI_FillRect(220,220,20,20,YELLOW);
//	
//	GUI_ShowString(0, 120, "Hello, world!", &Font8x16, CYAN, BLACK);
	

	
//	rtc_init();	
	
//	OLED_Init();

//	delay_ms(2000);
//	
//	if(!esp_at_init())
//	{
//		while(1);
//	}
//	if(!esp_at_wifi_init())
//	{
//		while(1);
//	}
//	if(!esp_at_wifi_connect(wifi_ssid, wifi_pwd))
//	{
//		while(1);
//	}

//	bool weather_ok = false;
////	bool sntp_ok = false;
//	uint32_t t = 0;
////	char *str;

	while(1)
	{
//		t++;
//		delay_ms(1000);
//		rtc_date_t date;
//		rtc_get_date(&date);
		
//		snprintf(str, sizeof(str), "%04d-%02d-%02d", date.year, date.month, date.day);
//		OLED_ShowString(4, 1, str);
//		snprintf(str, sizeof(str), "%02d:%02d:%02d", date.hour, date.minute, date.second);
//		OLED_ShowString(5, 1, str);
		
//		if(!weather_ok || t%60 == 0)
//		{
//			const char *rsp;
//			esp_at_http_get(url, &rsp, NULL, 10000);
//			
//			weather_t weather;
//			weather_parse(rsp, &weather);
//			
//			OLED_ShowString(1, 1, "Guangzhou");
//			OLED_ShowString(2, 1, "Weather:");
//			OLED_ShowString(2, 9, weather.weather);
//			OLED_ShowString(3, 1, "Temp:");
//			OLED_ShowString(3, 6, weather.temperature);
//		}
		
//		if(!sntp_ok || t %3600 == 0)
//		{
//			uint32_t ts;
//			esp_at_time_get(&ts);
//			rtc_set_timestamp(ts);
//		}
			
	}
}
