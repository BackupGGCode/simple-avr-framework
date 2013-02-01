/*
 * keyboard.h
 *	  Y0Y1Y2Y3
 *	X0-+-+-+-+
 *	X1-+-+-+-+
 *	X2-+-+-+-+
 *	X3-+-+-+-+
 *
 *
 *  Created on: 19-01-2013
 *      Author: Radomir Mazon
 *      Modul skanowania klawiatury matrycowej.
 *      Inicjacja odbywa sie poprzez dodanie linii X i linii Y
 *      za pomoca metod kb_addKeyX() i kb_addKeyY()
 *
 *      Modul zwraca nastepujace eventy: (value zawiera kod przycisku)
 *      EVENT_KB_PRESS_START - Wysylany jest po nacisnieciu przycisku
 *      EVENT_KB_PRESS_LONG  - Wysylany jest jesli przycisk jest przytrzymany i uplunal timeout
 *      EVENT_KB_PRESS       - Wysylany jest po zwolnieniu przycisku
 *
 *		Modul przyjmuje nastepujace eventy: (value nie istotne)
 *		EVENT_KB_ON			- Klawiatura zaczyna byc skanowana
 *		EVENT_KB_OFF		- Klawiatura nie jest juz skanowana i modul jest wylaczony
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include "../saf2core.h"
#include "input.h"


#define KB_X_SIZE 4
#define KB_Y_SIZE 3
#define KB_LONG_PRESS	0xEF

void kb_onEvent(saf_Event e);

void kb_addKeyX(uint8_t sfr, uint8_t bit);
void kb_addKeyY(uint8_t sfr, uint8_t bit);

#if SAF_TICK_ENABLED == 0
#error "SAF tick is required !"
#endif
#endif /* KEYBOARD_H_ */
