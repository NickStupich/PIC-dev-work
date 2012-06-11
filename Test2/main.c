#include "p33fxxxx.h"

#define USING_PLL	1

#if USING_PLL == 1
_FOSCSEL(FNOSC_FRCPLL);								//oscillator mode FRC ~ 8Mhz
#else
_FOSCSEL(FNOSC_FRC);
#endif

_FOSC(FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMD_NONE);	//ISCIO pin for RA3
_FWDT(FWDTEN_OFF);									//watchdog off

/* Delay by _ms */
void delay(unsigned long ms)
{
	unsigned int lastTime = TMR1;
	unsigned long timeout;

	#if USING_PLL == 1
	timeout = ms*45;	//23.1Mhz
	#else
	timeout = ms*7;		//3.7Mhz
	#endif

	while((TMR1 - lastTime) < timeout);
}

void initTimer1()
{
	T1CON = 0;	//in reset state
	IEC0bits.T1IE = 0;	//disable timer 1 interrupt
	T1CONbits.TCKPS = 0x3;	//1:256 = 1us?
	T1CONbits.TCS = 0;	//internal clock
	T1CONbits.TON = 1;	//enable timer 1
}

int main()
{
	initTimer1();
	LATA  = 0x0000; 				// set latch levels
	TRISA = 0x0000; 				// set IO as outputs
	LATB  = 0x0000; 				// set latch levels
	TRISB = 0x0000; 				// set IO as outputs

	while(1)
	{
		delay(10);
		LATA = ~LATA;
		LATB = ~LATB;
	}
}
