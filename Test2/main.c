//#include "p33fxxxx.h"
#include "p24hxxxx.h"

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

/*
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
		delay(100);
		LATA = ~LATA;
		LATB = ~LATB;
	}
}
*/

void InitTimer1( void )
{
	T1CON = 0;						/* ensure Timer 1 is in reset state */
 	IFS0bits.T1IF = 0;				/* reset Timer 1 interrupt flag */
	IPC0bits.T1IP = 4;				/* set Timer1 interrupt priority level to 4 */
 	IEC0bits.T1IE = 1;				/* enable Timer 1 interrupt */
	PR1 = 0xFFFF;					/* set Timer 1 period register */
	T1CONbits.TCKPS = 0x3;			/* select Timer1 Input Clock Prescale */
	T1CONbits.TCS = 0;			 	/* select external timer clock */
	T1CONbits.TON = 1;			 	/* enable Timer 1 and start the count */ 	
}

int main()
{
	LATA  = 0x0000; 				// set latch levels
	TRISA = 0x0000; 				// set IO as outputs
	LATB  = 0x0000; 				// set latch levels
	TRISB = 0x0000; 				// set IO as outputs
	
	InitTimer1();					// start the timer

	while(1);
}

void __attribute__((interrupt, auto_psv)) _T1Interrupt( void )
{
	LATA = ~LATA;
	LATB = ~LATB;
 	IFS0bits.T1IF = 0;				/* reset timer interrupt flag	*/
}	
