/**
 * @autor Radomir Mazon
 */


#include "safcore.h"
#include <avr/interrupt.h>
//#include <stdlib.h>



/*void * operator new(size_t size)
{
  return malloc(size);
}

void operator delete(void * ptr)
{
  free(ptr);
}
*/
__extension__ typedef int __guard __attribute__((mode (__DI__)));
extern "C" int __cxa_guard_acquire(__guard *);
extern "C" void __cxa_guard_release (__guard *);
extern "C" void __cxa_guard_abort (__guard *);
int __cxa_guard_acquire(__guard *g) {return !*(char *)(g);};
void __cxa_guard_release (__guard *g) {*(char *)g = 1;};
void __cxa_guard_abort (__guard *) {};
extern "C" void __cxa_pure_virtual(void);
void __cxa_pure_virtual(void) {};


EventBus EventBus::instance;

EventBus* EventBus::get(){
	return &instance;
}

void EventBus::send(uint8_t code, uint8_t value) {
	RingBufferEventBusModel event;
	event.code = code;
	event.value = value;
	buffer.add(event);
}

void EventBus::send(uint8_t code) {
	send(code, 0);
}

void EventBus::add(EventInterface* er) {
	if (index >= MAX_OBJECT) return;
	tab[index++] = er;
}

void EventBus::process() {
	while(buffer.available()) {
		RingBufferEventBusModel event = buffer.get();
		for (uint8_t i=0; i<index; i++) {
			tab[i]->onEvent(event.code, event.value);
		}
	}
}

RingBuffer::RingBuffer() {
	flush();
}


void RingBuffer::add(RingBufferEventBusModel c)
{
  uint8_t i = (head + 1) % BUFFOR_SIZE;

  buffer[head] = c;
  head = i;
  if (i == tail) {
	  tail = (tail + 1) % BUFFOR_SIZE;
  }
}

inline uint8_t RingBuffer::available()
{
  return (BUFFOR_SIZE + head - tail) % BUFFOR_SIZE;
}

RingBufferEventBusModel RingBuffer::get()
{
  // if the head isn't ahead of the tail, we don't have any characters
  if (head == tail) {
	RingBufferEventBusModel empty;
	empty.empty = 1;
    return empty;
  } else {
    RingBufferEventBusModel c = buffer[tail];
    tail = (tail + 1) % BUFFOR_SIZE;
    return c;
  }
}

void RingBuffer::flush()
{
  // don't reverse this or there may be problems if the RX interrupt
  // occurs after reading the value of rx_buffer_head but before writing
  // the value to rx_buffer_tail; the previous value of rx_buffer_head
  // may be written to rx_buffer_tail, making it appear as if the buffer
  // don't reverse this or there may be problems if the RX interrupt
  // occurs after reading the value of rx_buffer_head but before writing
  // the value to rx_buffer_tail; the previous value of rx_buffer_head
  // may be written to rx_buffer_tail, making it appear as if the buffer
  // were full, not empty.
  head = tail;
}

Clock Clock::instance;

Clock::Clock() {
	/*
	 * set up cpu clock divider. the TIMER0 overflow ISR toggles the
	 * output port after enough interrupts have happened.
	 * 16MHz (FCPU) / 1024 (CS0 = 5) -> 15625 incr/sec
	 * 15625 / 256 (number of values in TCNT0) -> 61 overflows/sec
	 */
	TCCR0B |= _BV(CS02) | _BV(CS00);

	/* Enable Timer Overflow Interrupts */
	TIMSK0 |= _BV(TOIE0);

	/* other set up */
	TCNT0 = 0;
	index = 0;
}

void Clock::add(ClockTick* tick, uint16_t interval) {
	tab[index] = tick;
	tabInterval[index] = interval;
	flags[index] = 0;
	index++;
}

void Clock::process() {
	for (uint8_t i=0; i<MAX_OBJECT; i++) {
		if (flags[i] >0) {
			tab[i]->onTick();
			flags[i] = 0;
		}
	}
}

void Clock::onTimerOvf() {
	counter++;
	for (uint8_t i=0; i<MAX_OBJECT; i++) {
		if (counter%tabInterval[i] == 0) {
			flags[i] = 1;
		}
	}
}

SIGNAL(TIMER0_OVF_vect) {
	Clock::get()->onTimerOvf();
}
