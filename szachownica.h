//
// Created by Mich2 on 23.12.2023.
//

#ifndef SZACHOWNICA_H
#define SZACHOWNICA_H

#define ROZMIAR 9

typedef struct {
    char szachownica[ROZMIAR][ROZMIAR];
} plansza;
void inicjalizujSzachownice(char szachownica[][ROZMIAR]);
void show(const plansza *p);
void zmiana(plansza *p, char sym, int starex, int starey, int nowex, int nowey);
void wstaw(plansza *p, char sym, int x, int y);
char symbol(const plansza *p, int x, int y);
void przekaz(const plansza *p, char *buffer);


#endif //SZACHOWNICA_H
