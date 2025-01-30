Repozytorium zawiera kod obsługujacy "nieinteligentny" panel z wyświetlaczami i enkoderami,
1. Odbiór danych - w pliku cpp_main.cpp znajduje się funkcja "messageReceived", która jest wywoływana przez przerwanie z pliku main.c. To w nim odbywa się parsowanie komunikatu, który jest następujący:
Vnxxxx -> odebrano wartość xxxx dla wyświetlacza n
Cnxxxx -> odebrano napis xxxx dla wyswietlacza n
2. Wysyłka danych ma miejsce w pliku cpp_main.cpp w funkcji "sendEncoderData", format nastęoujący:
En+v -> wysylka zmiany pozycji enkodera n, +/- zwiękzenie lub zmniejszenie, v - prędkość obrotu
