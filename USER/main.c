#include "stm32f10x.h"
#include "delay.h"
#include "motor.h"
#include "keysacn.h"
#include "IRSEARCH.h"
#include "IRAvoid.h"
#include "remote.h"   
#include "UltrasonicWave.h"
#include "timer.h"
#include "Server.h"
#include "usart2.h"
#include "usart.h"
#include "string.h"

#define KEYMODE_1   1  // ����ģʽ1
#define KEYMODE_2   2  // ����ģʽ2
#define KEYMODE_3   3  // ����ģʽ3
#define KEYMODE_4   4  // ����ģʽ4
#define KEYMODE_5   5  // ����ģʽ5
#define KEYMODE_6   6  // ����ģʽ6
#define KEYMODE_7   7  // ����ģʽ5
#define KEYMODE_8   8  // ����ģʽ6
uint8_t keyMode;      //ָ�޷���8Bit������



/*
void ZYSTM32_run(int speed,int time);       //ǰ������
void ZYSTM32_brake(int time);               //ɲ������
void ZYSTM32_Left(int speed,int time);      //��ת����
void ZYSTM32_Spin_Left(int speed,int time); //����ת����
void ZYSTM32_Right(int speed,int time);     //��ת����
void ZYSTM32_Spin_Right(int speed,int time);//����ת����
void ZYSTM32_back(int speed,int time);      //���˺���

*/
void KeyScanTask1(void)//�����ӳ���
{
	static u8 keypre = 0;//����������ʱ��1
	
	if((keypre == 0)&& (KEY))
	{
		keypre = 1; //��1������������°���ʱ�ٴν���˺�����
		switch(keyMode)
		{
			case KEYMODE_1:keyMode = KEYMODE_2; break;
			case KEYMODE_2:keyMode = KEYMODE_3; break;
			case KEYMODE_3:keyMode = KEYMODE_4; break;
            case KEYMODE_4:keyMode = KEYMODE_5; break;
            case KEYMODE_5:keyMode = KEYMODE_6; break;
            case KEYMODE_6:keyMode = KEYMODE_1; break;
			default: break;
		}
	}
	if(!KEY)  //�������ſ�
	{
		keypre = 0;//��0�������ٴ��л�LEDģʽ
	}
}

void Us2(void)
 {	
	 u8 reclen=0;

	 USART2_Init(9600);	 
	 ZYSTM32_brake(500);
	 while(keyMode == KEYMODE_1)
		{  		
            KeyScanTask1();
		if(USART2_RX_STA&0X8000)	//���յ�һ��������
		{
			reclen=USART2_RX_STA&0X7FFF;	//�õ����ݳ���
			USART2_RX_BUF[reclen]=0;	 	//���������
			printf("USART2_RX_BUF:%s\n",USART2_RX_BUF);
  		printf("reclen:%d\n",reclen);
			if(reclen==3||reclen==4) 		//����DS1���
			{
				if(strcmp((const char*)USART2_RX_BUF,"ONA")==0)
				{
					u2_printf("go forward!"); 
          ZYSTM32_run(80,100);
				}
				else if(strcmp((const char*)USART2_RX_BUF,"ONB")==0)
				{
					u2_printf("go back!"); 
          ZYSTM32_back(80,100);
				}
				else if(strcmp((const char*)USART2_RX_BUF,"ONC")==0)
				{
         u2_printf("go right!"); 
         ZYSTM32_Right(80,100);
				}
			  else if(strcmp((const char*)USART2_RX_BUF,"OND")==0)
				{
					u2_printf("go left!"); 
         ZYSTM32_Left(80,100);
				}
			  else if(strcmp((const char*)USART2_RX_BUF,"ONF")==0)
				{
         u2_printf("Stop!"); 
         ZYSTM32_brake(100);
				}
				else if(strcmp((const char*)USART2_RX_BUF,"ONE")==0)
				{
         u2_printf("Stop!"); 
         ZYSTM32_brake(100);
				}
			}
			USART2_RX_STA=0;		
		}	
	}
 }

void IR_IN()                             //����ң���ӳ���
{
	  u8 key;	
  //  ZYSTM32_brake(10);	
	   key=Remote_Scan();	
			switch(key)
			{    
				case 98:ZYSTM32_run(80,100);break;	    
				case 2:ZYSTM32_brake(100);break;		 	  
				case 194:ZYSTM32_Left(80,100);break;	   
				case 34:ZYSTM32_Right(80,100);break;		  
				case 224:ZYSTM32_Spin_Left(80,100);break;		  
				case 168:ZYSTM32_back(80,100);break;		   
				case 144:ZYSTM32_Spin_Right(80,100);break;		    
			}
}

// ������תͷ����
int front_detection()
{

//	ZYSTM32_brake(0);
	SetJointAngle(90);
	delay_ms(100);
	return UltrasonicWave_StartMeasure();
}
int left_detection()
{

//	ZYSTM32_brake(0);
	SetJointAngle(175);
	delay_ms(300);
	return UltrasonicWave_StartMeasure();
}
int right_detection()
{

//	ZYSTM32_brake(0);
	SetJointAngle(5);
	delay_ms(300);
	return UltrasonicWave_StartMeasure();
}
//extern int U_temp;
/*
void ZYSTM32_run(int speed,int time);       //ǰ������
void ZYSTM32_brake(int time);               //ɲ������
void ZYSTM32_Left(int speed,int time);      //��ת����
void ZYSTM32_Spin_Left(int speed,int time); //����ת����
void ZYSTM32_Right(int speed,int time);     //��ת����
void ZYSTM32_Spin_Right(int speed,int time);//����ת����
void ZYSTM32_back(int speed,int time);      //���˺���
*/

void UW(void)
 {	
   int Q_temp,L_temp,R_temp;

    //Timerx_Init(5000,7199);                //10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms 
     delay_init();
	 KEY_Init();
	 IRSearchInit();
	 IRAvoidInit();
     Timerx_Init(5000,7199);                //10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms 
	 UltrasonicWave_Configuration();        //�Գ�����ģ���ʼ��
	 uart_init(115200);
	 TIM4_PWM_Init(7199,0);                 //��ʼ��PWM
	 TIM5_PWM_Init(9999,143);               //����Ƶ��PWMƵ��=72*10^6/(9999+1)/(143+1)=50Hz	 
	 ZYSTM32_brake(500);
	

	 ZYSTM32_brake(500);
	while(keyMode == KEYMODE_3)
	{  		
        KeyScanTask1();
        
		
	//  printf("�⵽�ľ���ֵΪ��%d\n",Q_temp);
		Q_temp = front_detection();
		if(Q_temp<60 && Q_temp>0) //��������ֵ	
		{
			ZYSTM32_brake(500);		
			ZYSTM32_back(60,500);	
			ZYSTM32_brake(1000);	
			
			L_temp=left_detection();//��������ϰ���ľ���ֵ
			delay_ms(500);
			R_temp=right_detection();//�����ұ��ϰ���ľ���ֵ
			delay_ms(500);
			
			if((L_temp < 60 ) &&( R_temp < 60 ))//��������������ϰ��￿�ıȽϽ�
			{
				ZYSTM32_Spin_Left(60,1000);
			}				
      else if(L_temp > R_temp)
			{
				ZYSTM32_Left(60,1000);
				ZYSTM32_brake(500);
			}	
      else
			{
				ZYSTM32_Right(60,1000);
				ZYSTM32_brake(500);					
			}							
		}	
		else
		{
			ZYSTM32_run(60,10);
		}
	}
 }

 
 void track(void){
     while(keyMode == KEYMODE_5){
         KeyScanTask1();
    ZYSTM32_Right(70,1000);
    ZYSTM32_brake(500);
    ZYSTM32_run(70,600);
    ZYSTM32_brake(500);

	ZYSTM32_Left(70,1000);
	ZYSTM32_brake(500);
	ZYSTM32_run(70,700);
	ZYSTM32_brake(500);
	 
	ZYSTM32_Right(70,900);
	ZYSTM32_brake(500);
	ZYSTM32_run(70,400);
	ZYSTM32_brake(500);
	 
	ZYSTM32_Right(70,900);
	ZYSTM32_brake(500);
	ZYSTM32_run(70,800);
	ZYSTM32_brake(500);
		
	ZYSTM32_Spin_Right(70,1000);
	ZYSTM32_brake(500);
	ZYSTM32_run(70,700);
	  ZYSTM32_brake(500);
		
	ZYSTM32_Right(70,900);
	 ZYSTM32_brake(500);
    ZYSTM32_run(70,500);
	ZYSTM32_brake(500);
		
		 ZYSTM32_Left(70,1100);
	ZYSTM32_brake(500);
	ZYSTM32_run(70,800);
	ZYSTM32_run(70,500); 
	ZYSTM32_brake(500);
		
	ZYSTM32_Spin_Left(70,1050);
	ZYSTM32_brake(500);
	ZYSTM32_run(70,800); 
	ZYSTM32_run(70,800); 
	ZYSTM32_run(70,800);
	ZYSTM32_brake(500);
		
				
	ZYSTM32_run(70,2000);
	ZYSTM32_brake(500);
	ZYSTM32_Spin_Left(70,2800);
	ZYSTM32_brake(500);
	ZYSTM32_run(70,2200); 
	ZYSTM32_brake(500);
	ZYSTM32_Spin_Right(70,1400);
	ZYSTM32_brake(500);
	 //��?����
	 
	 
	ZYSTM32_run(70,3000);
	ZYSTM32_brake(500);
	ZYSTM32_Spin_Right(70,1400);
	ZYSTM32_brake(500);
	ZYSTM32_run(70,800); 
	ZYSTM32_brake(500);
		//4
	ZYSTM32_Spin_Left(70,1400);
	ZYSTM32_brake(500);
	ZYSTM32_run(70,3000);//?��??1S 
	ZYSTM32_brake(500);
		//6
	ZYSTM32_Right(70,1000);//������a1S
	ZYSTM32_brake(500);
    ZYSTM32_run(70,800);//?��??1S
	ZYSTM32_brake(500);
	 //7
	ZYSTM32_Left(70,1000);//������a1S
	ZYSTM32_brake(500);
	ZYSTM32_run(70,700);//?��??1S 
	ZYSTM32_brake(500);
		//8
	ZYSTM32_run(70,5000);//?��??1S 
	ZYSTM32_brake(500);
		//12
	ZYSTM32_Spin_Right(70,1400);
	ZYSTM32_brake(500);
	ZYSTM32_run(70,800);//?��??1S 
	ZYSTM32_brake(500);
		//13
	ZYSTM32_Right(70,1000);//������a1S
	ZYSTM32_brake(500);
    ZYSTM32_run(70,800);//?��??1S
	ZYSTM32_brake(500000);
         
     }
    
     

    
 }

void KeyScanTask(void)//�����ӳ���
{
	static u8 keypre = 0;//����������ʱ��1
	
	if((keypre == 0)&& (KEY))
	{
		keypre = 1; //��1������������°���ʱ�ٴν���˺�����
		switch(keyMode)
		{
			case KEYMODE_1:keyMode = KEYMODE_2; break;
			case KEYMODE_2:keyMode = KEYMODE_3; break;
			case KEYMODE_3:keyMode = KEYMODE_4; break;
            case KEYMODE_4:keyMode = KEYMODE_5; break;
            case KEYMODE_5:keyMode = KEYMODE_6; break;
            case KEYMODE_6:keyMode = KEYMODE_7; break;
            case KEYMODE_7:keyMode = KEYMODE_8; break;
            case KEYMODE_8:keyMode = KEYMODE_1; break;
			default: break;
		}
	}
	if(!KEY)  //�������ſ�
	{
		keypre = 0;//��0�������ٴ��л�LEDģʽ
	}
}

//����ѭ�������ϡ�ң��ģʽ����
void LEDTask()
{
		switch(keyMode)
		{
			case KEYMODE_1:
				LED_D4_SET;
			  LED_D3_RESET;
            Us2();
			  
			  break;
			case KEYMODE_3:
				LED_D4_RESET;
			    LED_D3_SET;
                UW();
			  break;
			case KEYMODE_5:
				LED_D4_SET;
			    LED_D3_SET;
            IR_IN();
                
			  break;
            case KEYMODE_7:
				LED_D4_RESET;
			    LED_D3_RESET;
            track();
                
			  break;
			 default:
         break;
		}
}
	

//	  int val;
//  val=KEY;  //��ȡ����
//  while(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2))//������û�б�����ʱ��һֱѭ��
//  {
//    val=KEY;//�ʾ��ʡ�ԣ�����ѭ���ܿ�
//  }
//  while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2))//������������ʱ
//  {
//    delay_ms(10);	  //��ʱ10ms
//    val=KEY;//��ȡ���ֶ˿�PC2�ڵ�ƽֵ����Val
//    if(val==1)  //�ڶ����жϰ����Ƿ񱻰���
//    {
//      BEEP_SET;		//��������
//      while(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2))	//�жϰ�ʱ�Ƿ��ɿ�
//        BEEP_RESET;	//������ֹͣ
//    }
//    else
//				BEEP_RESET;//������ֹͣ
//  }
//}


 int main(void)
 {	
	 delay_init();
	 KEY_Init();
     IRSearchInit();
	 IRAvoidInit();
     Timerx_Init(5000,7199);                //10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms 
	 UltrasonicWave_Configuration();        //�Գ�����ģ���ʼ��
	 uart_init(115200);
	 TIM4_PWM_Init(7199,0);                 //��ʼ��PWM
	 TIM5_PWM_Init(9999,143);               //����Ƶ��PWMƵ��=72*10^6/(9999+1)/(143+1)=50Hz	
	 Remote_Init();			    //������ճ�ʼ��	
	 ZYSTM32_brake(500);
	 //keysacn();		
	 keyMode = KEYMODE_1;
		while(1)
		{  
		 
            KeyScanTask();
            LEDTask();			

		}
 }

