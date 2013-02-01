/*
 * keyboard.c
 *
 *  Created on: 19-01-2013
 *      Author: Administrator
 */

#include "keyboard.h"

uint8_t _kb_x_sfr[KB_X_SIZE];
uint8_t _kb_x_bit[KB_X_SIZE];
uint8_t _kb_y_sfr[KB_Y_SIZE];
uint8_t _kb_y_bit[KB_Y_SIZE];
uint8_t _kb_state[KB_X_SIZE][KB_Y_SIZE];
uint8_t _kb_xIndex =0;
uint8_t _kb_yIndex =0;
uint8_t _kb_power =0;

#define _DDR 1
#define _PORT 2
void _kb_init() {
	for (uint8_t x=0; x<KB_X_SIZE; x++) {
		for (uint8_t y=0; y<KB_Y_SIZE; y++) {
			_kb_state[x][y] = 0;
		}
	}
}

/**
 * curState 0 - puszczony
 * 	        1 - wcisniety
 */
void _kb_scan(uint8_t y) {
	for (uint8_t x=0; x<_kb_xIndex; x++) {
		uint8_t curState = bit_is_clear(_SFR_IO8(_kb_x_sfr[x]), _kb_x_bit[x]);
		if (curState == 0 && _kb_state[x][y] >0) {
			if (_kb_state[x][y] < KB_LONG_PRESS) {
				saf_eventBusSend_(EVENT_KB_PRESS, x + y*(_kb_xIndex));
			} else {

			}
		}
		if (curState >0 && _kb_state[x][y] == 0) {
			saf_eventBusSend_(EVENT_KB_PRESS_START, x + y*(_kb_xIndex));
		}

		//normalizacja
		if (curState == 0) {
			_kb_state[x][y] = curState;
		} else {
			if (_kb_state[x][y]<(KB_LONG_PRESS-1)) {
				_kb_state[x][y]++;
			}
			if (_kb_state[x][y]==(KB_LONG_PRESS-1)) {
				saf_eventBusSend_(EVENT_KB_PRESS_LONG, x + y*(_kb_xIndex));
				_kb_state[x][y]++;
			}
		}
	}
}

void _kb_setPin(uint8_t pin) {
	//wylaczyc wszystkie piny Y
		for (uint8_t yy=0; yy<_kb_yIndex; yy++) {
			//DDR IN
			_SFR_IO8(_kb_y_sfr[yy] + _DDR) &= ~(1<<_kb_y_bit[yy]);
			//PULL-UP off
			_SFR_IO8(_kb_y_sfr[yy] + _PORT) &= ~(1<<_kb_y_bit[yy]);
		}
		//wlaczyc tylko wybrany pin
		_SFR_IO8(_kb_y_sfr[pin] + _DDR) |= 1<<_kb_y_bit[pin];
		_SFR_IO8(_kb_y_sfr[pin] + _PORT) &= ~(1<<_kb_y_bit[pin]);
}

void _kb_run() {
	if (_kb_yIndex == 0 || _kb_xIndex == 0) {
		return;
	}
	for (uint8_t y=0; y<_kb_yIndex; y++) {
		_kb_setPin(y);
		_kb_scan(y);
	}
}

void kb_onEvent(saf_Event e) {
	if (e.code == EVENT_KB_ON) {
		_kb_power = 1;
	}
	if (e.code == EVENT_KB_OFF) {
		_kb_power = 0;
	}
	if (_kb_power == 0) {
		return;
	}
	if (e.code == EVENT_INIT) {
		_kb_init();
	}
	if (e.code == EVENT_SAFTICK && (e.value%4 == 0)) {
			_kb_run();
	}
}

void kb_addKeyX(uint8_t sfr, uint8_t bit) {
	_kb_x_sfr[_kb_xIndex] = sfr;
	_kb_x_bit[_kb_xIndex++] = bit;
	//DDR IN
	_SFR_IO8(sfr + _DDR) &= ~(1<<bit);
	//PULL-UP on
	_SFR_IO8(sfr + _PORT) |= 1<<bit;
}

void kb_addKeyY(uint8_t sfr, uint8_t bit) {
	_kb_y_sfr[_kb_yIndex] = sfr;
	_kb_y_bit[_kb_yIndex++] = bit;
}
