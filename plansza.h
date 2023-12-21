//
// Created by Mich2 on 21.12.2023.
//

#ifndef PLANSZA_H
#define PLANSZA_H


class plansza{
public:
    plansza();
    void show() const;
    void zmiana(char sym,int starex,int starey , int nowex , int nowey);
    void wstaw(char sym,int x,int y);
    char symbol(int x,int y);
    static const int rozmiar=9;
protected:

    char szachownica[rozmiar][rozmiar];
    void inicjalizujPlansze();
    void odnowa();
};



#endif //PLANSZA_H
