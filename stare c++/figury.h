//
// Created by Mich2 on 21.12.2023.
//
/*
#ifndef FIGURY_H
#define FIGURY_H
#include "plansza.h"
class figura {
public:
    //konstruktor
    figura(plansza plan,char symbol,char kolor,int startx,int starty);

    // Metody dostępowe do pól klasy
    char getSymbol() const;
    char getKolor() const;
    int getx() const;
    int gety() const;
private:
    char symbol;
    char kolor;
protected:
    int x;
    int y;
    plansza plan;
};

class pion : public figura {
public:
    pion(plansza plan,char kolor,int startx,int starty);
    virtual void ruch(int rx , int ry);
};




#endif // FIGURY_H
*/

#ifndef FIGURY_H
#define FIGURY_H
#include "plansza.h"
char kolor(char kol);
void blad(int numer);
//figury
void pion(plansza& plan, char sym, int starex, int starey, int nowex, int nowey);
void wieza(plansza& plan,char sym , int starex , int starey , int nowex ,int nowey);
void skoczek(plansza& plan, char sym, int starex, int starey, int nowex, int nowey);
void goniec(plansza& plan, char sym, int starex, int starey, int nowex, int nowey);
void krol(plansza &plan, char sym, int starex, int starey, int nowex, int nowey);
void hetman(plansza &plan, char sym, int starex, int starey, int nowex, int nowey);
void ruch(plansza& plan, char figura, int stareX, int stareY, int noweX, int noweY);
#endif // FIGURY_H