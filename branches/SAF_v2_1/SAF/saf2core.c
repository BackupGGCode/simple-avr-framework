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
	saf_Event event;
	event.code = EVENT_INIT;
	saf_eventBusSend(event);
	//timer2
	/*
	 * set up cpu clock divider. the TIMER0 overflow ISR toggles the
	 * output port after enough interrupts have happened.
	 * 16MHz (FCPU) / 1024 (CS0 = 5) -> 15625 incr/sec
	 * 15625 / 256 (number of values in TCNT0) -> 61 overflows/sec
	 */
#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega168A__)
	TCCR2B |= _BV(CS21) | _BV(CS20);

	/* Enable Timer Overflow Interrupts */
	TIMSK2 |= _BV(TOIE2);

#elif defined (__AVR_ATmega8__)

	TCCR2 |= _BV(CS21) | _BV(CS20);

	/* Enable Timer Overflow Interrupts */
	TIMSK |= _BV(TOIE2);
#endif
	/* other set up */
	TCNT2 = 0;
}


void saf_addEventHandler(void (*callback)(saf_Event)){
	 saf.list.listenerList[ saf.list.listenerCount++ ] = callback;
}

void saf_process() {
	while(_saf_ringbufferAvailable()) {
			saf_Event event = _saf_ringbufferGet();
			for (uint8_t i=0; i<saf.list.listenerCount; i++) {
				saf.list.listenerList[i](event);
			}
		}
	sleep_mode();
}

SIGNAL(TIMER2_OVF_vect) {
	saf_eventBusSend_(EVENT_SAFTICK, saf.timeCounter++);
}

void saf_eventBusSend(saf_Event event) {
	_saf_ringbufferAdd(event);
}

#if SAF_CONFIG_EXTRA_EVENT_VALUE_ENABLE == 0
	void saf_eventBusSend_(uint8_t code, SAF_EVENT_VALUE_T value) {
		saf_Event event;
		event.code=code;
		event.value=value;
		_saf_ringbufferAdd(event);
	}
#endif

//ringbuffer
void _saf_ringbufferAdd(saf_Event c){
	uint8_t i = (saf.buffer.head + 1) % SAF_EVENT_BUFFOR_SIZE;

	saf.buffer.buffer[saf.buffer.head] = c;
	saf.buffer.head = i;
	if (i == saf.buffer.tail) {
	  saf.buffer.tail = (saf.buffer.tail + 1) % SAF_EVENT_BUFFOR_SIZE;
	}
}
saf_Event _saf_ringbufferGet(){
	 // if the head isn't ahead of the tail, we don't have any characters
	  if (saf.buffer.head == saf.buffer.tail) {
		saf_Event c;
		c.code = EVENT_NULL;
		c.value=0;
	    return c;
	  } else {
		  saf_Event c = saf.buffer.buffer[saf.buffer.tail];
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