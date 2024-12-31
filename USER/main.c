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

#define KEYMODE_1   1  // 定义模式1
#define KEYMODE_2   2  // 定义模式2
#define KEYMODE_3   3  // 定义模式3
#define KEYMODE_4   4  // 定义模式4
#define KEYMODE_5   5  // 定义模式5
#define KEYMODE_6   6  // 定义模式6
#define KEYMODE_7   7  // 定义模式5
#define KEYMODE_8   8  // 定义模式6
uint8_t keyMode;      //指无符号8Bit整形数



/*
void ZYSTM32_run(int speed,int time);       //前进函数
void ZYSTM32_brake(int time);               //刹车函数
void ZYSTM32_Left(int speed,int time);      //左转函数
void ZYSTM32_Spin_Left(int speed,int time); //左旋转函数
void ZYSTM32_Right(int speed,int time);     //右转函数
void ZYSTM32_Spin_Right(int speed,int time);//右旋转函数
void ZYSTM32_back(int speed,int time);      //后退函数

*/
void KeyScanTask1(void)//按键子程序
{
	static u8 keypre = 0;//按键被按下时置1
	
	if((keypre == 0)&& (KEY))
	{
		keypre = 1; //置1，避免持续按下按键时再次进入此函数。
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
	if(!KEY)  //按键被放开
	{
		keypre = 0;//置0，允许再次切换LED模式
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
		if(USART2_RX_STA&0X8000)	//接收到一次数据了
		{
			reclen=USART2_RX_STA&0X7FFF;	//得到数据长度
			USART2_RX_BUF[reclen]=0;	 	//加入结束符
			printf("USART2_RX_BUF:%s\n",USART2_RX_BUF);
  		printf("reclen:%d\n",reclen);
			if(reclen==3||reclen==4) 		//控制DS1检测
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

void IR_IN()                             //红外遥控子程序
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

// 超声波转头函数
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
void ZYSTM32_run(int speed,int time);       //前进函数
void ZYSTM32_brake(int time);               //刹车函数
void ZYSTM32_Left(int speed,int time);      //左转函数
void ZYSTM32_Spin_Left(int speed,int time); //左旋转函数
void ZYSTM32_Right(int speed,int time);     //右转函数
void ZYSTM32_Spin_Right(int speed,int time);//右旋转函数
void ZYSTM32_back(int speed,int time);      //后退函数
*/

void UW(void)
 {	
   int Q_temp,L_temp,R_temp;

    //Timerx_Init(5000,7199);                //10Khz的计数频率，计数到5000为500ms 
     delay_init();
	 KEY_Init();
	 IRSearchInit();
	 IRAvoidInit();
     Timerx_Init(5000,7199);                //10Khz的计数频率，计数到5000为500ms 
	 UltrasonicWave_Configuration();        //对超声波模块初始化
	 uart_init(115200);
	 TIM4_PWM_Init(7199,0);                 //初始化PWM
	 TIM5_PWM_Init(9999,143);               //不分频，PWM频率=72*10^6/(9999+1)/(143+1)=50Hz	 
	 ZYSTM32_brake(500);
	

	 ZYSTM32_brake(500);
	while(keyMode == KEYMODE_3)
	{  		
        KeyScanTask1();
        
		
	//  printf("测到的距离值为：%d\n",Q_temp);
		Q_temp = front_detection();
		if(Q_temp<60 && Q_temp>0) //测量距离值	
		{
			ZYSTM32_brake(500);		
			ZYSTM32_back(60,500);	
			ZYSTM32_brake(1000);	
			
			L_temp=left_detection();//测量左边障碍物的距离值
			delay_ms(500);
			R_temp=right_detection();//测量右边障碍物的距离值
			delay_ms(500);
			
			if((L_temp < 60 ) &&( R_temp < 60 ))//当左右两侧均有障碍物靠的比较近
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
	 //μ?í·
	 
	 
	ZYSTM32_run(70,3000);
	ZYSTM32_brake(500);
	ZYSTM32_Spin_Right(70,1400);
	ZYSTM32_brake(500);
	ZYSTM32_run(70,800); 
	ZYSTM32_brake(500);
		//4
	ZYSTM32_Spin_Left(70,1400);
	ZYSTM32_brake(500);
	ZYSTM32_run(70,3000);//?°??1S 
	ZYSTM32_brake(500);
		//6
	ZYSTM32_Right(70,1000);//óò×a1S
	ZYSTM32_brake(500);
    ZYSTM32_run(70,800);//?°??1S
	ZYSTM32_brake(500);
	 //7
	ZYSTM32_Left(70,1000);//×ó×a1S
	ZYSTM32_brake(500);
	ZYSTM32_run(70,700);//?°??1S 
	ZYSTM32_brake(500);
		//8
	ZYSTM32_run(70,5000);//?°??1S 
	ZYSTM32_brake(500);
		//12
	ZYSTM32_Spin_Right(70,1400);
	ZYSTM32_brake(500);
	ZYSTM32_run(70,800);//?°??1S 
	ZYSTM32_brake(500);
		//13
	ZYSTM32_Right(70,1000);//óò×a1S
	ZYSTM32_brake(500);
    ZYSTM32_run(70,800);//?°??1S
	ZYSTM32_brake(500000);
         
     }
    
     

    
 }

void KeyScanTask(void)//按键子程序
{
	static u8 keypre = 0;//按键被按下时置1
	
	if((keypre == 0)&& (KEY))
	{
		keypre = 1; //置1，避免持续按下按键时再次进入此函数。
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
	if(!KEY)  //按键被放开
	{
		keypre = 0;//置0，允许再次切换LED模式
	}
}

//任务：循迹、避障、遥控模式处理
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
//  val=KEY;  //读取按键
//  while(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2))//当按键没有被按下时，一直循环
//  {
//    val=KEY;//词句可省略，可让循环跑空
//  }
//  while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2))//当按键被按下时
//  {
//    delay_ms(10);	  //延时10ms
//    val=KEY;//读取数字端口PC2口电平值赋给Val
//    if(val==1)  //第二次判断按键是否被按下
//    {
//      BEEP_SET;		//蜂鸣器响
//      while(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2))	//判断按时是否被松开
//        BEEP_RESET;	//蜂鸣器停止
//    }
//    else
//				BEEP_RESET;//蜂鸣器停止
//  }
//}


 int main(void)
 {	
	 delay_init();
	 KEY_Init();
     IRSearchInit();
	 IRAvoidInit();
     Timerx_Init(5000,7199);                //10Khz的计数频率，计数到5000为500ms 
	 UltrasonicWave_Configuration();        //对超声波模块初始化
	 uart_init(115200);
	 TIM4_PWM_Init(7199,0);                 //初始化PWM
	 TIM5_PWM_Init(9999,143);               //不分频，PWM频率=72*10^6/(9999+1)/(143+1)=50Hz	
	 Remote_Init();			    //红外接收初始化	
	 ZYSTM32_brake(500);
	 //keysacn();		
	 keyMode = KEYMODE_1;
		while(1)
		{  
		 
            KeyScanTask();
            LEDTask();			

		}
 }

