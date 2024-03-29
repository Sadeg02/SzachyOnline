#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>
#include "szachownica.c"

#define PORT 1100

int online = 1;

int main();
void rozpocznijGre(int socket);
void interfejs(int numer,char kolor,int socket,int* flaga);
void nowaGra();
void linia();

void linia(){
    printf("\n");
    printf("-----------------------\n");
    printf("\n");
}

void nowaGra(){
    char odpowiedz[100];
    printf("\n");
    printf("Jesli chcesz szukac nowej gry wpisz NOWA lub wyjsc wpisz cokolwiek:");
    fgets(odpowiedz, sizeof(odpowiedz), stdin);

    if (strcmp(odpowiedz, "NOWA\n") == 0) {
        main();
    }else{
        exit(0);
    }
}

void interfejs(int numer,char kolor,int socket,int* flaga){
    switch (numer) {
        case 1://poczatek przywitanie
            printf("Witaj zaraz rozpocznie sie rozgrywka\n");
            if(kolor=='B'){
                printf("Bedziesz gral Bialymi figurami, ktore sa reprezentowane przez male litery \n");
            }else if(kolor =='C'){
                printf("Bedziesz gral Czarnymi figurami, ktore sa reprezentowane przez wielkie litery \n");
            }else{
                printf("blad nie wybralo kolru zresetuj gre\n");
            }
            printf("Wykonuj ruchy w nastepujacy sposob w formacie: \"figura ObecneXY PrzyszleXY\", czyli np.(p 21 31 ruch piona z pozycji 21 na pozycje 31)\n");
            printf("\n");
            break;
        case 2://oczekiwanie na drugiego gracza do gry
            if(*flaga==2){
                printf("Oczekiweanie na drugiego gracza...\n");
                printf("\n");
                *flaga=1;
            }
            break;
        case 3://obsluga ruchu aktywnego gracza
            plansza szachownica;
            if (recv(socket, &szachownica,sizeof(szachownica), 0) < 0) {
                perror("pokaz szachownice error");
                exit(EXIT_FAILURE);
            }
            linia();
            show(&szachownica);
            printf("\n");
            printf("Wykonaj ruch:");

            *flaga = 1;

            while(true) {
                char rozkaz[50];
                int pozwolenie;
                fgets(rozkaz, sizeof(rozkaz), stdin);
                char fig;
                int nowe,stare;
                if (sscanf(rozkaz, "%c %2d %2d",&fig,&stare,&nowe) == 3) {
                    if (0 > send(socket, rozkaz, sizeof(rozkaz), 0)) {
                        perror("blad wysylanie rozkazu");
                        exit(EXIT_FAILURE);
                    }
                    //printf("%s \n",rozkaz);
                    if (recv(socket, &pozwolenie,sizeof(int), 0) < 0) {
                        perror("dostepnosc error");
                        exit(EXIT_FAILURE);
                    }
                    //printf("poz %d \n",pozwolenie);
                    if(pozwolenie == 2){
                        if (recv(socket, &szachownica,sizeof(szachownica), 0) < 0) {
                            perror("pokaz szachownice error");
                            exit(EXIT_FAILURE);
                        }
                        printf("\n");
                        show(&szachownica);
                        printf("\n");
                        break;
                    }else{

                        // kod pomijajacy operacjie w petli glownej
                        if (0 > send(socket, &online, sizeof(int), 0)) {
                            perror("blad wysylania bycia online");
                            exit(EXIT_FAILURE);
                        }
                        int cos;
                        if (recv(socket, &cos, sizeof(int), 0) < 0) {
                            perror("stan error");
                            exit(EXIT_FAILURE);
                        }
                        if(cos==5){
                            interfejs(5,kolor,socket,flaga);
                        }
                        if (recv(socket, &szachownica,sizeof(szachownica), 0) < 0) {
                            perror("pokaz szachownice error");
                            exit(EXIT_FAILURE);
                        }
                        printf("Ruch byl zly wpisz jeszcze raz:");
                    }
                }else{
                    printf("Zly format wpisz jeszcze raz:");
                }
            }
            break;
        case 4://oczekiwanie na ruch drugiego gracza
            if(*flaga){
                printf("Drugi gracz robi ruch...\n");
                *flaga=0;
            }
            break;
        case 5://rozlaczenie gracza
            printf("Drugi gracz zostal rozlaczony\n");
            nowaGra();
        case 6://wygrana bialych
            if(kolor=='B'){
                printf("Gratulacje wygrales!!!\n");
            }else{
                if (recv(socket, &szachownica,sizeof(szachownica), 0) < 0) {
                    perror("pokaz szachownice error");
                    exit(EXIT_FAILURE);
                }
                linia();
                show(&szachownica);
                printf("\n");
                printf("Niestety Szach Mat przegrywasz :(\n");
            }
            nowaGra();
            break;
        case 7://wygrana czarnych
            if(kolor=='C'){
                printf("\n");
                printf("Gratulacje wygrales!!!\n");
            }else{
                if (recv(socket, &szachownica,sizeof(szachownica), 0) < 0) {
                    perror("pokaz szachownice error");
                    exit(EXIT_FAILURE);
                }
                linia();
                show(&szachownica);
                printf("\n");
                printf("Niestety Szach Mat przegrywasz :(\n");
            }
            nowaGra();
            break;
    }
}
void rozpocznijGre(int socket){
    char kolorgracza=' ';
    int stan=2;

    //int poprzedni=20;
    int flaga=2;
    //pobierz kolor od serwera
    if (recv(socket, &kolorgracza,sizeof(kolorgracza), 0) < 0) {
        perror("kolor error");
        exit(EXIT_FAILURE);
    }
    //printf("kolor to %c \n", kolorgracza);
    //wprowadzenie
    interfejs(1,kolorgracza,socket,&flaga);
    while(true) {
        if (0 > send(socket, &online, sizeof(int), 0)) {
            perror("blad wysylania bycia online");
            exit(EXIT_FAILURE);
        }
        //odbiera stan
        if (recv(socket, &stan, sizeof(int), 0) < 0) {
            perror("stan error");
            exit(EXIT_FAILURE);
        }
        //if (poprzedni != stan) {
        //printf("stan to %d \n", stan);
        //poprzedni=stan;
        //}
        //wykonuje odpowiednie komendy
        interfejs(stan,kolorgracza,socket,&flaga);
    }

}

int main() {
    int clientSocket;
    int dostepneStoly;

    struct sockaddr_in serverAddr;

    // Create the socket
    clientSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("Error creating client socket");
        return EXIT_FAILURE;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Error connecting to the server");
        close(clientSocket);
        return EXIT_FAILURE;
    }
    printf("\n");
    printf("Witaj w SzachyOnline sprobujemy znalezc stol dla ciebie :)\n");
    printf("\n");
    if (recv(clientSocket, &dostepneStoly,sizeof(int), 0) < 0) {
        perror("dostepnosc error");
        exit(EXIT_FAILURE);
    }
    if(dostepneStoly) {
        rozpocznijGre(clientSocket);
    }else{
        printf("Wszystkie stoly zajete przepraszamy, sproboj pózniej :(\n");
    }


    close(clientSocket);
    return 0;
}
