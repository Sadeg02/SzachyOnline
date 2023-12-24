#include <iostream>
#include <sstream>
#include "plansza.h"
#include "figury.h"
int main() {
    plansza szachownica;
    szachownica.show();

    while (true) {
        std::string rozkaz;
        std::cout << "Wprowadz ruch: ";
        std::getline(std::cin, rozkaz);

        if (rozkaz == "q") {
            std::cout << "Rozlaczanie" << std::endl;
            break;
        }

        char figura;
        std::string starePole, nowePole;

        std::istringstream linia(rozkaz);
        if (linia >> figura >> starePole >> nowePole) {
            try {
                int stareX = stoi(starePole.substr(0, 1));
                int stareY = stoi(starePole.substr(1, 1));
                int noweX = stoi(nowePole.substr(0, 1));
                int noweY = stoi(nowePole.substr(1, 1));

                ruch(szachownica,figura,stareX,stareY,noweX,noweY);
            } catch (const std::invalid_argument& e) {
                std::cout << "Bledny format ruchu. Poprawny format: [figura] [stareX][stareY] [noweX][noweY]" << std::endl;
            }
        } else {
            std::cout << "Bledny format ruchu. Poprawny format: [figura] [stareX][stareY] [noweX][noweY]" << std::endl;
        }

        szachownica.show();
    }

    return 0;
}


