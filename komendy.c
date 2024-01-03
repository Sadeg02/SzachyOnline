#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "szachownica.h"
#include "komendy.h"



#define ROZMIAR 9
#define TRWA 1
#define WINBIALE 2
#define WINCZARNE 3

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
        case 10:
            printf("Ruch poza plansze\n");
            break;
        default:
            printf("Nieznany bład\n");
    }
}

char kolor(char kol) {
    if (kol >= 'A' && kol <= 'Z') {
        return 'C';
    } else if (kol >= 'a' && kol <= 'z') {
        return 'B';
    } else {
        return '.';
    }
}

int pion(plansza *plan, char sym, int starex, int starey, int nowex, int nowey) {
    //ruch do przodu
    if (starey == nowey && (((starex - 1) == nowex && kolor(sym) == 'C' )|| ((starex + 1) == nowex && kolor(sym) == 'B')) &&
            symbol(plan, nowex, nowey) == '.') {
        zmiana(plan,  sym, starex, starey, nowex, nowey);
        return 2;
        // bicie
    } else if ((starey + 1 == nowey || starey - 1 == nowey) &&
               ((starex - 1 == nowex && kolor(sym) == 'C') || (starex + 1 == nowex && kolor(sym) == 'B')) &&
               kolor(sym) != kolor(symbol(plan, nowex, nowey))) {
        if(kolor(symbol(plan, nowex, nowey))=='.'){
            blad(1);
            return 1;
        }
        zmiana(plan,  sym, starex, starey, nowex, nowey);
        return 2;
    } else {
        blad(1);
        return 1;
    }

}

int wieza(plansza *plan, char sym, int starex, int starey, int nowex, int nowey) {
    if (starex == nowex || starey == nowey) {
        int stepX = (nowex > starex) ? 1 : (nowex < starex) ? -1 : 0;
        int stepY = (nowey > starey) ? 1 : (nowey < starey) ? -1 : 0;

        int currentX = starex + stepX;
        int currentY = starey + stepY;

        while (currentX != nowex || currentY != nowey) {
            if (plan->szachownica[currentX][currentY] != '.') {
                blad(2);
                return 1;
            }

            currentX += stepX;
            currentY += stepY;
        }

        if (kolor(sym) != kolor(plan->szachownica[nowex][nowey])) {
            zmiana(plan,  sym, starex, starey, nowex, nowey);
            return 2;
        } else {
            blad(4);
            return 1;
        }
    } else {
        blad(8);
        return 1;
    }

}

int skoczek(plansza *plan, char sym, int starex, int starey, int nowex, int nowey) {
    int deltaX = abs(nowex - starex);
    int deltaY = abs(nowey - starey);

    if ((deltaX == 1 && deltaY == 2) || (deltaX == 2 && deltaY == 1)) {
        if (kolor(sym) != kolor(plan->szachownica[nowex][nowey])) {
            zmiana(plan,  sym, starex, starey, nowex, nowey);
            return 2;
        } else {
            blad(4);
            return 1;
        }
    } else {
        blad(8);
        return 1;
    }

}

int goniec(plansza *plan, char sym, int starex, int starey, int nowex, int nowey) {
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
                return 1;
            }

            currentX += stepX;
            currentY += stepY;
        }

        if (kolor(sym) != kolor(plan->szachownica[nowex][nowey])) {
            zmiana(plan,  sym, starex, starey, nowex, nowey);
            return 2;
        } else {
            blad(4);
            return 1;
        }
    } else {
        blad(8);
        return 1;
    }

}

int krol(plansza *plan, char sym, int starex, int starey, int nowex, int nowey) {
    int deltaX = abs(nowex - starex);
    int deltaY = abs(nowey - starey);

    if ((deltaX <= 1 && deltaY <= 1) && (deltaX != 0 || deltaY != 0)) {
        if (kolor(sym) != kolor(plan->szachownica[nowex][nowey])) {
            zmiana(plan,  sym, starex, starey, nowex, nowey);
            return 2;
        } else {
            blad(4);
            return 1;
        }
    } else {
        blad(8);
        return 1;
    }

}

int hetman(plansza *plan, char sym, int starex, int starey, int nowex, int nowey) {
    if (starex == nowex || starey == nowey || abs(nowex - starex) == abs(nowey - starey)) {
        int stepX = (nowex > starex) ? 1 : (nowex < starex) ? -1 : 0;
        int stepY = (nowey > starey) ? 1 : (nowey < starey) ? -1 : 0;

        int currentX = starex + stepX;
        int currentY = starey + stepY;

        while (currentX != nowex || currentY != nowey) {
            if (plan->szachownica[currentX][currentY] != '.') {
                blad(2);
                return 1;
            }

            currentX += stepX;
            currentY += stepY;
        }

        if (kolor(sym) != kolor(plan->szachownica[nowex][nowey])) {
            zmiana(plan,  sym, starex, starey, nowex, nowey);
            return 2;
        } else {
            blad(4);
            return 1;
        }
    } else {
        blad(8);
        return 1;
    }

}

int ruch(plansza *plan, char rozkaz[]) {
    char figura;
    int starePole,nowePole;
    printf("%s \n",rozkaz);
    sscanf(rozkaz, "%c %2d %2d", &figura, &starePole, &nowePole);
    int stareX = starePole/10;
    int stareY = starePole%10;
    int noweX = nowePole/10;
    int noweY = nowePole%10;
    printf("Figura: %c\n", figura);
    printf("Stare pole: (%d, %d)\n", stareX, stareY);
    printf("Nowe pole: (%d, %d)\n", noweX, noweY);

    if (symbol(plan,stareX,stareY)!=figura){
        printf("%d ,%d , %c \n",stareX,stareY,figura);
        blad(9);
        return 1;
    }
    if (!zakresPlanszy(noweX, noweY)) {
        blad(10);
        return 1;
    } else {
        switch (figura) {
            case 'p':
            case 'P':
                return pion(plan, figura, stareX, stareY, noweX, noweY);

            case 'W':
            case 'w':
                return wieza(plan, figura, stareX, stareY, noweX, noweY);

            case 'S':
            case 's':
                return skoczek(plan, figura, stareX, stareY, noweX, noweY);

            case 'G':
            case 'g':
                return goniec(plan, figura, stareX, stareY, noweX, noweY);

            case 'K':
            case 'k':
                return krol(plan, figura, stareX, stareY, noweX, noweY);

            case 'H':
            case 'h':
                return hetman(plan, figura, stareX, stareY, noweX, noweY);

            default:
                blad(1);
                return 1;
        }
    }
    return 2;
}

bool zakresPlanszy(int nowex, int nowey) {
    return (nowex >= 1 && nowex <= 8) && (nowey >= 1 && nowey <= 8);
}

int czyKoniec(plansza *szachownica) {
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