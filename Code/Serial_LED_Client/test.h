#include <msp430.h>	

#ifndef LED_TILE_H_
#define LED_TILE_H_
#define period 1023 //period - 1 for PWM output

//Defines the on-board devices
#define F1 A7
#define F2 A5
#define F3 A4
#define MIC A0
#define S1 BIT3 //Port 1
#define S2 BIT0 //Port 2
#define R BIT0 //Port 1
#define G BIT1 //Port 2
#define B BIT4 //Port 2

unsigned char ADCDone;
int ADCValue;

//define three unsigned chars to keep track of linear representations of RGB values, prior to srgb conversion, to prevent the need to take sqrt.
unsigned char _r;
unsigned char _g;
unsigned char _b;


//CCR values for use with 'mangler'
int r_ccr = 0;
int g_ccr = 0;
int b_ccr = 0;

//Array of 'pseudorandom' values to mangle output registers with to reduce resonances of drive electronics
signed int mangler[] = {-317,167,-13,317,-167,13};
char rand_index = 0; //index of current mangler value (changed every 8ms)

int test(void);
void binit(void);
int stat(void);
int rval(void);

void set_lrgb(unsigned char r_new, unsigned char g_new, unsigned char b_new);
void set_rgb(unsigned char r_new, unsigned char g_new, unsigned char b_new);
void target_rgb(unsigned char r, unsigned char g, unsigned char  b, unsigned long time);
void sleed(int cycles);
int Single_Measure(unsigned int chan);
int _Single_Measure(unsigned int chan);



int test(){
	//do nothing!
	return 123;
}


void sleep(int cycles){ //wastes cpu cycles in increments of 100 cycles, for software delays
	while (cycles>0){
		__delay_cycles(100);
		cycles-=100;
	}
}


void binit(void){
	//set operating frequency to 16MHz
	DCOCTL = CALDCO_16MHZ;
	BCSCTL1 = CALBC1_16MHZ;

	//sets up the timer and sets ports to outputs/inputs


	//WDTCTL = WDTPW + WDTHOLD;             // Stop watchdog timer.


        //Set up watchdog timer to introduce random noise to the CCR registers
        WDTCTL = WDTPW+WDTTMSEL+WDTCNTCL+WDTIS0; //Set up watchdog timer for an interrupt every 8ms (125Hz)
        IE1 |= WDTIE;                       // Enable WDT interrupt
        






	P1DIR |= BIT6;// Set some pins to output direction

	//Set up Pin 1.3 as a button input
	P1DIR &= ~BIT3;
	P1REN |= BIT3;
	P1OUT |= BIT3;
	P1IE |= BIT3; // P1.3 interrupt enabled
	P1IFG &= ~BIT3; // P1.3 IFG cleared


	//******************************************

	P1DIR |= BIT6;             // P1.6 to output
	P1SEL |= BIT6;             // P1.6 to TA0.1
	P2DIR |= BIT1;             // P2.1 to output
	 P2SEL |= BIT1;             // P2.1 to TA1.1
	  P2DIR |= BIT4;             // P2.4 to output
	 P2SEL |= BIT4;             // P2.4 to TA1.2

 	  //*************************

 	//Set up timer
	CCTL1 = OUTMOD_7;                              // reset/set mode
	CCTL2 = OUTMOD_7;
	TA1CCTL1 = OUTMOD_7;
	TA1CCTL2 = OUTMOD_7;

	TACTL = TASSEL_2 + MC_1 + ID_0;           // SMCLK, upmode
	TA1CTL = TASSEL_2 + MC_1 + ID_0;

	CCR0 = period;        // period timer 0
	TA1CCR0 = period; //period timer 1
	CCR1 = 0;         // R
	CCR2 = 0;         // G
	_BIS_SR(GIE + TAIE); // Enable interrupts
	}

int _Single_Measure(unsigned int chan)
{
	ADC10CTL0 &= ~ENC;				// Disable ADC
	ADC10CTL0 = ADC10SHT_3 + ADC10ON + ADC10IE;	// 64 clock ticks, ADC On, enable ADC interrupt
	ADC10CTL1 = ADC10SSEL_3 + chan;				// Set 'chan', SMCLK
	ADC10CTL0 |= ENC + ADC10SC;             	// Enable and start conversion
	notdone:
	if(ADCDone==1)							// If the ADC is done with a measurement
			{
				ADCDone = 0;
				ADC10CTL0 |= ENC;				// Disable ADC

				return ADCValue;
			}
	else goto notdone;
}

int Single_Measure(unsigned int chan){
	return _Single_Measure(chan);// + _Single_Measure(chan) + _Single_Measure(chan) + _Single_Measure(chan) - 2) / 4; //take 4 readings, add them up and subtract 2 to take a true average
}



void target_rgb(unsigned char r, unsigned char g, unsigned char b, unsigned int time){


	unsigned long interval = time;
	float r_change =  _r;
	r_change -= r;
	r_change /= interval;

	float g_change =  _g;
	g_change -= g;
	g_change /= interval;

	float b_change =  _b;
	b_change -= b;
	b_change /= interval;

	//use existing values as a starting place - as floats, to allow very slow fading (change of <1 possible per iteration...)
	float r_temp = _r;
	float g_temp = _g;
	float b_temp = _b;


	long i;
	for(i = 0; i < interval; ++i){
			__delay_cycles(100);
			r_temp -= r_change;
			g_temp -= g_change;
			b_temp -= b_change;
			set_rgb(r_temp,g_temp,b_temp); //set the output to this intermediate color
	}
	//if all else failed, make sure the end result is right on target..., and update the internal linear values
	_r = r;
	_g = g;
	_b = b;
	set_rgb(r,g,b);
}



void set_lrgb(unsigned char r_new, unsigned char g_new, unsigned char b_new){ //linear RGB color space - not recommended for use!
	//update internal linear values
	_r = r_new;
	_g = g_new;
	_b = b_new;
	CCR1 = r_new*4;
        r_ccr = CCR1;
	TA1CCR1 = g_new*4;
        g_ccr = TA1CCR1;
	TA1CCR2 = b_new*4;
        b_ccr = TA1CCR2;
}

void set_rgb(unsigned char r_new, unsigned char g_new, unsigned char b_new){ //non-linear colorspace: instantly set output color
	//update internal linear values
	_r = r_new;
	_g = g_new;
	_b = b_new;

	 r_ccr = (float)((float)r_new / 255)*((float)r_new / 255)*period;
        CCR1 = r_ccr;
	g_ccr = (float)((float)g_new / 255)*((float)g_new / 255)*period;
        TA1CCR1 = g_ccr;
	b_ccr = (float)((float)b_new / 255)*((float)b_new / 255)*period;
        TA1CCR2 = b_ccr;

}

int stat(void){
	return CCR1;
}
int rval(void){
	return _r;
}






#endif /* LED_TILE_H_ */


