#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 12345
#define BUFFER_SIZE 256
void showBoard(char szachownica[],int rozmiar);
void showBoard(char szachownica[],int rozmiar) {
    // Wyświetl planszę szachową
    printf("Plansza:\n");
    for (int i = 0; i < rozmiar; ++i) {
        printf("%c",szachownica[i]);
    }
}

int kut() {
    int clientSocket;
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

    while (1) {
        if (recv(clientSocket, buffer, sizeof(buffer), 0)==-1){
        // Otrzymaj planszę od serwera
        memset(buffer, 0, sizeof(buffer));
        recv(clientSocket, buffer, sizeof(buffer), 0);
        showBoard(buffer,sizeof(buffer));

        // Wprowadź ruch
        printf("Enter your move (format: [figure] [stareX] [stareY] [noweX] [noweY]): ");
        fgets(move, sizeof(move), stdin);

        // Wyślij ruch do serwera
        send(clientSocket, move, strlen(move), 0);

        // Otrzymaj planszę po ruchu
        memset(buffer, 0, sizeof(buffer));
        recv(clientSocket, buffer, sizeof(buffer), 0);
        showBoard(buffer,sizeof(buffer));
        }else{
            printf("Wszystkie stoly zajete");
        }
    }

    close(clientSocket);
    return 0;
}
