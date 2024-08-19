#include "delay.h"
#include "sys.h"
#include "led.h"
#include "lcd_init.h"
#include "lcd.h"
#include "pic.h"
#include "FreeRTOS.h"
#include "task.h"
#include "bsp_exti.h"



int keyCode = 0;
void sendKeyCode(int code);
int readKeyCode(void);





void start_task(void *param);
void lcd_task(void *param);

#define START_TASK_PRIO		1           //任务优先级
#define START_STK_SIZE 		128         //任务堆栈大小
#define LCD_TASK_PRIO		2
#define LCD_STK_SIZE 		50 



TaskHandle_t StartTask_Handler;
TaskHandle_t LCDTask_Handler;






int main(void)
{
	delay_init();
	// 按键
	EXTI_Key_Config();
	//LED初始化
	LED_Init();
	//LCD初始化
	LCD_Init();
	
	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	
	LED=0;
	
	xTaskCreate(
		start_task,                 //任务函数
		"start_task",        				 //任务名称
		START_STK_SIZE,     			 //任务堆栈大小
		NULL, 							       //传递给任务函数的参数
		START_TASK_PRIO,          //任务优先级
		&StartTask_Handler         //任务句柄     
	);
	vTaskStartScheduler();          //开启任务调度
}



void start_task(void *param)
{
    taskENTER_CRITICAL();       //进入临界区
    
    //创建LED0任务
    xTaskCreate(lcd_task, "lcd0_task", LCD_STK_SIZE, NULL, LCD_TASK_PRIO, &LCDTask_Handler);

    vTaskDelete(StartTask_Handler);
    taskEXIT_CRITICAL();        //退出临界区
}



void lcd_task(void *param)
{
	u8 i,j;
	while(1)
	{
		
		// 读取按键码 keyCode
		int kc = readKeyCode();
		
		LCD_ShowChinese(0,0,"中景园电子",RED,WHITE,32,0);
		LCD_ShowString(0,40,"LCD_W:",RED,WHITE,16,0);
		LCD_ShowIntNum(48,40,LCD_W,3,RED,WHITE,16);
		LCD_ShowString(80,40,"LCD_H:",RED,WHITE,16,0);
		LCD_ShowIntNum(128,40,LCD_H,3,RED,WHITE,16);
		LCD_ShowString(80,40,"LCD_H:",RED,WHITE,16,0);
		LCD_ShowString(0,70,"Increaseing Nun:",RED,WHITE,16,0);
		
		//LCD_ShowFloatNum1(128,70,kc,1,RED,WHITE,16);
		LCD_ShowIntNum(128,70,kc,1,RED,WHITE,16);
		
		
		for(j=0;j<3;j++)
		{
			for(i=0;i<6;i++)
			{
				LCD_ShowPicture(40*i,120+j*40,40,40,gImage_1);
			}
		}
	}
}


// 发送键码
void sendKeyCode(int code){
 keyCode = code;
}
// 读走键码
int readKeyCode(void){
	int tempKey = keyCode;
	keyCode = 0;
	return tempKey;
}

