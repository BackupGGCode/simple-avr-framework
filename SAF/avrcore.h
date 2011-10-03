/*
 * core.h
 *
 *  Created on: 2010-09-29
 *      Author: rmazon
 *      Ver: 1.0
 */

#ifndef CORE_H_
#define CORE_H_

#include "Event.h"
#include <avr/io.h>



//config--------------------------------------------------------------------
#define MAX_OBJECT 10
#define BUFFOR_SIZE 128


//--------------------------------------------------------------------------


class RingBufferEventBusModel {
public:
	uint8_t code;
	uint8_t value;
	uint8_t empty;
	RingBufferEventBusModel() {
		empty = 0;
	}
};

class RingBuffer {
public:
	RingBuffer();
	void add(RingBufferEventBusModel c);
	RingBufferEventBusModel get();
	uint8_t available();
	void flush();
private:
	RingBufferEventBusModel buffer[BUFFOR_SIZE];
	uint8_t head;
	uint8_t tail;
};

class EventInterface {
public:
	virtual void onEvent(uint8_t code, uint8_t value)=0;
};

class  EventBus {
 private:
	EventInterface* tab[MAX_OBJECT];
	uint8_t index;
	RingBuffer buffer;
	static EventBus instance;
 protected:
	EventBus() {index =0;}
 public:
	static EventBus* get();
	void send(uint8_t code, uint8_t value);
	void send(uint8_t code);
	void add(EventInterface* er);
	EventBus& operator=(const EventBus&);
	void process();
};

class EventReceiver : public EventInterface {
 public:
	EventReceiver() {
		EventBus::get()->add(this);
	}
};

class ClockTick {
public:
	virtual void onTick()=0;
};

class Clock {
protected:
	Clock();
private:
	static Clock instance;
	uint16_t tabInterval[MAX_OBJECT];
	ClockTick* tab[MAX_OBJECT];
	uint8_t flags[MAX_OBJECT];
	uint8_t index;
	uint16_t counter;
public:
	static Clock* get() {return &instance;}
	void process();
	void onTimerOvf();
	void add(ClockTick* tick, uint16_t interval);
};


#endif /* CORE_H_ */
