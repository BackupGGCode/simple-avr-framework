/*
 * RSController.cpp
 *
 *  Created on: 09-10-2011
 *      Author: radomir.mazon
 */

#include "RSController.h"
#include "avr/interrupt.h"

RSRingBuffer RSController::sendRingBuffer;

RSController::RSController() {
	//Set baud rate
	unsigned int ubrr = UBRRVAL;
	_UBRRH = (unsigned char)(ubrr>>8);
	_UBRRL = (unsigned char)ubrr;

	//Enable Transmitter and Receiver and Interrupt on receive complete
	_UCSRB=(1<<_RXCIE)|(1<<_TXCIE)|(0<<_UDRIE)|(1<<_RXEN)|(1<<_TXEN)|(0<<_UCSZ2)|(0<<_RXB8)|(0<<_TXB8);

	//Set data frame format: asynchronous mode,no parity, 1 stop bit, 8 bit size
	//Parzystosc UPM01 =1 (wylaczona 0) UPM00: even = 0 odd = 1
	/*ATMEGA8
	 * 7 - URSEL - Register select
	 * 6 - UMSEL - USART Model select: 0-Asynch, 1-Synch
	 * 5 - UPM1 - 00-disable, 10-Even parity, 11-Odd parity
	 * 4 - UPM0
	 * 3 - USBS - Stop bit 0-1bit, 1-2bit
	 * 2 - UCSZ2 - 000 : 5-bit
	 * 1 - UZSZ1   001 : 6-bit
	 * 0 - UZSZ0   010 : 7-bit
	 * 		       011 : 8-bit
	 * 		       111 : 9-bit
	 */
	_UCSRC=(1<<_URSEL)|(0<<_UMSEL)|(0<<_UPM1)|(0<<_UPM0)|(0<<_USBS)|(1<<_UCSZ1)|(1<<_UCSZ0)|(1<<_UCPOL);


}

#if defined(SAF_ENABLE)
void RSController::onEvent(uint8_t code, int value) {
	if (code == EVENT_RS_SEND) {
		send(value);
	}
}
#endif

void RSController::sendLine(char* buffer) {
	while(*buffer != 0) {
		send(*buffer);
		buffer++;
	}
	send(EOL);
}

void RSController::send(char c) {
	send(c, 1);
}
void RSController::send(char c, char isSafe) {
	if (isSafe) {
		while(sendRingBuffer.available()) {
			onTx();
		}
	}
	sendRingBuffer.add(c);
	onTx();
}

void RSController::onTx() {
	if (bit_is_clear(_UCSRA, _UDRE)) {
		return;
	}
	if (!sendRingBuffer.available()) {
		return;
	}
	_UDR = sendRingBuffer.get();
}

/**
 * Obsluga przerwania USART_RX
 */
void RSController::onRx() {
	if (bit_is_clear(_UCSRA, _RXC)) {
		return;
	}

	#if defined(SAF_ENABLE)
	EventBus::get()->send(EVENT_RS_RECEIVE, _UDR);
	#else
	//TODO...
	#endif

}


void RSRingBuffer::add(char c)
{
  uint8_t i = (head + 1) % RS_BUFFOR_SIZE;

  buffer[head] = c;
  head = i;
  if (i == tail) {
	  tail = (tail + 1) % RS_BUFFOR_SIZE;
  }
}

inline uint8_t RSRingBuffer::available()
{
  return (RS_BUFFOR_SIZE + head - tail) % RS_BUFFOR_SIZE;
}

char RSRingBuffer::get()
{
  // if the head isn't ahead of the tail, we don't have any characters
  if (head == tail) {
    return 0;
  } else {
	  char c = buffer[tail];
    tail = (tail + 1) % RS_BUFFOR_SIZE;
    return c;
  }
}

//@{
#if defined(__AVR_ATmega168__)
	SIGNAL(USART_TX_vect) {
		RSController::onTx();
	}

	SIGNAL(USART_RX_vect) {
		RSController::onRx();
	}
#elif defined(__AVR_ATmega8__)
	SIGNAL(USART_TXC_vect) {
		RSController::onTx();
	}

	SIGNAL(USART_RXC_vect) {
		RSController::onRx();
	}
#else
//
// unsupported type
//
#error "Processor type not supported !"
#endif
//@}
