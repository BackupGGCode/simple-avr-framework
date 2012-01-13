/*
 * RSController.h
 *
 *  Created on: 09-10-2011
 *      Author: radomir.mazon
 */

#ifndef RSController_H_
#define RSController_H_

#define SAF_ENABLE 1		//RSController może działać z SAF lub jako oddzielna klasa
#define BAUDRATE 19200
#define UBRRVAL ((F_CPU/(BAUDRATE*16UL))-1)
#define EOL	((char)13)

#include <avr/io.h>
#if defined(SAF_ENABLE)
#include "../safcore.h"
	#if defined(EVENT_RS_SEND)
	#else
	#error "RSControler wspiera teraz SAF. Dodaj do pliku 'event.h' brakujace stale: EVENT_RS_SEND oraz EVENT_RS_RECEIVE";
	#endif
#endif


#define RS_BUFFOR_SIZE 64

//@{
#if defined(__AVR_ATmega168__)

#define _UBRRH UBRR0H
#define _UBRRL UBRR0L

#define _UDR UDR0

#define _UCSRA UCSR0A
#define _RXC RXC0
#define _TXC TXC0
#define _UDRE UDRE0
#define _FE FE0
#define _DOR DOR0
#define _PE PE0
#define _U2X U2X0
#define _MPCM MPCM0

#define _UCSRB UCSR0B
#define _RXCIE RXCIE0
#define _TXCIE TXCIE0
#define _UDRIE UDRIE0
#define _RXEN RXEN0
#define _TXEN TXEN0
#define _UCSZ2 UCSZ02
#define _RXB8 RXB80
#define _TXB8 TXB80

#define _UCSRC UCSR0C
#define _URSEL UMSEL01
#define _UMSEL UMSEL00
#define _UPM1 UPM01
#define _UPM0 UPM00
#define _USBS USBS0
#define _UCSZ1 UCSZ01
#define _UCSZ0 UCSZ00
#define _UCPOL UCPOL0

#elif defined(__AVR_ATmega8__)

#define _UBRRH UBRRH
#define _UBRRL UBRRL

#define _UDR UDR

#define _UCSRA UCSRA
#define _RXC RXC
#define _TXC TXC
#define _UDRE UDRE
#define _FE FE
#define _DOR DOR
#define _PE PE
#define _U2X U2X
#define _MPCM MPCM

#define _UCSRB UCSRB
#define _RXCIE RXCIE
#define _TXCIE TXCIE
#define _UDRIE UDRIE
#define _RXEN RXEN
#define _TXEN TXEN
#define _UCSZ2 UCSZ2
#define _RXB8 RXB8
#define _TXB8 TXB8

#define _UCSRC UCSRC
#define _URSEL URSEL
#define _UMSEL UMSEL
#define _UPM1 UPM1
#define _UPM0 UPM0
#define _USBS USBS
#define _UCSZ1 UCSZ1
#define _UCSZ0 UCSZ0
#define _UCPOL UCPOL

#else
//
// unsupported type
//
#error "Processor type not supported !"
#endif
//@}

class RSRingBuffer {
public:
	RSRingBuffer(){head = tail;}
	void add(char c);
	char get();
	uint8_t available();
	void flush();
private:
	char buffer[RS_BUFFOR_SIZE];
	uint8_t head;
	uint8_t tail;
};

#if defined(SAF_ENABLE)
	class RSController : public EventReceiver {
#else
	class RSController  {
#endif
public:
	RSController();
	void sendLine(char* buffer);
	void send(char string);
	void send(char string, char isSafe);
	static void onTx();
	static void onRx();
#if defined(SAF_ENABLE)
	void onEvent(uint8_t code, int value);
#endif

private:
	static RSRingBuffer sendRingBuffer;
};

#endif /* RSController_H_ */
