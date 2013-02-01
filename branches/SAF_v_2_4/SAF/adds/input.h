/*
 * InputButtons.h
 *
 *  Created on: 23-09-2011
 *      Author: radomir.mazon
 */

#ifndef INPUTBUTTONS_H_
#define INPUTBUTTONS_H_

#include "../saf2core.h"

//------------- setup
/*
 * przyklad dla portu PORTD bit 0
 * input_add(_D, 0);
 */
#define INPUT_SIZE 16

//-------------------

uint8_t input_add(uint8_t sfr, uint8_t bit);
void _input_setup();
void input_onEvent(saf_Event event);

#endif /* INPUTBUTTONS_H_ */
