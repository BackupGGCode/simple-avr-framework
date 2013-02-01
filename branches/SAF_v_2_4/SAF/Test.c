/*
 * Test.cpp
 *
 *  Created on: 03-10-2011
 *      Author: radomir.mazon
 */

#include "Test.h"
#include "adds/output.h"

uint8_t boolean_ =0;

void onTestEvent(saf_Event event)
{
	if (event.code == EVENT_IN_DOWN && event.value == 0) {
		saf_eventBusSend_(EVENT_OUT_BLINK_LOOP, 0b01100000);
	}
	if (event.code == EVENT_IN_DOWN && event.value == 1) {
		saf_eventBusSend_(EVENT_OUT_BLINK_LOOP, 0b00100001);
	}
	if (event.code == EVENT_IN_DOWN && event.value == 2) {
		saf_eventBusSend_(EVENT_OUT_BLINK_STOP, 0);
	}
	if (event.code == EVENT_IN_DOWN && event.value == 3) {
		saf_eventBusSend_(EVENT_OUT_BLINK_STOP, 1);
	}
}
