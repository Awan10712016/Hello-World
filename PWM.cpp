//�B�i���F 
#include "Sdf28335.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

float prd = 5000;  //T = 1/15k
float cmpa =3750;  //�u�@�g�� 50%

float _2PI = 6.28318530718;
float theta = 0.000;
float sum = 0.00000;
float i = 0;       //�p�� 
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
    //��l�Ƴ]�w 
	EALLOW;
    F335_GPAMUX1 |= 0x5555;                 //ePWM1A~ePWM4A�BePWM1B~ePWM4B 
	EDIS;
	setPWM();
   
   //�p�ɾ����_  �C1/15K��]�@�� 
    asm("  setc INTM");                      //�T����餤�_
    *(unsigned long*)0x01c = (unsigned long)Timer2_ISR;  //�]�w���_�V�q14���p�ɾ�2���_�A�ȵ{���C
	asm("  clrc    VMAP");                   //VMAP = 0 �A�ϥΦ�}0x00�}�l�O���鬰���_�ۦV�q�C
 	F335_TIMER2TCR = 0x0010;                 // �p�ɾ�2����
    F335_TIMER2TIM = 0x0000;                 // �M���p�ɾ�2
    F335_TIMER2TIMH = 0x0000;
    F335_TIMER2PRD = 0x3A98;                  // �p�ɾ�2�g���Ȧs���C�r��
    F335_TIMER2PRDH = 0x0;                   //��l�� 1 HZ
    F335_TIMER2TPR = 0x0000;                 //�p�ɾ�2������v�Ȧs��
    F335_TIMER2TPRH = 0x0000;
	F335_TIMER2TCR = 0x4c20;                 //�P��p�ɾ����_
	IER = 0x2000;                            //�P�� int14
    asm("  clrc INTM");


   //�]�w�j��ʧ@    
     while (1)
   {    
    	if ( ReadFromSCI_Data()=='a' )        //�p�G����ƿ�Ja
    	break;  
   }  
}
void setPWM(void)
{
/***************ePWM1***************/
	//TB �]�wPWM�g�� 
	F335_ePWM1_TBCTR = 0x0000;	         //��e�p�ƾ����e0 
	F335_ePWM1_TBPHS = 0x0000;           //�_�I
	F335_ePWM1_TBPRD = prd;	             //���˶g��
	F335_ePWM1_TBCTL = 0xE016;           // 11 1 000 000 0 01 0 1 10	 �W�U�ƼҦ� 

	//CC �]�wPWM�u�@�g�� 
	F335_ePWM1_CMPCTL = 0x0000;          //��l�ȧY�i 
	F335_ePWM1_CMPA = cmpa;              //PWM1A���u�@�g�� 
	
	//AQ �]�w�ʧ@ 
	F335_ePWM1_AQCTLA = 0x0090;          //0000 00 00 01 10 00 00 
	F335_ePWM1_AQCTLB = 0x0600;          //0000 01 10 00 00 00 00 
	F335_ePWM1_AQSFRC = 0x0024;	         //00000000 00 1 00 1 00	
	F335_ePWM1_AQCSFRC = 0x0000;	     //000000000000 00 00	

	//DB �]�w���� 
	F335_ePWM1_DBCTL = 0x000B;           //0000000000 00 10 11 
	F335_ePWM1_DBRED = 0x0064;		     //1us
	F335_ePWM1_DBFED = 0x0064;           //1us

/***************ePWM2***************/
	//TB �]�wPWM�g�� 
	F335_ePWM2_TBCTR = 0x0000;	         //��e�p�ƾ����e0 
	F335_ePWM2_TBPHS = 0x0000;           //�_�I
	F335_ePWM2_TBPRD = prd;	             //���˶g��
	F335_ePWM2_TBCTL = 0xE016;           // 11 1 000 000 0 01 0 1 10	 �W�U�ƼҦ� 

	//CC �]�wPWM�u�@�g�� 
	F335_ePWM2_CMPCTL = 0x0000;          //��l�ȧY�i 
	F335_ePWM2_CMPA = cmpa;              //PWM1A���u�@�g�� 
	
	//AQ �]�w�ʧ@ 
	F335_ePWM2_AQCTLA = 0x0090;          //0000 00 00 01 10 00 00 
	F335_ePWM2_AQCTLB = 0x0600;          //0000 01 10 00 00 00 00 
	F335_ePWM2_AQSFRC = 0x0024;	         //00000000 00 1 00 1 00	
	F335_ePWM2_AQCSFRC = 0x0000;	     //000000000000 00 00	

	//DB �]�w���� 
	F335_ePWM2_DBCTL = 0x000B;           //0000000000 00 10 11 
	F335_ePWM2_DBRED = 0x0064;		     //1us
	F335_ePWM2_DBFED = 0x0064;           //1us
	
/***************ePWM3***************/
	//TB �]�wPWM�g�� 
	F335_ePWM3_TBCTR = 0x0000;	         //��e�p�ƾ����e0 
	F335_ePWM3_TBPHS = 0x0000;           //�_�I
	F335_ePWM3_TBPRD = prd;	             //���˶g��
	F335_ePWM3_TBCTL = 0xE016;           // 11 1 000 000 0 01 0 1 10	 �W�U�ƼҦ� 

	//CC �]�wPWM�u�@�g�� 
	F335_ePWM3_CMPCTL = 0x0000;          //��l�ȧY�i 
	F335_ePWM3_CMPA = cmpa;              //PWM1A���u�@�g�� 
	
	//AQ �]�w�ʧ@ 
	F335_ePWM3_AQCTLA = 0x0090;          //0000 00 00 01 10 00 00 
	F335_ePWM3_AQCTLB = 0x0600;          //0000 01 10 00 00 00 00 
	F335_ePWM3_AQSFRC = 0x0024;	         //00000000 00 1 00 1 00	
	F335_ePWM3_AQCSFRC = 0x0000;	     //000000000000 00 00	

	//DB �]�w���� 
	F335_ePWM3_DBCTL = 0x000B;           //0000000000 00 10 11 
	F335_ePWM3_DBRED = 0x0064;		     //1us
	F335_ePWM3_DBFED = 0x0064;           //1us
	
	/***************ePWM4***************/
	//TB �]�wPWM�g�� 
	F335_ePWM4_TBCTR = 0x0000;	         //��e�p�ƾ����e0 
	F335_ePWM4_TBPHS = 0x0000;           //�_�I
	F335_ePWM4_TBPRD = prd;	             //���˶g��
	F335_ePWM4_TBCTL = 0xE016;           // 11 1 000 000 0 01 0 1 10	 �W�U�ƼҦ� 

	//CC �]�wPWM�u�@�g�� 
	F335_ePWM4_CMPCTL = 0x0000;          //��l�ȧY�i 
	F335_ePWM4_CMPA = cmpa;              //PWM1A���u�@�g�� 
	
	//AQ �]�w�ʧ@ 
	F335_ePWM4_AQCTLA = 0x0090;          //0000 00 00 01 10 00 00 
	F335_ePWM4_AQCTLB = 0x0600;          //0000 01 10 00 00 00 00 
	F335_ePWM4_AQSFRC = 0x0024;	         //00000000 00 1 00 1 00	
	F335_ePWM4_AQCSFRC = 0x0000;	     //000000000000 00 00	

	//DB �]�w���� 
	F335_ePWM4_DBCTL = 0x000B;           //0000000000 00 10 11 
	F335_ePWM4_DBRED = 0x0064;		     //1us
	F335_ePWM4_DBFED = 0x0064;           //1us	
	return;
}
