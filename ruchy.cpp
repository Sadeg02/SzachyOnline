//
// Created by Mich2 on 21.12.2023.
//
#include "figury.h"
#include "plansza.h"
#include <iostream>
/*
//konstruktory
figura::figura(plansza plan,char symbol,char kolor,int startx,int starty) : plan(plan),symbol(symbol),kolor(kolor),x(startx),y(starty){
    plan.wstaw(symbol,startx,starty);
}
pion::pion(plansza plan,char kolor,int startx,int starty) : figura(plan,'P',kolor,startx,starty){

}





//ruchy


// Implementacja metody dostępowej do pola symbol
char figura::getSymbol() const {
    return symbol;
}

// Implementacja metody dostępowej do pola kolor
char figura::getKolor() const {
    return kolor;
}

// Implementacja metody dostępowej do pola wspolrzedneX
int figura::getx() const {
    return x;
}

// Implementacja metody dostępowej do pola wspolrzedneY
int figura::gety() const {
    return y;
}

void pion::ruch(int rx , int ry) {
    // TO DO: sprawdzanie poprawnosci
    std::cout << "Ruch piona z (" << getx() << ", " << gety() << ") na ("<< rx <<","<<ry<<")"<< std::endl;
    plan.zmiana(x,y,rx,ry);
    x = rx;
    y = ry;
}
*/
char kolor(char kol) {
    if (kol >= 'A' && kol <= 'Z') {
        return 'B';
    } else {
        return 'C';
    }
}
void pion(plansza& plan,char sym , int starex , int starey , int nowex ,int nowey){

    if(starey==nowey && ((starex+1)==nowex && kolor(sym)=='C' || (starex-1)==nowex && kolor(sym)=='B') && plan.symbol(nowex,nowey)=='.'){
        std::cout<<"1"<<std::endl;
        plan.zmiana(sym,starex,starey,nowex,nowey);
    }else if((starey+1==nowey || starey-1==nowey)&&(starex+1==nowex && kolor(sym)=='C'||starex-1==nowex && kolor(sym)=='B')&&kolor(sym)!=kolor(plan.symbol(nowex,nowey))){
        plan.zmiana(sym,starex,starey,nowex,nowey);
        std::cout<<"2"<<std::endl;
    }else{
        std::cout<<"blad"<<std::endl;
    }
}