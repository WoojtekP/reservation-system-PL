# System rezerwacji sprzętu projekcyjnego #

## instrukcja obsługi ##

### Kompilacja ###
Do kompilacji programu potrzebna jest instalacja biblioteki gtk w wersji 3. Można to zrobić np. poleceniem :

`sudo apt-get install libgtk-3-dev`

Następnie wywołaniem `make` kompilujemy program.

### Uruchamianie programu ###
Jako parametry wywołania programu można podać kolejno: dzień, miesiąc, rok początku semestru, dzień, miesiąc, rok końca semestru. Dane powinny być w formacie liczbowym. Parametry te można pominąć. Domyśle wartości uruchamiają program dla semestru letniego 2018 roku (12.02.2018–15.06.2018). 

**Uwaga**: Przed zmianą semestru należy pamiętać o tym, aby wyczyścić starą bazę danych (czyli usunąć plik reservations.dat).

### Rezerwacja urządzenia ###
1. Wybierz opcję "REZERWUJ" z menu głównego.
1. Wypełnij formularz podając typ, markę i model urządzenia oraz swoje imię i nazwisko (Uwaga: musisz wiedzieć jakie urządzenia znajdują się w bazie. Aby to sprawdzić przejdź do zakładki *sprawdź dostępność* lub *pokaż rezerwacje urządzenia*).
1. Wybierz datę początku i końca rezerwacji na kalendarzach oraz wpisz godziny w polach tekstowych (godzina musi mieć format hh:mm). Rezerwując urządzenie na cały semestr można pominąć pola daty i godziny.
1. Kliknij przycisk "ZATWIERDŹ".

Jeśli urządzenie zostanie poprawnie zarezerwowane program wyświetli stosowny komunikat. W przeciwnym przypadku wyświetli się komunikat o błędzie bazy danych, niepoprawnie podanych danych daty i czasu, niewypełnionych polach formularza lub o tym, że urządzenie jest zajęte. 

**Uwaga**: Program nie pozwala na utworzenie rezerwacji, której termin wychodzi poza okres bieżącego semestru.

### Sprawdzanie dostępności urządzenia ###
1. Wybierz opcję "SPRAWDŹ DOSTĘPNOŚĆ" z menu głównego.
1. Wybierz z listy odpowiednie urządzenie.
1. Podaj czas i datę, w której dostępność ma zostać sprawdzona.
1. Kliknij przycisk "SPRAWDŹ".

Program po wprowadzeniu poprawnych danych wyświetli stosowny komunikat (urządzeni jest/nie jest dostępne w podanym terminie).

### Usuwanie rezerwacji ###
1. Wybierz opcję "USUŃ REZERWACJĘ" z menu głównego.
1. Wpisz swoje imię i nazwisko.
1. Kliknij przycisk "POKAŻ REZERWACJE".
1. Wybierz z rozwijanej listy wpis, który chcesz usunąć.
1. Kliknij przycisk "USUŃ".

### Wyświetlanie listy rezerwacji dla danego urządzenia ###
1. Wybierz opcję "POKAŻ REZERWACJE URZĄDZENIA" z menu głównego.
1. Wybierz z list rozwijanych rodzaj, markę i model urządzenia.
1. Kliknij przycisk "SPRAWDŹ".

Program powinien wyświetlić pełnąlistę rezerwacji wybranego urządzenia w danym semestrze. Wprzypadku, gdy nic sięnie wyświetla oznacza to, że nie dokonano w tym semestrze żadnych rezerwacji.

### Wyświetlanie listy rezerwacji dla danej osoby ###
1. Wybierz opcję "POKAŻ REZERWACJE OSOBY" z menu głównego.
1. Wpisz imię inazwisko szukanej osoby.
1. Kliknij przycisk "SPRAWDŹ".

Pusta lista (analogicznie jak w przypadku listy rezerwacji urządzenia) oznacza, żedana osoba nie dokonała żadnej rezerwacji.

### Dodawanie i usuwanie urządzeń ###
1. Wybierz opcję "DODAJ/USUŃ URZĄDZENIE" z menu głównego.
1. Wypełnij dane urządzenia: typ, markę i model (opis jest wartością opcjonalną).
1. Kliknij odpowiedni przycisk ("DODAJ" lub "USUŃ").

Program po zatwierdzeniu danych wyświetli komunikat o powodzeniu operacji lub komunikat o przyczynie błędu.

**Uwaga**: Po usunięciu urządzenia z bazy zostają usunięte wszystkie wpisy (rezerwacje), które tego urządzenia dotyczyły, o czym program nie ostrzega!

### Wyświetlanie/generowanie listy rezerwacji dla wybranego tygodnia ###
1. Wybierz opcję "DRUKUJ ROZKŁAD TYGODNIA" z menu głównego.
1. Wybierz z listy odpowiedni tydzień semestru.
1. Kliknij przycisk "GENERUJ" lub "WYŚWIETL".
1. Po wybraniu opcji *generuj* wyznacz miejsce, gdzie ma zostać zapisany plik z rozkładem rezerwacji wybranego tygodnia.

**Uwaga**: Plik musi być zapisany w formacie *\*.tex*. Jego zawartość jest przygotowana do wygenerowania pliku *\*.pdf*. Ścieżka do pliku nie może zawierać polskich znaków! Spowoduje to błędy w zapisie. Nie zaleca się również zapisywanie pliku na pulpicie.