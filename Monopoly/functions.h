#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED
#include <stdio.h>
#include <time.h>
#define MAXCHAR 24
#define DESK 50
#define TENT 100
#define ROOM_NUMBER 40
#define MAX_MONEY 12000
#define MOLTIPLICATORE_PEDAGGIO 200

/* Dichiaro le due enumerazioni usate in questo programma */
typedef enum {room=0, sfiga=1, tax=2, other=3, nothing=4}RoomType;
typedef enum {player1=0, player2=1, player3=2, player4=3, player5=4, player6=5, player7=6, player8=7, banco=8}PlayerNumber;

/* Dichiaro le due strutture usate in questo programma */
typedef struct{
    char playerName[MAXCHAR];
    PlayerNumber playerNumber;
    int playerBalance;
    int stopTurns;
    int roomAmount;
    int playerPosition;
    int deskAmount;
    int tentAmount;
    int roomValue;
    int flagOut;
}Player;

typedef struct{
    int price;
    int passPrice;
    int priceDesk;
    int priceTent;
    char roomName[MAXCHAR];
    RoomType n;
    _Bool desk;
    _Bool tent;
    PlayerNumber owner;
    int roomNumber;
    int taxesAmount;
    _Bool batCave;
}Room;

/*Semplice funzione che stampa delle stringhe come benvenuto al programma*/
void welcome();

/*Funzione con cui faccio il primo setup della partita in caso non ci sia da caricare alcun salvataggio*/
void setup(Player **, int *);

/*Controllo se il file e' vuoto*/
_Bool isEmpty(FILE *);

/*Controllo se il file esiste*/
_Bool exists (FILE *);

/*Funzione per allocare in memoria un array*/
void allocate(Player **, int);

/*Inizializzo i giocatori e i campi principali della struttura Player*/
Player* initializePlayers(Player *, int);

/*Assegno prezzi per il pedaggio a seconda della presenza di scrivania o tenda*/
Room* initializeRooms(Room *);

/*Simulazione tiro dei dadi*/
int diceRoll(_Bool *);

/*Funzione per spostare i giocatori nella mappa*/
Player* movePlayer(int, Player *, int, int);

/*Funzione che serve per controllare che tipo di stanza sia quella in cui capita il giocatore*/
int checkRoom (Player *, Room *, int);

/*Funzione che a seconda del risultato di checkroom() stampa un output diverso, potrei liberarmene*/
void checkRoomResult(int);

/*Funzione per controllare se il giocatore e' in un turno di stop*/
int checkStops(Player *, int);

/*Funzione per assegnare stop ai giocatori*/
void assignPenalty(Player *, int, int);

/*Funzione che stampa le informazioni base dei giocatori*/
void printPlayersInfos(Player *, int);

/*Funzione per l'assegnazione random delle stanze a inizio partita*/
void assignRooms(Player *, Room *, int);

/*Funzione per controllare chi e' il proprietario di una stanza*/
int checkOwner(Player *, Room *, int, int);

/*Funzione per controllare che tipo di "arredamento" e' presente in una stanza*/
int checkForniture (Room *, int);

/*Funzione che controlla il bilancio dei giocatori a fine turno per vedere se qualcuno e' andato in bancarotta*/
void checkBalance(Player *, int *, Room *);

/*Funzione che assegna una scrivania ad un giocatore*/
void buyDesk(Player *, Room *, int, int);

/*Funzione che assegna una tenda ad un giocatore*/
void buyTent(Player *, Room *, int, int);

/*Funzione per trasferire denaro fra giocatori*/
void payOwner(Player *, Room *, int, int, int);

/*Funzione per pagare le tasse*/
void payTaxes(Player *, Room *, int, int);

/*Funzione per assegnare le sfighe casualmente*/
void assignSfiga(Player *, int, FILE *);

/*Funzione per il salvataggio del gioco*/
void saveGame(FILE *, Player *, Room *, int, int, int);

/*Funzione per il caricamento del gioco*/
void readSaveFile(FILE *, int *, Player **, Room *, int *);

/*Semplice funzione per introdurre un intervallo di tempo*/
void waitFor (unsigned int);

/*Semplice funzione per la pulizia della console*/
void clear();

#endif // FUNCTIONS_H_INCLUDED
