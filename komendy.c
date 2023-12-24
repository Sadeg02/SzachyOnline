#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "szachownica.h"
#include "komendy.h"



#define ROZMIAR 9
#define TRWA 0
#define WINBIALE 1
#define WINCZARNE 2

void blad(int numer) {
    switch (numer) {
        case 1:
            printf("Nieprawidlowy ruch pionka\n");
            break;
        case 2:
            printf("Inny pionek blokuje ruch\n");
            break;
        case 4:
            printf("Pole docelowe zajete przez wlasnego pionka\n");
            break;
        case 8:
            printf("Nieprawidlowy ruch\n");
            break;
        case 9:
            printf("Nieprawidlowa figura\n");
            break;
        default:
            printf("Nieznany bład\n");
    }
}

char kolor(char kol) {
    if (kol >= 'A' && kol <= 'Z') {
        return 'B';
    } else if (kol >= 'a' && kol <= 'z') {
        return 'C';
    } else {
        return '.';
    }
}

void pion(plansza *plan, char sym, int starex, int starey, int nowex, int nowey) {
    if (starey == nowey && ((starex + 1) == nowex && kolor(sym) == 'C' || (starex - 1) == nowex && kolor(sym) == 'B') &&
        plan->szachownica[nowex][nowey] == '.') {
        plan->szachownica[nowex][nowey] = sym;
        plan->szachownica[starex][starey] = '.';
    } else if ((starey + 1 == nowey || starey - 1 == nowey) &&
               ((starex + 1 == nowex && kolor(sym) == 'C') || (starex - 1 == nowex && kolor(sym) == 'B')) &&
               kolor(sym) != kolor(plan->szachownica[nowex][nowey])) {
        plan->szachownica[nowex][nowey] = sym;
        plan->szachownica[starex][starey] = '.';
    } else {
        blad(1);
    }
}

void wieza(plansza *plan, char sym, int starex, int starey, int nowex, int nowey) {
    if (starex == nowex || starey == nowey) {
        int stepX = (nowex > starex) ? 1 : (nowex < starex) ? -1 : 0;
        int stepY = (nowey > starey) ? 1 : (nowey < starey) ? -1 : 0;

        int currentX = starex + stepX;
        int currentY = starey + stepY;

        while (currentX != nowex || currentY != nowey) {
            if (plan->szachownica[currentX][currentY] != '.') {
                blad(2);
                return;
            }

            currentX += stepX;
            currentY += stepY;
        }

        if (kolor(sym) != kolor(plan->szachownica[nowex][nowey])) {
            plan->szachownica[nowex][nowey] = sym;
            plan->szachownica[starex][starey] = '.';
        } else {
            blad(4);
        }
    } else {
        blad(8);
    }
}

void skoczek(plansza *plan, char sym, int starex, int starey, int nowex, int nowey) {
    int deltaX = abs(nowex - starex);
    int deltaY = abs(nowey - starey);

    if ((deltaX == 1 && deltaY == 2) || (deltaX == 2 && deltaY == 1)) {
        if (kolor(sym) != kolor(plan->szachownica[nowex][nowey])) {
            plan->szachownica[nowex][nowey] = sym;
            plan->szachownica[starex][starey] = '.';
        } else {
            blad(4);
        }
    } else {
        blad(8);
    }
}

void goniec(plansza *plan, char sym, int starex, int starey, int nowex, int nowey) {
    int deltaX = abs(nowex - starex);
    int deltaY = abs(nowey - starey);

    if (deltaX == deltaY) {
        int stepX = (nowex > starex) ? 1 : -1;
        int stepY = (nowey > starey) ? 1 : -1;

        int currentX = starex + stepX;
        int currentY = starey + stepY;

        while (currentX != nowex && currentY != nowey) {
            if (plan->szachownica[currentX][currentY] != '.') {
                blad(2);
                return;
            }

            currentX += stepX;
            currentY += stepY;
        }

        if (kolor(sym) != kolor(plan->szachownica[nowex][nowey])) {
            plan->szachownica[nowex][nowey] = sym;
            plan->szachownica[starex][starey] = '.';
        } else {
            blad(4);
        }
    } else {
        blad(8);
    }
}

void krol(plansza *plan, char sym, int starex, int starey, int nowex, int nowey) {
    int deltaX = abs(nowex - starex);
    int deltaY = abs(nowey - starey);

    if ((deltaX <= 1 && deltaY <= 1) && (deltaX != 0 || deltaY != 0)) {
        if (kolor(sym) != kolor(plan->szachownica[nowex][nowey])) {
            plan->szachownica[nowex][nowey] = sym;
            plan->szachownica[starex][starey] = '.';
        } else {
            blad(4);
        }
    } else {
        blad(8);
    }
}

void hetman(plansza *plan, char sym, int starex, int starey, int nowex, int nowey) {
    if (starex == nowex || starey == nowey || abs(nowex - starex) == abs(nowey - starey)) {
        int stepX = (nowex > starex) ? 1 : (nowex < starex) ? -1 : 0;
        int stepY = (nowey > starey) ? 1 : (nowey < starey) ? -1 : 0;

        int currentX = starex + stepX;
        int currentY = starey + stepY;

        while (currentX != nowex || currentY != nowey) {
            if (plan->szachownica[currentX][currentY] != '.') {
                blad(2);
                return;
            }

            currentX += stepX;
            currentY += stepY;
        }

        if (kolor(sym) != kolor(plan->szachownica[nowex][nowey])) {
            plan->szachownica[nowex][nowey] = sym;
            plan->szachownica[starex][starey] = '.';
        } else {
            blad(4);
        }
    } else {
        blad(8);
    }
}

void ruch(plansza *plan, char figura, int stareX, int stareY, int noweX, int noweY) {
    if (symbol(plan,stareX,stareY)!=figura){
        blad(9);
        return;
    }
    switch (figura) {
        case 'p':
        case 'P':
            pion(plan, figura, stareX, stareY, noweX, noweY);
            break;
        case 'W':
        case 'w':
            wieza(plan, figura, stareX, stareY, noweX, noweY);
            break;
        case 'S':
        case 's':
            skoczek(plan, figura, stareX, stareY, noweX, noweY);
            break;
        case 'G':
        case 'g':
            goniec(plan, figura, stareX, stareY, noweX, noweY);
            break;
        case 'K':
        case 'k':
            krol(plan, figura, stareX, stareY, noweX, noweY);
            break;
        case 'H':
        case 'h':
            hetman(plan, figura, stareX, stareY, noweX, noweY);
            break;
        default:
            blad(9);
    }
}

bool zakresPlanszy(int nowex, int nowey) {
    return (nowex >= 1 && nowex <= 8) && (nowey >= 1 && nowey <= 8);
}

int stanGry(plansza *szachownica) {
    bool krolbiale = false;
    bool krolczarne = false;
    // Przeszukaj tablicę w poszukiwaniu króla 'k' lub 'K'
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            char figura = szachownica->szachownica[i][j];
            if (figura == 'k') {
                krolbiale = true;
            } else if (figura == 'K') {
                krolczarne = true;
            }
        }
    }
    if (krolbiale && !krolczarne) {
        return WINBIALE;
    } else if (!krolbiale && krolczarne) {
        return WINCZARNE;
    } else {
        return TRWA;
    }
}