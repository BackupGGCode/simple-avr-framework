/*
 * saf2core.c
 *
 *  Created on: 21-11-2012
 *      Author: rma
 */

#include "saf2core.h"


SAF saf;

void saf_init() {
	//constructor od SAF
	saf.list.listenerCount = 0;
	_saf_ringbufferFlush();
}

#if defined(SAF_SUPPORT_REMOVE_EVENT_LISTENER) && (SAF_SUPPORT_REMOVE_EVENT_LISTENER == 1)
void saf_removeEventHandler(void (*callback)(uint8_t, int)){
	uint8_t i;
	  for( i = 0; i < saf.list.listenerCount; ++i )
	  {
	    if( saf.list.listenerList[i] == callback )
	    {
	    	saf.list.listenerCount--;
	    	saf.list.listenerList[i] =
	    			saf.list.listenerList[saf.list.listenerCount];
	    }
	  }
}
#endif

void saf_addEventHandler(void (*callback)(uint8_t, int)){
	#if defined(SAF_SUPPORT_REMOVE_EVENT_LISTENER) && (SAF_SUPPORT_REMOVE_EVENT_LISTENER == 1)
	 saf_removeEventHandler(callback);
	#endif
	 saf.list.listenerList[ saf.list.listenerCount++ ] = callback;
}

/*
 * Glowna petla SAF. Sprawdzane jest czy sa jakies eventy do obslugi.
 * TODO: dodac uspienie procesowa w celu oszczedzania energii
 */
void saf_process() {
	while(_saf_ringbufferAvailable()) {
			_saf_Event event = _saf_ringbufferGet();
			for (uint8_t i=0; i<saf.list.listenerCount; i++) {
				saf.list.listenerList[i](event.code, event.value);
			}
		}
}


//To TRZEBA przeniesc bezposredni do proceduty obsugi CLOCK !!!
void _saf_clockProc() {
	saf_eventBusSend(EVENT_SAFTICK, 0);
}

void saf_eventBusSend(uint8_t code, int value) {
	_saf_Event event;
	event.code = code;
	event.value = value;
	_saf_ringbufferAdd(event);
}

//ringbuffer
void _saf_ringbufferAdd(_saf_Event c){
	uint8_t i = (saf.buffer.head + 1) % SAF_EVENT_BUFFOR_SIZE;

	saf.buffer.buffer[saf.buffer.head] = c;
	saf.buffer.head = i;
	if (i == saf.buffer.tail) {
	  saf.buffer.tail = (saf.buffer.tail + 1) % SAF_EVENT_BUFFOR_SIZE;
	}
}
_saf_Event _saf_ringbufferGet(){
	 // if the head isn't ahead of the tail, we don't have any characters
	  if (saf.buffer.head == saf.buffer.tail) {
		_saf_Event c;
		c.code = EVENT_NULL;
	    return c;
	  } else {
		  _saf_Event c = saf.buffer.buffer[saf.buffer.tail];
		  saf.buffer.tail = (saf.buffer.tail + 1) % SAF_EVENT_BUFFOR_SIZE;
		  return c;
	  }
}
uint8_t 	_saf_ringbufferAvailable() {
	return (SAF_EVENT_BUFFOR_SIZE + saf.buffer.head - saf.buffer.tail) % SAF_EVENT_BUFFOR_SIZE;
}
void _saf_ringbufferFlush(){
	saf.buffer.head = saf.buffer.tail;
}
