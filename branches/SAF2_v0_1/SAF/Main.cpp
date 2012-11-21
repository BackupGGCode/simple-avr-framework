/**
 *
 */

#include "avr/interrupt.h"
#include "safcore.h"
#include "Event.h"
#include "Test.h"
#include "adds/RSController.h"

int main()
{
	/**
	 * Tworzenie obiektow
	 */
	Test test;
	RSController rs;

	/**
	 * Konfiguracja
	 */
	Clock::get()->add(&test, 10);


	/**
	 * Test portu RS, predkosc transmisni ustawiamy w pliku RSController.h
	 */
	EventBus::get()->send(EVENT_RS_SEND, 'H');
	EventBus::get()->send(EVENT_RS_SEND, 'e');
	EventBus::get()->send(EVENT_RS_SEND, 'l');
	EventBus::get()->send(EVENT_RS_SEND, 'l');
	EventBus::get()->send(EVENT_RS_SEND, 'o');

	/**
	 * Main. Gowna petla przekazuje steroanie do SAF.
	 * Nie nalezy jej zmieniac...
	 */
	sei();
	while(1) {
		Clock::get()->process();
		EventBus::get()->process();
	}
}

