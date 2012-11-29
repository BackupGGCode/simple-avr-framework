/*
 * Test.cpp
 *
 *  Created on: 03-10-2011
 *      Author: radomir.mazon
 */

#include "Test.h"

uint8_t boolean_ =0;

void onTestEvent(saf_Event event)
{
	if (event.code == EVENT_BUTTON_DOWN && event.value ==0) {
		DDRB = 0xff;
		PORTB = PORTB | 2;
	}
	if (event.code == EVENT_BUTTON_DOWN && event.value ==1) {
			DDRB = 0xff;
			PORTB = PORTB & ~(2);
	}

	if (event.code == EVENT_SAFTICK && event.value%255==0) {
		boolean_++;
		DDRB = 0xff;
		if (boolean_%2) {
			PORTB = PORTB | 0x01;
			saf_eventBusSend_(EVENT_RS_SEND, 'X');
		} else {
			PORTB = PORTB & 0xFE;
			saf_eventBusSend_(EVENT_RS_SEND, '-');
		}
	}
}
