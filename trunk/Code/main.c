//***************************************************************************************
//  MSP430 RGB LED Tile Control Software
//	Copyright 2012 Jorel Lalicki
//
//***************************************************************************************

//Board System Devices Port/Pin Numbers - These are NOT AVAILABLE FOR GENERAL USE unless disabled using shorting blocks:
// Port 1: ALL PINS
// Port 2: Pins 0,1,4



//Defines the on-board devices
//#define F1 INCH_7
//#define F2 INCH_5
//#define F3 INCH_4
//#define MIC INCH_0
//#define S1 BIT3 //Port 1
//#define S2 BIT0 //Port 2
//#define R BIT6 //Port 1
//#define G BIT1 //Port 2
//#define B BIT4 //Port 2



#include <msp430.h>				
#include "led_tile.h"
#include <stdlib.h>
//#include "launchpad_helpers.h"
int tempe;
char mode; //this global variable stores the current mode the LED TILE is operating in.
char rt;

//used for sound activated color rolling window averaging
char average[100];
char counter;


int main(void) //Program execution begins here:
{
	P1DIR &= ~BIT0;
	P1OUT &= ~BIT0;

	rt = 0;

	mode = 3; //This is the mode that the LED TILE will start in
	counter = 0;
	init();

  while(1){

	  int i;

   switch (mode){
  	  case 0:   {		  //very slow rgb fade

  			  target_rgb(255,0,0,1000000);
  			if (mode!= 0)break;

  			  target_rgb(255,255,0,1000000);
  			if (mode!= 0)break;

  			  target_rgb(0,255,0,1000000);
  			if (mode!= 0)break;

  			  target_rgb(0,255,255,1000000);
  			if (mode!= 0)break;

  			  target_rgb(0,0,255,1000000);
  			if (mode!= 0)break;

  			  target_rgb(255,0,255,1000000);
  		  break;
  	  }
//  	  case 1: //Sound Activated Color Thing! Quiet = blue, loud = RED
//  	  {
//  		  float gain = (float)Single_Measure(F1)/16+1; //set the gain from the R potentiometer - range 0-63
//  		  int threshold = Single_Measure(F2);//set the threshold from the R potentiometer
//  		  int temp = (Single_Measure(MIC));//value from 0-1023 indicating sound level. Room noise is around 100-150 with the air conditioner on... :D
//  		  temp -= threshold; //get rid of the ambient level
//  		  if (temp <0)temp=0; //if negative, make zero (leds cant show negative light)
//  		  temp*=gain; //multiply the level by 10 to get closer to the full LED brightness range
//  		  if (temp>255)temp=255; //if over the max level, make only the max level
//  		  //Keep a running average of 5 samples - this helps reduce flickering
//  		  int avg = 0;
//  		  int i;
//  		  for (i = 0; i < 100; i++){
//  			  avg += average[i];
//  		  }
//  		  avg /= 100;
//  		  average[counter] = temp;
//  		  counter++;
//  		  if (counter>=5)counter = 0;
//  		  target_rgb(255-avg,0,avg,1000);
//  		  break;
//  	  }
  	  case 1: //color temperature adjustment
  	  {
  		  int temp;
  		  temp = Single_Measure(F1) / 4;
  		  signed char offset = ((signed)temp - 128) / 2; //produce a signed offset that indicates "how far" to either side of "pure white"
  		  	  	  	  	  	  	  	  	  	  	  	  	 //INCH_7 is
  		  set_rgb(255-64+offset, 255-96,255-64-offset); //add the offset to the red, subtract it from the blue. More blue is a 'cooler'
  		  	  	  	  	  	  	  	  	  	  	  	  	//color temp.; more red is a 'warmer' color.
  		  break;
  	  }
  	  case 2: //Three knob color mixer
  		  //Using INCH_4, INCH_7 and INCH_5 for Red, Blue and Green components respectively
  	  {
  		  set_rgb((Single_Measure(F1) / 4), (Single_Measure(F2) / 4),(Single_Measure(F3) / 4));
  		  break;
  	  }
  	  case 4://strobe - with variable frequency, duty cycle, and color
  	{
  			for(i = 0; i < (float)(Single_Measure(F1))*(float)(Single_Measure(F2))/1024; ++i){
  				sleep(1000);//this is your frequency...
  		        }

  		  		int color = (Single_Measure(F3)) / 147;//grab the "hue" value, divide it up to give 7 values
  		  	  switch (color){ //select an output color based on the reading from F3
  		  	  	  	  	  	  //divide it into 7 discrete colors instead of a continuous spectrum
  		  	  case 0:
  		  		  set_rgb(255,0,0);
  		  		  break;
  		  	  case 1:
  		  		  set_rgb(0,255,0);
  		  		  break;

  		  	  case 2:
  		  		  set_rgb(0,0,255);
  		  		  break;

  		  	  case 3:
  		  		  set_rgb(255,255,0);
  		  		  break;

  		  	  case 4:
  		  		  set_rgb(255,0,255);
  		  		  break;

  		  	  case 5:
  		  		  set_rgb(0,255,255);
  		  		  break;

  		  	  case 6:
  		  		  set_rgb(255,255,255);
  		  		  break;
  		  	  }
  		  		for(i = 0; i < (float)(Single_Measure(F1))*(1-((float)(Single_Measure(F2))/1024)); ++i){
  		  			__delay_cycles(1000);//this is your duty cycle time...
  		  		}
  		  		set_rgb(0,0,0);
  		  break;
  	}
  	  case 3: //color fade to beat of music...
  	  {
  		  __delay_cycles(300000);
  		  int gain = 2;//_Single_Measure(F2)/16; //set the gain from the G potentiometer - range 0-63
  		  int threshold = _Single_Measure(F1);//set the threshold from the R potentiometer
  		  int temp = (_Single_Measure(MIC));//value from 0-1023 indicating sound level. Room noise is around 100-150 with the air conditioner on... :D
  		  temp -= threshold; //get rid of the ambient level
  		  if (temp <0)temp=0; //if negative, make zero (leds cant show negative light)
  		  temp*=gain; //multiply the level by 10 to get closer to the full LED brightness range
  		  if (temp>=255){//if the level is loud enough, it 'must' be a beat. HIT IT
  			  if (rt==0){
  				  //the color is red
  				target_rgb(255,0,0,200000);
  				++rt;
  			  }
  			  else if (rt == 1){
  				  //the color is green
  				target_rgb(0,255,0,200000);
  				++rt;
  			  }
  			  else if (rt == 2){
  				  //the color is blue
  				target_rgb(0,0,255,200000);
  				rt = 0;
  			  }
  		  }
  		  break;
  	  }
  	  }
  }
  }


// Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
   mode++;
   if (mode >=4) mode = 0;
   P1IFG &=~BIT3;                        // P1.3 IFG cleared
}






