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
//#include <scheduler.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char tmpA;
unsigned char pattern = 0x00;
unsigned char row = 0x00;
unsigned char patternA[3] = {0x3C, 0x24, 0x3C};
unsigned char rowA[3] = {0xFD, 0xFB, 0xF7};
unsigned char i = 0;

enum states {start, wait, top, middle, bottom, up, down, left, right, release} state;
void Tick() {

	switch(state) {
		case(start):
			state = wait;
			break;
		case(wait):
		//	i = 0;
			if (tmpA == 0x01) {
				i = 0;
				state = up;
			} else if (tmpA == 0x02) {
				i = 2;
				state = down;
			} else if (tmpA == 0x04) {
				i = 2;
				state = right;
			} else if (tmpA == 0x08) {
				i = 0;
				state = left;
			} else {
				state = top;
			} break;
		case(top):
			state = middle;
			break;
		case(middle):
			state = bottom;
			break;
		case(bottom):
			state = wait;
			break;
		case(up):
			state = release;
			break;
		case(down):
			state = release;
			break;
		case(left):
			state = release;
			break;
		case(right):
			state = release;
			break;
		case(release):
			if (tmpA == 0x00) {
				state = wait;
			} else {
				state = release;
			} break;
		default:
			state = start;
			break;
	}
	switch(state) {
		case(start):
			i = 0;
			break;
		case(wait):
			break;
		case(top):
			if (i == 2) {
				i = 0;
			}
			row = rowA[i];
			pattern = patternA[i];
			++i;
			break;
		case(middle):
			row = rowA[i];
			pattern = patternA[i];
			++i;
			break;
		case(bottom):
			row = rowA[i];
			pattern = patternA[i];
			break;
		case(up):
			if (rowA[i] < 0xFE) {
				rowA[i] = (rowA[i] >> 1) | 0x80;
				++i;
			}
			if (rowA[i] < 0xFE) {
				rowA[i] = (rowA[i] >> 1) | 0x80;
				++i;
			}
			if (rowA[i] < 0xFE) {
				rowA[i] = (rowA[i] >> 1) | 0x80;
			} break;
		case(down):
			if (rowA[i] > 0xEF) {
				rowA[i] = (rowA[i] << 1) | 0x01;
				--i;
			}
			if (rowA[i] > 0xEF) {
				rowA[i] = (rowA[i] << 1) | 0x01;
				--i;
			}
			if (rowA[i] > 0xEF) {
				rowA[i] = (rowA[i] << 1) | 0x01;
			} break;
		case(right):
			if (patternA[i] > 0x0F) {
				patternA[i] = patternA[i] >> 1;
				--i;
			}
			if (patternA[i] > 0x0F) {
				patternA[i] = patternA[i] >> 1;
				--i;
			}
			if (patternA[i] > 0x0F) {
				patternA[i] = patternA[i] >> 1;
			} break;
		case(left):
			if (patternA[i] < 0x80) {
				patternA[i] = patternA[i] << 1;
				++i;
			}
			if (patternA[i] < 0x80) {
				patternA[i] = patternA[i] << 1;
				++i;
			}
			if (patternA[i] < 0x80) {
				patternA[i] = patternA[i] << 1;
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
