#include "lcd.h"
#include "delay.h"
#include "run.h"
#include "single_mode.h"



#define WIFI_Symbol     		0x01 //addr 0xC5
#define WIFI_NO_Symbol 

#define LINE_Symbol             0x01 //addr  0xC2


#define AI_Symbol    			0x01 //addr 0xC3
#define AI_NO_Symbol            0x0


#define DRY_Symbol              0x02  //addr 0xC2 ->T4
#define DRY_NO_Symbol           0x0

#define KILL_Symbol             0x04  //addr 0xC2 ->T5
#define KILL_NO_Symbol           0x0

#define BUG_Symbol            0x08   //addr 0xC2 ->T6
#define BUG_NO_Symbol         0x0

#define TEMP_Symbol              0x01     //addr 0xC4
#define HUMI_Symbol              0x01      //addr  0xC9

#define TWO_DOT_Symbol           0x01       //addr 0xCB

#define T10                      0x08      //addr 0xCF
#define T11                      0x04      //addr 0xCF
#define T12                      0x02      //addr 0xCF
#define T16                      0x01     //addr 0xCF

#define T13                      0x01      //addr 0xCE
#define T14                      0x01      //addr 0xCC
#define T15                      0x01      //addr 0xCA
#define WIND_T16                 0x01      //addr 0xCF

#define T17_T18_T19				 0xE0  
#define WIND_SPEED_ONE           0x20
#define WIND_SPEED_TWO           0x60
#define WIND_SPEED_FULL          0xE0

#define   seg_b  0x02      //SEG1 , //seg_f = 0x20
#define   seg_g  0x04      //SEG2 , //seg_g = 0x40
#define   seg_c  0x08      //SEG3 , //seg_d = 0x08
//High BIT
#define   seg_a  0x10      //SEG4 , //seg_c = 0x04
#define   seg_f  0x20      //SEG5   //seg_a = 0x01
#define   seg_e  0x40      //SEG6   //seg_b = 0x02 
#define   seg_d  0x80      //SEG7 -> ":"


#define  COM0_L        0x01
#define  COM1_L        0X02
#define  COM2_L        0X04
#define  COM3_L        0X08


#define  COM0_H        0X10
#define  COM1_H        0X20
#define  COM2_H        0X40
#define  COM3_H        0X80

#define LUM_VALUE      0x94// 0x97


static void Display_Kill_Dry_Ster_Icon(void);
static void LCD_DisplayNumber_OneTwo_Icon_Handler(void);


const uint8_t lcdNumber1_Low[]={0x0A,0x0A,0x06,0x0E,0x0E,0x0C,0x0C,0x0A,0x0E,0x0E,0x00};


const unsigned char segNumber_Low[]={
 
         seg_b+seg_c,        		      // char "0"  0x00
		 seg_b+seg_c,                      // char "1"  0x01
		 seg_b+seg_g,              		  // char "2"  0x02
		 seg_b+seg_g+seg_c,               // char "3"  0x03
		 seg_b+seg_g+seg_c,               // char "4"  0x04
		 seg_g+seg_c,              		// char "5"  0x05
		 seg_g+seg_c,              // char "6"  0x06
		 seg_b+seg_c,                    	// char "7"  0x07
		 seg_b+seg_g+seg_c,  		        // char "8"  0x08
		 seg_b+seg_g+seg_c,        		// char "9"  0x09
	     0                                      // char "NLL"  0x16



};

const uint8_t  lcdNumber1_High[]={0xF0,0,0xD0,0x90,0x20,0xB0,0xF0,0x10,0xF0,0xB0,0x0};
const unsigned char segNumber_High[]={
         seg_a+seg_f+seg_e+seg_d,        // char "0"  0x00
		 0,                				 // char "1"  0x01
		 seg_a+seg_e+seg_d,              // char "2"  0x02
		 seg_a+seg_d,                    // char "3"  0x03
		 seg_f,                          // char "4"  0x04
		 seg_a+seg_f+seg_d,              // char "5"  0x05
		 seg_a+seg_f+seg_e+seg_d,        // char "6"  0x06
		 seg_a,                    		 // char "7"  0x07
		 seg_a+seg_f+seg_e+seg_d,  		 // char "8"  0x08
		 seg_a+seg_f+seg_d,        		 // char "9"  0x09
         0                               // char "NLL"  0x16



};

static const uint8_t lcdNumber2_Low[]  ={0x0A,0x0A,0x06,0x0E,0x0E,0x0C,0x0C,0x0A,0x0E,0x0E};
static const uint8_t lcdNumber2_High[] ={0xF0,0,0xD0,0x90,0x20,0xB0,0xF0,0x10,0xF0,0xB0};

static const uint8_t lcdNumber3_Low[]  ={0x0A,0x0A,0x06,0x0E,0x0E,0x0C,0x0C,0x0A,0x0E,0x0E};
static const uint8_t lcdNumber3_High[] ={0xF0,0,0xD0, 0x90,0x20,0xB0,0xF0,0x10,0xF0,0xB0};

static const uint8_t lcdNumber4_Low[]  ={0x0A,0x0A,0x06,0x0E,0x0E,0x0C,0x0C,0x0A,0x0E,0x0E};
static const uint8_t lcdNumber4_High[] ={0xF0,0,0xD0,0x90,0x20,0xB0,0xF0,0x10,0xF0,0xB0};

static const uint8_t lcdNumber5_Low[]  ={0x0A,0x0A,0x06,0x0E,0x0E,0x0C,0x0C,0x0A,0x0E,0x0E};
static const uint8_t lcdNumber5_High[] ={0xF0,0,0xD0,0x90,0x20,0xB0,0xF0,0x10,0xF0,0xB0,0x0};

static const uint8_t lcdNumber6_Low[]  ={0x0A,0x0A,0x06,0x0E,0x0E,0x0C,0x0C,0x0A,0x0E,0x0E,0x00};
static const uint8_t lcdNumber6_High[] ={0xF0,0,0xD0,0x90,0x20,0xB0,0xF0,0x10,0xF0,0xB0,0x0};

static const uint8_t lcdNumber7_Low[]  ={0x0A,0x0A,0x06,0x0E,0x0E,0x0C,0x0C,0x0A,0x0E,0x0E,0x00};
static const uint8_t lcdNumber7_High[] ={0xF0,0,0xD0,0x90,0x20,0xB0,0xF0,0x10,0xF0,0xB0,0x0};

static const uint8_t lcdNumber8_Low[]  ={0x0A,0x0A,0x06,0x0E,0x0E,0x0C,0x0C,0x0A,0x0E,0x0E,0x00};
static const uint8_t lcdNumber8_High[] ={0xF0,0,0xD0,0x90,0x20,0xB0,0xF0,0x10,0xF0,0xB0,0x0};






lcd_ref lcd_t; 

static void TM1723_Start(void);
static void TM1723_Stop(void);
static void TM1723_Write_OneByte(uint8_t data);
//static void TIM1723_Write_Cmd(uint8_t cmd);
static void TM1723_Write_Display_Data(uint8_t addr,uint8_t dat);



/*****************************************************
 * 
 * 
 * 
 * 
******************************************************/
static void TM1723_Start(void)
{
  TM1723_STB_SetLow();
	
}
static void TM1723_Stop(void)
{
   TM1723_STB_SetHigh();

}

static void TM1723_Write_OneByte(uint8_t data)
{
	uint8_t i; 
	
	for(i=0;i<8;i++){
		
	TM1723_CLK_SetLow();
	 if(data & 0x01){
	     TM1723_DIO_SetHigh(); //TM1723 high of data to IC
		  
	 }
	 else{
	     TM1723_DIO_SetLow(); //TM1723 low level of data to IC
		  
	 }
   
    delay_us(100);
    data =data>>1;//
	 
	 TM1723_CLK_SetHigh();
	 //data =data>>1;//
	  delay_us(2);
	// data >>=1;//
	
  
     
 }

}
void TIM1723_Write_Cmd(uint8_t cmd)
{
    TM1723_CLK_SetHigh();
	TM1723_Start();
	TM1723_Write_OneByte(cmd);

	TM1723_Stop();
 
}
static void TM1723_Write_Display_Data(uint8_t addr,uint8_t dat)
{
  
   TM1723_CLK_SetHigh();
   TM1723_STB_SetLow();
   TM1723_Write_OneByte(addr);
  
   TM1723_Write_OneByte(dat);
   
   TM1723_STB_SetHigh();

}

/*************************************************************************
 	*
 	* Function Name:void DisplayPanel_Ref_Handler(void)
 	* Function :lcd display panel 
 	* Input Ref:NO
 	* Return Ref:NO
 	* 
*************************************************************************/ 
void DisplayPanel_Ref_Handler(void)
{
    
     static uint8_t timer_timg_flag;
	 TIM1723_Write_Cmd(0x00);
	// TIM1723_Write_Cmd(0x40);
	 TIM1723_Write_Cmd(0x44);

   /***********************setup temperature value ********************************/
	 //digital 1,2 ->display "temperature"  blink  
	 if(run_t.setup_temperature_value ==1){
	     LCD_DisplayNumber_OneTwo_Icon_Handler();
         TIM1723_Write_Cmd(LUM_VALUE);//(0x9B);
	 }
	 else{ //digital "1,2" don't blink LED
	    //display address 0xC2 ->
	    Display_Kill_Dry_Ster_Icon();
		
		//display address 0xC3
		if(run_t.gModel==1){
	     TM1723_Write_Display_Data(0xC3,lcdNumber1_Low[lcd_t.number1_low]+AI_Symbol+lcdNumber2_High[lcd_t.number2_high]);//display  "AI icon"
		}
	   else { 
	 	TM1723_Write_Display_Data(0xC3,(lcdNumber1_Low[lcd_t.number1_low])+lcdNumber2_High[lcd_t.number2_high]);//don't display "AI icon"
		
	    }
	   
	   //display address 0xC4
        TM1723_Write_Display_Data(0xC4,(0x01+lcdNumber2_Low[lcd_t.number2_low]+lcdNumber3_High[lcd_t.number3_high])&0xff);
	 }
	 
	 /**********************************end temperature*****************************************/
     //T2->"WIFI" icon -> 0xC5
     if(run_t.wifi_connect_flag ==0){   // && run_t.gPower_On==1){ //hasn't wifi run_t.wifi_connect_flag
         if(run_t.wifi_led_fast_blink_flag==0){
			 if(lcd_t.gTimer_wifi_500ms >99 && lcd_t.gTimer_wifi_500ms<200){
		          
		           TM1723_Write_Display_Data(0xC5,(WIFI_Symbol+lcdNumber3_Low[lcd_t.number3_low] + lcdNumber4_High[lcd_t.number4_high]) & 0xff); //Wifi
					
			 }
		     else if(lcd_t.gTimer_wifi_500ms <100){
			 	 
		         TM1723_Write_Display_Data(0xC5,(WIFI_NO_Symbol+lcdNumber3_Low[lcd_t.number3_low] + lcdNumber4_High[lcd_t.number4_high]) & 0xff); //Wifi 
	            
			 }
		     else{
		        lcd_t.gTimer_wifi_500ms =0;
		     }
         }
		 else{ //be detected of wifi signal fast blink wiif icon

             if(lcd_t.gTimer_wifi_500ms >14 && lcd_t.gTimer_wifi_500ms<30){ //400ms
		          
		           TM1723_Write_Display_Data(0xC5,(WIFI_Symbol+lcdNumber3_Low[lcd_t.number3_low] + lcdNumber4_High[lcd_t.number4_high]) & 0xff); //Wifi
					
			 }
		     else if(lcd_t.gTimer_wifi_500ms <15){
			 	 
		         TM1723_Write_Display_Data(0xC5,(WIFI_NO_Symbol+lcdNumber3_Low[lcd_t.number3_low] + lcdNumber4_High[lcd_t.number4_high]) & 0xff); //Wifi 
	            
			 }
		     else{
		        lcd_t.gTimer_wifi_500ms =0;
               
			 }
             if(run_t.gTimer_wifi_connect_counter > 134){
                  run_t.gTimer_wifi_connect_counter=0;
                  run_t.wifi_led_fast_blink_flag=0;
			      

			 }

		 }
		
     }
	 else if(run_t.wifi_connect_flag ==1){//wifi be connect is OK 
           run_t.wifi_led_fast_blink_flag=0;
           TM1723_Write_Display_Data(0xC5,(WIFI_Symbol+lcdNumber3_Low[lcd_t.number3_low] + lcdNumber4_High[lcd_t.number4_high]) & 0xff); //Wifi
         //  TIM1723_Write_Cmd(LUM_VALUE);
	 }
	 /*T2 end*/
     //Humidity Icon "0xC9"-numbers "4-4B,4G,4C","5-5A,5F,5E,5D"
     if(run_t.Timer_mode_flag == 1 && run_t.gPower_On == 1 && run_t.setup_timer_timing_item==timer_time){ //digital -> 5,6,7,8 blink .

     	 if(run_t.gTimer_digital5678_ms < 3){
             TM1723_Write_Display_Data(0xC9,(0x01+lcdNumber4_Low[lcd_t.number4_low]+lcdNumber5_High[lcd_t.number5_high]) & 0xff);//display digital '4,5'
          	 TM1723_Write_Display_Data(0xCA,(T15+lcdNumber5_Low[lcd_t.number5_low]+lcdNumber6_High[lcd_t.number6_high]) & 0xff);
          	 TM1723_Write_Display_Data(0xCB,(0x01+lcdNumber6_Low[lcd_t.number6_low]+lcdNumber7_High[lcd_t.number7_high]) & 0xff);
          	 TM1723_Write_Display_Data(0xCC,(T14+lcdNumber7_Low[lcd_t.number7_low]+lcdNumber8_High[lcd_t.number8_high]) & 0xff);
			 if(run_t.disp_wind_speed_grade ==3){
			 	TM1723_Write_Display_Data(0xCE,(T13+lcdNumber8_Low[lcd_t.number8_low]+0xE0) & 0xff);
			  }
			  else if(run_t.disp_wind_speed_grade ==2){
			     TM1723_Write_Display_Data(0xCE,(T13+lcdNumber8_Low[lcd_t.number8_low]+WIND_SPEED_TWO) & 0xff);

			  }
			  else if(run_t.disp_wind_speed_grade ==1){
				 TM1723_Write_Display_Data(0xCE,(T13+lcdNumber8_Low[lcd_t.number8_low]+WIND_SPEED_ONE) & 0xff);
			  }
          }
	     else if(run_t.gTimer_digital5678_ms > 2 && run_t.gTimer_digital5678_ms < 5){
	     	 TM1723_Write_Display_Data(0xC9,(0x01+lcdNumber4_Low[lcd_t.number4_low]+lcdNumber5_High[lcd_t.number5_high]) & 0x0f);//display digital '4,5'
          	 TM1723_Write_Display_Data(0xCA,(T15+lcdNumber5_Low[lcd_t.number5_low]+lcdNumber6_High[lcd_t.number6_high]) & 0x01);
          	 TM1723_Write_Display_Data(0xCB,(0x01+lcdNumber6_Low[lcd_t.number6_low]+lcdNumber7_High[lcd_t.number7_high]) & 0x01);
          	 TM1723_Write_Display_Data(0xCC,(T14+lcdNumber7_Low[lcd_t.number7_low]+lcdNumber8_High[lcd_t.number8_high]) & 0x01);
			 if(run_t.disp_wind_speed_grade ==3){
			    TM1723_Write_Display_Data(0xCE,(T13+lcdNumber8_Low[lcd_t.number8_low]+0xE0) & 0xE1);
			 }
			 else if(run_t.disp_wind_speed_grade ==2){
				 TM1723_Write_Display_Data(0xCE,(T13+lcdNumber8_Low[lcd_t.number8_low]+WIND_SPEED_TWO) & 0xE1);
			 }
			  else if(run_t.disp_wind_speed_grade ==1){
				 TM1723_Write_Display_Data(0xCE,(T13+lcdNumber8_Low[lcd_t.number8_low]+WIND_SPEED_ONE) & 0xE1);
			 }
			 }
	     

	      if(run_t.gTimer_digital5678_ms > 4){
            run_t.gTimer_digital5678_ms=0;
             timer_timg_flag++;
		

		  if(run_t.gTimer_key_timing > 4){

		         run_t.Timer_mode_flag = 0;
				run_t.timer_time_minutes = 0;
				
				run_t.gTimer_timing=0;
			if(run_t.timer_time_hours !=0){	 
			      run_t.timer_timing_define_flag = timing_success;
				  run_t.setup_timer_flag = 1;
			}
			  run_t.setup_timer_timing_item=0;


		  }
            
            
	     }
     }
     else{
     	TM1723_Write_Display_Data(0xC9,(0x01+lcdNumber4_Low[lcd_t.number4_low]+lcdNumber5_High[lcd_t.number5_high]) & 0xff);//display digital '4,5'
     }
  	 
	 //T15,"0xCA"Numbers'5'->"5B,5G,5C",'6'->"6A,6F,6E,6D" -> "T15,T13,T11-ON-OFF"
	 if(run_t.disp_wind_speed_grade ==3){
	      if(lcd_t.gTimer_fan_10ms >9 && lcd_t.gTimer_fan_10ms<20){
	      	 if(run_t.Timer_mode_flag == 0)
	            TM1723_Write_Display_Data(0xCA,T15+lcdNumber5_Low[lcd_t.number5_low]+lcdNumber6_High[lcd_t.number6_high]);//display digital '5,6'
	        
	         }
	        else if(lcd_t.gTimer_fan_10ms <10){
		  	  if(run_t.Timer_mode_flag == 0)
			  	TM1723_Write_Display_Data(0xCA,lcdNumber5_Low[lcd_t.number5_low]+lcdNumber6_High[lcd_t.number6_high]);//display digital '5,6'
		
			  }
		    else if(lcd_t.gTimer_fan_10ms >19){
			   lcd_t.gTimer_fan_10ms =0;
		  }
	  }
	  else if(run_t.disp_wind_speed_grade ==2){
	      if(lcd_t.gTimer_fan_10ms >39 && lcd_t.gTimer_fan_10ms<80){
	      	 if(run_t.Timer_mode_flag == 0)
	         	TM1723_Write_Display_Data(0xCA,T15+lcdNumber5_Low[lcd_t.number5_low]+lcdNumber6_High[lcd_t.number6_high]);//display digital '5,6'
	      }
		  else if(lcd_t.gTimer_fan_10ms <40){
		  	   if(run_t.Timer_mode_flag == 0)
			  		TM1723_Write_Display_Data(0xCA,lcdNumber5_Low[lcd_t.number5_low]+lcdNumber6_High[lcd_t.number6_high]);//display digital '5,6'
			  
		  }
		  else if(lcd_t.gTimer_fan_10ms >79){
			   lcd_t.gTimer_fan_10ms =0;
		  }
	  }
	  else if(run_t.disp_wind_speed_grade ==1){
	      if(lcd_t.gTimer_fan_10ms >79 && lcd_t.gTimer_fan_10ms<160){
	      	  if(run_t.Timer_mode_flag == 0)
	         	TM1723_Write_Display_Data(0xCA,T15+lcdNumber5_Low[lcd_t.number5_low]+lcdNumber6_High[lcd_t.number6_high]);//display digital '5,6'
	      }
		  else if(lcd_t.gTimer_fan_10ms <80){
		  	   if(run_t.Timer_mode_flag == 0)
			  		TM1723_Write_Display_Data(0xCA,lcdNumber5_Low[lcd_t.number5_low]+lcdNumber6_High[lcd_t.number6_high]);//display digital '5,6'
		  }
		  else if(lcd_t.gTimer_fan_10ms >159){
			   lcd_t.gTimer_fan_10ms =0;
		  }
	  }
	  /*********************END T15***********************/
     //address"0xCB" ->numbers .T9":","6->6B,6G,6C","7->7A,7F,7E,7D"
     if(lcd_t.gTimer_colon_ms < 6){
     	 if(run_t.Timer_mode_flag == 0)
	   		TM1723_Write_Display_Data(0xCB,0x01+lcdNumber6_Low[lcd_t.number6_low]+lcdNumber7_High[lcd_t.number7_high]);//display "6,7"
	  }
     else if(lcd_t.gTimer_colon_ms > 5 && lcd_t.gTimer_colon_ms < 11){
     	 if(run_t.Timer_mode_flag == 0)
	 		TM1723_Write_Display_Data(0xCB,lcdNumber6_Low[lcd_t.number6_low]+lcdNumber7_High[lcd_t.number7_high]);//display "6,7"
	 	
	 }
	 else  lcd_t.gTimer_colon_ms = 0;
        
	 //T14,wind_speed ->numbers .high 4 bits -T14,"7->7B,7G,7C",low 4 bits "8->8A,8F,8E,8D"
     if(run_t.disp_wind_speed_grade ==3){
		 if(lcd_t.gTimer_fan_10ms >9 && lcd_t.gTimer_fan_10ms<20){
		 	 if(run_t.Timer_mode_flag == 0)
		      TM1723_Write_Display_Data(0xCC,lcdNumber7_Low[lcd_t.number7_low]+lcdNumber8_High[lcd_t.number8_high]);//display "7,8'

		 }
		 else if(lcd_t.gTimer_fan_10ms <10){
		 	 if(run_t.Timer_mode_flag == 0)
	  			TM1723_Write_Display_Data(0xCC,T14+lcdNumber7_Low[lcd_t.number7_low]+lcdNumber8_High[lcd_t.number8_high]);//display "t,c
	  	
		  }
		 else if(lcd_t.gTimer_fan_10ms > 19){
            lcd_t.gTimer_fan_10ms=0;
	     }
     }
	 else if(run_t.disp_wind_speed_grade ==2){
		if(lcd_t.gTimer_fan_10ms >39 && lcd_t.gTimer_fan_10ms<80){
			 if(run_t.Timer_mode_flag == 0)
		      TM1723_Write_Display_Data(0xCC,lcdNumber7_Low[lcd_t.number7_low]+lcdNumber8_High[lcd_t.number8_high]);//display "7,8'
		    

		 }
		 else if(lcd_t.gTimer_fan_10ms <40){
		 	 if(run_t.Timer_mode_flag == 0)
	  			TM1723_Write_Display_Data(0xCC,T14+lcdNumber7_Low[lcd_t.number7_low]+lcdNumber8_High[lcd_t.number8_high]);//display "t,c
	  		
		 }
		 else if(lcd_t.gTimer_fan_10ms > 79){
          lcd_t.gTimer_fan_10ms=0;
	     }

	}
	else if(run_t.disp_wind_speed_grade ==1){
		if(lcd_t.gTimer_fan_10ms >79 && lcd_t.gTimer_fan_10ms<160){
			 if(run_t.Timer_mode_flag == 0)
					TM1723_Write_Display_Data(0xCC,lcdNumber7_Low[lcd_t.number7_low]+lcdNumber8_High[lcd_t.number8_high]);//display "7,8'
	
	   }
	   else if(lcd_t.gTimer_fan_10ms <80){
	   	 if(run_t.Timer_mode_flag == 0)
			TM1723_Write_Display_Data(0xCC,T14+lcdNumber7_Low[lcd_t.number7_low]+lcdNumber8_High[lcd_t.number8_high]);//display "t,c
		
	   }
	   else if(lcd_t.gTimer_fan_10ms > 159){
          lcd_t.gTimer_fan_10ms=0;

	   	}
	}


     /**********T14 WIND SPEED**********/
    //T13,address "0xCE",numbers."8->8B,8G,8C","null,T19,T18,T17" ->"T13,T11,T15 ON-OFF"
     if(run_t.disp_wind_speed_grade ==3){
		 if(lcd_t.gTimer_fan_10ms>9 && lcd_t.gTimer_fan_10ms<20){
		 	 if(run_t.Timer_mode_flag == 0)
					TM1723_Write_Display_Data(0xCE,T13+lcdNumber8_Low[lcd_t.number8_low]+0xE0);//display "t,c"
		  }
		  else if(lcd_t.gTimer_fan_10ms <10){
		  	 if(run_t.Timer_mode_flag == 0)
		         TM1723_Write_Display_Data(0xCE,lcdNumber8_Low[lcd_t.number8_low]+0xE0);//display "t,c"
		   }
		  else if(lcd_t.gTimer_fan_10ms > 19){
	           lcd_t.gTimer_fan_10ms=0;
	       }
		  
      }
	  if(run_t.disp_wind_speed_grade ==2){
		 if(lcd_t.gTimer_fan_10ms >39 && lcd_t.gTimer_fan_10ms<80){
		 	 if(run_t.Timer_mode_flag == 0)
					TM1723_Write_Display_Data(0xCE,T13+lcdNumber8_Low[lcd_t.number8_low]+WIND_SPEED_TWO);//display "t,c"
		
		  }
		  else if(lcd_t.gTimer_fan_10ms <40){
		  	 if(run_t.Timer_mode_flag == 0)
		         TM1723_Write_Display_Data(0xCE,lcdNumber8_Low[lcd_t.number8_low]+WIND_SPEED_TWO);//display "t,c"
		    

		  }
		  else if(lcd_t.gTimer_fan_10ms > 79){
	           lcd_t.gTimer_fan_10ms=0;
	       }
		  
      }
	  if(run_t.disp_wind_speed_grade ==1){
		 if(lcd_t.gTimer_fan_10ms >79 && lcd_t.gTimer_fan_10ms<160){
		 	 if(run_t.Timer_mode_flag == 0)
					TM1723_Write_Display_Data(0xCE,T13+lcdNumber8_Low[lcd_t.number8_low]+WIND_SPEED_ONE);//display "t,c"
			
		  }
		  else if(lcd_t.gTimer_fan_10ms <80){
		  	 if(run_t.Timer_mode_flag == 0)
		         TM1723_Write_Display_Data(0xCE,lcdNumber8_Low[lcd_t.number8_low]+WIND_SPEED_ONE);//display "t,c"

		  }
		  else if(lcd_t.gTimer_fan_10ms >159){
	           lcd_t.gTimer_fan_10ms=0;
	       }
		  
      }
	  /*-------------END T13-----------------*/
	  //T16 WIND SPPEDD ->address "0xCF",T12,T11,T10
	  if(run_t.disp_wind_speed_grade ==3){
	      if(lcd_t.gTimer_fan_10ms >9 && lcd_t.gTimer_fan_10ms<20){
	      	    if(run_t.Timer_mode_flag ==0)
	               TM1723_Write_Display_Data(0xCF,((T11+T16) & 0x05));//
	      }
  
		  else if(lcd_t.gTimer_fan_10ms <10){
		  	if(run_t.Timer_mode_flag ==0)
		         TM1723_Write_Display_Data(0xCF,((T16+T12+T10)& 0x0B));//

		  }
		  else if(lcd_t.gTimer_fan_10ms > 19){
        		lcd_t.gTimer_fan_10ms=0;
         }
	  }
	  else if(run_t.disp_wind_speed_grade ==2){
	      if(lcd_t.gTimer_fan_10ms >39 && lcd_t.gTimer_fan_10ms<80){
	      	if(run_t.Timer_mode_flag ==0)
	            TM1723_Write_Display_Data(0xCF,((T11+T16)& 0x05));//
	      }
  		 else if(lcd_t.gTimer_fan_10ms <40){
  		 	if(run_t.Timer_mode_flag ==0)
		         TM1723_Write_Display_Data(0xCF,((T16+T12+T10)&0x0B));//

		  }
		  else if(lcd_t.gTimer_fan_10ms >79){
        		lcd_t.gTimer_fan_10ms=0;
         }
	  }
	  else if(run_t.disp_wind_speed_grade ==1){
	      if(lcd_t.gTimer_fan_10ms >79 && lcd_t.gTimer_fan_10ms<160){
	      	if(run_t.Timer_mode_flag ==0)
	            TM1723_Write_Display_Data(0xCF,((T11+T16)&0x05));//
	      }
  		  else if(lcd_t.gTimer_fan_10ms <80){
  		  	if(run_t.Timer_mode_flag ==0)
		         TM1723_Write_Display_Data(0xCF,((T16+T12+T10)& 0x0B));//

		  }
		  else if(lcd_t.gTimer_fan_10ms >159){
        		lcd_t.gTimer_fan_10ms=0;
         }
	  }
	  /****************T16 WIND SPEED********************/
	//open display
	 TIM1723_Write_Cmd(LUM_VALUE);//(0x97);//(0x94);//(0x9B);

}

/******************************************************************************
	*
	*Function Name:static void Display_Kill_Dry_Ster_Icon(void)
	*Function: display of icon , "1" -> ON ,"0"-> OFF
	*
	*
	*
******************************************************************************/
static void Display_Kill_Dry_Ster_Icon(void)
{

   //number "1,2" -> temperature

	  if(run_t.gDry==1 && run_t.gPlasma==1 && run_t.gUltransonic==1){

	     //display address 0xc2
	     TM1723_Write_Display_Data(0xC2,((0X01+DRY_Symbol+KILL_Symbol+BUG_Symbol)+lcdNumber1_High[lcd_t.number1_high])&0xff);//display digital "temp
	     
		 
	  }
	  else if(run_t.gDry==0 && run_t.gPlasma==1 && run_t.gUltransonic==1){

	      TM1723_Write_Display_Data(0xC2,((0X01+DRY_NO_Symbol+KILL_Symbol+BUG_Symbol))+lcdNumber1_High[lcd_t.number1_high]);//display digital "temp
	    
	   }
	   else if(run_t.gDry==0 && run_t.gPlasma==0 && run_t.gUltransonic ==1){

	        TM1723_Write_Display_Data(0xC2,((0X01+DRY_NO_Symbol+KILL_NO_Symbol+BUG_Symbol))+lcdNumber1_High[lcd_t.number1_high]);//display digit
	        
	   }
	  else if(run_t.gDry==0 && run_t.gPlasma==0 && run_t.gUltransonic ==0){

	        TM1723_Write_Display_Data(0xC2,0X01+DRY_NO_Symbol+KILL_NO_Symbol+BUG_NO_Symbol+lcdNumber1_High[lcd_t.number1_high]);//display digit
	       
	  }
	  else if(run_t.gDry==1 && run_t.gPlasma==0 && run_t.gUltransonic ==1){

	        TM1723_Write_Display_Data(0xC2,((0X01+DRY_Symbol+KILL_NO_Symbol+BUG_Symbol))+lcdNumber1_High[lcd_t.number1_high]);//display digit
	       
	  }
	  else if(run_t.gDry==1 && run_t.gPlasma==0 && run_t.gUltransonic ==0){

	        TM1723_Write_Display_Data(0xC2,((0X01+DRY_Symbol+KILL_NO_Symbol+BUG_NO_Symbol))+lcdNumber1_High[lcd_t.number1_high]);//display digit
	       
	  }
	  else if(run_t.gDry==0 && run_t.gPlasma==1 && run_t.gUltransonic ==0){
	   
			  TM1723_Write_Display_Data(0xC2,((0X01+DRY_NO_Symbol+KILL_Symbol+BUG_NO_Symbol))+lcdNumber1_High[lcd_t.number1_high]);//display digit
			  
	 }
	  else if(run_t.gDry==1 && run_t.gPlasma==1 && run_t.gUltransonic ==0){
		 
				TM1723_Write_Display_Data(0xC2,((0X01+DRY_Symbol+KILL_Symbol+BUG_NO_Symbol))+lcdNumber1_High[lcd_t.number1_high]);//display digit
			
	   }

  
   TIM1723_Write_Cmd(LUM_VALUE);
   
}
/*************************************************************************************
	*
	*Function Name: static void LCD_DisplayNumber_OneTwo_Icon_Handler(void)
	*Function : set up temperature value by blink 
	*
	*
	*
	*
*************************************************************************************/
static void LCD_DisplayNumber_OneTwo_Icon_Handler(void)
{
     static uint8_t number_blink_times;
	 if(run_t.gTimer_numbers_one_two_blink < 6  ){ //disp number
	     //display address 0xC2
	     if(run_t.gDry ==1 && run_t.gPlasma ==1  && run_t.gUltransonic==1)
		 	TM1723_Write_Display_Data(0xC2,((0X01+DRY_Symbol+KILL_Symbol+BUG_Symbol)+lcdNumber1_High[lcd_t.number1_high]) & 0xff);//display digital "temp
         else if(run_t.gDry ==0 && run_t.gPlasma ==1 && run_t.gUltransonic==1)
		 	TM1723_Write_Display_Data(0xC2,((0X01+KILL_Symbol+BUG_Symbol)+lcdNumber1_High[lcd_t.number1_high]) & 0xff);//display digital "temp
         else if(run_t.gDry ==0 && run_t.gPlasma ==0 && run_t.gUltransonic==1)
		 	TM1723_Write_Display_Data(0xC2,(0X01+BUG_Symbol+lcdNumber1_High[lcd_t.number1_high]) & 0xff);//display digital "temp
		 else if(run_t.gDry ==0 && run_t.gPlasma ==0 && run_t.gUltransonic==0){
		 	TM1723_Write_Display_Data(0xC2,(0X01+lcdNumber1_High[lcd_t.number1_high]) & 0xff);//display digital "temp
		 }
		 else if(run_t.gDry ==1 && run_t.gPlasma ==1 && run_t.gUltransonic==0){
		 	TM1723_Write_Display_Data(0xC2,((0X01+DRY_Symbol+KILL_Symbol)+lcdNumber1_High[lcd_t.number1_high]) & 0xff);//display digital "temp
		 }
		 else if(run_t.gDry ==0 && run_t.gPlasma ==1 && run_t.gUltransonic==0){
		 	TM1723_Write_Display_Data(0xC2,((0X01+KILL_Symbol)+lcdNumber1_High[lcd_t.number1_high]) & 0xff);//display digital "temp
		 }
		 else if(run_t.gDry ==1 && run_t.gPlasma ==0 && run_t.gUltransonic==0){
		 	TM1723_Write_Display_Data(0xC2,((0X01+DRY_Symbol)+lcdNumber1_High[lcd_t.number1_high]) & 0xff);//display digital "temp
		 }
		 else if(run_t.gDry ==1 && run_t.gPlasma ==0 && run_t.gUltransonic==1){
		 	TM1723_Write_Display_Data(0xC2,((0X01+DRY_Symbol+BUG_Symbol)+lcdNumber1_High[lcd_t.number1_high]) & 0xff);//display digital "temp
		 }
		 //display addres 0xC3 -> AI icon
		 if(run_t.gModel ==1)
	        TM1723_Write_Display_Data(0xC3,(lcdNumber1_Low[lcd_t.number1_low]+AI_Symbol+lcdNumber2_High[lcd_t.number2_high]) & 0xff);//display  "AI icon
         else
		 	TM1723_Write_Display_Data(0xC3,(lcdNumber1_Low[lcd_t.number1_low]+AI_NO_Symbol+lcdNumber2_High[lcd_t.number2_high]) & 0xfe);//display  "AI icon

		 //display address 0xC4 -> temperature icon T7
		 TM1723_Write_Display_Data(0xC4,(0x01+lcdNumber2_Low[lcd_t.number2_low]+lcdNumber3_High[lcd_t.number3_high])&0xff);//display "t,c"
		 
		 
	 	 }//don't Display numbers
		 else if(run_t.gTimer_numbers_one_two_blink > 5  && run_t.gTimer_numbers_one_two_blink <11){ //don't display 
            //display address 0xC2 -> 
	          if(run_t.gDry ==1 && run_t.gPlasma ==1  && run_t.gUltransonic==1)
			 	TM1723_Write_Display_Data(0xC2,((0X01+DRY_Symbol+KILL_Symbol+BUG_Symbol)+lcdNumber1_High[lcd_t.number1_high]) & 0x0f);//display digital "temp
	         else if(run_t.gDry ==0 && run_t.gPlasma ==1 && run_t.gUltransonic==1)
			 	TM1723_Write_Display_Data(0xC2,((0X01+KILL_Symbol+BUG_Symbol)+lcdNumber1_High[lcd_t.number1_high]) & 0x0f);//display digital "temp
	         else if(run_t.gDry ==0 && run_t.gPlasma ==0 && run_t.gUltransonic==1)
			 	TM1723_Write_Display_Data(0xC2,((0X01+BUG_Symbol)+lcdNumber1_High[lcd_t.number1_high]) & 0x0f);//display digital "temp
			 else if(run_t.gDry ==0 && run_t.gPlasma ==0 && run_t.gUltransonic==0){
			 	TM1723_Write_Display_Data(0xC2,((0X01)+lcdNumber1_High[lcd_t.number1_high]) & 0x0f);//display digital "temp
			 }
			 else if(run_t.gDry ==1 && run_t.gPlasma ==1 && run_t.gUltransonic==0){
			 	TM1723_Write_Display_Data(0xC2,((0X01+DRY_Symbol+KILL_Symbol)+lcdNumber1_High[lcd_t.number1_high]) & 0x0f);//display digital "temp
			 }
			 else if(run_t.gDry ==0 && run_t.gPlasma ==1 && run_t.gUltransonic==0){
			 	TM1723_Write_Display_Data(0xC2,((0X01+KILL_Symbol)+lcdNumber1_High[lcd_t.number1_high]) & 0x0f);//display digital "temp
			 }
			 else if(run_t.gDry ==1 && run_t.gPlasma ==0 && run_t.gUltransonic==0){
			 	TM1723_Write_Display_Data(0xC2,((0X01+DRY_Symbol)+lcdNumber1_High[lcd_t.number1_high]) & 0x0f);//display digital "temp
			 }
			 else if(run_t.gDry ==1 && run_t.gPlasma ==0 && run_t.gUltransonic==1){
			 	TM1723_Write_Display_Data(0xC2,((0X01+DRY_Symbol+BUG_Symbol)+lcdNumber1_High[lcd_t.number1_high]) & 0x0f);//display digital "temp
			 }
			 
				//TM1723_Write_Display_Data(0xC2,(((0X01+DRY_Symbol+KILL_Symbol+BUG_Symbol)+lcdNumber1_High[lcd_t.number1_high]) & 0x0F));
	           // display address 0xC3
	           if(run_t.gModel ==1)
			     TM1723_Write_Display_Data(0xC3,((lcdNumber1_Low[lcd_t.number1_low]+AI_Symbol+lcdNumber2_High[lcd_t.number2_high])) & 0x1);
	           else
			   	 TM1723_Write_Display_Data(0xC3,((lcdNumber1_Low[lcd_t.number1_low]+AI_NO_Symbol+lcdNumber2_High[lcd_t.number2_high])) & 0x0);
	           //display address 0xC4
				TM1723_Write_Display_Data(0xC4,(0x01+lcdNumber2_Low[lcd_t.number2_low]+lcdNumber3_High[lcd_t.number3_high])&0xF1);//display "t,c"
        }
		else {
             run_t.gTimer_numbers_one_two_blink =0;
			 number_blink_times++;
		     if(number_blink_times > 3){
                 number_blink_times =0;
				 run_t.setup_temperature_value =0;
			     run_t.panel_key_setup_timer_flag=0;
				 run_t.temperature_set_flag = 1;
				 run_t.gTimer_temp_delay =67;
				
				 
			 }

		}
//	 

}
/*************************************************************************************
	*
	*Function Name: static void LCD_DisplayNumber_OneTwo_Icon_Handler(void)
	*
	*
	*
	*
	*
*************************************************************************************/

void LCD_Display_Wind_Icon_Handler(void)
{
	 TIM1723_Write_Cmd(0x00);
	 TIM1723_Write_Cmd(0x40);
	 TIM1723_Write_Cmd(0x44);

	  TM1723_Write_Display_Data(0xC2,0x0);
	  TM1723_Write_Display_Data(0xC3,0x0);
	  TM1723_Write_Display_Data(0xC4,0x0);
	  TM1723_Write_Display_Data(0xC5,0x0);
	  TM1723_Write_Display_Data(0xC9,0x0);
	  if(run_t.Timer_mode_flag ==0)
	  	TM1723_Write_Display_Data(0xCB,0x0);

     //only display wind speed "icon"
      if(run_t.disp_wind_speed_grade ==1){
	      if(lcd_t.gTimer_fan_10ms >79 && lcd_t.gTimer_fan_10ms<160){
	      	   if(run_t.Timer_mode_flag ==0){
		  	   TM1723_Write_Display_Data(0xCA,(T15+lcdNumber5_Low[lcd_t.number5_low]+lcdNumber6_High[lcd_t.number6_high])&0x01);//"T15"
	      		TM1723_Write_Display_Data(0xCC,(lcdNumber7_Low[lcd_t.number7_low]+lcdNumber8_High[lcd_t.number8_high])&0x00);//"T14"
		  	    TM1723_Write_Display_Data(0xCE,(T13+lcdNumber8_Low[lcd_t.number8_low]+0xE0)&0x01); //"T13"
	            TM1723_Write_Display_Data(0xCF,((T11+T16)&0x05));//
	           }
	           else{
	           		if(run_t.gTimer_digital5678_ms > 2 && run_t.gTimer_digital5678_ms < 5){
	           			 TM1723_Write_Display_Data(0xCA,(T15+lcdNumber5_Low[lcd_t.number5_low]+lcdNumber6_High[lcd_t.number6_high])&0x01);//"T15"
				      		TM1723_Write_Display_Data(0xCC,(lcdNumber7_Low[lcd_t.number7_low]+lcdNumber8_High[lcd_t.number8_high])&0x00);//"T14"
					  	    TM1723_Write_Display_Data(0xCE,(T13+lcdNumber8_Low[lcd_t.number8_low]+0xE0)&0x01); //"T13"
				            TM1723_Write_Display_Data(0xCF,((T11+T16)&0x05));//

	           		}	
	           }
	         

	      }
  		  else if(lcd_t.gTimer_fan_10ms <80){
  		  	   if(run_t.Timer_mode_flag ==0){
			  	   TM1723_Write_Display_Data(0xCA,(lcdNumber5_Low[lcd_t.number5_low]+lcdNumber6_High[lcd_t.number6_high])&0x00);//"T15"
			  	   TM1723_Write_Display_Data(0xCC,(T14+lcdNumber7_Low[lcd_t.number7_low]+lcdNumber8_High[lcd_t.number8_high])&0x01);//"T14"
			  	   TM1723_Write_Display_Data(0xCE,(lcdNumber8_Low[lcd_t.number8_low]+0xE0)&0x00);//display "T13"
			       TM1723_Write_Display_Data(0xCF,((T16+T12+T10)& 0x0B));//
		       }
		       else{
		       	if(run_t.gTimer_digital5678_ms > 2 && run_t.gTimer_digital5678_ms < 5){
		       	   TM1723_Write_Display_Data(0xCA,(lcdNumber5_Low[lcd_t.number5_low]+lcdNumber6_High[lcd_t.number6_high])&0x00);//"T15"
			  	   TM1723_Write_Display_Data(0xCC,(T14+lcdNumber7_Low[lcd_t.number7_low]+lcdNumber8_High[lcd_t.number8_high])&0x01);//"T14"
			  	   TM1723_Write_Display_Data(0xCE,(lcdNumber8_Low[lcd_t.number8_low]+0xE0)&0x00);//display "T13"
			       TM1723_Write_Display_Data(0xCF,((T16+T12+T10)& 0x0B));//

		       	}

		       }


		  }
		  else if(lcd_t.gTimer_fan_10ms >159){
        		lcd_t.gTimer_fan_10ms=0;
         }
      }
	//open display
	 TIM1723_Write_Cmd(LUM_VALUE);//(0x9B);



}
