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
#include "adds/output.h"

int main()
{
	//init...
	saf_init();
	input_add(_D, 2);
	input_add(_D, 3);
	input_add(_D, 4);
	input_add(_D, 5);
	output_add(_B, 5, 0);
	output_add(_B, 0, 1);

	//bindowanie...
	saf_addEventHandler(onTestEvent);
	saf_addEventHandler(rs_onEvent);
	saf_addEventHandler(input_onEvent);
	saf_addEventHandler(output_onEvent);

	//test
	saf_eventBusSend_(EVENT_RS_SEND, 'R');
	saf_eventBusSend_(EVENT_RS_SEND, 'a');
	saf_eventBusSend_(EVENT_RS_SEND, 'd');
	saf_eventBusSend_(EVENT_RS_SEND, 'e');
	saf_eventBusSend_(EVENT_RS_SEND, 'k');

	/**
	 * Main. Gowna petla przekazuje steroanie do SAF.
	 * Nie nalezy jej zmieniac...
	 */
	while(1) {
		saf_process();
	}
}


