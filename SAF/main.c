/*
 * main.c
 *
 *  Created on: 21-11-2012
 *      Author: radomir mazon
 */

#include "saf2core.h"
#include "Test.h"
#include "adds/rscom.h"
#include <util/delay.h>

int main()
{
	saf_init();


	//bindowanie
	saf_addEventHandler(onTestEvent);
	saf_addEventHandler(rs_onEvent);


	/**
	 * Main. Gowna petla przekazuje steroanie do SAF.
	 * Nie nalezy jej zmieniac...
	 */
	sei();
	while(1) {
		//Clock::get()->process();
		//EventBus::get()->process();
		_delay_ms(100);
		_saf_clockProc();
		saf_process();
	}
}


