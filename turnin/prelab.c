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

enum States {shift} State;
void Tick() {
	static unsigned char pattern = 0x80; //LED pattern - 0: LED off; 1: LED on
	static unsigned char row = 0xFE; //Row(s) displaying pattern.
					// 0: display pattern on row
					// 1: do NOT display pattern on row
	switch(State) {
		case shift:
			break;
		default:
			State = shift;
			break;
	}
	switch(State) {
		case shift:
			if (row == 0xEF && pattern == 0x01) {
				pattern = 0x80;
				row = 0xFE;
			} else if (pattern == 0x01) {
				pattern = 0x80;
				row = (row << 1) | 0x01;
			} else {
				pattern >>= 1;
			} break;
		default:
			break;
	}
	PORTC = pattern;
	PORTD = row;
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRC = 0xFF; PORTC = 0x00; //output
	DDRD = 0xFF; PORTD = 0x00; //output
	TimerSet(100);
	TimerOn();
    /* Insert your solution below */
    while (1) {
	Tick();
	while(!TimerFlag){}
	TimerFlag = 0;
    }
    return 1;
}
