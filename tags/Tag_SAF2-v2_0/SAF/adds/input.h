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
#define INPUT_SIZE 4
#define _DDR 1
#define _PORT 2

//-------------------
#if defined(__AVR_ATmega168__)
#define _B    0x03
#define _C    0x06
#define _D    0x09
#elif defined(__AVR_ATmega8__)
#define _B    0x16
#define _C    0x13
#define _D    0x10
#else
#error "Processor type not supported !"
#endif

#if defined(EVENT_RS_SEND)
#else
#error "RSControler wspiera teraz SAF. Dodaj do pliku 'event.h' brakujace stale: EVENT_RS_SEND oraz EVENT_RS_RECEIVE";
#endif

#if defined(EVENT_RS_RECEIVE)
#else
#error "RSControler wspiera teraz SAF. Dodaj do pliku 'event.h' brakujace stale: EVENT_RS_SEND oraz EVENT_RS_RECEIVE";
#endif


//TODO: upakowac w jedna tablice...
//dostep to tego realizowany jest przez dedykowane metody
//trzeba tylko zmienic ich tresc...
//wtedy nie bedzie potrzebne input_type
typedef struct {
	uint8_t state[INPUT_SIZE];
	uint8_t bit[INPUT_SIZE];
	uint8_t sfr[INPUT_SIZE];
} _input_type;

void input_add(uint8_t sfr, uint8_t bit);
void _input_setup();
void input_onEvent(uint8_t code, int value);

#endif /* INPUTBUTTONS_H_ */
