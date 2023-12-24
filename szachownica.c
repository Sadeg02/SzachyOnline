//
// Created by Mich2 on 23.12.2023.
//
#include <stdio.h>
#include "szachownica.h"

void inicjalizujSzachownice(char szachownica[][ROZMIAR]) {
    for (int i = 0; i < ROZMIAR; i++) {
        szachownica[0][i] = (char)(i + '0');
        szachownica[i][0] = (char)(i + '0');
    }
    for (int i = 3; i < ROZMIAR - 2; i++) {
        for (int j = 1; j < ROZMIAR; j++) {
            szachownica[i][j] = '.';
        }
    }
    for (int i = 1; i < ROZMIAR; i++) {
        szachownica[2][i] = 'p';
        szachownica[7][i] = 'P';
    }
    szachownica[1][1] = 'w';
    szachownica[1][8] = 'w';
    szachownica[4][4] = 'w'; // test
    szachownica[8][1] = 'W';
    szachownica[8][8] = 'W';
    szachownica[1][2] = 's';
    szachownica[1][7] = 's';
    szachownica[4][5] = 's'; // test
    szachownica[8][2] = 'S';
    szachownica[8][7] = 'S';
    szachownica[1][3] = 'g';
    szachownica[1][6] = 'g';
    szachownica[8][3] = 'G';
    szachownica[8][6] = 'G';
    szachownica[1][4] = 'k';
    szachownica[1][5] = 'h';
    szachownica[8][4] = 'K';
    szachownica[8][5] = 'H';
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