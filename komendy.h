//
// Created by Mich2 on 23.12.2023.
//



#ifndef KOMENDY_H
#define KOMENDY_H

#include "szachownica.h"

void blad(int numer);
char kolor(char kol);
int pion(plansza *plan, char sym, int starex, int starey, int nowex, int nowey);
int wieza(plansza *plan, char sym, int starex, int starey, int nowex, int nowey);
int skoczek(plansza *plan, char sym, int starex, int starey, int nowex, int nowey);
int goniec(plansza *plan, char sym, int starex, int starey, int nowex, int nowey);
int krol(plansza *plan, char sym, int starex, int starey, int nowex, int nowey);
int hetman(plansza *plan, char sym, int starex, int starey, int nowex, int nowey);
int ruch(plansza *plan, char rozkaz[],char koloraktywnegogracza);
bool zakresPlanszy(int nowex, int nowey);
int czyKoniec(plansza *szachownica);

#endif // KOMENDY_H

