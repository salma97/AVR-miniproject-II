/*
 * main.c
 *
 *  Created on: Sep 26, 2018
 *      Author: Salma Tarek
 */
#include "lcd.h"
#include "adc.h"
#include "Timer0.h"

uint8 flag = 0;


ISR(INT2_vect)
{
		flag = 1;
		GIFR |= (1<<INTF2);
}

void INT2_Init(void)
{
	SREG  &= ~(1<<7);
	DDRB  &= (~(1<<PB2));
	MCUCR |= (1<<ISC2);
	GICR  |= (1<<INT2);
	SREG  |= (1<<7);
}

int main (void)
{
	INT2_Init();
	LCD_init();
	ADC_init();

	uint16 Pot_value = 512;
	float32 speed = 128;
	uint8 speed_percentage = 50;

	LCD_clearScreen();
	LCD_displayString("Motor Speed:");
	LCD_goToRowColumn(1,3);
	LCD_displayCharacter('%');

	DDRB |= (1<<PB4) | (1<<PB5);

	PWM_Timer0_Init(128);
	SET_BIT (PORTB,PB4);
	CLEAR_BIT(PORTB,PB5);

	while(1)
	{
		LCD_goToRowColumn(1,0);

		Pot_value = ADC_readChannel(0);
		speed = (((float)Pot_value*256)/1023);
		speed_percentage = ((speed/256)*100);

		LCD_intgerToString(speed_percentage);

		OCR0 = speed;

		if (flag == 1)
		{
			TOGGLE_BIT(PORTB,PB4);
			TOGGLE_BIT(PORTB,PB5);
			flag = 0;
		}
	}
}

