#include "led.h"
#include "run.h"
#include "lcd.h"
#include "delay.h"
void (*panel_led_fun)(void);


IWDG_HandleTypeDef IWDG_Handler;

void IWDG_Init(uint8_t prer,uint16_t rlr)
{
    IWDG_Handler.Instance=IWDG;
    IWDG_Handler.Init.Prescaler=prer;	//����IWDG��Ƶϵ��
    IWDG_Handler.Init.Window = rlr;
    IWDG_Handler.Init.Reload=rlr;		//��װ��ֵ
  		//��ʼ��IWDG,Ĭ�ϻῪ���������Ź�	
  if (HAL_IWDG_Init(&IWDG_Handler) != HAL_OK)
  {
    Error_Handler();
  }
}
  

//ι�������Ź�
void IWDG_Feed(void)
{   
    HAL_IWDG_Refresh(&IWDG_Handler); 	//ι��
}


/*******************************************************************************************
 	*
 	* Function Name:void Panel_Led_OnOff_Function(void)
 	* Function :panel of led turn on or off which key function 
 	* Input Ref:NO
 	* Return Ref:NO
 	* 
*******************************************************************************************/  
void Breath_Led(void)
{


	static uint32_t k;
   
    k++;

	if(k<40000){

	POWER_ON_LED() ;//LED_POWER_ON();



	}
    if(k>40000 && k <80000){

	   POWER_OFF_LED() ; //LED_POWER_OFF();

	}
	if(k>80000 && k< 120000){

	  POWER_TOGGLE();


	}

	if(k>120000){

        k=0;
	}


}
/*******************************************************************************************
 	*
 	* Function Name:void Panel_Led_OnOff_Function(void)
 	* Function :panel of led turn on or off which key function 
 	* Input Ref:NO
 	* Return Ref:NO
 	* 
*******************************************************************************************/  
void Lcd_PowerOn_Fun(void)
{
    POWER_ON_LED()	;//key of led turn On
    LED_MODEL_ON();
	LCD_BACK_LIGHT_ON();
	

}


void Lcd_PowerOff_Fun(void)
{

	POWER_OFF_LED()  ;
	LED_MODEL_OFF();
	LCD_BACK_LIGHT_OFF();
    TIM1723_Write_Cmd(0x80);//(0x94);//(0x9B);;


}


/*******************************************************************************************
 	*
 	* Function Name:void Panel_Led_OnOff_Function(void)
 	* Function :panel of led turn on or off which key function 
 	* Input Ref:NO
 	* Return Ref:NO
 	* 
*******************************************************************************************/  
void Panel_Led_OnOff_Function(void)
{
	
 
}

/*******************************************************************************************
 	*
 	* Function Name:void Panel_Led_OnOff_Function(void)
 	* Function :panel of led turn on or off which key function 
 	* Input Ref:NO
 	* Return Ref:NO
 	* 
*******************************************************************************************/  
void Panel_Led_OnOff_RunCmd(void (*panelledHandler)(void))
{

	 panel_led_fun= panelledHandler;
}

