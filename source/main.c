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
unsigned char pattern = 0xFF;
unsigned char row = 0xFE;

enum States {start, wait, up, down, release} State;
void Tick() {

	switch(State) {
		case(start):
			State = wait;
			break;
		case(wait):
			if (tmpA == 0x01) {
				State = up;
			} else if (tmpA == 0x02) {
				State = down;
			} else {
				State = wait;
			} break;
		case(up):
			State = release;
			break;
		case(down):
			State = release;
			break;
		case(release):
			if (tmpA == 0x00) {
				State = wait;
			} else {
				State = release;
			} break;
		default:
			State = start;
			break;
	}
	switch(State) {
		case(start):
			break;
		case(wait):
			break;
		case(up):
			if (row < 0xFE) {
				row = (row >> 1) | 0x80;
			} break;
		case(down):
			if (row > 0xEF) {
				row = (row << 1) | 0x01;
			} break;
		case(release):
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
	TimerSet(50);
	TimerOn();
	while(1) {
		tmpA = ~PINA;
		Tick();
		while(!TimerFlag){}
		TimerFlag = 0;
	}
	return 1;
}
