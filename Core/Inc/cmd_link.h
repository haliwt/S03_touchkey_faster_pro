#ifndef __CMD_LINK_H_
#define __CMD_LINK_H_

#include "main.h"
#define  MAX_BUFFER_SIZE   8

enum Signal{
	PANEL_DATA=1,
	WIFI_INFO,//2
	WIFI_TIME,//3
	WIFI_REAL_TEMP,//4
	WIFI_CMD,//5
	WIFI_WIND_SPEED,//6
	WIFI_BEIJING_TIME,//7
	WIFI_SET_GMT_MINUTE,//8
	WIFI_SET_GMT_SECOND,//9
	WIFI_SET_TIMING,//10
	WIFI_SET_TEMPERATURE//11
	
};



extern uint8_t inputBuf[MAX_BUFFER_SIZE];
extern uint8_t outputBuf[8];


void SendData_PowerOnOff(uint8_t index);

void SendData_Set_Wifi(uint8_t hdata);

void SendData_Set_Command(uint8_t hdata);
void SendData_Temp_Data(uint8_t tdata);
void SendData_Time_Data(uint8_t tdata);


void SendData_Buzzer(void);


void USART1_Cmd_Error_Handler(void);



#endif 




