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

    const int oczekiwanie = 2;
    const int pozwolenieRuch =3;
    const int czekanieNaTure =4;

    if (send(newSocket, kolor, 1, 0) < 0){
        perror("wyslanie koloru");
        exit(EXIT_FAILURE);
    }
    while(true){
        if(s->dostepny==true){
            if (send(newSocket,oczekiwanie, 1, 0) < 0){
                perror("wyslanie oczekiwanie");
                exit(EXIT_FAILURE);
            }
        }else if(s->tura==kolor){
            char rozkaz[50];
            int odp;
            if (send(newSocket, pozwolenieRuch, 1, 0) < 0){
                perror("wyslanie pozwolenia na ruch");
                exit(EXIT_FAILURE);
            }
            if (recv(newSocket, &rozkaz,sizeof(rozkaz), 0) < 0) {
                perror("dostepnosc error");
                exit(EXIT_FAILURE);
            }
            //sprawdzanie rozkazu
            odp=ruch(&(s->szachownica),rozkaz);
            if(odp==2){
                if (send(newSocket, odp, 1, 0) < 0){
                    perror("dobry ruch");
                    exit(EXIT_FAILURE);
                }
                if (send(newSocket, s->szachownica, sizeof(s->szachownica), 0) < 0){
                    perror("wyslanie szachownicy");
                    exit(EXIT_FAILURE);
                }
                if(s->tura=='C'){
                    s->tura='B';
                }else{
                    s->tura='C';
                }
            }else if(odp==1){
                if (send(newSocket, odp, 1, 0) < 0){
                    perror("zly ruch");
                    exit(EXIT_FAILURE);
                }
            }
            //jesli tak wysylasz pozwolenie 1 lub 0 nie
            //po pozwoleniu wyslij tablice

        }else{
            if (send(newSocket, czekanieNaTure, 1, 0) < 0){
                perror("wyslanie oczekiwanie na ruch gracza");
                exit(EXIT_FAILURE);
            }
        }
    }
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


    // Zamknij gniazdo serwera
    close(serverSocket);
    return 0;
}