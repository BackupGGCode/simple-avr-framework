/*
 * output.h
 *
 *  Created on: 17-12-2012
 *      Author: Radomir Mazon
 *
 *      Rejestracja pinow wyjsciowych : output_add() gdzie:
 *      stf - to zmienna okreslajaca port: _B, _C, _D
 *      bit - numer bitu w porcie
 *      isNegative - domyslna wartoscia poczatkowa portu jest 0.
 *      			jesli chcemy to zmienic isNegative powinno byc > 0.
 *
 *      Eventy wysylane przez modul:
 *      EVENT_OUT_BLINK_STOP	- value zawiera numer, ktory nalezy zakonczyc miganie (tylko jesli wczesniej
 *      						dla tego numeru miganie zostalo wlaczone przez EVENT_OUT_BLINK_LOOP
 *      Eventy przyjmowane przez modul: (value zawiera numer wyjscia)
 *      EVENT_OUT_DOWN	- Zapis do bitu portu 0 (lub 1 w przypadku isNegative>0)
 *		EVENT_OUT_UP	- Zapis do bitu portu 1 (lub 0 w przypadku isNegative>0)
 *		EVENT_OUT_BLINK	- Pojedyncze migniecie. Czas zapisany jest w masce 0b11100000.
 *		EVENT_OUT_BLINK_LOOP - Migniecie z automatycznym odnawianiem (miganie do momentu odebrania EVENT_OUT_BLINK_STOP).
 *								Czas zapisany jest w masce 0b11100000.
 *
 *		UWAGA: numer wyjscia to numer indeksu liczonego od 0 w kolejnosci rejestrowania wyjscia przez output_add()
 */
#ifndef OUTPUT_H_
#define OUTPUT_H_

#include "../saf2core.h"

//------------- setup
#define OUTPUT_SIZE					8
//DELAY max 4095
#define OUTPUT_BLINK_DELAY			255
#define OUTPUT_BLINK_SIZE			4
#define OUTPUT_BLINK_LOOP_CTRL		1
#define OUTPUT_BLINK_LOOP_CTRL_SIZE SAF_TIMER_SIZE/2

//-------------------
/**
 * byte dla EVENT_OUT_BLINK i EVENT_OUT_BLINK_LOOP
 * bit7-5 = mnożnik OUTPUT_BLINK_DELAY (wartosc od 0 do 7)
 * bit4-0 = id wyjscia dodanego output_add() (wartosc od 0 do 31)
 */
#define outputBlink(delay, item) (uint8_t)((delay<<5) | item)
void output_add(uint8_t sfr, uint8_t bit, uint8_t isNegative);
void _output_setup();
void output_onEvent(saf_Event event);

#if SAF_TIMER_ENABLED == 0
#error "SAF timer is required !"
#endif
#if SAF_TICK_ENABLED == 0
#error "SAF tick is required !"
#endif
#endif /* OUTPUT_H_ */
