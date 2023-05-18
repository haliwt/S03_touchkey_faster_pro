#include "display.h"
#include "cmd_link.h"
#include "run.h"
#include "key.h"
#include "led.h"
#include "single_mode.h"
#include "lcd.h"


/*************************************************************
 * 
 * Function Name:void DisplaySMG_LED(void)
 * Function :
 * 
*************************************************************/
void Display_Temperature_Humidity_Value(void)
{
     static uint8_t m,n,p,q;
    if(run_t.gPower_On==1){

		
		panel_led_fun();

	    m = (run_t.dispTime_hours/10) ;
		n=	(run_t.gTime_hours%10); 
		p = (run_t.dispTime_minutes/10);
		q=  (run_t.dispTime_minutes%10);

        //hours
		 lcd_t.number5_high = m;
		 lcd_t.number5_low = m;

		  lcd_t.number6_high = n;
		 lcd_t.number6_low = n;

		 //minutes 
		 lcd_t.number7_high = p;
		 lcd_t.number7_low = p;

		  lcd_t.number8_high = q;
		 lcd_t.number8_low = q;

		 DisplayPanel_Ref_Handler();
		

   }

		   
}


/******************************************************************************
*
*Function Name:static void Setup_Timer_Times(void)
*Funcion:display setup timer times  //__asm("NOP");//等待1个指令周期，系统主频24M
*Iinput Ref:NO
*Return Ref:NO
*
******************************************************************************/


