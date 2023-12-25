#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>
#include "szachownica.h"

#define PORT 12345
#define BUFFER_SIZE 256

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
            printf("Oczekiweanie na drugiego gracza\n");
            break;
        case 3:
            printf("Wykonaj ruch:");
            *flaga = 1;
            char rozkaz[50];
            int pozwolenie;
            plansza szachownica;
            while(true) {
                fgets(rozkaz, sizeof(rozkaz), stdin);
                if (sscanf(rozkaz, "%*c %*2d %*2d") == 3) {
                    if (0 > send(socket, rozkaz, strlen(rozkaz), 0)) {
                        perror("blad wysylanie rozkazu");
                        exit(EXIT_FAILURE);
                    }
                    if (recv(socket, &pozwolenie,1, 0) < 0) {
                        perror("dostepnosc error");
                        exit(EXIT_FAILURE);
                    }
                    if(pozwolenie == 2){
                        if (recv(socket, &szachownica,sizeof(szachownica), 0) < 0) {
                            perror("pokaz szachownice error");
                            exit(EXIT_FAILURE);
                        }
                        show(&szachownica);
                        break;
                    }else{
                        printf("Ruch byl zly wpisz jeszcze raz:");
                    }
                }else{
                    printf("Zly format wpisz jeszcze raz:");
                }
            }
            break;
        case 4:
            if(flaga){
                printf("Drugi gracz robi ruch\n");
                *flaga=0;
            }
            break;
    }
}
void rozpocznijGre(int socket){
    char kolorgracza;
    int stan;
    int flaga=1;
    //pobierz kolor od serwera
    if (recv(socket, &kolorgracza,1, 0) < 0) {
        perror("dostepnosc error");
        exit(EXIT_FAILURE);
    }
    //wprowadzenie
    interfejs(1,kolorgracza,socket,&flaga);
    while(true) {
        //odbiera stan
        if (recv(socket, &stan, 1, 0) < 0) {
            perror("stan error");
            exit(EXIT_FAILURE);
        }
        //wykonuje odpowiednie komendy
        interfejs(stan,kolorgracza,socket,&flaga);
    }

}

int main() {
    int clientSocket;
    int dostepneStoly;

    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];
    char move[BUFFER_SIZE];

    // Create the socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
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
    if (recv(clientSocket, &dostepneStoly,1, 0) < 0) {
        perror("dostepnosc error");
        exit(EXIT_FAILURE);
    }
    if(dostepneStoly) {
        rozpocznijGre(clientSocket);
    }else{
        printf("Wszystkie stoly zajete");
    }


    close(clientSocket);
    return 0;
}
