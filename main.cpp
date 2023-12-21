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
            break;
        }
        char figura;
        int stareX, stareY, noweX, noweY;
        if (linia >> figura >> stareX >> stareY >> noweX >> noweY) {
            // Tutaj możesz dodać dodatkową logikę, np. sprawdzenie poprawności ruchu
            pion(szachownica, figura, stareX, stareY, noweX, noweY);
        } else {
            std::cout << "Bledny format ruchu. Poprawny format: [figura] [stareX] [stareY] [noweX] [noweY]" << std::endl;
        }
        szachownica.show();
    }
}


