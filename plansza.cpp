//
// Created by Mich2 on 21.12.2023.
//
#include <iostream>
#include "plansza.h"
//konstruktory


plansza::plansza() {
    inicjalizujPlansze();
}

//funkcje


void plansza::inicjalizujPlansze() {
for(int i=0;i<rozmiar;i++){
    szachownica[0][i]=std::to_string(i)[0];
    szachownica[i][0]=std::to_string(i)[0];
}
    for(int i=3;i<rozmiar-2;i++){
        for(int j=1;j<rozmiar;j++){
            szachownica[i][j]='.';
        }
    }
    for(int i=1;i<rozmiar;i++){
        szachownica[2][i]='p';
        szachownica[7][i]='P';
    }
    szachownica[1][1]='w';
    szachownica[1][8]='w';
    szachownica[4][4]='w'; //test
    szachownica[8][1]='W';
    szachownica[8][8]='W';
    szachownica[1][2]='s';
    szachownica[1][7]='s';
    szachownica[4][5]='s'; //test
    szachownica[8][2]='S';
    szachownica[8][7]='S';
    szachownica[1][3]='g';
    szachownica[1][6]='g';
    szachownica[8][3]='G';
    szachownica[8][6]='G';
    szachownica[1][4]='k';
    szachownica[1][5]='h';
    szachownica[8][4]='K';
    szachownica[8][5]='H';

}

void plansza::show() const {
    for (auto i : szachownica) {
        for (int j = 0; j < rozmiar; ++j) {
            std::cout << i[j] << ' ';
        }
        std::cout << std::endl;
    }
}

void plansza::zmiana(char sym, int starex, int starey, int nowex, int nowey) {
    szachownica[starex][starey] = '.';
    szachownica[nowex][nowey] = sym;
}

void plansza::wstaw(char sym, int x, int y) {
    szachownica[x][y] = sym;
}

char plansza::symbol(int x, int y) {
    return szachownica[x][y];
}

void plansza::odnowa() {
    inicjalizujPlansze();
}


