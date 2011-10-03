/**
 *
 */

#include "avr/interrupt.h"
#include "safcore.h"
#include "Event.h"
#include "Test.h"

int main()
{
	/**
	 * Tworzenie obiektow
	 */


	Test test;

	/**
	 * Konfiguracja
	 */
	Clock::get()->add(&test, 10);

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

