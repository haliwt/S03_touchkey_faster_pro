#ifndef __KEY_H_
#define __KEY_H_
#include "main.h"


#define INT_KEY_VALUE()              HAL_GPIO_ReadPin(TOUCH_KEY_INT_GPIO_Port ,TOUCH_KEY_INT_Pin)//

#define POWER_KEY_VALUE()            HAL_GPIO_ReadPin(KEY_POWER_GPIO_Port ,KEY_POWER_Pin)

#define MODE_KEY_VALUE()             HAL_GPIO_ReadPin(KEY_MODE_GPIO_Port,KEY_MODE_Pin)


#define DEC_KEY_VALUE()              HAL_GPIO_ReadPin(KEY_DEC_GPIO_Port,KEY_DEC_Pin)
#define ADD_KEY_VALUE()              HAL_GPIO_ReadPin(KEY_ADD_GPIO_Port,KEY_ADD_Pin)

#define VK36N4D_INT_VALUE()          HAL_GPIO_ReadPin(VK36N4D_INT_GPIO_Port,VK36N4D_INT_Pin) 



#define		_KEY_ALL_OFF				 0XFF

//normal times be pressed key
#define		_KEY_POWER     		0x01  //key power
#define		_KEY_MODE     		0x02  //key mode
#define		_KEY_DEC     			0x04  //key dec
#define		_KEY_ADD     			0x08  //key add



//long times be pressed
#define		_KEY_CONT_1_POWER     	     0x81    //??????--???
#define		_KEY_CONT_2_MODE   		     0x82    //??????--???
#define		_KEY_CONT_3_DEC    	         0x84    //?????? ?????
#define		_KEY_CONT_4_ADD     	     0x88    //??? --?????

#define KEY0_PRES 	0x01
#define KEY1_PRES	0x02
#define KEY2_PRES	0x04
#define WKUP_PRES   0x08



#define HIGH_LEVEL                   1  /* 根据原理图设计，KEY1按下时引脚为低电平，所以这里设置为0 */


typedef enum{

	POWER_OFF_ITEM,
	POWER_ON_ITEM,
	LINK_WIFI_ITEM,
	MODE_KEY_ITEM,
	MODE_KEY_LONG_TIME_KEY,
	ADD_KEY_ITEM,
	DEC_KEY_ITEM,
	KEY_NULL
}key_state;





typedef enum
{
  KEY_UP   = 0,
  KEY_DOWN = 1,
}KEYState_TypeDef;

typedef enum _key_value{

  KEY_POWER= 0X80,KEY_MODE=0X40,KEY_DEC=0X20,KEY_ADD=0X10,KEY_LONG_POWER=0X81

}key_value;


typedef  struct  _state_
{
 unsigned char         read;
 unsigned char         buffer;
 unsigned long          value;
 unsigned char        off_time;
 unsigned long        on_time;

 enum{
  start  = 0,
  first  = 1,
  second = 2,
  cont   = 3,
  end    = 4,
  finish = 5,
 }state;
}key_types;



void SplitDispose_Key(uint8_t value);
uint8_t KEY_Scan(void);


uint8_t  KEY_GPIO_Scan(uint8_t mode);

void WaitingPowerOn_Fun(void);
void Wifi_Detected_KeyScan(void);

void Key_TheSecond_Scan(void);


#endif 


