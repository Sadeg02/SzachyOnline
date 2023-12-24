//
// Created by Mich2 on 23.12.2023.
//



#ifndef KOMENDY_H
#define KOMENDY_H

#include "szachownica.h"

void blad(int numer);
char kolor(char kol);
void pion(plansza *plan, char sym, int starex, int starey, int nowex, int nowey);
void wieza(plansza *plan, char sym, int starex, int starey, int nowex, int nowey);
void skoczek(plansza *plan, char sym, int starex, int starey, int nowex, int nowey);
void goniec(plansza *plan, char sym, int starex, int starey, int nowex, int nowey);
void krol(plansza *plan, char sym, int starex, int starey, int nowex, int nowey);
void hetman(plansza *plan, char sym, int starex, int starey, int nowex, int nowey);
void ruch(plansza *plan, char figura, int stareX, int stareY, int noweX, int noweY);
bool zakresPlanszy(int nowex, int nowey);
int stanGry(plansza *szachownica);

#endif // KOMENDY_H

