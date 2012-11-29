/*
 * main.c
 *
 *  Created on: 21-11-2012
 *      Author: radomir mazon
 */

#include "saf2core.h"
#include "Test.h"
#include "adds/rscom.h"
#include "adds/input.h"

int main()
{
	//init...
	saf_init();
	input_add(_D, 2);
	input_add(_D, 3);
	input_add(_D, 4);
	input_add(_D, 5);

	//bindowanie...
	saf_addEventHandler(onTestEvent);
	saf_addEventHandler(rs_onEvent);
	saf_addEventHandler(input_onEvent);


	/**
	 * Main. Gowna petla przekazuje steroanie do SAF.
	 * Nie nalezy jej zmieniac...
	 */
	sei();
	while(1) {
		saf_process();
	}
}


