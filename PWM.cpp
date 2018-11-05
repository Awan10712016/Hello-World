//步進馬達 
#include "Sdf28335.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

float prd = 5000;  //T = 1/15k
float cmpa =3750;  //工作週期 50%

float _2PI = 6.28318530718;
float theta = 0.000;
float sum = 0.00000;
float i = 0;       //計數 
float freq = 1;
int a=0;

#define CMPA1 F335_ePWM1_CMPA
#define CMPA2 F335_ePWM2_CMPA
#define CMPA3 F335_ePWM3_CMPA
#define CMPA4 F335_ePWM4_CMPA
void setPWM(void);
extern "C" interrupt void Timer2_ISR()
{    
    if( i > (5000/freq) ) 
	{
		a++;
		i = 0;
		if(a == 4) a=0;
	}
    if(a==0)
	{
		CMPA1 = prd;
		CMPA2 = 0;
		CMPA3 = 0;
		CMPA4 = 0;
	}
	if(a==1)
	{
		CMPA1 = 0;
		CMPA2 = prd;
		CMPA3 = 0;
		CMPA4 = 0;
	}
	if(a==2)
	{
		CMPA1 = 0;
		CMPA2 = 0;
		CMPA3 = prd;
		CMPA4 = 0;
	}
    if(a==3)
    {
    	CMPA1 = 0;
		CMPA2 = 0;
		CMPA3 = 0;
		CMPA4 = prd;
	}
	i++;
}

int  main() 
{                        
    //初始化設定 
	EALLOW;
    F335_GPAMUX1 |= 0x5555;                 //ePWM1A~ePWM4A、ePWM1B~ePWM4B 
	EDIS;
	setPWM();
   
   //計時器中斷  每1/15K秒跑一次 
    asm("  setc INTM");                      //禁能整體中斷
    *(unsigned long*)0x01c = (unsigned long)Timer2_ISR;  //設定中斷向量14為計時器2中斷服務程式。
	asm("  clrc    VMAP");                   //VMAP = 0 ，使用位址0x00開始記憶體為中斷相向量。
 	F335_TIMER2TCR = 0x0010;                 // 計時器2停止
    F335_TIMER2TIM = 0x0000;                 // 清除計時器2
    F335_TIMER2TIMH = 0x0000;
    F335_TIMER2PRD = 0x3A98;                  // 計時器2週期暫存器低字元
    F335_TIMER2PRDH = 0x0;                   //初始為 1 HZ
    F335_TIMER2TPR = 0x0000;                 //計時器2先期比率暫存器
    F335_TIMER2TPRH = 0x0000;
	F335_TIMER2TCR = 0x4c20;                 //致能計時器中斷
	IER = 0x2000;                            //致能 int14
    asm("  clrc INTM");


   //設定迴圈動作    
     while (1)
   {    
    	if ( ReadFromSCI_Data()=='a' )        //如果有資料輸入a
    	break;  
   }  
}
void setPWM(void)
{
/***************ePWM1***************/
	//TB 設定PWM週期 
	F335_ePWM1_TBCTR = 0x0000;	         //當前計數器內容0 
	F335_ePWM1_TBPHS = 0x0000;           //起點
	F335_ePWM1_TBPRD = prd;	             //取樣週期
	F335_ePWM1_TBCTL = 0xE016;           // 11 1 000 000 0 01 0 1 10	 上下數模式 

	//CC 設定PWM工作週期 
	F335_ePWM1_CMPCTL = 0x0000;          //初始值即可 
	F335_ePWM1_CMPA = cmpa;              //PWM1A的工作週期 
	
	//AQ 設定動作 
	F335_ePWM1_AQCTLA = 0x0090;          //0000 00 00 01 10 00 00 
	F335_ePWM1_AQCTLB = 0x0600;          //0000 01 10 00 00 00 00 
	F335_ePWM1_AQSFRC = 0x0024;	         //00000000 00 1 00 1 00	
	F335_ePWM1_AQCSFRC = 0x0000;	     //000000000000 00 00	

	//DB 設定死區 
	F335_ePWM1_DBCTL = 0x000B;           //0000000000 00 10 11 
	F335_ePWM1_DBRED = 0x0064;		     //1us
	F335_ePWM1_DBFED = 0x0064;           //1us

/***************ePWM2***************/
	//TB 設定PWM週期 
	F335_ePWM2_TBCTR = 0x0000;	         //當前計數器內容0 
	F335_ePWM2_TBPHS = 0x0000;           //起點
	F335_ePWM2_TBPRD = prd;	             //取樣週期
	F335_ePWM2_TBCTL = 0xE016;           // 11 1 000 000 0 01 0 1 10	 上下數模式 

	//CC 設定PWM工作週期 
	F335_ePWM2_CMPCTL = 0x0000;          //初始值即可 
	F335_ePWM2_CMPA = cmpa;              //PWM1A的工作週期 
	
	//AQ 設定動作 
	F335_ePWM2_AQCTLA = 0x0090;          //0000 00 00 01 10 00 00 
	F335_ePWM2_AQCTLB = 0x0600;          //0000 01 10 00 00 00 00 
	F335_ePWM2_AQSFRC = 0x0024;	         //00000000 00 1 00 1 00	
	F335_ePWM2_AQCSFRC = 0x0000;	     //000000000000 00 00	

	//DB 設定死區 
	F335_ePWM2_DBCTL = 0x000B;           //0000000000 00 10 11 
	F335_ePWM2_DBRED = 0x0064;		     //1us
	F335_ePWM2_DBFED = 0x0064;           //1us
	
/***************ePWM3***************/
	//TB 設定PWM週期 
	F335_ePWM3_TBCTR = 0x0000;	         //當前計數器內容0 
	F335_ePWM3_TBPHS = 0x0000;           //起點
	F335_ePWM3_TBPRD = prd;	             //取樣週期
	F335_ePWM3_TBCTL = 0xE016;           // 11 1 000 000 0 01 0 1 10	 上下數模式 

	//CC 設定PWM工作週期 
	F335_ePWM3_CMPCTL = 0x0000;          //初始值即可 
	F335_ePWM3_CMPA = cmpa;              //PWM1A的工作週期 
	
	//AQ 設定動作 
	F335_ePWM3_AQCTLA = 0x0090;          //0000 00 00 01 10 00 00 
	F335_ePWM3_AQCTLB = 0x0600;          //0000 01 10 00 00 00 00 
	F335_ePWM3_AQSFRC = 0x0024;	         //00000000 00 1 00 1 00	
	F335_ePWM3_AQCSFRC = 0x0000;	     //000000000000 00 00	

	//DB 設定死區 
	F335_ePWM3_DBCTL = 0x000B;           //0000000000 00 10 11 
	F335_ePWM3_DBRED = 0x0064;		     //1us
	F335_ePWM3_DBFED = 0x0064;           //1us
	
	/***************ePWM4***************/
	//TB 設定PWM週期 
	F335_ePWM4_TBCTR = 0x0000;	         //當前計數器內容0 
	F335_ePWM4_TBPHS = 0x0000;           //起點
	F335_ePWM4_TBPRD = prd;	             //取樣週期
	F335_ePWM4_TBCTL = 0xE016;           // 11 1 000 000 0 01 0 1 10	 上下數模式 

	//CC 設定PWM工作週期 
	F335_ePWM4_CMPCTL = 0x0000;          //初始值即可 
	F335_ePWM4_CMPA = cmpa;              //PWM1A的工作週期 
	
	//AQ 設定動作 
	F335_ePWM4_AQCTLA = 0x0090;          //0000 00 00 01 10 00 00 
	F335_ePWM4_AQCTLB = 0x0600;          //0000 01 10 00 00 00 00 
	F335_ePWM4_AQSFRC = 0x0024;	         //00000000 00 1 00 1 00	
	F335_ePWM4_AQCSFRC = 0x0000;	     //000000000000 00 00	

	//DB 設定死區 
	F335_ePWM4_DBCTL = 0x000B;           //0000000000 00 10 11 
	F335_ePWM4_DBRED = 0x0064;		     //1us
	F335_ePWM4_DBFED = 0x0064;           //1us	
	return;
}
