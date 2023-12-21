#include <iostream>
#include <sstream>
#include "plansza.h"
#include "figury.h"
int main() {
    plansza szachownica;
    szachownica.show();
    while(true){
        std::string rozkaz;
        std::cout << "Wprowadz ruch:  ";
        std::getline(std::cin, rozkaz);
        std::istringstream linia(rozkaz);
        if(rozkaz == "q"){
            std::cout << "Rozłączanie"<<std::endl;
            break;
        }
        char figura;
        std::string starePole, nowePole;
        if (linia >> figura >> starePole >> nowePole) {
            int stareX = stoi(starePole.substr(0, 1));
            int stareY = stoi(starePole.substr(1, 1));
            int noweX = stoi(nowePole.substr(0, 1));
            int noweY = stoi(nowePole.substr(1, 1));
            pion(szachownica, figura, stareX, stareY, noweX, noweY);
        } else {
            std::cout << "Bledny format ruchu. Poprawny format: [figura] [stareX] [stareY] [noweX] [noweY]" << std::endl;
        }
        szachownica.show();
    }
}


