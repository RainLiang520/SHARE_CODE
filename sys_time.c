
#include "N76E003.h"
#include "Common.h"
#include "Delay.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "sys_time.h"
#include "led.h"
#include "button.h"

#define	C_TIM0_Reload		(65535-1600)		//100us

static uint8_t time_systick_100us = 0;

extern bit BoxState;
extern bit Sw1State;


void sys_tim_Init(void)
{
		//system clock division
		CKDIV = 0;				//????16M,1/4??,4MHz??
	
	
		//TIMER0_MODE1_ENABLE;
		TMOD&=0xF0;
		TMOD|=0x01;
	
		clr_T0M;
					
		TH0 = C_TIM0_Reload/256;
		TL0 = C_TIM0_Reload%256;

		set_T0M;
		set_ET0;
		set_EA;
		set_TR0;
//		clr_T0M;
//		clr_ET0;
//		clr_EA;
//		clr_TR0;
}


uint32_t TimeGetTime(void)
{
	return time1_ms;
}

uint32_t Tim1GetDuration(uint32_t last)
{
	if (time1_ms > last)
		return time1_ms - last;
	else
		return (0xFFFFFFFFL - last) + time1_ms + 1;
}

void Time1OnTick(void)
{
	time1_ms++;
}

void BoxStates(void)
{
	static uint8_t DelayTime;
	static bit old;
		if(button1 == 0)
		{
			DelayTime++;
			if((DelayTime >= 5)||(old == 1))
			{
				old = 1;
				DelayTime = 0;
				BoxState = 1;
			}
			else
			{
				old = 0;
				BoxState = 0;
			}
		}
		else
		{
			old = 0;
			DelayTime = 0;
			BoxState = 0;
		}
		
}

uint8_t ButtonState(void)
{
		if(SW1 == BIN_DOWN)
		{
					return BIN_DOWN;
		}
		else
		{
				return BIN_UP;
		}	
}


void sys_tim_Interrupt(void)  interrupt 1  //interrupt address is 0x000B
{
		//100us
		//Read_Encoder();
		TH0 += C_TIM0_Reload/256;
    TL0 += C_TIM0_Reload%256;
		time_systick_100us++;
		if(time_systick_100us >= 10)
	{
		//1ms
		time_systick_100us = 0;
		Time1OnTick();
		BoxStates();
		ButtonState();
		//LED1 = !LED1;
	}
	
}

