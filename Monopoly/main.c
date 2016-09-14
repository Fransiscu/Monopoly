#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "functions.h"
#include "functions.c"

/*--------------------------------------
| Autore: Francesco soru
| Anno accademico: 2015-2016
| Scopo: Simulare funzionamento del
| gioco del monopoli.
--------------------------------------*/


int main(){
    srand(time(NULL));                          //inizializzo il seed per i numeri random
    int i=0, k, dices, choice, nPlayers, endRound, check, stop, rounds=1, playerTurn=0, toBuy, maxSame=1; //dichiaro variabili varie che usero' nel programma
    char scelta;    //dichiaro variabiel char usata nel programma
    Player * array=NULL;    //dichiaro puntatore ad array e inizializzo a NULL in attesa di essere allocato
    _Bool same, done, endGame=false;        //dichiaro variabili booleane che saranno utilizzate per lo piu' a scopo di gestire i loop del programma
    FILE *fp=fopen("save.dat", "rb");           //dichiaro fp e apro file di salvaaggio
    FILE *ListaSfighe=NULL;                     //dichiaro puntatore a ListaSfighe e inizializzo a 0, sara' usato in una funzione
    Room rooms[ROOM_NUMBER]={{0, 0, 0, 0, "Go", 0, 0, 0, 0, 0, 0, 0}, {60, 0, 0, 0, "Aula G", 0, 0, 0, 0, 1, 0, 0},             //inizializzo array delle stanze
                              {0, 0, 0, 0, "Sfiga", 1, 0, 0, 9, 2, 0, 0}, {60, 0, 0, 0, "Aula C", 0, 0, 0, 0, 3, 0, 0},
                              {80, 0, 0, 0, "Aula F", 0, 0, 0, 0, 4, 0, 0}, {0, 0, 0, 0, "Tasse!", 2, 0, 0, 9, 5, 100, 0},
                              {0, 0, 0, 0, "Aula studio", 0, 0, 0, 0, 6, 0, 0}, {0, 0, 0, 0, "Sfiga", 1, 0, 0, 9, 7, 0, 0},
                              {0, 0, 0, 0, "SIMAZ", 0, 0, 0, 0, 8, 0, 0}, {0, 0, 0, 0, "Lab T", 0, 0, 0, 0, 9, 0, 0},
                              {0, 0, 0, 0, "Bagni", 0, 0, 0, 9, 10, 0, 1}, {140, 0, 0, 0, "VAX", 0, 0, 0, 0, 11, 0, 0},
                              {0, 0, 0, 0, "Sfiga", 1, 0, 0, 0, 12, 0, 0}, {140, 0, 0, 0, "GAR", 0, 0, 0, 0, 13, 0, 0},
                              {160, 0, 0, 0, "Biblioteca", 0, 0, 0, 0, 14, 0, 0}, {0, 0, 0, 0, "Tasse", 2, 0, 0, 9, 15, 150, 0},
                              {180, 0, 0, 0, "Aula B", 0, 0, 0, 0, 16, 0, 0}, {0, 0, 0, 0, "Sfiga", 1, 0, 0, 9, 17, 0, 0},
                              {180, 0, 0, 0, "Segreteria", 0, 0, 0, 0, 18, 0, 0}, {200, 0, 0, 0, "Aula A", 0, 0, 0, 19, 0, 0},
                              {0, 0, 0, 0, "Corridoio", 0, 0, 0, 9, 20, 0, 0}, {220, 0, 0, 0, "Ufficio Scateni", 0, 0, 0, 0, 21, 0, 0},
                              {0, 0, 0, 0, "Sfiga", 1, 0, 0, 9, 22, 0, 0}, {220, 0, 0, 0, "Bagno prof.", 0, 0, 0, 0, 23, 0, 0},
                              {240, 0, 0, 0, "Aula D", 0, 0, 0, 0, 24, 0, 0}, {0, 0, 0, 0, "Tasse!", 2, 0, 0, 9, 25, 250, 0},
                              {260, 0, 0, 0, "Lab 3", 0, 0, 0, 0, 26, 0, 0}, {0, 0, 0, 0, "Sfiga", 1, 0, 0, 9, 27, 0, 0},
                              {260, 0, 0, 0, "Lab 4", 0, 0, 0, 0, 28, 0, 0}, {280, 0, 0, 0, "Lab M", 0, 0, 0, 0, 29, 0, 0},
                              {0, 0, 0, 0, "Batcaverna", 3, 0, 0, 0, 30, 0, 0}, {300, 0, 0, 0, "Aula M Fisica", 0, 0, 0, 0, 31, 0, 0},
                              {0, 0, 0, 0, "Sfiga", 1, 0, 0, 9, 32, 0, 0}, {300, 0, 0, 0, "Aula M Chimica", 0, 0, 0, 0, 33, 0, 0},
                              {320, 0, 0, 0, "Aula M Matematica", 0, 0, 0, 0, 34, 0, 0}, {0, 0, 0, 0, "Tasse!", 2, 0, 0, 9, 35, 350, 0},
                              {360, 0, 0, 0, "Bar rotondo", 0, 0, 0, 0, 36, 0, 0}, {0, 0, 0, 0, "Sfiga", 1, 0, 0, 9, 37, 0, 0},
                              {360, 0, 0, 0, "Aula anatomica", 0, 0, 0, 0, 38, 0, 0}, {400, 0, 0, 0, "Aula costa", 0, 0, 0, 0, 39, 0, 0}};

    welcome();          //funzione welcome, richiamta sul main

    /*Inizio un ciclo do-while che serve per chiedere all'utente cosa ha intenzione di fare all'interno del programma*/
    do{
    printf("1. Inizia nuova partita\t2. Riprendi partita\n");
    scanf("%d", &choice);                   //registro la scelta
    getchar();                              //getchar per mangiare il carattere invio di troppo
    switch(choice){                         //switchcase con la scelta del giocatore
    case 1: clear(); setup(&array, &nPlayers); assignRooms(array, rooms, nPlayers); initializeRooms(rooms); done=true; getchar(); break;   //se sceglie 1 inizializzo tutto l'ambiente di "lavoro" richiamando tutte le funzioni di setup che ho creato
    case 2:             //se l'utente sceglie 2
        if (exists(fp)==true){          //controllo l'esistenza del file
            if (isEmpty(fp)==false){     //controllo che non sia vuoto
                printf("File vuoto.\n"); done=false; break;     //nel caso lo dico e ripeto il ciclo con break
            }
        readSaveFile(fp, &nPlayers, &array, rooms, &k);     //altrimenti richiamo la funzione per leggere il file di salvataggio
        done=true; break;                                    //done=true vuol dire che possiamo uscire dal while
        }

         else{
            printf("File non esistente.\n"); done=false; break;     //stampo che il file non esiste in caso sia necessario
         };
    default: printf("Input non valido\n");done=false; break;            //se l'input inserito non soddisfa le richieste lo scrivo e faccio ripetere il ciclo settando done=false
    }}while (done==false);
    waitFor(2);     // tempo di attesa
    clear();        //pulisco console


    if (nPlayers==1){
        printf("La partita e' terminata\nIl giocatore %s e' il vincitore!\n", array[0].playerName);
        exit(1);
    }
    if (nPlayers==0){
        printf("Partita finita in parita'!\n");
        exit(1);
    }
    /**Inizio il ciclo while principale del programma che ci permettera' di tirare i dadi e fare le mosse finche' il giocatore non decidera' di interromperlo**/
    do{

    printPlayersInfos(array, nPlayers); //stampo informazioni di base sui giocatori a inizio di ogni round come richiesto nelle specifiche
    getchar();                          //getchar
    do{                                 //ciclo while per chiedere all'utente cosa fare a inizio round
    printf("Come vuoi procedere?\n'T' Tira i dadi\t  'S' Salva la partita \t'E' Salva ed esci\n");
    scanf("%c", &scelta);               //registro la scelta qui
    switch(scelta){
    case 't':
    case 'T': done=true; break;         //se sceglie t allora vuole tirare quindi esco subitodal ciclo
    case 's':
    case 'S': printf("Partita salvata con successo.\n"); saveGame(fp, array, rooms, nPlayers, i, rounds); done=true; break; //se vuole salvare la partita richiamo la funzione savegame() ed esco dal ciclo
    case 'E':
    case 'e':printf("Partita salvata con successo.\n"); saveGame(fp, array, rooms, nPlayers, i, rounds); exit (1);      //se vuole salvare ed uscire richiamo savegame ed esco con exit(1)
    default: printf("Inserire uno fra i valori specificati.\n"); done=false; waitFor(1); clear(); done=false; break;     //nel caso venga inserito un valore non specificato alora faccio ripetere il ciclo
    }}while(done==false);


    /*Questo ciclo for non dovrebbe presentare al suo interno un i=0, l'ho dovuto ripristinare in seguito a problemi relativi al caricamento del giocatore che deve
    tirare il dado, in quanto mi e' risultato impossibile farlo funzionare.. non sono riuscito a trovare l'errore.
    In fondo ho dunque eliminato il salvataggio in mezzo ai turni dei singoli giocatori per evitare problemi di ripetizione dei turni */
    for (i=0; i<nPlayers; i++){         //inizio ciclo for che ci fara' tirare i dadi e muovere i giocatori con conseguenti modifiche
        stop = 0;                       //inizializzo la variabile stop
        printf("\nE' il turno di %s\n", (array)[i].playerName);     //dico di chi e' il turno
        stop=checkStops(array, i);                  //uso stop per controllare che il giocatore non sia in un turno di stop
        if (stop!=0){                           // se lo e'
            printf("Stop! Non puoi muoverti per altri %d turni\n", stop);       //stampo a schermo
            array[i].stopTurns-=1;                                          //diminuisco i turni di stop di 1
            continue;                       //faccio scalare di 1 il ciclo for
        }
        dices=diceRoll(&same);              //tiro i dadi con la funzione diceRoll e salvo su same 1 o 0 a seconda che i dadi siano doppi o meno
        printf("Hai tirato i dadi. Il risultato e': %d\n", dices);      //risultato
        printf("La tua posizione precedente era %d\n", array[i].playerPosition);        //posizione precedente
        movePlayer(dices, array, i, 0);                                                 //moveplayer per muovere il giocatore
        printf("La tua posizione attuale e' %d\n", array[i].playerPosition);            //nuova posizione

        check=checkRoom(array, rooms, i);                                           //controllo la stanza in cui mi trovo
        checkRoomResult(check);                                                     //uso checkroomresult per stampare a schermo una frase che riguarda il tipo di stanza
        if (check==0){                                  //se check e' = 0
            if (checkOwner(array, rooms, i, array[i].playerPosition)==1){       //controllo proprietario della stanza con checkowner
                printf("Sei il proprietario della stanza!\n");                  //se e'=1 allora il gicoatore possiede la stanza
                if(checkForniture(rooms, array[i].playerPosition)==3){          //controlliamo cosa c'e' dentro la stanza
                    printf("Possiedi sia scrivania che tenda\n");               //se ==3 allora il giocatore ha sia scrivania che tenda
                }
                    else if (checkForniture(rooms, array[i].playerPosition)==1){        //se ?=1 allora non possiede niente
                        printf("Non possiedi alcuna scrivania in questa stanza, desideri comprarla? Premi 1 per confermare\nIl prezzo e' %d\n", DESK);      //viene richiesto di comprare
                        scanf("%d", &toBuy);        //registro risposta
                        if (toBuy==1){              //se ==1
                        buyDesk(array, rooms, i, array[i].playerPosition);  //compro la scrivania
                        array[i].deskAmount+=1;                                 //aumento numero di scrivanie totali
                        printf("Acquisto completato con successo\n");           //conferma
                        }
                        else printf("Non verra' effetuato alcun addebito\n");   //altrmenti non tolgo soldi al giocatore
                    }
                    else if (checkForniture(rooms, array[i].playerPosition)==2){    //se ==2 allora il giocatore ha scrivania e non tenda
                        printf("Possiedi una scrivania ma non una tenda, desideri comprarla? Premi 1 per confermare\nIl prezzo e' %d\n", TENT);
                            scanf("%d", &toBuy);            //registro l'azione del giocatore
                            if (toBuy==1){
                                buyTent(array, rooms, i, array[i].playerPosition);      //se vuole comprare, tolgo soldi
                                array[i].tentAmount+=1;                                     //aumento numero tende
                            }
                            else printf("Non verra' effetuato alcun addebito\n");           //se non vuole allora nessun addebito
                    }
                    else if (checkForniture(rooms, array[i].playerPosition)==0){            //righe scritte solo perche' la funzione ha un return 0;
                        printf("???");                                                      //chiaramente un caso del genere non deve mai capitare, non so se si possano cancellare tranquillamente e nel dubbio le lascio
                    }

            }
            else if (checkOwner(array, rooms, i, array[i].playerPosition)==8){      //se il proprietario e' il banco (possessore precedente eliminato)
                printf("Il banco e' il proprietario della stanza, paga!\n");
                payTaxes(array, rooms, i, rooms[array[i].playerPosition].passPrice);            //faccio pagare i soldi sotto forma di tasse
            }
            else if ((array[rooms[array[i].playerPosition].owner].stopTurns==0) && (rooms[array[i].playerPosition].owner !=8)){     //se un alro giocatore e' proprietario
                    printf("Il giocatore %d e' proprietario/a di questa stanza, dovrai pagare il pedaggio!\n", rooms[array[i].playerPosition].owner); //lo scrivo
                    if (array[rooms[array[i].playerPosition].owner].stopTurns!=0){      //se il possessore e' in un turno di stop
                        printf("Il giocatore si trova in un turno di stop, quindi non dovrai pagare!\n");       //non si paga
                    }
                    payOwner(array, rooms, i, rooms[array[i].playerPosition].owner, array[i].playerPosition);       //altrimenti faccio pagare il totale al proprietario
                }
                else if (array[i].playerPosition==10){      //gestisco a parte il caso particolare della casella 10
                    printf("Sei capitato nei bagni!\n");            //in cui non succede niente di particolare
                }

        }
        if (check==1){      //se il tipo di stanza e' 1
            assignSfiga(array, i, ListaSfighe);     //chiamo la funzione assignsfiga con il file puntatore listasfighe
        }
        if (check==2){          //se il tipo di stanza e' 2
            printf("Pagherai ora le tasse!\nTi viene a costare la bellezza di: %d\n", rooms[array[i].playerPosition].taxesAmount);
            payTaxes(array, rooms, i, 0);       //faccio pagare le tasse
        }
        if (check==3){      //se il tipo di stanza e' 3
            movePlayer(0, array, i, 10);    //sposto il giocatore nei bagni
            assignPenalty(array, i, 3);     //assegno3 turni di stop
        }
        if (same==true){    //se i dadi sono doppi
            stop=checkStops(array, i); //controllo gli stop
                if (stop!=0){
                printf("Stop! Non puoi muoverti per altri %d turni\n", stop);   //stampo quanti turni di stop ci sono se ci sono
                array[i].stopTurns-=1;      //decremento di 1
                continue; //continuo il ciclo for
            }
            printf("\nEntrambi i dadi hanno dato lo stesso numero %d, puoi ritirare!\n", dices/2);      //se hanno dato entrambi un numero uguale
            while (same==true){         //inizio ciclo for che si ripetera' finche i dadi daranno uguali oppure maxsame sara' uguale a 3
                stop=checkStops(array, i); //controllo ancora i turni di stop nel caso in cui il giocatore sia incappato in un turno di stop durante l'ultimo tiro di dadi doppi
                if (stop!=0){           //se e' in stop
                printf("Stop! Non puoi muoverti per altri %d turni\n", stop);
                array[i].stopTurns-=1;  //diminuisco il numero di turni
                continue; //vado avanti
                }
                else {      //altrimenti
                    waitFor(1);
                    dices=diceRoll(&same); //ritiro i dadi
                    printf("Ritiro i dadi! Risultato %d\n", dices);
                    movePlayer(dices, array, i, 0);     //sposto giocatore
                    printf("La tua posizione e' ora %d\n", array[i].playerPosition);
                    check=checkRoom(array, rooms, i);       //controllo il tipo di stanza
                    checkRoomResult(check);
                    if (check==0){                                  //se check e' = 0
                        if (checkOwner(array, rooms, i, array[i].playerPosition)==1){       //controllo proprietario della stanza con checkowner
                            printf("Sei il proprietario della stanza!\n");                  //se e'=1 allora il gicoatore possiede la stanza
                        if(checkForniture(rooms, array[i].playerPosition)==3){          //controlliamo cosa c'e' dentro la stanza
                        printf("Possiedi sia scrivania che tenda\n");               //se ==3 allora il giocatore ha sia scrivania che tenda
                        }
                        else if (checkForniture(rooms, array[i].playerPosition)==1){        //se ?=1 allora non possiede niente
                            printf("Non possiedi alcuna scrivania in questa stanza, desideri comprarla? Premi 1 per confermare\nIl prezzo e' %d\n", DESK);      //viene richiesto di comprare
                            scanf("%d", &toBuy);        //registro risposta
                                if (toBuy==1){              //se ==1
                                buyDesk(array, rooms, i, array[i].playerPosition);  //compro la scrivania
                                array[i].deskAmount+=1;                                 //aumento numero di scrivanie totali
                            printf("Acquisto completato con successo\n");           //conferma
                            }
                            else printf("Non verra' effetuato alcun addebito\n");   //altrmenti non tolgo soldi al giocatore
                        }
                            else if (checkForniture(rooms, array[i].playerPosition)==2){    //se ==2 allora il giocatore ha scrivania e non tenda
                                printf("Possiedi una scrivania ma non una tenda, desideri comprarla? Premi 1 per confermare\nIl prezzo e' %d\n", TENT);
                                scanf("%d", &toBuy);            //registro l'azione del giocatore
                                if (toBuy==1){
                                buyTent(array, rooms, i, array[i].playerPosition);      //se vuole comprare, tolgo soldi
                                array[i].tentAmount+=1;                                     //aumento numero tende
                                }
                                else printf("Non verra' effetuato alcun addebito\n");           //se non vuole allora nessun addebito
                            }
                        else if (checkForniture(rooms, array[i].playerPosition)==0){            //righe scritte solo perche' la funzione ha un return 0;
                            printf("???");                                                      //chiaramente un caso del genere non deve mai capitare, non so se si possano cancellare tranquillamente e nel dubbio le lascio
                        }

                    }
                    else if (checkOwner(array, rooms, i, array[i].playerPosition)==8){      //se il proprietario e' il banco (possessore precedente eliminato)
                    printf("Il banco e' il proprietario della stanza, paga!\n");
                    payTaxes(array, rooms, i, rooms[array[i].playerPosition].passPrice);            //faccio pagare i soldi sotto forma di tasse
                    }
                else if ((array[rooms[array[i].playerPosition].owner].stopTurns==0) && (rooms[array[i].playerPosition].owner !=8)){     //se un alro giocatore e' proprietario
                    printf("Il giocatore %d e' proprietario/a di questa stanza, dovrai pagare il pedaggio!\n", rooms[array[i].playerPosition].owner); //lo scrivo
                    if (array[rooms[array[i].playerPosition].owner].stopTurns!=0){      //se il possessore e' in un turno di stop
                        printf("Il giocatore si trova in un turno di stop, quindi non dovrai pagare!\n");       //non si paga
                    }
                    payOwner(array, rooms, i, rooms[array[i].playerPosition].owner, array[i].playerPosition);       //altrimenti faccio pagare il totale al proprietario
                }
                else if (array[i].playerPosition==10){      //gestisco a parte il caso particolare della casella 10
                    printf("Sei capitato nei bagni!\n");            //in cui non succede niente di particolare
                }

        }
        if (check==1){      //se il tipo di stanza e' 1
            assignSfiga(array, i, ListaSfighe);     //chiamo la funzione assignsfiga con il file puntatore listasfighe
        }
        if (check==2){          //se il tipo di stanza e' 2
            printf("Pagherai ora le tasse!\nTi viene a costare la bellezza di: %d\n", rooms[array[i].playerPosition].taxesAmount);
            payTaxes(array, rooms, i, 0);       //faccio pagare le tasse
        }
        if (check==3){      //se il tipo di stanza e' 3
            movePlayer(0, array, i, 10);    //sposto il giocatore nei bagni
            assignPenalty(array, i, 3);     //assegno3 turni di stop
        }

                if (same==true){        //se same e' ancora ture
                    maxSame++;          //incremento il numero di same (che a inizio programma avevo inizializzato a 1)
                    printf("\nAttento! Puoi fare dadi doppi solo 3 volte di fila! N. %d\n", maxSame);   //stampo un avvertimento
                }
                if (maxSame==3){         //se maxsame ==3
                movePlayer(0, array, i, 10);    //sposto giocatore nei bagni
                assignPenalty(array, i, 3);     //assegno turni di stop
                printf("Bagni! Nuova posizione %d. Passerai li' %d turni!\n", array[i].playerPosition, array[i].stopTurns); //stampo a schermo un messaggio
                continue;   //vado avanti con il ciclo for
                }
                printf("\n");
                waitFor(1);
                }
            }
            maxSame=1;      //resetto valore maxsame
        }
        playerTurn++;       //incremento valore playerturn
        printf("\nInserire 1 per continuare, 2 per salvare o un altro tasto per uscire\n"); //chiedo cosa fare per andare avanti nel programma
        scanf("%d", &endRound);
        getchar();
        if (endRound==1){
            saveGame(fp,array, rooms, nPlayers, playerTurn, rounds);
        }                                           //regitro la scelta
        if (endRound==2){                                                   //se ==2
            saveGame(fp, array, rooms, nPlayers, playerTurn, rounds);       //salvo la partita comunque
        }
        else if (endRound!=1 && endRound!=2){
            saveGame(fp, array, rooms, nPlayers, playerTurn, rounds);
            exit (1);
        }
        waitFor(3); printf("\n");
    }
    saveGame(fp, array, rooms, nPlayers, playerTurn, rounds);                   //altrimenti salvo la partita
    checkBalance(array, &nPlayers, rooms);
    //array=realloc(array, (nPlayers*sizeof(Player *)));                                  //richiamo checkbalance
    if (nPlayers==1){                                                           //se e' rimasto un solo giocatore
        printf("Il giocatore %s e' il vincitore!", array[0].playerName);        //stampo a schermo
        waitFor(.5);
        endGame=true;
        saveGame(fp, array, rooms, nPlayers, playerTurn, rounds);                                                            //setto endgame=true
        return 0;                                                                //return 0 per terminare il programma
    }
    if (nPlayers<1){
        printf("Tutti i giocatori sono stati elminati l'ulimo round!\n");       //qui tengo conto del caso in cui tutti i giocatori rimanenti siano stati eliminati all'ultimo round
        waitFor(.5);
        endGame=true;
        saveGame(fp, array, rooms, nPlayers, playerTurn, rounds);
        return 0;
    }
    saveGame(fp, array, rooms, nPlayers, i, rounds);                    //salvo partita
    rounds++;                                                           //incremento rounds
    waitFor(.5); clear();
    }while(endGame==false);          // FINE while() iniziato prima del ciclo for

    fclose(fp);                     //chiudo il file fp
    free(array);                    //libero lo spazio di memoria dell'array Player
    return 0;                       //rendo 0
}

