#include <stdio.h>
#include <stdlib.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
#include <string.h>
//#include <arpa/inet.h>
#include <unistd.h>
#include "komendy.h"
#include "szachownica.h"
#include <stdbool.h>



typedef struct{
    plansza szachownica;
    int gracze;
    int tura;
    int koniec; // 0=trwa  1=pierwszy gracz wygral 2=drugi gracz wygral
    bool dostepny;
}stol;