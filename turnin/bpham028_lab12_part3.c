/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <timer.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char tmpA;
unsigned char pattern = 0x00;
unsigned char row = 0x00;

enum states {start, top, middle, bottom} state;
void Tick() {

	switch(state) {
		case(start):
			state = top;
			break;
		case(top):
			state = middle;
			break;
		case(middle):
			state = bottom;
			break;
		case(bottom):
			state = top;
			break;
		default:
			state = start;
			break;
	}
	switch(state) {
		case(start):
			break;
		case(top):
			row = 0xFD;
			pattern = 0x3C;
			break;
		case(middle):
			row = 0xFB;
			pattern = 0x24;
			break;
		case(bottom):
			row = 0xF7;
			pattern = 0x3C;
			break;
		default:
			break;
	}
	PORTC = pattern;
	PORTD = row;

}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	TimerSet(1);
	TimerOn();
	while(1) {
		tmpA = ~PINA;
		Tick();
		while(!TimerFlag){}
		TimerFlag = 0;
	}
	return 1;
}

