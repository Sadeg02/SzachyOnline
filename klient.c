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

void interfejs(int numer,char kolor,int socket,int* flaga){
    switch (numer) {
        case 1:
            printf("Witaj zaraz rozpocznie sie rozgrywka\n");
            if(kolor=='B'){
                printf("Bedziesz gral Bialymi figurami, ktre sa reprezentowane przez male litery \n");
            }else if(kolor =='C'){
                printf("Bedziesz gral Czarnymi figurami, ktre sa reprezentowane przez wielkie litery \n");
            }else{
                printf("blad nie wybralo kolru zresetuj gre\n");
            }
            printf("Wykonuj ruchy w nastepujacy sposob w formacie: \"figura ObecneXY PrzyszleXY\", czyli np.(p 21 31 ruch piona z pozycji 21 na pozycje 31)\n");
            break;
        case 2:
            if(*flaga==2){
                printf("Oczekiweanie na drugiego gracza\n");
                *flaga=1;
            }
            break;
        case 3:
            plansza szachownica;
            if (recv(socket, &szachownica,sizeof(szachownica), 0) < 0) {
                perror("pokaz szachownice error");
                exit(EXIT_FAILURE);
            }
            printf("\n");
            printf("-----------------------\n");
            printf("\n");
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
                    printf("%s \n",rozkaz);
                    if (0 > send(socket, rozkaz, sizeof(rozkaz), 0)) {
                        perror("blad wysylanie rozkazu");
                        exit(EXIT_FAILURE);
                    }
                    printf("%s \n",rozkaz);
                    if (recv(socket, &pozwolenie,sizeof(int), 0) < 0) {
                        perror("dostepnosc error");
                        exit(EXIT_FAILURE);
                    }
                    printf("poz %d \n",pozwolenie);
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
                        printf("Ruch byl zly wpisz jeszcze raz:");
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
                        if (recv(socket, &szachownica,sizeof(szachownica), 0) < 0) {
                            perror("pokaz szachownice error");
                            exit(EXIT_FAILURE);
                        }
                    }
                }else{
                    printf("Zly format wpisz jeszcze raz:");
                }
            }
            break;
        case 4:
            if(*flaga){
                printf("Drugi gracz robi ruch...\n");
                *flaga=0;
            }
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
    printf("kolor to %c \n", kolorgracza);
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
    printf("Witaj w SzachyOnline sprobujemy znalezc stol dla ciebie :)\n");
    if (recv(clientSocket, &dostepneStoly,sizeof(int), 0) < 0) {
        perror("dostepnosc error");
        exit(EXIT_FAILURE);
    }
    printf("dostpene %d\n",dostepneStoly);
    if(dostepneStoly) {
        rozpocznijGre(clientSocket);
    }else{
        printf("Wszystkie stoly zajete");
    }


    close(clientSocket);
    return 0;
}
