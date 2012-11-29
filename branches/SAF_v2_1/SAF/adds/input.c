/*
 * InputButtons.cpp
 *
 *  Created on: 23-09-2011
 *      Author: radomir.mazon
 */

#include "input.h"

#define getSFR(i) _input_table.sfr[i]
#define getBit(i) _input_table.bit[i]
#define getState(i) _input_table.state[i]

#define setSFR(i,v) _input_table.sfr[i]=v
#define setBit(i,v) _input_table.bit[i]=v
#define setState(i,v) _input_table.state[i]=v


_input_type _input_table;
uint8_t _input_index = 0;

void input_add(uint8_t sfr, uint8_t bit) {
	setSFR(_input_index, sfr);
	setBit(_input_index, bit);
	_input_index++;
	_input_setup();
}

void _input_setup() {
	for (uint8_t i=0; i<_input_index; i++) {
		//DDR
		_SFR_IO8(getSFR(i) + _DDR) &= ~(1<<getBit(i));
		//PULL-UP
		_SFR_IO8(getSFR(i) + _PORT) |= 1<<getBit(i);
		//state
		setState(i, bit_is_clear(_SFR_IO8(getSFR(i)), getBit(i)));
	}
}

void input_onEvent(uint8_t code, int value) {
	for (uint8_t i=0; i<_input_index; i++) {
		uint8_t curState = bit_is_clear(_SFR_IO8(getSFR(i)), getBit(i));
		if (getState(i) != curState)
		{
			setState(i, curState);
			if (curState == 1) {
				saf_eventBusSend(EVENT_BUTTON_DOWN, i);
			} else {
				saf_eventBusSend(EVENT_BUTTON_UP, i);
			}
		}
	}
}
