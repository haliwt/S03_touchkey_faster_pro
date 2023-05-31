#include "cmd_link.h"
#include "usart.h"
#include "gpio.h"
#include "run.h"
#include "display.h"
#include "led.h"
#include "single_mode.h"
#include "lcd.h"


volatile static uint8_t transOngoingFlag; //interrupt Transmit flag bit , 1---stop,0--run
uint8_t outputBuf[8];
static uint8_t transferSize;
static uint8_t state;
uint8_t inputBuf[MAX_BUFFER_SIZE];


/****************************************************************************************************
**
*Function Name:static void selectLight(uint8_t index)
*Function: UART2 transmit interrupt process ---4D 58 4C 53 32 30 32 
*Input Ref: LED number 
*Return Ref:NO
*
****************************************************************************************************/
void SendData_PowerOnOff(uint8_t index)
{
	
   //crc=0x55;
	outputBuf[0]='T'; //4D //Touch Key
	outputBuf[1]='K'; //58
	outputBuf[2]='P'; //4C	// 'P' power
	outputBuf[3]=index; //53	// 'S' select light command, 'C' close all light command
	
    transferSize=4;
	if(transferSize)
	{
		while(transOngoingFlag);
		transOngoingFlag=1;
		HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
	}
	
}
void SendData_Buzzer(void)
{
	
   //crc=0x55;
	outputBuf[0]='T'; //4D
	outputBuf[1]='K'; //58
	outputBuf[2]='Z'; //4C	// 'L' for light board
	outputBuf[3]='Z'; //53	// 'S' select light command, 'C' close all light command
	
    transferSize=4;
	if(transferSize)
	{
		while(transOngoingFlag);
		transOngoingFlag=1;
		HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
	}
	
}

void SendData_Set_Command(uint8_t hdata)
{
	    outputBuf[0]='T'; //4D
		outputBuf[1]='K'; //58
		outputBuf[2]='C'; //"C" ->command
		outputBuf[3]=hdata; //53	//
		
		transferSize=4;
		if(transferSize)
		{
			while(transOngoingFlag);
			transOngoingFlag=1;
			HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
		}
	
}
void SendData_Set_Wifi(uint8_t hdata)
{
	    outputBuf[0]='T'; //
		outputBuf[1]='K'; //58
		outputBuf[2]='W'; //W ->WIFI
		outputBuf[3]=hdata; 
		
		transferSize=4;
		if(transferSize)
		{
			while(transOngoingFlag);
			transOngoingFlag=1;
			HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
		}
	
}
/*********************************************************
 * 
 * Function Name:void SendData_Temp_Data(uint8_t tdata)
 * Function:send temperature value 
 * 
*********************************************************/
void SendData_Temp_Data(uint8_t tdata)
{

        outputBuf[0]='T'; //4D
		outputBuf[1]='K'; //58
		outputBuf[2]='M'; //"T"->temperature
		outputBuf[3]=tdata; //53	//
		
		transferSize=4;
		if(transferSize)
		{
			while(transOngoingFlag);
			transOngoingFlag=1;
			HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
		}

}
/*********************************************************
 * 
 * Function Name:void SendData_Time_Data(uint8_t tdata)
 * Function:send to set  timer timing value
 * 
*********************************************************/
void SendData_Time_Data(uint8_t tdata)
{

        outputBuf[0]='T'; //4D
		outputBuf[1]='K'; //58
		outputBuf[2]='T'; //"T"->temperature
		outputBuf[3]=tdata; //53	//
		
		transferSize=4;
		if(transferSize)
		{
			while(transOngoingFlag);
			transOngoingFlag=1;
			HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
		}

}
/********************************************************************************
	**
	*Function Name:void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
	*Function :UART callback function  for UART interrupt for receive data
	*Input Ref: structure UART_HandleTypeDef pointer
	*Return Ref:NO
	*
*******************************************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
   
	if(huart==&huart1) // Motor Board receive data (filter)
	{
		switch(state)
		{
		case 0:  //#0
			if(inputBuf[0]=='M')  //hex :4D - "M" -fixed mainboard
				state=1; //=1
			break;
		case 1: //#1
			if(inputBuf[0]=='A') //hex : 41 -'A'  -fixed master
			{
				state=2; 
			}
			else
				state=0; 
			break;
		case 2://#2
			if(inputBuf[0]=='D' || inputBuf[0]=='W'   || inputBuf[0]=='P' ||inputBuf[0] =='C' || inputBuf[0] == 'B' \
			 ||inputBuf[0] == 'S' || inputBuf[0]=='T'||inputBuf[0]=='E'|| inputBuf[0] =='N'|| inputBuf[0] =='M') //'D'->data , 'W' ->wifi
			{
				
				if(inputBuf[0]=='D') run_t.single_data=PANEL_DATA; //receive data is single data
                else if(inputBuf[0]=='W') run_t.single_data = WIFI_INFO; //wifi data
                else if(inputBuf[0]=='P') run_t.single_data = WIFI_REAL_TEMP;//temperature 
				else if(inputBuf[0]=='C') run_t.single_data = WIFI_CMD; //command 
				else if(inputBuf[0]=='B') run_t.single_data = WIFI_BEIJING_TIME;
				else if(inputBuf[0]=='S') run_t.single_data = WIFI_WIND_SPEED;
				else if(inputBuf[0]=='T') run_t.single_data = WIFI_SET_TIMING;
				else if(inputBuf[0]=='E') run_t.single_data = WIFI_SET_TEMPERATURE;
				else if(inputBuf[0]=='M') run_t.single_data = WIFI_SET_GMT_MINUTE;
				else if(inputBuf[0]=='N') run_t.single_data = WIFI_SET_GMT_SECOND;
			    state=3;
			}
			else
				state=0;
			break;
            
        case 3:

            switch(run_t.single_data){
             case PANEL_DATA://1
                 run_t.gReal_humtemp[0]=inputBuf[0]; //Humidity value 
                 run_t.step_run_power_on_tag=1;
                 state = 4;  
            break;
            case WIFI_INFO ://2
                  if(inputBuf[0]==0x01){
                     run_t.wifi_connect_flag =1;
					 
				     
                  }
                  else if(inputBuf[0]==0x0)
                     run_t.wifi_connect_flag =0;
                 
                    run_t.decodeFlag=0; 
                    state=0;
                 
             
            break;

            case WIFI_REAL_TEMP ://3 //wifi modify temperature of value
                 run_t.wifi_set_temperature=inputBuf[0]; 
                 state=0;
                 run_t.decodeFlag=1;

            break;

            case WIFI_CMD://5
                 run_t.wifiCmd[0] =inputBuf[0];
                 state=0;
                 run_t.decodeFlag=1; 
            break;

            case WIFI_WIND_SPEED:
                 run_t.wifi_set_wind_speed=inputBuf[0];
				 run_t.disp_wind_speed_grade=inputBuf[0];
                 state=0;
                 run_t.decodeFlag=1; 

             break;
			

             case WIFI_SET_TIMING:
             		run_t.dispTime_hours  = inputBuf[0];
             		 state=0;
                    run_t.decodeFlag=1; 

             break;

			 case WIFI_SET_TEMPERATURE:
			     run_t.wifi_set_oneself_temperature=inputBuf[0]; 
                 state=0;
                 run_t.decodeFlag=1;

			 break;


			  case WIFI_BEIJING_TIME:
			 	
			  	    run_t.gmt_time_hours =inputBuf[0];
                    run_t.dispTime_hours  = inputBuf[0];
				
					 state=0;
		             run_t.decodeFlag=1;
              
                 
             break;

			 case  WIFI_SET_GMT_MINUTE:
			 	
			 	  run_t.gmt_time_minutes = inputBuf[0];
				  run_t.dispTime_minutes = inputBuf[0];

				  run_t.single_data = WIFI_BEIJING_TIME;

                  
				   state=0;
		           run_t.decodeFlag=1;
			 		


			 break;

			 case WIFI_SET_GMT_SECOND:
				
				run_t.gmt_time_seconds = inputBuf[0]+1;
			    run_t.dispTime_seconds = inputBuf[0]+1;
				run_t.gTimer_minute_Counter =inputBuf[0]+1;
				run_t.decodeFlag=1;
				state=0;
				
		 
                 

			 break;

         	}

            
        break;
        
		case 4: //

		switch(run_t.single_data==PANEL_DATA){

				case PANEL_DATA:

		
              run_t.gReal_humtemp[1]=inputBuf[0]; //temperature value
			
		     state=0;
		     run_t.decodeFlag=1;
	         run_t.step_run_power_on_tag;
           
		    break;

                case WIFI_BEIJING_TIME:
		           run_t.gmt_time_minutes = inputBuf[0];
				   run_t.dispTime_minutes = inputBuf[0];


                  
				   state=0;
		           run_t.decodeFlag=1;
	
		 
		     break;
			}

		 break;
           
    
		
		
		default:
			state=0;
			run_t.decodeFlag=0;  //
		break;

		}
      HAL_UART_Receive_IT(&huart1,inputBuf,1);//UART receive data interrupt 1 byte
	}
    
}
/********************************************************************************
	**
	*Function Name:void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
	*Function :UART callback function  for UART interrupt for receive data
	*Input Ref: structure UART_HandleTypeDef pointer
	*Return Ref:NO
	*
*******************************************************************************/
void USART1_Cmd_Error_Handler(void)
{
    uint32_t temp;
	static uint8_t power_error;


        if(run_t.gTimer_usart_error > 9 ){
			power_error++;
			run_t.gTimer_usart_error=0;
			  __HAL_UART_CLEAR_OREFLAG(&huart1);
	        
	          temp = USART1->RDR;
	     
	     
			  UART_Start_Receive_IT(&huart1,inputBuf,1);
			
	          
         }
        

}
/********************************************************************************
**
*Function Name:void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
*Function :UART callback function  for UART interrupt for transmit data
*Input Ref: structure UART_HandleTypeDef pointer
*Return Ref:NO
*
*******************************************************************************/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart==&huart1)
	{
		transOngoingFlag=0; //UART Transmit interrupt flag =0 ,RUN
	}
	
}

