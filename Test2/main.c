#include "p33fxxxx.h"
//#include "p24hxxxx.h"
#include "fftSettings.h"
#include "dsp.h"

#define USING_PLL	1

#if USING_PLL == 1
_FOSCSEL(FNOSC_FRCPLL);								//oscillator mode FRC ~ 8Mhz
#else
_FOSCSEL(FNOSC_FRC);
#endif

_FOSC(FCKSM_CSDCMD & OSCIOFNC_ON & POSCMD_NONE);	//ISCIO pin for RA3
_FWDT(FWDTEN_OFF);									//watchdog off


extern const fractcomplex twiddleFactors[FFT_BLOCK_LENGTH/2]	/* Twiddle Factor array in Program memory */
__attribute__ ((space(auto_psv), aligned (FFT_BLOCK_LENGTH*2)));

fractcomplex __attribute__((far)) _YBSS(FFT_BLOCK_LENGTH*4) sigCmpx[FFT_BLOCK_LENGTH];
fractional magnitudes[FFT_BLOCK_LENGTH>>1];

long __attribute__((far)) _YBSS(FFT_BLOCK_LENGTH*4) sigReal[FFT_BLOCK_LENGTH];

void RunTest();
int	peakFrequencyBin = 0;

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
	
	//InitTimer1();					// start the timer


	while(1)
	{
/*
		int i;
		for(i=0;i<FFT_BLOCK_LENGTH;i++)
		{
			if((i/4)%2 == 0)
				sigCmpx[i].real = 0x7FFF;
			else
				sigCmpx[i].real = 0x8001;

//			sigCmpx[i].real = (i / 4)%2 * 0.8 - 0.4;
			sigCmpx[i].imag = 0;
			//sigReal[i] = ((i/4) % 2) * 10 + 3;
		}*/

		RunTest();
		LATA = ~LATA;
		LATB = ~LATB;
		break;
	}

	while(1);
}

void __attribute__((interrupt, auto_psv)) _T1Interrupt( void )
{
	LATA = ~LATA;
	LATB = ~LATB;
 	IFS0bits.T1IF = 0;				/* reset timer interrupt flag	*/
}	

void RunTest()
{
	
	FFTComplexIP(	LOG2_BLOCK_LENGTH,
					&sigCmpx[0],
					(fractcomplex *) __builtin_psvoffset(&twiddleFactors[0]), 
					(int) __builtin_psvpage(&twiddleFactors[0])
				);

/* Store output samples in bit-reversed order of their addresses */
	BitReverseComplex (LOG2_BLOCK_LENGTH, &sigCmpx[0]);

	/* Compute the square magnitude of the complex FFT output array so we have a Real output vetor */
	SquareMagnitudeCplx(FFT_BLOCK_LENGTH, &sigCmpx[0], &sigCmpx[0].real);

	/* Find the frequency Bin ( = index into the SigCmpx[] array) that has the largest energy*/
	/* i.e., the largest spectral component */
//	VectorMax(FFT_BLOCK_LENGTH/2, &sigCmpx[0].real, &peakFrequencyBin);

	/* Compute the frequency (in Hz) of the largest spectral component */

	//peakFrequency = peakFrequencyBin*(SAMPLING_RATE/FFT_BLOCK_LENGTH);

/*
extern long* FFTReal32bIP(
			int log2N, 
			int N,
			long *ipBuff, 
			long *tfBuf,
			int factPage

	FFTReal32bIP(	LOG2_BLOCK_LENGTH,
					FFT_BLOCK_LENGTH,
					&sigReal[0],
					(long *) __builtin_psvoffset(&twiddleFactors[0]), 
					(int) __builtin_psvpage(&twiddleFactors[0])
				);*/

	
}
