#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 12345
#define BUFFER_SIZE 256

void rozpocznijGre(int socket);
void interfejs(int numer,char kolor);

void interfejs(int numer,char kolor){
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
        case 2:

    }
}
void rozpocznijGre(int socket){
    char kolorgracza;
    //pobierz kolor od serwera
    if (recv(socket, &kolorgracza,1, 0) < 0) {
        perror("dostepnosc error");
        exit(EXIT_FAILURE);
    }
    //wprowadzenie
    interfejs(1,kolorgracza);
}

int kut() {
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
