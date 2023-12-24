//
// Created by Mich2 on 23.12.2023.
//
#include <stdio.h>
#include "szachownica.h"

void inicjalizujSzachownice(plansza *p) {
    for (int i = 0; i < ROZMIAR; i++) {
        p->szachownica[0][i] = (char)(i + '0');
        p->szachownica[i][0] = (char)(i + '0');
    }

    for (int i = 3; i < ROZMIAR - 2; i++) {
        for (int j = 1; j < ROZMIAR; j++) {
            p->szachownica[i][j] = '.';
        }
    }

    for (int i = 1; i < ROZMIAR; i++) {
        p->szachownica[2][i] = 'p';
        p->szachownica[7][i] = 'P';
    }

    p->szachownica[1][1] = 'w';
    p->szachownica[1][8] = 'w';
    p->szachownica[4][4] = 'w'; // test
    p->szachownica[8][1] = 'W';
    p->szachownica[8][8] = 'W';

    p->szachownica[1][2] = 's';
    p->szachownica[1][7] = 's';
    p->szachownica[4][5] = 's'; // test
    p->szachownica[8][2] = 'S';
    p->szachownica[8][7] = 'S';

    p->szachownica[1][3] = 'g';
    p->szachownica[1][6] = 'g';
    p->szachownica[8][3] = 'G';
    p->szachownica[8][6] = 'G';

    p->szachownica[1][4] = 'k';
    p->szachownica[1][5] = 'h';
    p->szachownica[8][4] = 'K';
    p->szachownica[8][5] = 'H';
}


void show(const plansza *p) {
    for (int i = 0; i < ROZMIAR; i++) {
        for (int j = 0; j < ROZMIAR; j++) {
            printf("%c ", p->szachownica[i][j]);
        }
        printf("\n");
    }
}

void zmiana(plansza *p, char sym, int starex, int starey, int nowex, int nowey) {
    p->szachownica[starex][starey] = '.';
    p->szachownica[nowex][nowey] = sym;
}

void wstaw(plansza *p, char sym, int x, int y) {
    p->szachownica[x][y] = sym;
}

char symbol(const plansza *p, int x, int y) {
    return p->szachownica[x][y];
}


void przekaz(const plansza *p, char *buffer) {
    int index = 0;
    for (int i = 0; i < ROZMIAR; ++i) {
        for (int j = 0; j < ROZMIAR; ++j) {
            buffer[index++] = p->szachownica[i][j];
            buffer[index++] = ' ';  // Dodaj odstęp między znakami
        }
        buffer[index++] = '\n';  // Dodaj nową linię po każdym wierszu
    }

    buffer[index] = '\0';  // Dodaj null terminator na końcu bufora
}