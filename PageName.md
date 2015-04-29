# Zadanie #

Do dyspozycji mamy jeden przycisk i jedną diodę LED (załączony schemat).

1. Po kliknięciu przycisku, LED powinna miganiem nadać sygnał SOS zakodowany alfabetem morsa.
2. Jeśli klikniemy podczas nadawania sygnału, nadawanie sygnału SOS powinno zostać przerwane.


# Schemat #
![http://simple-avr-framework.googlecode.com/svn/wiki/schemat1/test-circuit-led_th.gif](http://simple-avr-framework.googlecode.com/svn/wiki/schemat1/test-circuit-led_th.gif)

# Analiza problemu #
Zagadnienie możemy podzielić na dwie części. Potrzebujemy pewnej logiki, która wie jak posługiwać się alfabetem morsa, oraz kodu, który wie jak obsługiwać LED i wie jak zebrać informację o przycisku. Dla prostych przycisków jest już gotowa implementacja input.c w katalogi ADDS.
Zatem otwieramy nowy projekt, i dodajemy plik saf2core h i c, oraz main.c.

Zanim zastanowimy się jak ma działać program, możemy intuicyjnie dodać trzy moduły (pliki c i h), które będą nam potrzebne.

  * soslogic.c i h- w której dodajemy metodę obsługi eventu sl\_onEvent(saf\_Event event).


  * ledoutput.c i h - ten moduł jedynie zapala i gasi LED. Będziemy potrzebować zdarzeń  więc dodajemy metodę lo\_onEvent(saf\_Event event)


Podzieliliśmy już naszą aplikację na moduły. Każdy z nich jest odpowiedzialny za inne zadanie. Wszystkie mają wspólny interfejs komunikacji, polegający na rozsyłaniu zdarzeń. Co nam to daje?

### Zalety ###
Możemy w dowolnym momencie zmienić sposób wyzwalania logiki, lub sposób prezentowania jej wyników, dodając lub podmieniając klasy Input i/lub Output. Na przykład możemy napisać moduł, która generuje falę prostokątną o częstotliwości słyszalnej i na porcie, który obsługuje podłączyć głośnik. Ponieważ metoda interfajsu SAF saf\_eventBusSend() jest uniwersalna nie musimy robić nic więcej. Jedynie wskazać w main że metoda logiki będzie otrzymywała zdarzenia, oraz zaimplementować ich obsługę

Dzięki uniwersalnej metodzie komunikacji klas za pośrednictwem EventBus i zdarzeń, możemy testować sam moduł ledoutput, lub sam moduł input wysyłając odpowiednie komunikaty, bez konieczności wykonywania (lub nawet tworzenia) logiki ich działania.

### Wady ###
W tym miejscu pisałem o tym, że obiektowe podejście do tematu jest obszerne i niewiele miejsca zostaje dla programisty. Od SAF 2.0 framework przepisany jest na C i jak na razie wad nie widzę :)



Oto jak powinna wyglądać nasza metoda main()

```

#inlude "saf2code.h"
#inlude "soslogic.h"
#inlude "ledoutput.h"

int main() {
    saf_init();
    input_add(_C, 4);
    saf_addEventHandler(sl_onEvent);
    saf_addEventHandler(lo_onEvent);

    sei();
    while(1) {
        saf_proccess();
    }
```

oraz nasz plik logiki

```

    #inlude "soslogic.h"

    void sl_onEvent(saf_Event event) {
        if (event.code == EVENT_BUTTON_DOWN) {
            //reakcja na przycisk
        }
        if (event.code == EVENT_SAF_TICK && event.value%255==0) {
           //stetowanie apfabetem morsa, gasimy
           saf_eventBusSend(EVENT_LED, 0);
           //lub zapalamy
           saf_eventBusSend(EVENT_LED, 1);
        }
    }

```


oraz plik sterowania sterownia led

```
    #inlude "ledoutput.h"

    void ol_onEvent(saf_Event event) {
        if (event.code == EVENT_LED) {
            if (event.value == 0) {
                //zapal
            } else {
                //zgas
            }
        }
    }
```

Może ten przykład wydawać się śmieszny, gdzie dominuje forma nad treścią, jednak najważniejsze jest to że wejście/wyjście (przycisk i led) oraz logika działania programu są całkowicie od siebie niezależne.

To właśnie jest najważniejsze w SAF !

