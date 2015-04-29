Lista metod dostępnych w Simple AVR Framework

# API 2.2 #

## SAF ##
  * setup - znajduje się w pliku saf2core.h
    * SAF\_LISTENER\_SIZE
    * SAF\_EVENT\_BUFFOR\_SIZE
    * SAF\_EVENT\_VALUE\_T
    * SAF\_TIMER\_ENABLED
    * SAF\_TIMER\_SIZE
    * SAF\_CONFIG\_EXTRA\_EVENT\_VALUE\_ENABLE 0
    * SAF\_CONFIG\_EXTRA\_EVENT\_VALUE1(,2 i 3)
  * zdarzenia - znajduja sie w pliku Event.h
    * EVENT\_NULL
    * EVENT\_INIT
    * EVENT\_START\_APP
    * EVENT\_SAFTICK
    * EVENT\_ERROR
  * struktura saf\_Event
  * saf\_Event()
  * saf\_addEventHandler()
  * saf\_process()
  * saf\_eventBusSend(saf\_Event event)
  * saf\_startTimer(uint16\_t interval, uint8\_t eventCode, uint8\_t value)

## ADDS/INPUT ##

## ADDS/RSCOM ##

## ADDS/ALFANUMERIC\_LCD ##

## main() ##

Przykład podstawowej metody main()

```
#include "saf2core.h"
#include "mylogic.h"

int main()
{
	//init...
	saf_init();

	//bind...
	saf_addEventHandler(mylogic_onEvent);

	/**
	 * Main. Gowna petla przekazuje steroanie do SAF.
	 * Nie nalezy jej zmieniac...
	 */
	while(1) {
		saf_process();
	}
}

```