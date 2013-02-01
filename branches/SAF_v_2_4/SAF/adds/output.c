/*
 * output.c
 *
 *  Created on: 17-12-2012
 *      Author: radomir.mazon
 */
#include "output.h"

#define _out_getSFR(i) _out_table.sfr[i]
#define _out_getBit(i) (_out_table.bit[i]&0x0F)
#define out_getState(i) ((_out_table.bit[i]&0xF0)>>4)

#define _out_setSFR(i,v) _out_table.sfr[i]=v
#define _out_setBit(i,v) _out_table.bit[i]=v
#define _out_setState(i,v) _out_table.bit[i] = (_out_table.bit[i]&0x0F) | (v<<4)

#define _out_setLo(i) _SFR_IO8(_out_getSFR(i) + _PORT) &= ~(1<<_out_getBit(i));
#define _out_setHi(i) _SFR_IO8(_out_getSFR(i) + _PORT) |= 1<<_out_getBit(i);

#define FREE_ITEM 0xff
#define _DDR 1
#define _PORT 2

/**
 * Stan zostal zapisany w tablicy bit pod maska 0xF0
 */
typedef struct {
	uint8_t bit[OUTPUT_SIZE];
	uint8_t sfr[OUTPUT_SIZE];
} _io_type;

_io_type _out_table;
uint8_t _out_index = 0;
uint8_t _out_blink[OUTPUT_BLINK_SIZE];

#if OUTPUT_BLINK_LOOP_CTRL == 1
uint8_t _loopCounter[OUTPUT_BLINK_LOOP_CTRL_SIZE];
#endif


void output_add(uint8_t sfr, uint8_t bit, uint8_t isNegative) {
	_out_setSFR(_out_index, sfr);
	_out_setBit(_out_index, bit);
	_out_setState(_out_index, isNegative);
	_out_index++;
	_output_setup(isNegative);
}

void _output_setup() {
#if OUTPUT_BLINK_LOOP_CTRL == 1
	for (uint8_t i=0; i<OUTPUT_BLINK_LOOP_CTRL_SIZE; i++) {
		_loopCounter[i] = FREE_ITEM;
	}
#endif
	for (uint8_t i=0; i<_out_index; i++) {
		//DDR
		_SFR_IO8(_out_getSFR(i) + _DDR) |= 1<<_out_getBit(i);
		//state
		if (out_getState(i) == 0) {
			_out_setLo(i);
		} else {
			_out_setHi(i);
		}

	}
}

void output_onEvent(saf_Event event) {
	if (event.code == EVENT_OUT_DOWN) {
		if (event.value < _out_index){
				if (out_getState(event.value) == 0) {
					_out_setLo(event.value);
				} else {
					_out_setHi(event.value);
				}
		}
	}

	if (event.code == EVENT_OUT_UP) {
		if (event.value < _out_index){
			if (out_getState(event.value) == 0) {
				_out_setHi(event.value);
			} else {
				_out_setLo(event.value);
			}
		}
	}

	if (event.code == EVENT_OUT_BLINK) {
		uint16_t interval = OUTPUT_BLINK_DELAY*(event.value >> 5);
		if (interval == 0) {
			interval = 1;
		}
		saf_eventBusSend_(EVENT_OUT_UP, event.value & 0b00011111);
		saf_startTimer(interval*2, EVENT_OUT_DOWN, event.value & 0b00011111);
	}

#if OUTPUT_BLINK_LOOP_CTRL == 1
	if (event.code == EVENT_OUT_BLINK_LOOP) {
		uint16_t interval = OUTPUT_BLINK_DELAY*(event.value >> 5) + 1;
		uint8_t value = event.value & 0b00011111;
		saf_eventBusSend_(EVENT_OUT_UP, value);
		saf_startTimer(interval, EVENT_OUT_DOWN, value);
		saf_startTimer(interval*2, EVENT_OUT_BLINK_LOOP, event.value);

		uint8_t index = 0xFF;
		for (uint8_t i=0; i<OUTPUT_BLINK_LOOP_CTRL_SIZE; i++) {
			if (_loopCounter[i] == FREE_ITEM) {
				index = i;
			}
			if ((_loopCounter[i]&0b00011111) == value) {
				index = i;
				break;
			}
		}
		if (index!=0xFF) {
			_loopCounter[index] = event.value;
		}

	}

	if (event.code == EVENT_OUT_BLINK_STOP) {
		uint8_t value = event.value & 0b00011111;
		for (uint8_t i=0; i<OUTPUT_BLINK_LOOP_CTRL_SIZE; i++) {
			if ((_loopCounter[i]&0b00011111) == value) {
				saf_cancelTimer(EVENT_OUT_DOWN, value);
				saf_cancelTimer(EVENT_OUT_BLINK_LOOP, _loopCounter[i]);
				_loopCounter[i] = FREE_ITEM;
				saf_eventBusSend_(EVENT_OUT_DOWN, value);
				break;
			}
		}
	}
#endif
}
