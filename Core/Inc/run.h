#ifndef __RUN_H_
#define __RUN_H_
#include "main.h"


typedef enum WIFI_STATE_T{
   
    WIFI_POWER_ON = 0x80,
    WIFI_POWER_OFF=0X81,
    WIFI_MODE_1=0X08,   //state ->normal works
    WIFI_MODE_2=0X18,   //state->sleeping works
    WIFI_KILL_ON=0x04,  //Anion(plasma)
    WIFI_KILL_OFF=0x14,
    WIFI_PTC_ON = 0x02, 
    WIFI_PTC_OFF = 0x12,
    WIFI_SONIC_ON = 0x01,       //ultrasonic
    WIFI_SONIC_OFF = 0x11,
    WIFI_WIND_SPEED_ITEM = 0x90,
    WIFI_TEMPERATURE = 0xA0,
    IWDG_DATA= 0xB0


}wifi_state_t;

typedef enum TIMING_T{

   timing_not_definition,
   timing_success 
}timing_t;


typedef struct __RUN{

  
   uint8_t power_key;
   uint8_t gTemperature_timer_flag;
   uint8_t gPower_On;
   uint8_t gRun_flag;
   uint8_t power_times;
   uint8_t vk36n4d_interrupt_flag;
   uint8_t gKey_command_tag ;
   
   uint8_t gTemperature;
 
   uint8_t gTimer_minute_Counter;
   
  uint8_t gTimer_setup_zero;

  uint8_t gFan_RunContinue;

  uint8_t temperature_flag;
  
  uint8_t timer_timing_define_flag;
  uint8_t input_key_flag;
 
  
   uint8_t usart_flag;
   uint8_t decodeFlag;
   uint8_t gTemp_check;
   

   uint8_t  gPlasma;

   uint8_t  gDry;

   uint8_t  gModel;
   uint8_t  gUltransonic;
  
   uint8_t  disp_wind_speed_grade;
   uint8_t  gTimer_fan_counter;

   uint8_t single_data;
   uint8_t fan_off_60s;

   uint8_t wifi_connect_flag;

   uint8_t wifi_set_temperature;
   uint8_t wifi_set_timing;
   uint8_t wifi_set_wind_speed;
   uint8_t setup_temperature_value ;
   uint8_t wifi_send_buzzer_sound;

   uint8_t wifi_led_fast_blink_flag ;
 
   uint8_t  setup_timer_timing_item;
   uint8_t  wifi_set_oneself_temperature;
   uint8_t   wifi_set_temperature_value_flag;
 
   uint8_t auto_model_shut_off_ptc_flag;

   //iwdg ref
   uint8_t process_run_guarantee_flag;


 
   uint8_t key_read_value;
   uint8_t gTimer_usart_error;
   uint8_t gPower_repeat_times_flag;
   //timer 
   uint8_t setup_timer_flag;
  

   //works two houre after stop 10 minute
   uint8_t works_counter_time_value;
   uint8_t gTimer_work_break_times;
   //beijing times
   uint8_t gmt_time_hours;
   uint8_t  gmt_time_minutes;
   uint8_t  gmt_time_seconds;
   uint8_t display_set_timer_timing;

   uint8_t gTimer_key_start_counter;
   uint8_t gTimer_key_counter;
   uint8_t recoder_start_conuter_flag;


 
   
   uint8_t Timer_mode_flag;
   uint8_t temperature_set_flag;

   uint8_t gReal_humtemp[2];
   uint8_t gInputCmd[2];
   uint8_t wifisetTime[1];
   uint8_t wifiCmd[2];

   uint8_t gTimer_wifi_connect_counter;
   uint8_t panel_key_setup_timer_flag;
   uint8_t gTimer_temp_delay;
   uint8_t gTimer_set_temp_times;
   uint8_t gTimer_digital5678_ms;
   uint8_t gTimer_key_timing;
   uint8_t gTimer_timing;
   uint8_t gTimer_connect_wifi ;

    int8_t dispTime_hours;
	int8_t dispTime_minutes;
	int8_t dispTime_seconds;

	int8_t  timer_time_hours;
   int8_t 	timer_time_minutes ;



	
    int8_t 	gTime_hours;

   
	
	
     uint16_t gTimer_numbers_one_two_blink;
     uint32_t wifi_key_counter;

   
 }RUN_T;

extern RUN_T run_t;






void RunCommand_Handler(void);


#endif 



