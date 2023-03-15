/*
 * Miniproject2.c
 *
 *  Created on: Sep 20, 2022
 *      Author: amaar
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

unsigned char sec = 0;
unsigned char min = 0;
unsigned char hour = 0;

void intrupt0(void){
	DDRD &= ~(1<<2);
	MCUCR |= (1<<ISC01);
	MCUCR &= ~(1<<ISC00);
	GICR = (1<<6);
}

void intrupt1(void){
	DDRD &= ~(1<<3);
	MCUCR |= (1<<ISC11);
	MCUCR |= (1<<ISC10);
	GICR = (1<<7);
}
void intrupt2(void){
	DDRB &= ~(1<<2);
	GICR = (1<<5);
	MCUCSR &= ~(1<<6);
}

void Timer1(void){
	TCNT1 = 0;
	OCR1A = 976; // COMPARE VALUE FOR 1 SECOND
	TIMSK |= 1<<OCIE1A; //ENABLE INTERRUPT
	TCCR1A |= 1<<FOC1A; // NON PWM MODE
	TCCR1B |= 1<<WGM12 | 1<<CS12 | 1<<CS10; // CTC MODE WITH 1024 PRESCALE
}


ISR(TIMER1_COMPA_vect){

	sec ++;
	if(sec == 60){
		sec = 0;
		min ++;
	}
	if(min == 60){
		min++;
		hour++;
	}
	if(hour == 60){
		sec = 0;
		min = 0;
		hour = 0;
	}
}

ISR(INT0_vect){
	sec = 0;
	min = 0;
	hour = 0;
}

ISR(INT1_vect){
	TCCR1B &=~ ((1<<CS11) | (1<<CS10) | (1<<CS12));
}

ISR(INT2_vect){
	TCCR1B |=  (1<<CS10) | (1<<CS12);
}


int main(void){
	SREG |= (1<<7);
	DDRA |= 0x3F;

	DDRC |= 0x0F;
	PORTC = 0x00;

	intrupt0();
	intrupt1();
	intrupt2();
	Timer1();


	while(1){

		PORTA = (1<<0);
		PORTC = (sec % 10);
		_delay_us(30);

		PORTA = (1<<1);
		PORTC = (sec / 10);
		_delay_us(30);

		PORTA = (1<<2);
		PORTC = (min % 10);
		_delay_us(30);

		PORTA = (1<<3);
		PORTC = (min / 10);
		_delay_us(30);

		PORTA = (1<<4);
		PORTC = (hour % 10);
		_delay_us(30);

		PORTA = (1<<5);
		PORTC = (hour / 10);
		_delay_us(30);
	}
}

