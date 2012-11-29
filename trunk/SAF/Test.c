/*
 * Test.cpp
 *
 *  Created on: 03-10-2011
 *      Author: radomir.mazon
 */

#include "Test.h"

uint8_t boolean_ =0;

void onTestEvent(uint8_t code, int value)
{
	if (code == EVENT_BUTTON_DOWN && value ==0) {
		DDRB = 0xff;
		PORTB = PORTB | 2;
	}
	if (code == EVENT_BUTTON_DOWN && value ==1) {
			DDRB = 0xff;
			PORTB = PORTB & ~(2);
	}

	if (code == EVENT_SAFTICK && timePrescaler(255)) {
		boolean_++;
		DDRB = 0xff;
		if (boolean_%2) {
			PORTB = PORTB | 0x01;
			saf_eventBusSend(EVENT_RS_SEND, 'X');
		} else {
			PORTB = PORTB & 0xFE;
			saf_eventBusSend(EVENT_RS_SEND, '-');
		}
	}
}
