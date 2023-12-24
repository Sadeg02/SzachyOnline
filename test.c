//
// Created by Mich2 on 23.12.2023.
//

#include <stdio.h>
#include "szachownica.h"
#include "komendy.h"

int main() {
    plansza szachownica;

    // Initialize the chessboard
    inicjalizujSzachownice(&szachownica);

    // Display the initial chessboard
    printf("Initial Chessboard:\n");
    show(&szachownica);

    while (1) {
        printf("Wprowadz ruch: ");
        char rozkaz[50];
        fgets(rozkaz, sizeof(rozkaz), stdin);

        if (rozkaz[0] == 'q') {
            printf("Rozlaczanie\n");
            break;
        }

        char figura;
        char starePole[3], nowePole[3];

        if (sscanf(rozkaz, "%c %2s %2s", &figura, starePole, nowePole) == 3) {
            int stareX = starePole[0] - '0';
            int stareY = starePole[1] - '0';
            int noweX = nowePole[0] - '0';
            int noweY = nowePole[1] - '0';

            ruch(&szachownica, figura, stareX, stareY, noweX, noweY);
        } else {
            printf("Bledny format ruchu. Poprawny format: [figura] [stareX][stareY] [noweX][noweY]\n");
        }

        show(&szachownica);
    }

    return 0;
}