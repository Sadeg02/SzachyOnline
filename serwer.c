#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include "komendy.c"
#include "szachownica.c"
#include <stdbool.h>


#define MAXSTOLY 3 //liczba stolow na serwerze
#define PORT 1100

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
void rozpocznij(int newSocket,char kolorgracza,stol* s);
void rozlaczenie(char komunikat[]);

void stworzStoly(int ile, stol stoly[]) {
    for (int i = 0; i < ile; i++) {
        inicjalizujSzachownice(&(stoly[i].szachownica));
        stoly[i].ilegraczy = 0;
        stoly[i].koniec = 0;
        stoly[i].dostepny = true;
        stoly[i].tura='B';
    }
}
void rozlaczenie(char komunikat[]){
    printf("%s \n",komunikat);
}

void *socketThread(void *arg) {
    printf("nowy gracz \n");
    int newSocket = *((int *) arg);
    int brak_stolow = 0;
    int sa_stoly = 1;

    char kolorgracza = ' ';
    int id = -1;
    // Tutaj możesz sprawdzić dostępność stołów
    for (int i = 0; i < MAXSTOLY; i++) {
        if (stoly[i].dostepny) {
            id = i;
            stoly[i].ilegraczy += 1;
            kolorgracza = 'B';
            if (stoly[i].ilegraczy == 2) {
                kolorgracza = 'C';
                stoly[i].dostepny = false;
            }
            break;
        }
    }
    //informacja o stole
    if (id == -1) {
        if (send(newSocket, &brak_stolow, sizeof(int), 0) < 0) {
            perror("blad brak stolow");
            //exit(EXIT_FAILURE);
        }
        pthread_exit(NULL);
    } else {
        if (0 > send(newSocket, &sa_stoly, sizeof(int), 0)) {
            perror("blad sa stoly");
            //exit(EXIT_FAILURE);
        }
    }

    //jest stol rozpocznij rozgrywke
    rozpocznij(newSocket, kolorgracza, &stoly[id]);

    // Zamknij gniazdo klienta i zwolnij stol
    printf("odlaczony klient \n");
    stoly[id].ilegraczy -= 1;
    if (stoly[id].ilegraczy == 0) {
        stoly[id].dostepny=true;
        inicjalizujSzachownice(&(stoly[id].szachownica));
    }
    close(newSocket);
    pthread_exit(NULL);
}

void rozpocznij(int newSocket,char kolorgracza,stol* s){
    //komendy  do interfejsu klienta
    const int oczekiwanie = 2;
    const int pozwolenieRuch =3;
    const int czekanieNaTure =4;
    const int rozlaceniegracza=5;
    const int winBiale=6;
    const int winCzarne=7;

    int online;
    printf("kolor %c \n",kolorgracza);
    if (send(newSocket, &kolorgracza, sizeof(kolorgracza), 0) < 0){
        perror("wyslanie koloru");
        //exit(EXIT_FAILURE);
    }
    while(true){
        if (recv(newSocket,&online ,sizeof(int), 0) < 1) {
            rozlaczenie("Rozlaczylo gracza");
            break;
        }
        if(s->dostepny==true){
            if (send(newSocket,&oczekiwanie, sizeof(int), 0) < 0){
                perror("wyslanie oczekiwanie");
                exit(EXIT_FAILURE);
            }
        }else if(czyKoniec(&(s->szachownica))>0){
            if(czyKoniec(&(s->szachownica))==1){//wygraly biale
                if (send(newSocket, &winBiale, sizeof(int), 0) < 0){
                    perror("wyslanie win biale");
                    exit(EXIT_FAILURE);
                }
                if(kolorgracza=='C'){
                    if (send(newSocket, &(s->szachownica), sizeof(s->szachownica), 0) < 0){
                        perror("wyslanie szachownicy");
                        exit(EXIT_FAILURE);
                    }
                }
                printf("Gre wygral bialy gracz\n");
                break;
            }else if(czyKoniec(&(s->szachownica))==2){//wygraly czarne
                if (send(newSocket, &winCzarne, sizeof(int), 0) < 0){
                    perror("wyslanie win czarne");
                    exit(EXIT_FAILURE);
                }
                if(kolorgracza=='B'){
                    if (send(newSocket, &(s->szachownica), sizeof(s->szachownica), 0) < 0){
                        perror("wyslanie szachownicy");
                        exit(EXIT_FAILURE);
                    }
                }
                printf("Gre wygral czarny gracz\n");
                break;
            }
        }else if(s->ilegraczy<2){
            //inny gracz odlaczyl sie
            rozlaczenie("Rozlaczylo z powodu utraty drugiego gracza");
            if (send(newSocket, &rozlaceniegracza, sizeof(int), 0) < 0){
                perror("wyslanie pozwolenia na ruch");
                exit(EXIT_FAILURE);
            }
            break;
        }else if(s->tura==kolorgracza){
            char rozkaz[50];
            int odp;
            if (send(newSocket, &pozwolenieRuch, sizeof(int), 0) < 0){
                perror("wyslanie pozwolenia na ruch");
                exit(EXIT_FAILURE);
            }
            if (send(newSocket, &(s->szachownica), sizeof(s->szachownica), 0) < 0){
                perror("wyslanie szachownicy");
                exit(EXIT_FAILURE);
            }
            if (recv(newSocket, &rozkaz,sizeof(rozkaz), 0) < 0){
                rozlaczenie("dostepnosc error");
                break;
                //exit(EXIT_FAILURE);
            }
            //sprawdzanie rozkazu
            odp=ruch(&(s->szachownica),rozkaz,kolorgracza);

            if(odp==2){
                if (send(newSocket, &odp, sizeof(int), 0) < 0){
                    perror("dobry ruch");
                    exit(EXIT_FAILURE);
                }
                if (send(newSocket, &(s->szachownica), sizeof(s->szachownica), 0) < 0){
                    perror("wyslanie szachownicy");
                    exit(EXIT_FAILURE);
                }
                if(s->tura=='C'){
                    s->tura='B';
                }else{
                    s->tura='C';
                }
            }else if(odp==1){
                if (send(newSocket, &odp, sizeof(int), 0) < 0){
                    perror("zly ruch");
                    exit(EXIT_FAILURE);
                }
            }
            //jesli tak wysylasz pozwolenie 1 lub 0 nie
            //po pozwoleniu wyslij tablice

        }else{
            if (send(newSocket, &czekanieNaTure, sizeof (int), 0) < 0){
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
    serverSocket = socket(PF_INET, SOCK_STREAM, 0);
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
    if (listen(serverSocket, 20) == 0)
        printf("Serwer nasłuchuje na porcie %d\n", PORT);
    else {
        perror("Błąd podczas nasłuchiwania");
        exit(EXIT_FAILURE);
    }

    pthread_t thread_id;

    while (1) {
        // Akceptuj połączenia i twórz nowe wątki dla każdego klienta.
        addr_size = sizeof serverStorage;
        newSocket = accept(serverSocket, (struct sockaddr *) &serverStorage, &addr_size);

        if (pthread_create(&thread_id, NULL, socketThread, &newSocket) != 0) {
            perror("Błąd podczas tworzenia wątku obsługi klienta");
            close(newSocket);
            continue;
        }
        // Detach wątku, aby zwolnić zasoby automatycznie po zakończeniu wątku.
        pthread_detach(thread_id);
    }

    fflush(stdout);
    // Zamknij gniazdo serwera
    close(serverSocket);
    return 0;
}