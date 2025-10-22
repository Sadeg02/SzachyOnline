# Szachy Online

Projekt powstał jako zadanie na przedmiot Sieci Komputerowe 2. To prosta konsolowa implementacja gry w szachy działająca w trybie klient-serwer. Serwer obsługuje kilka stołów (maks. 3) i paruje dwóch graczy przy każdym stole. Logika ruchów i walidacja wykonywana jest po stronie serwera.

## Zawartość repozytorium

- `serwer.c` — serwer gry (nasłuchiwanie na porcie 1100, obsługa wielu stołów, wątki dla klientów)
- `klient.c` — prosty klient konsolowy łączący się z serwerem i umożliwiający wpisywanie ruchów
- `szachownica.c` / `szachownica.h` — struktura planszy, inicjalizacja i pomocnicze funkcje
- `komendy.c` / `komendy.h` — reguły ruchów na planszy (walidacja ruchów)
- `test.c` — prosty program do testowania logiki planszy lokalnie
- `CMakeLists.txt` — plik CMake (uwaga w sekcji "Kompilacja" niżej)

## Szybkie info o działaniu

- Serwer nasłuchuje na porcie 1100 (stała `PORT` w `serwer.c`).
- Pierwszy gracz przypisany do stołu otrzymuje kolor 'B' (białe) i używa małych liter dla figur.
- Drugi gracz otrzymuje kolor 'C' (czarne) i używa wielkich liter dla figur.
- Maksymalna liczba stołów (równoległych rozgrywek) to 3 (stała `MAXSTOLY` w `serwer.c`).

## Zasady gry (implementacja)

- Plansza: indeksy używane są od 1 do 8 (w `szachownica.c` na brzegach są cyfry pomocnicze).
- Format ruchu: `Figura StareXY NoweXY` (np. `p 21 31`).
	- Pierwsze pole to litera figury (mała dla białych, wielka dla czarnych). Możliwe litery:
		- `p` / `P` – pion (pawn)
		- `w` / `W` – wieża (rook)
		- `s` / `S` – skoczek (knight)
		- `g` / `G` – goniec (bishop)
		- `k` / `K` – król (king)
		- `h` / `H` – hetman (queen)
	- `StareXY` i `NoweXY` to dwucyfrowe wartości: pierwsza cyfra to wiersz (x), druga to kolumna (y). Zakres 1..8.
	- Przykład: `p 21 31` — pionek z wiersza 2, kolumny 1 na wiersz 3, kolumnę 1.

Ograniczenia i uproszczenia implementacji:
- Brak obsługi dwóch pól ruchem początkowym pionka (tylko ruch o jedno pole do przodu).
- Brak promocji pionka.
- Brak roszady (castling) i en-passant.
- Brak pełnej detekcji szacha/mata — gra kończy się wykryciem braku jednego z królów (`czyKoniec` sprawdza obecność 'k' i 'K').

Serwer przyjmuje ruchy i zwraca status:
- 2 — poprawny ruch (serwer zaktualizował planszę i zmienia turę)
- 1 — ruch niepoprawny (klient otrzyma komunikat i musi wysłać ruch ponownie)

## Wymagania

- System Unix-like (Linux/macOS) z obsługą POSIX sockets i pthreads.
- Kompilator C (gcc lub clang).
- (Opcjonalnie) CMake jeżeli chcesz korzystać z konfiguracji CMake (uwaga: domyślny `CMakeLists.txt` w repo może zawierać wszystkie pliki źródłowe razem — zobacz sekcję "Kompilacja" niżej).

## Kompilacja i uruchomienie

Uwaga: w repo znajduje się prosty `CMakeLists.txt`, który aktualnie dodaje wszystkie pliki źródłowe do jednego celu — to spowoduje konflikt, ponieważ w projekcie są 3 pliki z funkcją `main` (`serwer.c`, `klient.c`, `test.c`). Najszybszym i najpewniejszym sposobem jest skompilowanie oddzielnych binarek ręcznie przy użyciu `gcc`.

Przykładowe polecenia (w katalogu projektu):

Kompilacja serwera:

```bash
gcc -o serwer serwer.c komendy.c szachownica.c -pthread
```

Kompilacja klienta (klient zawiera bezpośrednie `#include "szachownica.c"`, więc zwykłe skompilowanie klienta wystarczy):

```bash
gcc -o klient klient.c
```

Kompilacja programu testowego (lokalna symulacja logiki planszy):

```bash
gcc -o test test.c komendy.c szachownica.c
```

Uruchomienie:

1. W jednym terminalu uruchom serwer:

```bash
./serwer
```

2. W dwóch osobnych terminalach uruchom klienta (po jednym na każdego gracza):

```bash
./klient
```

Klient spróbuje połączyć się na `127.0.0.1:1100` i jeśli znajdzie wolny stół zostanie sparowany z drugim graczem.

Jeśli wolisz użyć CMake, możesz zmodyfikować `CMakeLists.txt` aby tworzyć oddzielne cele: `serwer`, `klient` i `test`. Przykładowy fragment CMake (do wklejenia/zmiany):

```cmake
add_executable(serwer serwer.c komendy.c szachownica.c)
target_link_libraries(serwer pthread)

add_executable(klient klient.c)

add_executable(test test.c komendy.c szachownica.c)
```

Po takiej zmianie możesz użyć standardowego przepływu:

```bash
mkdir -p build
cd build
cmake ..
make
```

## Przykładowa rozgrywka

- Po uruchomieniu dwóch klientów serwer przydzieli pierwszyemu graczowi kolor `B` (biały, małe litery), drugiemu kolor `C` (czarny, wielkie litery).
- Plansza zostanie przesłana do klienta, a klient poprosi o ruch gdy nadejdzie jego tura.
- Wpisz ruch w formacie: `p 21 31` i naciśnij Enter.

Jeśli ruch jest niepoprawny klient otrzyma komunikat i będzie mógł wysłać ruch ponownie.

## Typowe problemy i debugowanie

- "Address already in use" przy uruchamianiu serwera — upewnij się, że nie działa już inny proces nasłuchujący na porcie 1100 lub zmień port w `serwer.c`.
- Błędy linkera związane z wieloma definicjami `main` — nie kompiluj wszystkich plików z `main` razem. Użyj rekomendowanych poleceń `gcc` albo zmodyfikuj `CMakeLists.txt` aby tworzyć osobne cele.
- Problemy z połączeniem klienta — sprawdź, czy serwer działa na `127.0.0.1:1100` oraz zaporę sieciową.

## Dalsze pomysły / możliwe usprawnienia

- Dodać obsługę promocji pionka, ruchu o dwa pola, en-passant i roszady.
- Rozszerzyć walidację o sprawdzanie szacha / mata zamiast jedynie wykrywania zniknięcia króla.
- Poprawić protokół sieciowy, dodać wiadomości tekstowe z informacją o stanie (chat, podgląd historii ruchów itp.).
- Dodać skrypt uruchamiający / systemd service aby uruchamiać serwer łatwiej.

