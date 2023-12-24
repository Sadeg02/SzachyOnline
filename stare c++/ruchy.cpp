//
// Created by Mich2 on 21.12.2023.
//
#include "figury.h"
#include "plansza.h"
#include <iostream>
void blad(int numer) {
    switch (numer) {
        case 1:
            std::cout << "Nieprawidłowy ruch pionka" << std::endl;
            break;
        case 2:
            std::cout << "Inny pionek blokuje ruch" << std::endl;
            break;
        case 4:
            std::cout << "Pole docelowe zajęte przez własnego pionka" << std::endl;
            break;
        case 8:
            std::cout << "Nieprawidłowy ruch" << std::endl;
            break;
        case 9:
            std::cout << "Nieprawidłowa figura" << std::endl;
            break;
        default:
            std::cout << "Nieznany błąd" << std::endl;
    }
}
char kolor(char kol) {
    if (kol >= 'A' && kol <= 'Z') {
        return 'B';
    } else if(kol >= 'a' && kol <= 'z'){
        return 'C';
    }else{
        return '.';
    }
}
void pion(plansza& plan,char sym , int starex , int starey , int nowex ,int nowey){

    if(starey==nowey && ((starex+1)==nowex && kolor(sym)=='C' || (starex-1)==nowex && kolor(sym)=='B') && plan.symbol(nowex,nowey)=='.'){
        plan.zmiana(sym,starex,starey,nowex,nowey);
    }else if((starey+1==nowey || starey-1==nowey)&&(starex+1==nowex && kolor(sym)=='C'||starex-1==nowex && kolor(sym)=='B')&&kolor(sym)!=kolor(plan.symbol(nowex,nowey))){
        plan.zmiana(sym,starex,starey,nowex,nowey);
    }else{
        blad(1);
        return;
    }
}

void wieza(plansza& plan, char sym, int starex, int starey, int nowex, int nowey) {
    if (starex == nowex || starey == nowey) {
        int stepX = (nowex > starex) ? 1 : (nowex < starex) ? -1 : 0;
        int stepY = (nowey > starey) ? 1 : (nowey < starey) ? -1 : 0;

        int currentX = starex + stepX;
        int currentY = starey + stepY;

        while (currentX != nowex || currentY != nowey) {
            if (plan.symbol(currentX, currentY) != '.') {
                blad(2);
                return;
            }

            currentX += stepX;
            currentY += stepY;
        }

        if (kolor(sym) != kolor(plan.symbol(nowex, nowey))) {
            plan.zmiana(sym, starex, starey, nowex, nowey);
        } else {
            blad(4);
        }
    } else {
        blad(8);
    }
}
void skoczek(plansza& plan, char sym, int starex, int starey, int nowex, int nowey) {
    int deltaX = abs(nowex - starex);
    int deltaY = abs(nowey - starey);

    if ((deltaX == 1 && deltaY == 2) || (deltaX == 2 && deltaY == 1)) {
        // Ruch skoczka jest zgodny z zasadami
        if (kolor(sym) != kolor(plan.symbol(nowex, nowey))) {
            // Na polu docelowym nie ma pionka lub jest pionek przeciwnika
            plan.zmiana(sym, starex, starey, nowex, nowey);
        } else {
            blad(4);
        }
    } else {
        blad(8);
    }
}

void goniec(plansza &plan, char sym, int starex, int starey, int nowex, int nowey) {
    // Sprawdź, czy ruch gońca jest dozwolony
    int deltaX = abs(nowex - starex);
    int deltaY = abs(nowey - starey);

    if (deltaX == deltaY) {
        // Ruch gońca jest zgodny z zasadami
        int stepX = (nowex > starex) ? 1 : -1; // Kierunek ruchu po osi X
        int stepY = (nowey > starey) ? 1 : -1; // Kierunek ruchu po osi Y

        int currentX = starex + stepX;
        int currentY = starey + stepY;

        while (currentX != nowex && currentY != nowey) {
            // Sprawdź, czy na drodze gońca nie ma żadnych pionków
            if (plan.symbol(currentX, currentY) != '.') {
                blad(2);
                return;
            }

            // Przesuwaj się na kolejne pole
            currentX += stepX;
            currentY += stepY;
        }

        // Sprawdź, czy na polu docelowym nie ma pionka, lub jeśli jest, czy jest to pionek przeciwnika
        if (kolor(sym) != kolor(plan.symbol(nowex, nowey))) {
            // Na polu docelowym nie ma pionka lub jest pionek przeciwnika
            plan.zmiana(sym, starex, starey, nowex, nowey);
        } else {
            blad(4);
        }
    } else {
        blad(8);
    }
}

void krol(plansza &plan, char sym, int starex, int starey, int nowex, int nowey) {
    int deltaX = abs(nowex - starex);
    int deltaY = abs(nowey - starey);

    if ((deltaX <= 1 && deltaY <= 1) && (deltaX != 0 || deltaY != 0)) {
        // Ruch króla jest zgodny z zasadami
        if (kolor(sym) != kolor(plan.symbol(nowex, nowey))) {
            // Na polu docelowym nie ma pionka lub jest pionek przeciwnika
            plan.zmiana(sym, starex, starey, nowex, nowey);
        } else {
            blad(4);
        }
    } else {
        blad(8);
    }
}

void hetman(plansza& plan, char sym, int starex, int starey, int nowex, int nowey) {
    // Sprawdź, czy ruch hetmana jest w pionie, poziomie lub po przekątnej
    if (starex == nowex || starey == nowey || abs(nowex - starex) == abs(nowey - starey)) {
        // Określ kierunek ruchu w osi X
        int stepX = (nowex > starex) ? 1 : (nowex < starex) ? -1 : 0;
        // Określ kierunek ruchu w osi Y
        int stepY = (nowey > starey) ? 1 : (nowey < starey) ? -1 : 0;

        // Inicjalizacja współrzędnych aktualnego pola na planszy
        int currentX = starex + stepX;
        int currentY = starey + stepY;

        // Sprawdź, czy na drodze nie ma żadnych pionków
        while (currentX != nowex || currentY != nowey) {
            if (plan.symbol(currentX, currentY) != '.') {
                blad(2); // Błąd: Pionek blokuje trasę hetmana
                return;
            }

            // Przesuwaj się na kolejne pole
            currentX += stepX;
            currentY += stepY;
        }

        // Sprawdź, czy na polu docelowym nie ma pionka, lub jeśli jest, czy jest to pionek przeciwnika
        if (kolor(sym) != kolor(plan.symbol(nowex, nowey))) {
            // Na polu docelowym nie ma pionka lub jest pionek przeciwnika
            plan.zmiana(sym, starex, starey, nowex, nowey);
        } else {
            blad(4); // Błąd: Pole docelowe zajęte przez własnego pionka
        }
    } else {
        blad(8); // Błąd: Ruch hetmana jest nieprawidłowy
    }
}

void ruch(plansza& plan, char figura, int stareX, int stareY, int noweX, int noweY) {
    try {
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
                // Obsługa błędnego symbolu figury
                blad(9);
        }
    } catch (const std::invalid_argument& e) {
        std::cout << "Bledny format ruchu. Poprawny format: [figura] [stareX][stareY] [noweX][noweY]" << std::endl;
    }
}