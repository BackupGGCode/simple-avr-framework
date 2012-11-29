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
#define SAF_EVENT_BUFFOR_SIZE				16
#define _SAF_CONFIG_EXTRA_EVENT_VALUE_ENABLE 0
#define _SAF_CONFIG_EXTRA_EVENT_VALUE1	//mozesz dodac dodatkowe pola struktury saf_Event
#define _SAF_CONFIG_EXTRA_EVENT_VALUE2	//przyklad
#define _SAF_CONFIG_EXTRA_EVENT_VALUE3	//int extraVal;

#if defined(EVENT_SAFTICK)
#else
#error "event EVENT_SAFTICK is required (in event.h)";
#endif

#if defined(EVENT_NULL)
#else
#error "event EVENT_NULL is required (in event.h)";
#endif

//----------------
typedef struct
{
	uint8_t code;
	uint8_t value;
#if _SAF_CONFIG_EXTRA_EVENT_VALUE_ENABLE == 1
	_SAF_CONFIG_EXTRA_EVENT_VALUE1
	_SAF_CONFIG_EXTRA_EVENT_VALUE2
	_SAF_CONFIG_EXTRA_EVENT_VALUE3
#endif
} saf_Event;

typedef struct
{
  int listenerCount;
  void (*listenerList[SAF_LISTENER_SIZE])(saf_Event);
} _saf_EventListener;

typedef struct
{
	saf_Event buffer[SAF_EVENT_BUFFOR_SIZE];
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

void saf_addEventHandler(void (*callback)(saf_Event));

void saf_process();

void saf_eventBusSend(saf_Event event);

//ring buffer
void 		_saf_ringbufferAdd(saf_Event c);
saf_Event 	_saf_ringbufferGet();
uint8_t 	_saf_ringbufferAvailable();
void _saf_ringbufferFlush();

//helper
#if _SAF_CONFIG_EXTRA_EVENT_VALUE_ENABLE == 0
	void saf_eventBusSend_(uint8_t code, uint8_t value);
#endif

#endif /* SAF2CORE_H_ */
