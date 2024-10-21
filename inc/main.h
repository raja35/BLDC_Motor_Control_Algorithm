


#ifndef MAIN_H_
#define MAIN_H_


/* gates signals */
#define UL_MODER DDRB
#define UL_ODR PORTB
#define UL_PIN 0x20

#define UH_MODER DDRB
#define UH_ODR PORTB
#define UH_PIN 0x40

#define VL_MODER DDRE
#define VL_ODR PORTE
#define VL_PIN 0x08

#define VH_MODER DDRE
#define VH_ODR PORTE
#define VH_PIN 0x10

#define WL_MODER DDRH
#define WL_ODR PORTH
#define WL_PIN 0x08

#define WH_MODER DDRH
#define WH_ODR PORTH
#define WH_PIN 0x10

/*accelerator pin*/
#define ACC_PUSH_MODER DDRC
#define ACC_PUSH_IDR PINC
#define ACC_PUSH_PIN 0x01

#define ACC_RLS_MODER DDRC
#define ACC_RLS_IDR PINC
#define ACC_RLS_PIN 0x02

//function declaration
void bldc_move(void);


#endif /* MAIN_H_ */