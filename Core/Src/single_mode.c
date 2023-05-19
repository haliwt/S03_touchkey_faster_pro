#include "single_mode.h"
#include "run.h"
#include "display.h"
#include "gpio.h"
#include "cmd_link.h"
#include "led.h"
#include "key.h"
#include "lcd.h"

uint16_t k;
uint8_t keyvalue;
uint8_t decade_hour;
uint8_t unit_hour;
uint8_t decade_temp;
uint8_t decade_minute;
uint8_t unit_minute;
uint8_t decade_second;
uint8_t unit_second;
uint8_t unit_temp ;
uint8_t power_on_off_flag;
uint8_t keyvalue;


void (*single_ai_fun)(uint8_t cmd);
void (*single_add_fun)(void);
void (*single_buzzer_fun)(void);
void (*sendAi_usart_fun)(uint8_t senddat);

static void Receive_Wifi_Cmd(uint8_t cmd);

static void RunKeyOrder_Handler(void);
static void Timing_Handler(void);
//static void Power_Off_Fun(void);
static void Power_On_Fun(void);
static void Setup_Timer_Times(void);
static void Works_Counter_Time(void);
void Setup_Timer_Times_Donot_Display(void);


/************************************************************************
	*
	*Function Name: void Process_Key_Handler(uint8_t keylabel)
	*Function : key by pressed which is key numbers process 
	*Input Ref: key be pressed value 
	*Return Ref:No
	*
************************************************************************/
void Process_Key_Handler(uint8_t keylabel)
{
    static uint8_t power_on_flag_times,power_on_fisrt_flag,changed_lcd_display_model,display_model;
    static uint8_t set_timer_flag,temp_bit_1_hours,temp_bit_2_hours,temp_bit_1_minute,temp_bit_2_minute;
  
    switch(keylabel){

      case POWER_OFF_ITEM://case power_key:
            Power_Off_Fun();
            run_t.input_key_flag =POWER_OFF_ITEM;
			run_t.temperature_set_flag = 0;
			run_t.wifi_set_temperature_value_flag=0;
		    run_t.gTimer_set_temp_times=0; //conflict with send temperatur value 
            run_t.wifi_led_fast_blink_flag=0;
            run_t.Timer_mode_flag = 0;
			run_t.works_counter_time_value=0;
			run_t.panel_key_setup_timer_flag=0;
            run_t.setup_temperature_value=0;
		    run_t.timer_time_hours =0;
			run_t.timer_time_minutes =0;
			run_t.timer_timing_define_flag = timing_not_definition;

			run_t.gKey_command_tag = KEY_NULL;

	  break;

	  case POWER_ON_ITEM:
	  		Power_On_Fun();
			run_t.gTimer_set_temp_times=0; //conflict with send temperatur value 
			
			run_t.input_key_flag =POWER_ON_ITEM;
			
			run_t.recoder_start_conuter_flag=0;

			run_t.gKey_command_tag = KEY_NULL;

		
		break;


	  case LINK_WIFI_ITEM: //case link_cloud_key:
	     run_t.recoder_start_conuter_flag=0;
	     run_t.gTimer_key_start_counter=0;
		run_t.gTimer_key_counter=0;
         run_t.gTimer_set_temp_times=0; //conflict with send temperatur value 
         SendData_Set_Wifi(0x01);
	     HAL_Delay(1);
		run_t.wifi_led_fast_blink_flag=1;
		run_t.wifi_connect_flag =0;
		run_t.gTimer_wifi_connect_counter=0;
		
        run_t.gKey_command_tag = KEY_NULL;
	  break;

	  case MODE_KEY_ITEM://case model_key:
		if(run_t.gPower_On ==1){
			//SendData_Buzzer();

		   if(run_t.display_set_timer_timing == beijing_time){
		
               //timer time + don't has ai item
               run_t.display_set_timer_timing = timer_time;
			   run_t.gModel=0;
		      SendData_Set_Wifi(MODE_TIMER);
			  HAL_Delay(1);
               
		   	}
		    else if(run_t.display_set_timer_timing == timer_time){
                //beijing time + ai item
                run_t.display_set_timer_timing = beijing_time;
             
	           run_t.gModel=1;
			  SendData_Set_Wifi(MODE_AI);
			  HAL_Delay(1);
				
			}
			
			
				
		 }
	  run_t.gKey_command_tag = KEY_NULL;

	  break;

	  
	  case MODE_KEY_LONG_TIME_KEY://case model_long_key:
	  	if(run_t.gPower_On ==1){
		  //  SendData_Buzzer();
			run_t.gModel=0;
		   run_t.setup_timer_timing_item=1;//run_t.gModel =2;
		   run_t.display_set_timer_timing  =timer_time;
		   run_t.gTimer_key_timing=0;
		   set_timer_flag=0;
           display_model =1;
		   run_t.Timer_mode_flag=1;
		   
		   
		   SendData_Set_Wifi(MODE_TIMER);
		  // HAL_Delay(50);
		   
	  	 }

		run_t.gKey_command_tag = KEY_NULL;
	  break;

	  case ADD_KEY_ITEM: //case add_key:
	  	 if(run_t.gPower_On ==1){
		
			SendData_Buzzer();
		

		    switch(run_t.setup_timer_timing_item){

			case 0: //set temperature value add number
      
				run_t.wifi_set_temperature_value_flag =0;
				run_t.wifi_set_temperature ++;
	            if(run_t.wifi_set_temperature < 20){
				    run_t.wifi_set_temperature=20;
				}
				
				if(run_t.wifi_set_temperature > 40)run_t.wifi_set_temperature= 20;

				if(power_on_fisrt_flag ==0){
				     power_on_fisrt_flag ++;
			     	run_t.wifi_set_temperature =40;


			      }
            
			    decade_temp =  run_t.wifi_set_temperature / 10 ;
				unit_temp =  run_t.wifi_set_temperature % 10; //
                
				lcd_t.number1_low=decade_temp;
				lcd_t.number1_high =decade_temp;

				lcd_t.number2_low = unit_temp;
				lcd_t.number2_high = unit_temp;

				run_t.panel_key_setup_timer_flag = 1;
                
             
					
			
			   break;

			   case 1:
				    display_model++;
					run_t.gTimer_key_timing =0;
                    set_timer_flag=0;
					run_t.timer_time_hours++ ;//run_t.dispTime_minutes = run_t.dispTime_minutes + 60;
				    if(run_t.timer_time_hours > 24){ //if(run_t.dispTime_minutes > 59){

		                 run_t.timer_time_hours=0;//run_t.dispTime_hours =0;
		                

					}
				
                  
					temp_bit_2_hours = run_t.timer_time_hours /10 ;
					temp_bit_1_hours = run_t.timer_time_hours %10;
					run_t.timer_time_minutes  =0;

					temp_bit_2_minute =0;
					temp_bit_1_minute =0;
                    
					lcd_t.number5_low=temp_bit_2_hours;
					lcd_t.number5_high =temp_bit_2_hours;

					lcd_t.number6_low = temp_bit_1_hours;
					lcd_t.number6_high = temp_bit_1_hours;

					lcd_t.number7_low=temp_bit_2_minute;
					lcd_t.number7_high =temp_bit_2_minute;

					lcd_t.number8_low = temp_bit_1_minute;
					lcd_t.number8_high = temp_bit_1_minute;

				
				break;
				}	
			
				
            }

		 run_t.gKey_command_tag = KEY_NULL;
	  break;

	  case DEC_KEY_ITEM: //case dec_key:
	   if(run_t.gPower_On ==1){
	   	SendData_Buzzer();
	     switch(run_t.setup_timer_timing_item){

		   case 0: 
	
	        run_t.wifi_set_temperature_value_flag =0;
			run_t.wifi_set_temperature--;
			if(run_t.wifi_set_temperature<20) run_t.wifi_set_temperature=40;
	        if(run_t.wifi_set_temperature >40)run_t.wifi_set_temperature=40;

			if(power_on_fisrt_flag ==0){
				power_on_fisrt_flag ++;
			  run_t.wifi_set_temperature =40;


			}

	        decade_temp =  run_t.wifi_set_temperature / 10;
			unit_temp =  run_t.wifi_set_temperature % 10; //
         //    HAL_Delay(5);
			lcd_t.number1_low=decade_temp;
			lcd_t.number1_high =decade_temp;

			lcd_t.number2_low = unit_temp;
			lcd_t.number2_high = unit_temp;
			
			run_t.panel_key_setup_timer_flag = 1;
	    	

		    break;

			case 1:
	    
			    display_model++;
				run_t.gTimer_key_timing =0;
                set_timer_flag=0;
				run_t.timer_time_hours -- ;//run_t.dispTime_minutes = run_t.dispTime_minutes - 1;
				if(run_t.timer_time_hours < 0){//if(run_t.dispTime_minutes < 0){

				    run_t.timer_time_hours =24;//run_t.dispTime_hours --;
					
					
				}
				    temp_bit_2_minute = run_t.timer_time_hours /10 ;
					temp_bit_1_minute = run_t.timer_time_hours %10;
               
					temp_bit_2_hours = run_t.timer_time_hours /10 ;
					temp_bit_1_hours = run_t.timer_time_hours  %10;
					run_t.timer_time_minutes  =0;

					temp_bit_2_minute=0;
					temp_bit_1_minute=0;
                 

					lcd_t.number5_low=temp_bit_2_hours;
					lcd_t.number5_high =temp_bit_2_hours;

					lcd_t.number6_low = temp_bit_1_hours;
					lcd_t.number6_high = temp_bit_1_hours;

					lcd_t.number7_low=temp_bit_2_minute;
					lcd_t.number7_high =temp_bit_2_minute;

					lcd_t.number8_low = temp_bit_1_minute;
					lcd_t.number8_high = temp_bit_1_minute;

             break;

	    	}
		}

      run_t.gKey_command_tag = KEY_NULL;
	  break;

	  default:

	  break;

	}
	
}
/************************************************************************
	*
	*Function Name: static void Power_Off_Fun(void)
	*
	*
	*
	*
************************************************************************/
 void Power_Off_Fun(void)
{
	
        run_t.gModel =0; //WT.EDIT 2022.09.01
		run_t.gPlasma=0;
		run_t.gDry =0;
		run_t.gUltransonic =0;
		
		 run_t.wifi_led_fast_blink_flag=0;
		
			
      
		run_t.timer_timing_define_flag = timing_not_definition;

		run_t.power_key =2;
		run_t.gFan_RunContinue=1;
		run_t.disp_wind_speed_grade =1;	
		run_t.gPower_On=0;
		run_t.fan_off_60s =0;
		power_on_off_flag=1;

   
		

  
} 




static void Power_On_Fun(void)
{
                
	run_t.gPower_On=1;

	run_t.power_key =1;
	run_t.gFan_RunContinue=0;

	run_t.gModel =1; //WT.EDIT 2022.09.01
	run_t.gPlasma=1;
	run_t.gDry =1;
	run_t.gUltransonic =1;
	
	run_t.temperature_set_flag = 0; //WT.EDIT 2023.01.31
    run_t.setup_temperature_value=0; // //WT.EDIT 2023.01.31
    run_t.disp_wind_speed_grade =3;
	
	run_t.wifi_send_buzzer_sound=0xff;

	 run_t.gTimer_minute_Counter=0;
     run_t.gTimer_timing=0;

	 run_t.timer_time_hours =0;
	 run_t.timer_time_minutes =0;
	 run_t.setup_timer_timing_item=0;

	 run_t.timer_timing_define_flag = timing_not_definition;

	 

	 if(lcd_t.display_beijing_time_flag == 0 ){

	 run_t.dispTime_seconds=0;
	 run_t.dispTime_hours=0;
     run_t.dispTime_minutes=0;
	 
		 
	

	 lcd_t.number5_low=(run_t.dispTime_hours ) /10;
     lcd_t.number5_high =(run_t.dispTime_hours) /10;

	 lcd_t.number6_low = (run_t.dispTime_hours ) %10;;
	 lcd_t.number6_high = (run_t.dispTime_hours ) %10;
     
     lcd_t.number7_low = (run_t.dispTime_minutes )/10;
	 lcd_t.number7_high = (run_t.dispTime_minutes )/10;

	 lcd_t.number8_low = (run_t.dispTime_minutes )%10;
	 lcd_t.number8_high = (run_t.dispTime_minutes )%10;
	
	 
	 }

}
  
/************************************************************************
	*
	*Function Name: void Wifi_Key_Fun(void)
	*
	*
	*
	*
************************************************************************/  
static void Timing_Handler(void)
{
     switch(run_t.display_set_timer_timing ){
         
     case beijing_time:
	 	run_t.Timer_mode_flag = 0;
	    if(run_t.gTimer_minute_Counter >0){ //minute

			run_t.gTimer_minute_Counter=0;
            run_t.dispTime_minutes ++;
           
            
			if(run_t.dispTime_minutes > 59){
				run_t.dispTime_minutes=0;
				run_t.dispTime_hours ++;
			    run_t.works_counter_time_value++; //works two hours ,after stop 10 minutes, than works 
				if(run_t.dispTime_hours >24){
					run_t.dispTime_hours=0;

					}

			}
	    	}
            Setup_Timer_Times_Donot_Display();
                
			lcd_t.number5_low=(run_t.dispTime_hours ) /10;
			lcd_t.number5_high =(run_t.dispTime_hours) /10;

			lcd_t.number6_low = (run_t.dispTime_hours ) %10;;
			lcd_t.number6_high = (run_t.dispTime_hours ) %10;

			lcd_t.number7_low = (run_t.dispTime_minutes )/10;
			lcd_t.number7_high = (run_t.dispTime_minutes )/10;

			lcd_t.number8_low = (run_t.dispTime_minutes )%10;
			lcd_t.number8_high = (run_t.dispTime_minutes )%10;
						 
            
	

	
    break;
    
    case timer_time:
	
		Setup_Timer_Times();
		Works_Counter_Time();
	
     
     break;
		
    }
}
/*************************************************************************
	*
	*Functin Name:static void Setup_Timer_Times(void)
	*Function : set up timer timing function
	*
	*
	*
*************************************************************************/
static void Setup_Timer_Times(void)
{


      if(run_t.gTimer_timing > 59){ //
        
        run_t.gTimer_timing =0;
		 run_t.timer_time_minutes --;
	    if(run_t.timer_time_minutes < 0){
		     run_t.timer_time_hours -- ;
			 run_t.timer_time_minutes =59;
           
			if(run_t.timer_time_hours < 0 ){

	           if(run_t.timer_timing_define_flag == timing_success){
			    run_t.timer_time_hours=0;
				run_t.timer_time_minutes=0;
				run_t.wifi_send_buzzer_sound=0xff;
				Power_Off_Fun();

			
				run_t.gPower_On =0 ;
			    run_t.gFan_RunContinue=1;
				run_t.fan_off_60s = 0;
	           
	          
                
                }
                 else{
     
                     run_t.timer_time_hours =0;
                     run_t.timer_time_minutes =0;
				     run_t.display_set_timer_timing=beijing_time;
                     run_t.gModel=1;
					 SendData_Set_Wifi(MODE_AI);
                 }
                            
                
                }
              }
            
		     }

	     
     
   
			lcd_t.number5_low=(run_t.timer_time_hours ) /10;
			lcd_t.number5_high =(run_t.timer_time_hours) /10;

			lcd_t.number6_low = (run_t.timer_time_hours ) %10;;
			lcd_t.number6_high = (run_t.timer_time_hours ) %10;

			lcd_t.number7_low = (run_t.timer_time_minutes )/10;
			lcd_t.number7_high = (run_t.timer_time_minutes)/10;

			lcd_t.number8_low = (run_t.timer_time_minutes)%10;
			lcd_t.number8_high = (run_t.timer_time_minutes )%10;
}
/*************************************************************************
	*
	*Functin Name:void Setup_Timer_Times_Donot_Display(void)
	*
	*
	*
	*
*************************************************************************/       
void Setup_Timer_Times_Donot_Display(void)
{
   if(run_t.gTimer_timing > 59){ //
        
        run_t.gTimer_timing =0;
		 run_t.timer_time_minutes --;
	    if(run_t.timer_time_minutes < 0){
		     run_t.timer_time_hours -- ;
			 run_t.timer_time_minutes =59;
           
			if(run_t.timer_time_hours < 0 ){

	           if(run_t.timer_timing_define_flag == timing_success){
			    run_t.timer_time_hours=0;
				run_t.timer_time_minutes=0;
				run_t.wifi_send_buzzer_sound=0xff;
				Power_Off_Fun();

			
				run_t.gPower_On =0 ;
			    run_t.gFan_RunContinue=1;
				run_t.fan_off_60s = 0;
	           
	          
                
                }
                 else{
     
                     run_t.timer_time_hours =0;
                     run_t.timer_time_minutes =0;
                 
                 }
                            
                
                }
              }
            
		     }




}

/***************************************************************
 * 
 * Function Name:
 * 
 *
 * 
 **************************************************************/
static void Works_Counter_Time(void)
{
  if(run_t.timer_timing_define_flag == timing_success){
	  if(run_t.gTimer_minute_Counter >0){ //minute
		
		run_t.gTimer_minute_Counter=0;
        run_t.dispTime_minutes ++;
       
          
		if(run_t.dispTime_minutes > 59){
			run_t.dispTime_minutes=0;
			run_t.dispTime_hours ++;
		    run_t.works_counter_time_value++;
		if(run_t.dispTime_hours >24){
			run_t.dispTime_hours=0;

		}

		}


	  }
  }
}

/******************************************************************************
*
*Function Name:void RunPocess_Command_Handler(void)
*Funcion: handle of tall process 
*
*
******************************************************************************/
void RunPocess_Command_Handler(void)
{
   //key input run function

   static uint8_t temp1,temp2,decade_temp,unit_temp,key_set_temp_flag, link_wifi_success;
   static uint8_t power_on_fisrt_send_temperature_value, works_break_flag;
   switch(run_t.gPower_On){

   case RUN_POWER_ON:

       if(run_t.input_key_flag ==POWER_ON_ITEM){
			run_t.input_key_flag=KEY_NULL;
            SendData_PowerOnOff(1);
			HAL_Delay(1);

		}
  
	    Lcd_PowerOn_Fun();
	    Timing_Handler();
	    DisplayPanel_Ref_Handler();

		

      //send timer timing value to main board 
      if(run_t.setup_timer_flag==1){
		   run_t.setup_timer_flag++;

	       SendData_Time_Data(run_t.dispTime_hours);
		  HAL_Delay(5);


	  }
        
		
       //Enable digital "1,2" -> blink LED
       //Enable digital "1,2" -> blink LED
	   if(run_t.panel_key_setup_timer_flag==1){
           run_t.panel_key_setup_timer_flag=0;
		   key_set_temp_flag =1;
		  run_t.setup_temperature_value=1;
		   run_t.gTimer_numbers_one_two_blink=0;
	     	  
	   }

	   //digital "1,2" ->display is dhtd11 real temperature value
	   if(run_t.setup_temperature_value ==0 && key_set_temp_flag ==1 ){
	   	    key_set_temp_flag = 0;

	        temp1 = run_t.gReal_humtemp[1]/10 %10;  // temperature
            temp2 = run_t.gReal_humtemp[1]%10;

		    lcd_t.number1_low=temp1;
			lcd_t.number1_high =temp1;

			lcd_t.number2_low = temp2;
			lcd_t.number2_high = temp2;
		
			
		}
	   
	   //set up temparature value 
	  if(run_t.temperature_set_flag ==1 && run_t.gTimer_temp_delay > 61){
               run_t.gTimer_temp_delay =0;
		 
		  
		  if(run_t.wifi_set_temperature <= run_t.gReal_humtemp[1] || run_t.gReal_humtemp[1] >39){//envirment temperature
	  
				run_t.gDry = 0;
			    SendData_Set_Command(DRY_OFF_NO_BUZZER);
                 HAL_Delay(200);

            }
			else if((run_t.wifi_set_temperature -3) > run_t.gReal_humtemp[1] ||  run_t.gReal_humtemp[1] < 37){
	  
		         run_t.gDry = 1;
			     

		         
	                SendData_Set_Command(DRY_ON_NO_BUZZER);
                     HAL_Delay(200);
		         }
				 
		  }
        
	 
   
   //receive from mainboard data 
   //displayPannel set temperature vale to main board value 
    if(run_t.gTimer_set_temp_times >9){ // set up temperature value [20,40]
	     run_t.gTimer_set_temp_times=0;
		 if(run_t.wifi_set_temperature==0)run_t.wifi_set_temperature=20;
		 if(power_on_fisrt_send_temperature_value ==0){
		       power_on_fisrt_send_temperature_value++;
		     run_t.wifi_set_temperature=40;


		 }
		  if(run_t.wifi_set_temperature_value_flag != 1){
		  	  SendData_Temp_Data(run_t.wifi_set_temperature);
               HAL_Delay(2);
			}
    }

    if(run_t.wifi_connect_flag ==0 && link_wifi_success==0 && run_t.gTimer_connect_wifi > 4){
           run_t.gTimer_connect_wifi=0;
           link_wifi_success=0;
           SendData_Set_Command(WIFI_CONNECT_FAIL);//0x55
           HAL_Delay(10);

     }

     if(run_t.wifi_connect_flag ==1 && link_wifi_success==0 ){
            link_wifi_success++;
            SendData_Set_Command(WIFI_CONNECT_SUCCESS);

     }

	 //works Two hours after stop 10 minutes 
	 if((run_t.works_counter_time_value > 2 || run_t.works_counter_time_value==2) && works_break_flag==0){
	 	      
			  	  works_break_flag++;
				  run_t.gTimer_work_break_times=0;
                  SendData_PowerOnOff(0x55); //
	 	    
			 
	 }
	 if(works_break_flag==1){
	         if(run_t.gTimer_work_break_times > 9){
                run_t.gTimer_work_break_times=0;
				 works_break_flag=0;
			     run_t.works_counter_time_value=0;
                 SendData_PowerOnOff(0xAA); //


			 }
	         


	      }
  
     break;

	 case RUN_POWER_OFF:
	    if(run_t.input_key_flag ==POWER_OFF_ITEM){
			run_t.input_key_flag=KEY_NULL;
            SendData_PowerOnOff(0);
			HAL_Delay(1);

		}
   	      Breath_Led();
         
         if(run_t.gFan_RunContinue == 1){
           if(run_t.fan_off_60s < 61){
		      LED_MODEL_OFF();
			  POWER_ON_LED();
		      LCD_Display_Wind_Icon_Handler();
           	}
		   else{
               run_t.gFan_RunContinue =0;
			   Lcd_PowerOff_Fun();

		   }

         }
   
    break;
   	}
}
/******************************************************************************
*
*Function Name:void Single_RunCmd(void)
*Funcion: handle of receive by usart data
*
*
******************************************************************************/
static void RunKeyOrder_Handler(void)
{
	
  


	 Lcd_PowerOn_Fun();
	 Timing_Handler();
	 DisplayPanel_Ref_Handler();
     
    
    
	 
}

/**********************************************************************************************************
**
*Function Name:static void notifyStatusToHost(uint8_t lightNum,uint8_t filterNum,uint8_t unionNum)
*Function : 
*Input Ref:lightNum--LED ,filterNum -filter number, unionNum - smart menu number
*Return Ref:NO
*
*********************************************************************************************************/
void Decode_Function(void)
{
   
   Receive_MainBoard_Data_Handler(run_t.single_data);
    
}

/**********************************************************************************************************
	**
	*Function Name:void Receive_MainBoard_Data_Handler(uint8_t cmd)
	*Function : mainboard of command 
	*Input Ref:
	*Return Ref:NO
	*
*********************************************************************************************************/
void Receive_MainBoard_Data_Handler(uint8_t cmd)
{
    static uint8_t temperature_decade, temperature_unit;
	static uint8_t hum1,hum2,temp1,temp2; 
	
    switch(cmd){


	 case WIFI_CMD://5
	 	 
	 	 Receive_Wifi_Cmd(run_t.wifiCmd[0]);
	 break;

	 case  WIFI_WIND_SPEED://6
	 	if(run_t.gPower_On ==1){
		  

		   if(run_t.wifi_set_wind_speed <40){
		   	 
                run_t.disp_wind_speed_grade = 1;
		   	}
			else if(run_t.wifi_set_wind_speed >39 && run_t.wifi_set_wind_speed < 67){
			    run_t.disp_wind_speed_grade = 2;


			}
			else if(run_t.wifi_set_wind_speed >66){

				run_t.disp_wind_speed_grade = 3;

			}



		}
    
	  
	 break;

	 case WIFI_REAL_TEMP: //4//set temperature value
	       if(run_t.gPower_On ==1){
		   	   


		      temperature_decade= run_t.wifi_set_temperature /10 ;
			  temperature_unit = run_t.wifi_set_temperature %10;
		   
	         lcd_t.number1_high = temperature_decade;
			 lcd_t.number1_low = temperature_decade;

			 
		    lcd_t.number2_high =  temperature_unit;
			lcd_t.number2_low = temperature_unit;

			
		//	run_t.gTimer_numbers_one_two_blink=0;
	      }

	 break;

	 case PANEL_DATA://1
	   
        if(run_t.gPower_On ==1){
        hum1 =  run_t.gReal_humtemp[0]/10 ;  //Humidity 
        hum2 =  run_t.gReal_humtemp[0]%10;
        
        temp1 = run_t.gReal_humtemp[1]/10 ;  // temperature
        temp2 = run_t.gReal_humtemp[1]%10;
       
         //temperature 
		 lcd_t.number1_high = temp1;
		 lcd_t.number1_low = temp1;

		  lcd_t.number2_high = temp2;
		 lcd_t.number2_low = temp2;

		 //humidity
		 
		 lcd_t.number3_high = hum1;
		 lcd_t.number3_low = hum1;
		 
		 lcd_t.number4_high = hum2;
		 lcd_t.number4_low = hum2;

		 DisplayPanel_Ref_Handler();
        }

      break;

       case WIFI_BEIJING_TIME: //7//run_t.wifi_connect_flag
         if(run_t.gPower_On==1){
           if(run_t.timer_timing_define_flag==timing_not_definition ){
			 lcd_t.number5_low=(run_t.dispTime_hours ) /10;
	         lcd_t.number5_high =(run_t.dispTime_hours) /10;
        
			 lcd_t.number6_low = (run_t.dispTime_hours ) %10;
			 lcd_t.number6_high = (run_t.dispTime_hours ) %10;
	        


			lcd_t.number7_low = (run_t.dispTime_minutes )/10;
			lcd_t.number7_high = (run_t.dispTime_minutes )/10;
              HAL_Delay(5);
			lcd_t.number8_low = (run_t.dispTime_minutes )%10;
			lcd_t.number8_high = (run_t.dispTime_minutes )%10;
              HAL_Delay(5);
	        DisplayPanel_Ref_Handler();

			lcd_t.display_beijing_time_flag = 1;
	      }
         }
		    
        
 
      break;

      case WIFI_SET_TIMING://10
        
        if(run_t.dispTime_hours !=0){
            run_t.timer_timing_define_flag = timing_success ;
            run_t.Timer_mode_flag = 1;
            run_t.dispTime_minutes = 0;
             lcd_t.number5_low=(run_t.dispTime_hours ) /10;
	         lcd_t.number5_high =(run_t.dispTime_hours) /10;
              HAL_Delay(5);
			 lcd_t.number6_low = (run_t.dispTime_hours ) %10;;
			 lcd_t.number6_high = (run_t.dispTime_hours ) %10;
	          HAL_Delay(5);


			lcd_t.number7_low = run_t.dispTime_minutes ;
			lcd_t.number7_high = run_t.dispTime_minutes ;

			lcd_t.number8_low = run_t.dispTime_minutes ;
			lcd_t.number8_high = run_t.dispTime_minutes;
            
        }

      break;

	  case WIFI_SET_TEMPERATURE://11

	  		if(run_t.gPower_On ==1){

			run_t.panel_key_setup_timer_flag=1;

		  
			  run_t.wifi_set_temperature_value_flag =1;

			  run_t.wifi_set_temperature = run_t.wifi_set_oneself_temperature;

		      temperature_decade=  run_t.wifi_set_temperature /10 ;
			  temperature_unit =  run_t.wifi_set_temperature %10;
		       HAL_Delay(5);
	         lcd_t.number1_high = temperature_decade;
			 lcd_t.number1_low = temperature_decade;

			 
		    lcd_t.number2_high =  temperature_unit;
			lcd_t.number2_low = temperature_unit;

			
			run_t.gTimer_numbers_one_two_blink=0;
	      }


	  break;

	  case IWDG_DATA:        
          run_t.process_run_guarantee_flag=1;
      break;


      default:
        
      break;

	}


}
/**********************************************************************
*
*Functin Name: void Receive_ManiBoard_Cmd(uint8_t cmd)
*Function :  wifi recieve data
*Input Ref:  receive wifi send order
*Return Ref: NO
*
**********************************************************************/
void Receive_Wifi_Cmd(uint8_t cmd)
{
	switch(cmd){


		   case WIFI_POWER_ON: //turn on 
		 	
           
              run_t.wifi_send_buzzer_sound = WIFI_POWER_ON_ITEM;
	          Power_On_Fun();
		      run_t.wifi_connect_flag =1;
         
			  cmd=0xff;

	         break;

			 case WIFI_POWER_OFF: //turn off 
                
			   run_t.wifi_connect_flag =1;
			   run_t.wifi_send_buzzer_sound = WIFI_POWER_OFF_ITEM;
				
			    Power_Off_Fun();
              //  HAL_Delay(200);
				
              cmd=0xff;

			 break;

			case WIFI_MODE_1: //AI turn on -> AI icon display 
                if(run_t.gPower_On==1){
			
					if(run_t.display_set_timer_timing == beijing_time){

					//timer time + don't has ai item
					run_t.display_set_timer_timing = timer_time;
					run_t.gModel=0;

					}
					else if(run_t.display_set_timer_timing == timer_time){
					//beijing time + ai item
					run_t.display_set_timer_timing = beijing_time;

					run_t.gModel=1;

					}
		    	}
			break;

			 case WIFI_MODE_2: //icon don't display 
                 if(run_t.gPower_On==1){
					  if(run_t.display_set_timer_timing == beijing_time){

						//timer time + don't has ai item
						run_t.display_set_timer_timing = timer_time;
						run_t.gModel=0;

						}
						else if(run_t.display_set_timer_timing == timer_time){
						//beijing time + ai item
						run_t.display_set_timer_timing = beijing_time;

						run_t.gModel=1;

						}
			 	   
                 }
             break;

			 case WIFI_KILL_ON: //kill turn on plasma
			  if(run_t.gPower_On==1){
                    run_t.gPlasma = 1;
			        run_t.gFan_RunContinue =0;
                 // HAL_Delay(200);
                } 
			 break;

			 case WIFI_KILL_OFF: //kill turn off
                if(run_t.gPower_On==1){
			 	  run_t.gPlasma =0;
				  
		          run_t.gFan_RunContinue =0;
                   // HAL_Delay(200);
                }
			 break;

			 case WIFI_PTC_ON://dry turn on
                if(run_t.gPower_On==1){
			        run_t.gDry =1;
                    run_t.gFan_RunContinue =0;
                   // HAL_Delay(200);
                 
                }
			 break;

			 case WIFI_PTC_OFF: //dry turn off
               
			 	if(run_t.gPower_On==1){
					run_t.gDry=0;
                 
		            run_t.gFan_RunContinue =0;
                    //HAL_Delay(200);
			 	}

			 break;

			 case WIFI_SONIC_ON:  //drive bug
		
				 if(run_t.gPower_On==1){		   
				  run_t.gUltransonic =1; //turn on 
			
				 run_t.gFan_RunContinue =0;
                
			    }

			 break;

			 case WIFI_SONIC_OFF: //drive bug turn off
			 	if(run_t.gPower_On==1){
				    run_t.gUltransonic=0;
					run_t.gFan_RunContinue =0;
                   
			   }
			 break;

			 case IWDG_DATA:

			   run_t.process_run_guarantee_flag =1;

			 break;

		


	         default :
                  cmd =0;
			 break;

			 
        }
   
}

/****************************************************************
 * 
 * Function Name:
 * Function :function of pointer 
 * 
 * 
****************************************************************/
void Single_Add_RunCmd(void(*addHandler)(void))
{
    single_add_fun = addHandler;   

}

void Single_SendBuzzer_RunCmd(void(*buzzerHandler)(void))
{
	single_buzzer_fun = buzzerHandler;

}
void Single_SendAi_Usart_RunCmd(void(*sendaiHandler)(uint8_t seddat))
{
    sendAi_usart_fun = sendaiHandler;

}



