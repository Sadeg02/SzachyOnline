#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include "komendy.h"
#include "szachownica.h"
#include <stdbool.h>

#define PORT 12345
#define MAXSTOLY 2
#define BRAK_STOLOW 0
#define SA_STOLY 1

typedef struct {
    plansza szachownica;
    int ilegraczy;
    char tura;
    int koniec;
    bool dostepny;
} stol;

stol stoly[MAXSTOLY];
int main();
void stworzStoly(int ile, stol stoly[]);
void *socketThread(void *arg);
void rozpocznij(int newSocket,char kolor,stol* s);

void stworzStoly(int ile, stol stoly[]) {
    for (int i = 0; i < ile; i++) {
        inicjalizujSzachownice(&(stoly[i].szachownica));
        stoly[i].ilegraczy = 0;
        stoly[i].koniec = 0;
        stoly[i].dostepny = true;
    }
}

void *socketThread(void *arg) {
    int newSocket = *((int *) arg);
    int n;
    char kolorgracza;
    int id=-1;
    // Tutaj możesz sprawdzić dostępność stołów
    for (int i = 0; i < MAXSTOLY; i++) {
        if (stoly[i].dostepny) {
            id=i;
            stoly[i].ilegraczy+=1;
            kolorgracza='B';
            if(stoly[i].ilegraczy==2){
                kolorgracza='C';
                stoly[i].dostepny=false;
            }
            break;
        }
    }
    //informacja o stole
    if(id==-1){
        if ( send(newSocket, BRAK_STOLOW, sizeof(int), 0) < 0){
            perror("blad brak stolow");
            exit(EXIT_FAILURE);
        }
        pthread_exit(NULL);
    }else{
        if (0 > send(newSocket, SA_STOLY, sizeof(int), 0)){
            perror("blad sa stoly");
            exit(EXIT_FAILURE);
        }
    }

    //jest stol rozpocznij rozgrywke
    rozpocznij(newSocket,kolorgracza,&stoly[id]);

    // Zamknij gniazdo klienta i zwolnij zasoby
    close(newSocket);

    pthread_exit(NULL);
}

void rozpocznij(int newSocket,char kolor,stol* s){

}



int main() {
    // Inicjalizacja stolów
    stworzStoly(MAXSTOLY, stoly);

    int serverSocket, newSocket;
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;

    // Utwórz gniazdo.
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Błąd podczas tworzenia gniazda serwera");
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Przypisz strukturę adresu do gniazda.
    bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    // Nasłuchuj na gnieździe i czekaj na połączenia.
    if (listen(serverSocket, 10) == 0)
        printf("Serwer nasłuchuje na porcie %d\n", PORT);
    else {
        perror("Błąd podczas nasłuchiwania");
        exit(EXIT_FAILURE);
    }

    pthread_t thread_id;

    while (1) {
        // Akceptuj połączenia i twórz nowe wątki dla każdego klienta.
        addr_size = sizeof serverStorage;
        newSocket = accept(serverSocket, (struct sockaddr *)&serverStorage, &addr_size);

        if (pthread_create(&thread_id, NULL, socketThread, &newSocket) != 0)
            perror("Błąd podczas tworzenia wątku obsługi klienta");

        // Detach wątku, aby zwolnić zasoby automatycznie po zakończeniu wątku.
        pthread_detach(thread_id);
    }

    // Zwolnij mutex
    pthread_mutex_destroy(&mutexStoly);

    // Zamknij gniazdo serwera
    close(serverSocket);
    return 0;
}