/*
 * saf2core.h
 *
 *  Created on: 21-11-2012
 *      Author: radomir mazon
 */

#ifndef SAF2CORE_H_
#define SAF2CORE_H_
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "Event.h"

//-----------setup
#define SAF_LISTENER_SIZE 					8
#define SAF_SUPPORT_REMOVE_EVENT_LISTENER 	1
#define SAF_EVENT_BUFFOR_SIZE				16


#if defined(EVENT_SAFTICK)
#else
#error "event EVENT_SAFTICK is required (in event.h)";
#endif

#if defined(EVENT_NULL)
#else
#error "event EVENT_NULL is required (in event.h)";
#endif

#define timePrescaler(i) value%i==0

//----------------
typedef struct
{
	uint8_t code;
	int value;
} _saf_Event;

typedef struct
{
  int listenerCount;
  void (*listenerList[SAF_LISTENER_SIZE])(uint8_t, int);
} _saf_EventListener;

typedef struct
{
	_saf_Event buffer[SAF_EVENT_BUFFOR_SIZE];
	uint8_t head;
	uint8_t tail;
} _saf_RingBuffer;

typedef struct
{
	_saf_EventListener list;
	_saf_RingBuffer buffer;
	int timeCounter;
} SAF;

void saf_init();

#if defined(SAF_SUPPORT_REMOVE_EVENT_LISTENER) && (SAF_SUPPORT_REMOVE_EVENT_LISTENER == 1)
void saf_removeEventHandler(void (*callback)(uint8_t, int));
#endif


void saf_addEventHandler(void (*callback)(uint8_t, int));

void saf_process();

void saf_eventBusSend(uint8_t code, int value);

//ring buffer
void 		_saf_ringbufferAdd(_saf_Event c);
_saf_Event 	_saf_ringbufferGet();
uint8_t 	_saf_ringbufferAvailable();
void _saf_ringbufferFlush();

#endif /* SAF2CORE_H_ */
