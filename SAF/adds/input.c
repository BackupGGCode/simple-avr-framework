/*
 * InputButtons.cpp
 *
 *  Created on: 23-09-2011
 *      Author: radomir.mazon
 */

#include "input.h"

#define _io_getSFR(i) _input_table.sfr[i]
#define _io_getBit(i) (_input_table.bit[i]&0x0F)
#define io_getState(i) ((_input_table.bit[i]&0xF0)>>4)

#define _io_setSFR(i,v) _input_table.sfr[i]=v
#define _io_setBit(i,v) _input_table.bit[i]=v
#define _io_setState(i,v) _input_table.bit[i] = (_input_table.bit[i]&0x0F) | (v<<4)

#define _DDR 1
#define _PORT 2

/**
 * Stan zostal zapisany w tablicy bit pod maska 0xF0
 */
typedef struct {
	uint8_t bit[INPUT_SIZE];
	uint8_t sfr[INPUT_SIZE];
} _io_type;

_io_type _input_table;
uint8_t _input_index = 0;

uint8_t input_add(uint8_t sfr, uint8_t bit) {
	_io_setSFR(_input_index, sfr);
	_io_setBit(_input_index, bit);
	_input_index++;
	_input_setup();
	return _input_index-1;
}

void _input_setup() {
	for (uint8_t i=0; i<_input_index; i++) {
		//DDR
		_SFR_IO8(_io_getSFR(i) + _DDR) &= ~(1<<_io_getBit(i));
		//PULL-UP
		_SFR_IO8(_io_getSFR(i) + _PORT) |= 1<<_io_getBit(i);
		//state
		_io_setState(i, bit_is_clear(_SFR_IO8(_io_getSFR(i)), _io_getBit(i)));
	}
}

void input_onEvent(saf_Event event) {
	if (event.code == EVENT_SAFTICK && (event.value%4) == 0) {
		for (uint8_t i=0; i<_input_index; i++) {
			uint8_t curState = bit_is_clear(_SFR_IO8(_io_getSFR(i)), _io_getBit(i));
			if (io_getState(i) != curState)
			{
				_io_setState(i, curState);
				saf_Event newEvent;
				newEvent.value = i;
				if (curState == 1) {
					newEvent.code = EVENT_IN_DOWN;
					saf_eventBusSend(newEvent);
				} else {
					newEvent.code = EVENT_IN_UP;
					saf_eventBusSend(newEvent);
				}
			}
		}
	}
}
