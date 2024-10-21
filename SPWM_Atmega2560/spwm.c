/*Generate PWM
Type - 8 bit Fast PWM 
Controller - Atmega2560
Method - Using timer we are going to turn on/off a pin wrt required duty cycle.
Timer - TIMER0 (timer 0 to MAX - Period)(comparation is duty cycle OCRA)
PWM PIN - PD6 (OC0A)
Frequency - Default
*/

#include <avr/io.h>
#include <avr/interrupt.h>

static double dutyCycle = 0;

static int lookUp1[] = {5 		,10 	,15 	,20 	,25 	,30 	,35 	,40 	,45 	,50 
				,55 	,60 	,65 	,70 	,75 	,80 	,85 	,90 	,95 	,100 
				,105 	,110 	,115 	,120 	,125 	,130 	,135 	,140 	,145 	,150 
				,155 	,160 	,165 	,170 	,175 	,180 	,185 	,190 	,195 	,200 
				,205 	,210 	,215 	,220 	,225 	,230 	,235 	,240 	,245 	,250 
				,245 	,240 	,235 	,230 	,225 	,220 	,215 	,210 	,205 	,200 
				,195 	,190 	,185 	,180 	,175 	,170 	,165 	,160 	,155 	,150 
				,145 	,140 	,135 	,130 	,125 	,120 	,115 	,110 	,105 	,100
				,95 	,85 	,80 	,75 	,70 	,65 	,60 	,55 	,50 	,45
				,40 	,35 	,30 	,25 	,20 	,15 	,10 	,5 		,0 		,0
				,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 
				,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 
				,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 
				,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 
				,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 
				,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 
				,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 
				,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 
				,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 
				,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0};
				
static int lookUp2[] = {0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 
				,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 
				,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 
				,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 
				,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 
				,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 
				,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 
				,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 
				,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 
				,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 		,0 
				,5 		,10 	,15 	,20 	,25 	,30 	,35 	,40 	,45 	,50 
				,55 	,60 	,65 	,70 	,75 	,80 	,85 	,90 	,95 	,100 
				,105 	,110 	,115 	,120 	,125 	,130 	,135 	,140 	,145 	,150 
				,155 	,160 	,165 	,170 	,175 	,180 	,185 	,190 	,195 	,200 
				,205 	,210 	,215 	,220 	,225 	,230 	,235 	,240 	,245 	,250 
				,245 	,240 	,235 	,230 	,225 	,220 	,215 	,210 	,205 	,200 
				,195 	,190 	,185 	,180 	,175 	,170 	,165 	,160 	,155 	,150 
				,145 	,140 	,135 	,130 	,125 	,120 	,115 	,110 	,105 	,100
				,95 	,85 	,80 	,75 	,70 	,65 	,60 	,55 	,50 	,45
				,40 	,35 	,30 	,25 	,20 	,15 	,10 	,5 		,0 		,0};

int main(void)
	{
		/*enable PB7 as outpot*/
		DDRB = (1 << PORTB7);
		/*Set PIN Mode to PWM
		  set PWM Mode to fast Mode*/
		TCCR0A = (1 << COM0A1) | (1 << WGM00) | (1 << WGM01);
		TCCR0B = 0;
		/*set Interupt mask*/
		TIMSK0 = (1 << TOIE0);
		/*set duty cycle*/
		OCR0A = (dutyCycle/100)*255;
		OCR0B = (dutyCycle/100)*255;
		/*set external interrupt*/
		sei();
		/*set the prescaler value*/
		TCCR0B = (1 << CS00);
		while(1)
		{
			//do nothing
		}
	}

ISR(TIMER0_OVF_vect)
	{
		/*if we want to change duty cycle in between program*/
		//OCR0A = (dutyCycle/100)*255;
		static int num;
		// change duty-cycle every period.
		OCR0A = lookUp1[num];
		OCR0B = lookUp2[num];
		num++;
		if(num >= 200)
		{
			num = 0;
		}
	}