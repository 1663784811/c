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

#define START_TASK_PRIO		1           //�������ȼ�
#define START_STK_SIZE 		128         //�����ջ��С
#define LCD_TASK_PRIO		2
#define LCD_STK_SIZE 		50 



TaskHandle_t StartTask_Handler;
TaskHandle_t LCDTask_Handler;






int main(void)
{
	delay_init();
	// ����
	EXTI_Key_Config();
	//LED��ʼ��
	LED_Init();
	//LCD��ʼ��
	LCD_Init();
	
	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	
	LED=0;
	
	xTaskCreate(
		start_task,                 //������
		"start_task",        				 //��������
		START_STK_SIZE,     			 //�����ջ��С
		NULL, 							       //���ݸ��������Ĳ���
		START_TASK_PRIO,          //�������ȼ�
		&StartTask_Handler         //������     
	);
	vTaskStartScheduler();          //�����������
}



void start_task(void *param)
{
    taskENTER_CRITICAL();       //�����ٽ���
    
    //����LED0����
    xTaskCreate(lcd_task, "lcd0_task", LCD_STK_SIZE, NULL, LCD_TASK_PRIO, &LCDTask_Handler);

    vTaskDelete(StartTask_Handler);
    taskEXIT_CRITICAL();        //�˳��ٽ���
}



void lcd_task(void *param)
{
	u8 i,j;
	while(1)
	{
		
		// ��ȡ������ keyCode
		int kc = readKeyCode();
		
		LCD_ShowChinese(0,0,"�о�԰����",RED,WHITE,32,0);
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


// ���ͼ���
void sendKeyCode(int code){
 keyCode = code;
}
// ���߼���
int readKeyCode(void){
	int tempKey = keyCode;
	keyCode = 0;
	return tempKey;
}

