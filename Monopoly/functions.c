#include <stdio.h>
#include "functions.h"

/*Funzione benvenuto in cui stampo alcune frasi a schermo*/
void welcome(){
    printf("Benvenuto al gioco del monopoly!\n");
    printf("Progetto di PR1, AA 2015/2016\n");
    printf("Desideri iniziare una nuova partita o caricarne una salvata in precedenza?\n");
}


_Bool isEmpty(FILE *fp){
    fseek(fp, 0, SEEK_END);
    if (ftell(fp) == 0){
        return false;
    }
    rewind(fp);
    fclose(fp);
    return true;
}


Player* allocate(Player **a, int n) { //passo un doppio puntatore perche' dovro' chiamare questa funzione da un'altra e la voglio rendere visibile dal main
  *a = (Player *) malloc (sizeof(Player)*n); //funzione malloc per allocare la memoria
  return *a;                                    //rendo puntatore ad array
}


void setup(Player **array, int *nPlayers){
    int done=0;                                 //done per delimitare il ciclo while
    while (done==0){
    printf("Quanti giocatori vuoi usare?\n");
    scanf("%d", nPlayers);                      //registro numero giocatori
    if (*nPlayers <2 || *nPlayers>8){
        printf("Dichiarare un numero di giocatori compreso fra 2 e 8\n");
            waitFor(2);
            clear();
               done=0;
    }
    else done=1;
    }
    allocate(array, *nPlayers);                 //richiamo la funzione citata prima
    initializePlayers(*array, *nPlayers);          //uso la funzione initializeplayers riportata qui sotto
}

Player* initializePlayers(Player *a, int n){  //prendo in input l'array player e la n del numero giocatori
    int i=0;                                    //i per ciclo for
    char *tmp;                                  // puntatore tmp per salvare il nome
    tmp=malloc(sizeof(char)*MAXCHAR);           //alloco il puntatore tmp
    for(i=0; i<n; i++){                         //ciclo per chiedere il nome dei giocatori e inizializzare i valori di base
        do{
        printf("Inserire nome giocatore %d\n", i);
        scanf("%s", tmp);                       //salvo il nome sul puntatore tmp
        strcpy(a[i].playerName,tmp);              //lo copio nel campo nome del giocatore
        }while(strlen(tmp)<1 || strlen(tmp)>23);    //se vengono rispettati questi parametri
        printf("Giocatore %s registrato.\n", a[i].playerName);  //conferma registrazione
        a[i].playerNumber=i;                                    //assegno al giocatore un numero identificativo che usero' nel programma
        //a[i].playerBalance=(MAX_MONEY/n)-(a[i].roomValue); SPOSTATO IN ASSIGNROOMS PER COMODITA'
        a[i].playerPosition=0;                  //assegno posizione di partenza
        a[i].stopTurns=0;                       //assegno turni di stop di default
        a[i].roomAmount=0;                      //inizializzo il totale delle stanze a 0 in attesa di essere cambiato da assignrooms()
        a[i].roomValue=0;                       //inizializzo il valore complessivo delle aule possedute a 0
        a[i].deskAmount=0;                      //inizializzo totale cattedre possedute
        a[i].tentAmount=0;                      //inizializzo totale tende possedute
    }
    free(tmp);
    return a;                                   //rendo il vettore modificato
}

int checkForniture(Room * rooms, int n){            //prendo in ingresso l'array delle stanze e il numero della stanza da controllare
    if (rooms[n].desk==0) {return 1;}               //se non c'e' neanche la scrivania rendo 1
       else if (rooms[n].desk==1 && rooms[n].tent==0) {return 2;}   //se c'e' scrivania e non tenda rendo 2
            else if (rooms[n].desk==1 && rooms[n].tent==1){     //se ci sono entrambe rendo 3
                return 3;
            }

    return 0;
}

Room* initializeRooms(Room * rooms){
    int i;                                          //i per ciclo for
    for (i=0; i<ROOM_NUMBER; i++){                  //ciclo le stanze chiamando per ognuna la funzione checkforniture per vedere cosa e' presente nella stanza
        if (checkForniture(rooms, i)==1){           // se rende 1
        rooms[i].passPrice=(rooms[i].price/20)*MOLTIPLICATORE_PEDAGGIO;}    //calcolo il valore /20
        else if (checkForniture(rooms, i)==2){                              // se rende 2
        rooms[i].priceDesk=(rooms[i].price/10)*MOLTIPLICATORE_PEDAGGIO;}      //valore /10
        else if (checkForniture(rooms, i)==3){                                  //se rende 3
        rooms[i].priceTent=(rooms[i].price/5)*MOLTIPLICATORE_PEDAGGIO;}         //valore /5
    }
    return rooms;                       //rendo il vettore modificato
}


void printPlayersInfos(Player *a, int n){//prendo in ingresso l'array dei giocatori e il numero giocatori totale
    int i;
    for(i=0; i<n; i++){     //stampo a schermo le informazioni base cicando l'array di giocatori
        printf("Nome:%-24s\t N:%d\t Bilancio:%d\t Stanze:%d\t Desks:%d\tTents:%d\t Stopturns:%d\n", a[i].playerName, a[i].playerNumber, a[i].playerBalance, a[i].roomAmount, a[i].deskAmount, a[i].tentAmount, a[i].stopTurns);
    }
    printf("\n");
}

int diceRoll(_Bool *same){//prendo in ingresso un puntatore di tipo bool per tenere in considerazione se i dadi sono doppi o meno
    int n1, n2, res;
    srand(time(NULL));      //inizializzo per avere numeri random
    n1=1+rand()%6;          //primo dado
    n2=1+rand()%6;          //secondo dado
    res=n1+n2;              //sommo
    if (n1==n2){
        *same=1;            //modifico same a seconda che i dadi siano doppi o meno
    }
    else{
        *same=0;
    }
    return res;             //rendo res
}


_Bool exists (FILE *fp){                //prendo in ingresso puntatore a file
    if((fopen("save.dat","r"))!=NULL){      //se non si apre il vile
            fclose(fp);                     //chiudo
            return true;                    //rendo true
        }
    else{
            return false;                   //altrimenti rendo flase
        }
}

void assignRooms(Player * a, Room * r, int p){      //prendo in ingresso l'array dei giocatori, delle stanze e il numero giocatori totale
    srand(time(NULL));                              //inizializzo per numeri random
    int i, b, c, tmp;                               //variabili utilizzate in questa funzione
    for(i=1; i<40; i++){                            //ciclo le stanze
        b=0+rand()%p;                               //assegno a b un valore casuale fra 1 e numerogicoatori
        if (r[i].roomNumber==10 || r[i].roomNumber==20){    //salto le stanze 10 e 20
            continue;
        }
        if (r[i].n==0){                             //se il tipo della stanza == 0, ossia stanza normale
            r[i].owner=b;                           //assegno proprietario random
            a[b].roomAmount++;                      //aumento numero totale stanze di 1
            a[b].roomValue+=r[i].price;             //sommo prezzo totale delle aule
            c=1+rand()%9;               //cerco un numero random fra 1 e 9
            if (c>0 && c<4){            //se e' 1 2 3 ossia 1/3 di 9
                r[i].desk=1;            //assengo desk
                a[b].deskAmount++; // aumento conto desk
            }
        }
    }
    for(i=0;i<p;i++){               //parte di codce creata per risolvere un probema che a volte mi portava ad avere un bilancio
    tmp=(MAX_MONEY/p)-(a[i].roomValue); //iniziale inferiore a 0 perche' il valore delle stanze superava quello del budget iniziale
    if (tmp<0){
        tmp=1250;                   //assegno un valore simbolico
        }
    a[i].playerBalance=tmp;
    }
}

Player* movePlayer(int steps, Player * player, int i, int room){ //prendo in ingresso i passi da fare, array dei giocatori, numero giocatore, stanza in cui muoversi
    if (steps!=0){                          //se i passi sono diversi da 0 allora sposto il giocatore di tot passi
        (player)[i].playerPosition+=steps;
        if ((player)[i].playerPosition>39){     //tengo in considerazione il passaggio oltre l'inizio della tabella
            (player)[i].playerPosition-=40;
            (player)[i].playerBalance+=20;      //aggiungo 20 per il passaggio
        }
    }
    else {
        player[i].playerPosition=room;          //se i passi sono 0 allora sposto direttamente il gicoatore in una casella precisa
    }
    return player;                              //rendo l'array
}

int checkRoom(Player * player, Room * room, int a){     //prendo in ingresso l'array player, rooms, e il numero del giocatore
    int i;
    for (i=0; i<ROOM_NUMBER;i++){       //ciclo for per ciclare le stanze
        if (room[i].n!=0 && player[a].playerPosition==room[i].roomNumber){ //se il tipo di stanza non e' 0 (normale) e la psizione del giocatore non coincide con quella del ciclo
            if (room[i].n == 1){        //controllo il tipo di stanza
                return 1;               //rendo 1
            }
            else if (room[i].n == 2){
                return 2;                  //rendo 2
            }
            else if (room[i].n == 3){
                return 3;                  //rendo 3
            }
        }
    }
    return 0;
}

void checkBalance(Player * a, int *nPlayers, Room * rooms){ //prendo in ingresso l'array player, numero giocatori via vettore e array stanze
    int i, j, k, n;             //variabili usate all'interno di questa funzione
    for(i=0; i<*nPlayers;i++){      //ciclo giocatori
        if (a[i].playerBalance<1){      //se il bilancio <1
            printf("%s e' stato eliminato!\n", a[i].playerName); //stampo eliminazione
            waitFor(2);
            for (k=0; k<ROOM_NUMBER; k++){      //trasferisco le stanze al giocatore 8, il banco
                if (rooms[k].owner==i){
                    rooms[k].owner=8;
                }
            }
            for (j=0; j<MAXCHAR; j++){
                a[i].playerName[j]=a[i+1].playerName[j+1];  //trasferisco il nome
            }
            for(n=i; n<*nPlayers; n++){         //faccio scalare tutti i valori di uno a sx
            a[n].playerBalance=a[n+1].playerBalance;
            a[n].playerPosition=a[n+1].playerPosition;
            a[n].roomAmount=a[n+1].roomAmount;
            a[n].deskAmount=a[n+1].deskAmount;
            a[n].tentAmount=a[n+1].tentAmount;
            a[n].stopTurns=a[n+1].stopTurns;
            a[n].roomValue=a[n+1].roomValue;
            }
            *nPlayers-=1;                        //diminuisco il numero dei giocatori totali via puntatore, che verra' letto dal main
            a=realloc(a, (*nPlayers*sizeof(Player *)));
        }
    }
}

void checkRoomResult(int i){    //prendo in ingresso iltipo di stanza e stampo a schermo una frase a seconda di questa
    if (i==1){                  //funzione base
            printf("Sfiga!\n");
        }
        else if (i==2){
            printf("Tasse!\n");
        }
        else if (i==3){
            printf("Stop! Ti verranno assegnati turni di pausa\n");
        }
}

int checkOwner(Player * a, Room * rooms, int i, int n){//prendo in ingresso array giocatori, stanze, numero giocatore e numero stanza
    if (a[i].playerNumber==rooms[n].owner)  //se il giocatore e' il proprietario della stanza
        return 1;                            //rendo 1
    else if (rooms[n].owner==8){            //se il banco e' proprietario
        return 8;                               //rendo 8
    }
    return 0;                               //altrimenti rendo 0
}

void buyDesk(Player * a, Room * rooms, int i, int n){   //prendo in ingresso array player, rooms, numero giocatore e numero stanza
    rooms[n].desk=true;                                 //inizializzo la scrivania nell'array rooms
    a[i].playerBalance-=DESK;                           //faccio scalare il bilancio
}

void buyTent(Player * a, Room * rooms, int i, int n){   //prendo in ingresso array player, rooms, numero giocatore e numero stanza
    rooms[n].tent=true;                                 //inizializzo la tenda nell'array rooms
    a[i].playerBalance-=TENT;                           //faccio scalare il bilancio
}

void payOwner(Player * a, Room * rooms, int i, int p, int n){   //prendo in ingresso l'array player, rooms, giocatore, possessore dela stanza, stanza
    if (checkForniture(rooms, n)==1){           //se non e' presente la scrivania
        a[i].playerBalance-=rooms[n].passPrice; //scalo soldi dal giocatore
        a[p].playerBalance+=rooms[n].passPrice; //aggiungo soldi al proprietario
    }
    else if (checkForniture(rooms, n)==2){      //se c'e' la scrivania
        a[i].playerBalance-=rooms[n].priceDesk; //scalo soldi dal giocatore
        a[p].playerBalance+=rooms[n].priceDesk; //aggiungo soldi al proprietario
    }
    else if (checkForniture(rooms, n)==3){      //se ci sono entrambe
        a[i].playerBalance-=rooms[n].priceTent; //scalo soldi dal giocatore
        a[p].playerBalance+=rooms[n].priceTent; //aggiungo soldi al proprietario
    }
}

void payTaxes(Player * a, Room * rooms, int i, int amount){//prendo in ingresso l'array player, rooms, giocatore e importo da pagare
    if (amount==0){     //se amount =0
    a[i].playerBalance-=rooms[a[i].playerPosition].taxesAmount; //ricerco dall'array rooms il prezzo di tasse da pagare
    }
    else if (amount!=0){                //se passo invece un valore diverso da 0
        a[i].playerBalance-=amount;     //faccio pagare direttamente il totale passato alla funzione
    }
}

int checkStops(Player * a, int i){  //prendo in ingresso l'array a e il numero del giocatore
    int res;                        //introduco res che se diverso da 0 rendo in risultato
    if (a[i].stopTurns==0){         //se non ha turni di pausa rendo 0
        return 0;
    }
    else {
        res=a[i].stopTurns;         //se ci sono dei turni di pausa, li assegno a res
    }
    return res;                     //e li rendo in output
}

void assignSfiga(Player * giocatore, int i, FILE * fp){ //prendo in ingresso array giocatore, numero giocatore e puntatore al file
    srand(time(NULL));                                  //inizializzo per numeri random
    char b[256];                                    //inizializzo array temporaneo
    int a[4];                                       //altro array temporaneo
    int nSfighe, num, count=0, sfiga, sum;          //introduco variabili usate nella funzione

    fp=fopen("ListaSfighe.txt","r");                //apro il vile di testo in modalita' lettura
    if (fp!=NULL){                                  //se va a buon fine

    fseek(fp, 0, SEEK_SET);                                     //rewind()
    fscanf(fp, "%d %d %d %d", &a[0], &a[1], &a[2], &a[3]);      //cerco quante righe totali ci sono nel file
    nSfighe=a[0]+a[1]+a[2]+a[3];                                //le assegno a nsfighe

    sfiga=1+rand()%nSfighe;                                     //do un valore random a sfiga
    count=1;                                                    //assegno 1 a count per saltare la riga 0 del file di testo

        while(count<=sfiga){                //inizio un ciclo while con cui scorro tutto il file di testo
        if (count==0){                      //essendo questa la riga 0 non verra' mai visualizzata per via di count=1;
        fseek(fp, 0, SEEK_SET);
            fscanf(fp, "%d %d %d %d", &a[0], &a[1], &a[2], &a[3]);  //assegno i primi 4 valori all'array a
            if (count==sfiga){
            printf("\n%d %d %d %d", a[0], a[1], a[2], a[3]);        //stampo a schermo
            }
            count++;                    //aumento count di 1
        }
        /*Metto le condizioni come numeri esatti piuttosto che come variabili (potrei mettere a[0]...a[3], perche' do per scontato che il
        file delle sfighe non sia stato modificiato, altrimenti sostituendo i valori li' il tutto funziona comunque */
        if (count>0&&count<11){         //se il conto e' compreso fra 1 e 10
            fseek(fp, 0, SEEK_CUR);
                fscanf(fp, "%d %[^\n]s", &num, b);  //assegno a num e b i valori memorizzati nel file di testo
                if (count==sfiga){                  //se il numero di count coincide con quello random della sfiga
                printf("%s \n", b);                //stampo a schermo
                payTaxes(giocatore, 0, i, num);     //faccio pagare tasse
                printf("\nHai pagato!\n");
                }
        count++;                                    //incremento di 1

        }
        if (count>=10 && count <19){                //se count e' compreso fra 10 e 18
            fseek(fp, 0, SEEK_CUR);
                fscanf(fp, "%d %d %d %[^\n]s", &a[1], &a[2], &a[3], b); //assegno valori all'array a e a b
                if (count==sfiga){                                      //se count coincide con sfiga
                printf("\n%s \n", b);                                  //stampo a schermo la frase
                printf("\nDevi pagare una tassa dipendente da quante scrivanie e tende possiedi\nScrivanie:%d\tTende:%d\n", giocatore[i].deskAmount, giocatore[i].tentAmount);
                sum=((a[1]*giocatore[i].roomAmount)+(a[2]*giocatore[i].deskAmount)+(a[3]*giocatore[i].tentAmount));
                giocatore[i].playerBalance-=sum;
                //faccio pagare al giocatore un totale calcolato a seconda delle sue scrivanie e tende possedute ^
                }
        count++;                                //incremento di 1

        }

        if (count>18 && count <30){         //se count e' compreso fra 19 e 29
            fseek(fp, 0, SEEK_CUR);
                fscanf(fp, " %[^\n]s ", b);     //assegno a b la frase del file di testo
                if (count==sfiga){
                printf("%s \n", b);            //la stampo a schermo
                }
            count++;                            //incremento di 1

        }

        if (count>=30 && count <40){        //se count e' compreso fra 30 e 39
            fseek(fp, 0, SEEK_CUR);
                fscanf(fp, "%d %[^\n]s", &a[1], b); //assegno il valore numerico alla casella a[1] del vettore a e la frase a b
                if (count==sfiga){              //se count e sfiga coincidono
                printf("%s \n", b);                //stampo la frase
                movePlayer(0, giocatore, i, a[1]);      //sposto giocatore su a[1]
                printf("\nNuova posizione: %d\n", giocatore[i].playerPosition); //stampo posizione finale
                }
        count++;    //incremento di 1

        }
        }

    }
    else{
    printf("File not existent\n"); //se il file non esiste, stampo questo
    }
    fclose(fp);                     //chiudo file
}

void saveGame(FILE * fp, Player * a, Room * r, int nPlayers, int playeri, int rounds){//prendo in ingresso puntatore al file, array player, rooms, numero giocatori, i del giocatore che deve tirare, e rounds
    fp=fopen("save.dat", "wb"); //apro file in modalita' di scrittura binaria
    int vett[3];                //inizializzo vettore di 3 posti
    vett[0]=nPlayers;           //a cui assegno i valori di nplayers
    vett[1]=rounds;             //rounds
    vett[2]=playeri;            //giocatore che deve tirare

    if (fp==NULL){              //se ci sono errori con il file
        printf("wut\n");
    }
    else{
        fwrite(vett, sizeof(int), 3, fp);       //scrivo sul file prima questi 3 elementi ripoartati su
        fwrite(a, sizeof(Player), nPlayers, fp);    //l'array player
        fwrite(r, sizeof(Room), ROOM_NUMBER, fp);   //l'array rooms
    }
    fclose(fp);             //chiudo il file
}

void readSaveFile(FILE *fp, int * nPlayers, Player ** a, Room * rooms, int *k){//prendo in ingresso il puntatore al file, puntatore a numero gicoatori, puntatore ad array players, rooms e il turno del giocatore
    fp=fopen("save.dat", "rb"); //apro il file in modalita' lettura binaria
    int array[3];               //inizializzo array a 3 posti
    int num;                    //inizializzo num
    fread(array, sizeof(int), 3, fp);       //leggo i primi 3 elementi e li assegno ad array
    printf("Number of players:%d Number of rounds:%d Player turn:%d", array[0], array[1], array[2]); //li stampo
    *k=array[2];    //do a k il valore dell'ultimo elemento della'rray
    num=array[0];           //do a num il valore del primo
    *nPlayers=num;          //che poi giro al puntatore nplayers
    printf("\n\n%d", *nPlayers);    //stampo a schermo
    allocate(a, *nPlayers);     //alloco array
    printf("\n");
    fread(*a, sizeof(Player), num, fp);     //leggo sequenzialmente l'array dei giocatori
    fread(rooms, sizeof(Room), ROOM_NUMBER, fp);    //e quello delle stanze
    fclose(fp);                                 //chiudo file
}

void assignPenalty(Player *a, int  i, int stop){//prendo in ingresso l'array player, il giocatore in questione e i turni di stop
    a[i].stopTurns+=stop;                    //assegno  i turni di stop
}

/*Funzione creata perche' mi sembrava che la generazione dei numeri casuali non fosse poi cosi casuale.
Cercando su internet ho letto che a volte puo' aiutare aggiungere un minimo di delay per renderli effettivamente casuali poiche'
la funzione srand() prende i suoi dati da un seed che si basa sul tempo */
void waitFor (unsigned int secs){     //prendo in ingresso un numero che sara' il tempo che la console aspettera' prima di continuare
    unsigned int retTime = time(0) + secs;
    while (time(0) < retTime);
}

void clear(){system("cls");};
