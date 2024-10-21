/*
 * Description: control sensored BLDC motor with openloop method
 */ 
 
#include <avr/io.h>
#include <avr/interrupt.h>

#include "main.h"



//local function declaration
static void GpioInit(void);
static void PWM_Init(void);
static void Set_Dyty_Cycle(unsigned int duty);
static void Adc_Comp_Init(void);

static void UH_VL(void);
static void UH_WL(void);
static void VH_WL(void);
static void VH_UL(void);
static void WH_UL(void);
static void WH_VL(void);

static void BEMF_W_RISING(void);
static void BEMF_V_FALLING(void);
static void BEMF_U_RISING(void);
static void BEMF_W_FALLING(void);
static void BEMF_V_RISING(void);
static void BEMF_U_FALLING(void);

//macros
#define PWM_START_DUTY    100
#define PWM_MAX_DUTY      255
#define PWM_MIN_DUTY      50


//local variables
static unsigned int bldc_step;
static unsigned int motor_speed;

int main(void)
{
	/* Peripherals initializations */
	GpioInit();
	PWM_Init();
	Adc_Comp_Init();
	
	/* enable global interrupts  */
	sei();
	
	Set_Dyty_Cycle(PWM_START_DUTY);    // Setup starting PWM with duty cycle = PWM_START_DUTY
	i = 5000;
	// Motor start in openloop
	while(i > 100) 
	{
		delayMicroseconds(i);
		bldc_move();
		bldc_step++;
		bldc_step %= 6;
		i = i - 20;
	}
	//application code
	motor_speed = PWM_START_DUTY;
	while(1)
	{
		if((ACC_PUSH_IDR & (1 << ACC_PUSH_PIN)) && motor_speed < PWM_MAX_DUTY)
		{
			motor_speed++;
			Set_Dyty_Cycle(motor_speed);
			delay(100);
		}
		if((ACC_RLS_IDR & (1 << ACC_RLS_PIN)) && motor_speed > PWM_MIN_DUTY)
		{
			motor_speed--;
			Set_Dyty_Cycle(motor_speed);
			delay(100);
		}
	}
}


// BLDC motor commutation function
void bldc_move()
{   
  switch(bldc_step)
  {
    case 0:
      UH_VL();
	  BEMF_W_RISING();
      break;
    case 1:
      UH_WL();
	  BEMF_V_FALLING();
      break;
    case 2:
      VH_WL();
	  BEMF_U_RISING();
      break;
    case 3:
      VH_UL();
	  BEMF_W_FALLING();
      break;
    case 4:
      WH_UL();
	  BEMF_V_RISING();
      break;
    case 5:
      WH_VL();
	  BEMF_U_FALLING();
      break;
  }
}

static void GpioInit(void)
{
	/* gates signals output mode */
	UL_MODER |= UL_PIN;	
	UH_MODER |= UH_PIN;
	VL_MODER |= VL_PIN;
	VH_MODER |= VH_PIN;
	WL_MODER |= WL_PIN;
	WH_MODER |= WH_PIN;
	
	/*accelerator pins*/
	ACC_PUSH_MODER &= ~ACC_PUSH_PIN;
	ACC_RLS_MODER &= ~ACC_RLS_PIN;
}

// Initialise PWM to Fast PWM
static void PWM_Init(void)
{
	TCCR1A = (1 << COM1A1) | (1 << COM1B1);
	TCCR1B = (1 << WGM12) | (1 << CS10);
	OCR1A = PWM_START_DUTY;
	
	TCCR3A = (1 << COM3A1) | (1 << COM3B1);
	TCCR3B = (1 << WGM32) | (1 << CS30);
	OCR3A = PWM_START_DUTY;
	
	TCCR4A = (1 << COM4A1) | (1 << COM4B1);
	TCCR4B = (1 << WGM42) | (1 << CS40);
	OCR4A = PWM_START_DUTY;
	
}

static void Set_Dyty_Cycle(unsigned int duty)
{
	if(duty < PWM_MIN_DUTY)
		duty  = PWM_MIN_DUTY;
	if(duty > PWM_MAX_DUTY)
		duty  = PWM_MAX_DUTY;
	OCR1A = duty;
	OCR3A = duty;
	OCR4A = duty;
}

// initialise Analog comparator
static void Adc_Comp_Init(void)
{
	ADCSRB  = (1 << ACME);
	ADCSRA  = (0 << ADEN);
	ACSR    = (1 << ACI) | (1 << ACIE);
	
}

static void UH_VL(void)
{
	UH_ODR |= UH_PIN;
	VL_ODR |= VL_PIN;
	
	UL_ODR &= ~UL_PIN;
	VH_ODR &= ~VH_PIN;
	WH_ODR &= ~WH_PIN;
	WL_ODR &= ~WL_PIN;
	
}
static void UH_WL(void)
{
	UH_ODR |= UH_PIN;
	WL_ODR |= WL_PIN;
	
	VL_ODR &= ~VL_PIN;
	UL_ODR &= ~UL_PIN;
	VH_ODR &= ~VH_PIN;
	WH_ODR &= ~WH_PIN;
}
static void VH_WL(void)
{
	VH_ODR |= VH_PIN;
	WL_ODR |= WL_PIN;
	
	UH_ODR &= ~UH_PIN;
	VL_ODR &= ~VL_PIN;
	UL_ODR &= ~UL_PIN;
	WH_ODR &= ~WH_PIN;
}
static void VH_UL(void)
{
	VH_ODR |= VH_PIN;
	UL_ODR |= UL_PIN;
	
	WL_ODR &= ~WL_PIN;
	UH_ODR &= ~UH_PIN;
	VL_ODR &= ~VL_PIN;
	WH_ODR &= ~WH_PIN;
	
}
static void WH_UL(void)
{
	UL_ODR |= UL_PIN;
	WH_ODR |= WH_PIN;
	
	VH_ODR &= ~VH_PIN;
	WL_ODR &= ~WL_PIN;
	UH_ODR &= ~UH_PIN;
	VL_ODR &= ~VL_PIN;
	
}
static void WH_VL(void)
{
	WH_ODR |= WH_PIN;
	VL_ODR |= VL_PIN;
	
	UL_ODR &= ~UL_PIN;
	VH_ODR &= ~VH_PIN;
	WL_ODR &= ~WL_PIN;
	UH_ODR &= ~UH_PIN;
}

static void BEMF_U_RISING(void)
{
	ADMUX  = (0 << MUX2) | (0 << MUX1) | (0 << MUX0);
	ACSR  |= (1 << ACIS1) | (1 << ACIS0);
}

static void BEMF_U_FALLING(void)
{
	ADMUX  = (0 << MUX2) | (0 << MUX1) | (0 << MUX0);
	ACSR  |= (1 << ACIS1) | (0 << ACIS0);
}

static void BEMF_V_RISING(void)
{
	ADMUX  = (0 << MUX2) | (0 << MUX1) | (1 << MUX0);
	ACSR  |= (1 << ACIS1) | (1 << ACIS0);	
}

static void BEMF_V_FALLING(void)
{
	ADMUX  = (0 << MUX2) | (0 << MUX1) | (1 << MUX0);
	ACSR  |= (1 << ACIS1) | (0 << ACIS0);	
}

static void BEMF_W_RISING(void)
{
	ADMUX  = (0 << MUX2) | (1 << MUX1) | (0 << MUX0);
	ACSR  |= (1 << ACIS1) | (1 << ACIS0);	
}

static void BEMF_W_FALLING(void)
{
	ADMUX  = (0 << MUX2) | (1 << MUX1) | (0 << MUX0);
	ACSR  |= (1 << ACIS1) | (0 << ACIS0);	
}

// Analog comparator ISR
ISR (ANALOG_COMP_vect) 
{
  bldc_step %= 5;
  bldc_move();
  bldc_step++;
}

