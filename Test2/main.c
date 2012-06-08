#include "p33fxxxx.h"

_FOSCSEL(FNOSC_FRC);								//oscillator mode FRC ~ 8Mhz
_FOSC(FCKSM_CSDCMD & OSCIOFNC_ON & POSCMD_NONE);	//ISCIO pin for RA3
_FWDT(FWDTEN_OFF);									//watchdog off

void delay(unsigned long ms)
{
	long i;
	for(i=0;i<ms*20;i++)
	{
		Nop();
	}
}

int main()
{
	LATA  = 0x0000; 				// set latch levels
	TRISA = 0x0000; 				// set IO as outputs
	LATB  = 0x0000; 				// set latch levels
	TRISB = 0x0000; 				// set IO as outputs

	while(1)
	{
		delay(1000);
		LATA = ~LATA;
		LATB = ~LATB;
	}
}
