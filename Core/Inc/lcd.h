#ifndef __LCD_H_
#define __LCD_H_
#include "main.h"

#define TM1723_POWER_ON()				HAL_GPIO_WritePin(LCD_POWER_GPIO_Port, LCD_POWER_Pin,GPIO_PIN_SET)
#define TM1723_POWER_OFF()             HAL_GPIO_WritePin(LCD_POWER_GPIO_Port, LCD_POWER_Pin,GPIO_PIN_RESET)


#define LCD_BACK_LIGHT_ON()				HAL_GPIO_WritePin(BACK_LIGHT_GPIO_Port,BACK_LIGHT_Pin,GPIO_PIN_SET)
#define LCD_BACK_LIGHT_OFF()            HAL_GPIO_WritePin(BACK_LIGHT_GPIO_Port,BACK_LIGHT_Pin,GPIO_PIN_RESET)




#define TM1723_CLK_SetHigh()            HAL_GPIO_WritePin(TM1723_SCLK_GPIO_Port,TM1723_SCLK_Pin ,GPIO_PIN_SET)    // ����ߵ�ƽ
#define TM1723_CLK_SetLow()             HAL_GPIO_WritePin(TM1723_SCLK_GPIO_Port,TM1723_SCLK_Pin ,GPIO_PIN_RESET)  // ����͵�ƽ

#define TM1723_DIO_SetHigh()            HAL_GPIO_WritePin(TM1723_DIO_GPIO_Port,TM1723_DIO_Pin,GPIO_PIN_SET)    // ����ߵ�ƽ
#define TM1723_DIO_SetLow()             HAL_GPIO_WritePin(TM1723_DIO_GPIO_Port,TM1723_DIO_Pin,GPIO_PIN_RESET)    // output low level

#define TM1723_STB_SetHigh()            HAL_GPIO_WritePin(TM1723_STB_GPIO_Port,TM1723_STB_Pin,GPIO_PIN_SET)
#define TM1723_STB_SetLow()             HAL_GPIO_WritePin(TM1723_STB_GPIO_Port,TM1723_STB_Pin,GPIO_PIN_RESET)

//extern volatile uint8_t DispData[3];//??LED??
#define		BitSET(x,y)		x|=(1<<y)				//?1
#define		BitCLR(x,y)		x&=~(1<<y)				//?0


#define DisplayReg           0x40// to write display register 
  
#define AddrFixed 	         0x44// 
#define AddrAutoAdd         0x40 
 
   
  
#define OpenDispTM1723          0x97 // full screen is tunr on light
#define OpenDispTM1723_5        0X9f
#define TM1723Default          	0x87
#define CloseDispTM1723		    0x80// 



 //Display Address 
#define Addr00H  		0xC0//??00H
#define Addr01H  		0xC1//??01H

#define Addr02H  		0xC2//??02H
#define Addr03H  		0xC3//??03H
//digital 3
#define Addr04H  		0xC4//??03H
#define Addr05H  		0xC5//??03H
//digital 4
#define Addr06H  		0xC6//??00H
#define Addr07H  		0xC7//??01H


//Humidity SMG address
#define Addr08H  		0xC8//??02H
#define Addr09H  		0xC9//??03H

#define Addr0AH  		0xCA//??03H
#define Addr0BH  		0xCB//??03H

//temperature SMG address
#define Addr0CH         0xCC//??0CH
#define Addr0DH         0xCD//??0DH

#define Addr0EH         0xCE//??0CH
#define Addr0FH         0xCF//??0DH




typedef struct _LCD_Ref{

     uint8_t wifi_glint_flag;

	 uint8_t gTimer_wifi_500ms;
	 uint8_t gTimer_fan_10ms;
     uint8_t display_beijing_time_flag ;

	 
	 uint8_t number1_low;
	 uint8_t number1_high;
	 uint8_t number2_low;
	 uint8_t number2_high;
	 uint8_t number3_low;
	 uint8_t number3_high;
	 uint8_t number4_low;
	 uint8_t number4_high;
	 
	 uint8_t number5_low;
	 uint8_t number5_high;

	 uint8_t number6_low;
	 uint8_t number6_high;

	 uint8_t number7_low;
	 uint8_t number7_high;

	 uint8_t number8_low;
	 uint8_t number8_high;

	 uint8_t  gTimer_colon_ms;


}lcd_ref;



extern lcd_ref lcd_t; 

void DisplayPanel_TestHandler(void);

void DisplayPanel_Ref_Handler(void);

void TIM1723_Write_Cmd(uint8_t cmd);
void LCD_Display_Wind_Icon_Handler(void);



#endif 
