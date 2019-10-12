/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2016 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

//***********************************************************************************************************
//  Nuvoton Technoledge Corp. 
//  Website: http://www.nuvoton.com
//  E-Mail : MicroC-8bit@nuvoton.com
//  Date   : Apr/21/2016
//***********************************************************************************************************

#include "N76E003.h"
#include "Common.h"
#include "Delay.h"
#include "SFR_Macro.h"
#include "Function_define.h"

//#ifdef	DEBUG_UART1
////Ring buf
//#define Length_TXBuf	50
//static xdata uint8_t buf_TX[Length_TXBuf] = {0};
//static uint8_t pos_TX_OUT = 0;
//static uint8_t pos_TX_IN = 0;	

//static xdata uint8_t cnt_TX = 0;
//static bit	Flag_TX_Full = 0;
//static bit	Flag_TX_Empty = 1;

//#endif

//----------------------------------------------------------------------------------
// UART1 printf() int
//----------------------------------------------------------------------------------

char putchar (char ch)
{
#ifdef	DEBUG_UART1 
	//Send_Data_To_UART1(ch);
	Send_Data_To_UART1(ch);
#endif
	return(ch);
}
#ifdef DEBUG_UART1
void Send_Data_To_UART1 (UINT8 str)
{
    TI_1 = 0;
    SBUF_1 = str;
    while(TI_1==0);
}
#endif
//----------------------------------------------------------------------------------
// UART0 baud rate initial setting 
//----------------------------------------------------------------------------------
void InitialUART0_Timer1(UINT32 u32Baudrate)    //T1M = 1, SMOD = 1
{
		P06_Quasi_Mode;		//Setting UART pin as Quasi mode for transmit
		P07_Quasi_Mode;		//Setting UART pin as Quasi mode for transmit
	
    SCON = 0x50;     	//UART0 Mode1,REN=1,TI=1
    TMOD |= 0x20;    	//Timer1 Mode1
    
    set_SMOD;        	//UART0 Double Rate Enable
    set_T1M;
    clr_BRCK;        	//Serial port 0 baud rate clock source = Timer1
 
#ifdef FOSC_160000
    TH1 = 256 - (1000000/u32Baudrate+1);               /*16 MHz */
#endif    	
#ifdef FOSC_166000
    TH1 = 256 - (1037500/u32Baudrate);         		     /*16.6 MHz */
#endif
    set_TR1;
		set_TI;						//For printf function must setting TI = 1
}
//---------------------------------------------------------------
void InitialUART0_Timer3(UINT32 u32Baudrate) //use timer3 as Baudrate generator
{
		P06_Quasi_Mode;		//Setting UART pin as Quasi mode for transmit
		P07_Quasi_Mode;		//Setting UART pin as Quasi mode for transmit	
	
    SCON = 0x40;     //UART0 Mode1,REN=1,TI=1
    set_SMOD;        //UART0 Double Rate Enable
    T3CON &= 0xF8;   //T3PS2=0,T3PS1=0,T3PS0=0(Prescale=1)
    set_BRCK;        //UART0 baud rate clock source = Timer3

#ifdef FOSC_160000
		RH3    = HIBYTE(65536 - (1000000/u32Baudrate));  		/*16 MHz */
		RL3    = LOBYTE(65536 - (1000000/u32Baudrate));			/*16 MHz */
#endif
#ifdef FOSC_166000
    RH3    = HIBYTE(65536 - (1037500/u32Baudrate)); 			/*16.6 MHz */
    RL3    = LOBYTE(65536 - (1037500/u32Baudrate)); 			/*16.6 MHz */
#endif
		//set_REN;
		//RI = 0;
		IP |=0x10;
		IPH |= 0x10;
		
		//set_ES;
    set_TR3;         //Trigger Timer3
		set_TI;					 //For printf function must setting TI = 1
}

UINT8 Receive_Data_From_UART0(void)
{
    UINT8 c;
    while (!RI);
    c = SBUF;
    RI = 0;
    return (c);
}

void Send_Data_To_UART0 (UINT8 SenData)
{
    TI = 0;
    SBUF = SenData;
    while(TI==0);
}



//----------------------------------------------------------------------------------
// UART1 baud rate initial setting 
//----------------------------------------------------------------------------------
void InitialUART1_Timer3(UINT32 u32Baudrate) //use timer3 as Baudrate generator
{
		P02_Quasi_Mode;		//Setting UART pin as Quasi mode for transmit
		P16_Quasi_Mode;		//Setting UART pin as Quasi mode for transmit
	
	  SCON_1 = 0x50;   	//UART1 Mode1,REN_1=1,TI_1=1
    T3CON &= 0xF8;   	//T3PS2=0,T3PS1=0,T3PS0=0(Prescale=1), UART1 in MODE 1
		set_SMOD_1;
		set_BRCK;
	
#ifdef FOSC_160000
		RH3    = HIBYTE(65536 - (1000000/u32Baudrate));  		/*16 MHz */
		RL3    = LOBYTE(65536 - (1000000/u32Baudrate));			/*16 MHz */
#endif
#ifdef FOSC_166000
    RH3    = HIBYTE(65536 - (1037500/u32Baudrate)); 			/*16.6 MHz */
    RL3    = LOBYTE(65536 - (1037500/u32Baudrate)); 			/*16.6 MHz */
#endif
	
		IP |=0x10;
		IPH |= 0x10;
    set_TR3;         //Trigger Timer3
		set_TI_1;
}

UINT8 Receive_Data_From_UART1(void)
{
    UINT8 c;
    
    while (!RI_1);
    c = SBUF_1;
    RI_1 = 0;
    return (c);
}



//#ifdef	DEBUG_UART1 
///*
//* Redefine of printf
//*/
//void Send_Data_To_UART1 (uint8_t c)
//{
//	while(Flag_TX_Full);
////uart1_Interrupt_OFF();
//	clr_EA;
//	buf_TX[pos_TX_IN] = c;
//	pos_TX_IN++;					//??????
//	if(pos_TX_IN >= Length_TXBuf)
//		pos_TX_IN = 0; 

//	cnt_TX++;	 					//????

//	if(cnt_TX>=Length_TXBuf)
//	 	Flag_TX_Full = 1;			//????	  
////uart1_Interrupt_ON();
//	set_EA;
//	if(Flag_TX_Empty)
//	{
//		//set_uart1_TXMode();
//		set_ES_1;
//		SBUF_1 = (unsigned char)buf_TX[pos_TX_OUT];
//		pos_TX_OUT++;
//		if(pos_TX_OUT >= Length_TXBuf)
//			pos_TX_OUT = 0;

//		Flag_TX_Empty = 0;
//	}	
//}

//#endif

/*==========================================================================*/
#ifdef SW_Reset
void SW_Reset(void)
{
    TA = 0xAA;
    TA = 0x55;
    set_SWRST;
}
#endif
/*==========================================================================*/
