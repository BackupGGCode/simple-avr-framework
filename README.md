# simple-avr-framework
## Nowa wersja 2.0 już dostepna!!! ##
Co nowego:

Problem z obecną wersją (1.3) SAF polega na tym, że powstało w C++ i jest obiektowe. Obiekty są dobre, ale niestety nie dla tego rodzaju procesorów. Pomysł okazał się zbyt kosztowny pod kontem pojemności kodu. Przy 8kB ROMu w procesorach ATMEGA8 niewiele zostaje miejsca. 90% pamięci zajmuje SAF. Powstał pomysł aby przepisać SAF w wersji dla programistów C, oraz zminimalizować przestrzeń jaką SAF potrzebuje.

Co więcej? SAF ma już wszystko co trzeba... oprócz oszczędzania energii. Obecna wersja SAF większość czasu czeka w nieskończonej pustej pętli na jakieś zdarzenie. Nowa wersja będzie hibernowała procesor i dopiero gdy takie zdarzenie nastąpi będzie go wybudzała. To powinno znacząco obniżyć pobór mocy i tym samym pozwolić na wykorzystanie zasilania bateryjnego.

PS. Zachęcam do przyłączenia się do projektu... Czekam na Wasz kontakt.

### wersja 2.1 ###

  * poprawki drobnych błędów
  * globalna inicjalizacjia za pośrednictwem EVENT\_INIT
  * możliwość zmiany przekazywanej struktury w event
  * optymalizacja modułu /adds/input.h
  * zmiana w main() - nie trzeba teraz wstawiać sei() oraz sleep\_mode()

### wersja 2.2 ###
  * dodano do ADDS:
    * obsługa alfanumerycznego LCD
    * obsługa termometru cyfrowego dallas DS1820
  * timer - można zlecić wysłanie dowolnego EVENT z opóźnieniem (domyślnie timer jest  wyłaczony w setup (plik h))
  * możliwe jest wyłączenie zdarzenia EVENT\_SAFTICK w setup
  * dodano EVENT\_START\_APP, który wysyłany jest po wykonaniu obsługi EVENT\_INIT

### wersja 2.3 ###
  * Obsługa pinów wyjściowych - output.c
    * init - podobnie jak w przypadku input.c
    * obsługa: UP, DOWN, BLINK, BLINK2 - w przypadku BLINK można ustawić długość trwania impulsu
  * Zmieniono nazwę EVENT\_BUTTON\_UP/DOWN na EVENT\_IN\_UP/DOWN

### wersja 2.4 ###
  * Poprawa błędów w module output.
  * Dodane do opcji BLINK dla wyjść procesora, czasu i migania w pętli.
  * Nowy moduł obsługi klawiatury matrycowej o dowolnej wielkości. Wykrywanie długiego przytrzymania.
  * Nowy moduł PIN. Używa modułu klawiatury i pozwala na weryfikację PIN (dowolna długość), oraz mechanizm zmiany pinu.


## WSPARCIE ##
obecnie wspierane są dwa typy kontrolerów: ATMEGA168 i ATMEGA8

## SAF ##
to połączenie kilku prostych wzorców projektowych, które łącznie tworzą pewną filozofię tworzenia oprogramowania dla mikrokontrolerów. Teraz możesz szybciej i łatwiej tworzyć oprogramowanie. Kod jest skalowalny, modułowy i możesz go wykorzystywać w dowolnym swoim projekcie, bez żadnych zmian. Dodatkowo dostajesz możliwość testowania fragmentów Twojego kodu.

## Zalety: ##
1. Aplikacja składa się z modułów - dzięki zastosowaniu EventBus i metodzie asynchronicznego rozgłaszania zdarzeń, wyeliminowano powiązania pomiędzy klasami.

2. Dobra praktyka - ponieważ brak jest powiązań pomiędzy klasami, możliwe było oddzielenie logiki działania od interfejsu IO. Językiem potocznym: każda klasa zajmuje się tym do czego była stworzona.

3. Prawie jak wątki - Klasy jeśli wykonują coś cyklicznie, dzięki wykorzystaniu timera, są cyklicznie proszone o wykonanie swoich operacji. Okres ich działania jest konfigurowalny.


Więcej na wiki...


Projekt wykonany z użyciem SAF 1.3.

<a href='http://www.youtube.com/watch?feature=player_embedded&v=8SAMYijMvW0' target='_blank'><img src='http://img.youtube.com/vi/8SAMYijMvW0/0.jpg' width='425' height=344 /></a>

Jak napisać prosty program w SAF 2.0

<a href='http://www.youtube.com/watch?feature=player_embedded&v=PsHCyPs0e8s' target='_blank'><img src='http://img.youtube.com/vi/PsHCyPs0e8s/0.jpg' width='425' height=344 /></a>


Projekty powiązane:

https://github.com/BackupGGCode/jatmega/

https://github.com/BackupGGCode/avr-wav-interface/
