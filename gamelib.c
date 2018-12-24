#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "gamelib.h"

static struct Cella *scacchiera = NULL;
static struct Giocatore Ninja;
static struct Giocatore Ciccio;
static unsigned int n = 0;
static unsigned int turno = 0;
static unsigned int numero_giochi = 0;

void crea_mappa(){

  clear();
  int scelta;
  do{
  printf("\n\tMenu per la creazione della mappa\n1-Crea la scacchiera.\n2-Stampa la scacchiera.\n3-Termina la creazione\n\n");
  scanf("%d",&scelta);

  switch(scelta){
    case 1:
    crea_scacchiera();
    scelta = 4;
    break;
    case 2:
    stampa_scacchiera();
    scelta = 4;
    break;
    case 3:
    termina_creazione();
    break;
    default:
    clear();
    printf("Errore: opzione non valida (scegliere solo tra le 3 opzioni).\n");
    break;
  }
}while(scelta > 3);
}

void crea_scacchiera(){
  if(scacchiera == NULL){
  clear();
  printf("Inserisci la dimensione della mappa: ");
  scanf("%u",&n);
  printf("Creazione della mappa in corso\n");
  scacchiera = (struct Cella*) malloc(n*n*sizeof(struct Cella));
  if(scacchiera == NULL){
    printf("Impossibile creare la scacchiera.\n");
  }else{
    time_t t;
    srand((unsigned) time(&t));
    randomizza_pericoli(n,scacchiera);
    randomizza_oggetti(n,scacchiera);
    Ciccio.x = rand() % n;
    Ciccio.y = rand() % n;
    Ciccio.stato = solo_vita;
    Ninja.x = rand() % n;
    Ninja.y = rand() % n;
    Ninja.stato = solo_vita;
    printf("Ciccio inizializzato alle coordinate (%d,%d) stato: %s\n", Ciccio.x, Ciccio.y,ritorna_stato(Ciccio.stato));
    printf("Ninja inizializzato alle coordinate (%d,%d) stato: %s\n", Ninja.x, Ninja.y, ritorna_stato(Ninja.stato));
  }
}else{
  clear();
  printf("Impossibile creare la scacchiera, scacchiera già creata.\n");
}
}

void randomizza_pericoli(unsigned int n,struct Cella *scacchiera){
  int prob[3];
  int somma = 0;

  while(somma<=100){
    printf("Inserisci la probabilità che non ci sia alcun pericolo: ");
    prob[0] = verifica();
    somma += prob[0];
    printf("Inserisci la probabilità ci sia una trappola: ");
    prob[1]= verifica();
    somma += prob[1];
    printf("Inserisci la probabilità ci sia un alieno: ");
    prob[2] = verifica();
    somma+=prob[2];
    if(somma>100 || somma <100){
      printf("Errore: la somma delle probabilità deve essere 100, riprovare\n");
      somma = 0;
      continue;
    }else{
      break;
    }
  }

  int random = 0;
  for(int i=0; i<n;i++){
    for(int j=0; j<n;j++){
      random = rand() % 101;
      if(random <= prob[0]){
        scacchiera[i*n+j].pericolo = 0;
      }else{
        if(random <= prob[0]+prob[1]){
          scacchiera[i*n+j].pericolo = 1;
        }else{
          scacchiera[i*n+j].pericolo = 2;
        }
      }
      }
    }
}

void randomizza_oggetti(unsigned int n,struct Cella *scacchiera){
  int prob[5];
  int somma=0;

  while(somma<=100){
    printf("\nInserisci la probabilità che non ci sia alcun oggetto: ");
    prob[0]= verifica();
    somma+=prob[0];
    printf("Inserisci la probabilità che ci sia un medikit: ");
    prob[1]= verifica();
    somma+=prob[1];
    printf("Inserisci la probabilità ci sia una pozione: ");
    prob[2]= verifica();
    somma+=prob[2];
    printf("Inserisci la probabilità ci sia un materiale: ");
    prob[3]= verifica();
    somma+=prob[3];
    printf("Inserisci la probabilità ci siano colpi lanciarazzi: ");
    prob[4]= verifica();
    somma+=prob[4];
    if(somma>100 || somma <100){
      printf("Errore: la somma delle probabilità deve essere 100, riprovare\n");
      somma=0;
      continue;
    }else{
      break;
    }
  }
  int random = 0;
  for(int i=0; i<n;i++){
    for(int j=0; j<n;j++){
      random = rand() % 101;
      if(random <= prob[0]){
        scacchiera[i*n+j].oggetto = 0;
      }else{
        if(random <= prob[0]+prob[1]){
          scacchiera[i*n+j].oggetto = 1;
        }else{
          if(random <= prob[0]+prob[1]+prob[2]){
            scacchiera[i*n+j].oggetto = 2;
          }else{
            if(random <= prob[0]+prob[1]+prob[2]+prob[3]){
              scacchiera[i*n+j].oggetto = 3;
            }else{
              scacchiera[i*n+j].oggetto = 4;
            }
          }
        }
      }
      }
    }
}

void stampa_scacchiera(){
  clear();
  if(scacchiera == NULL){
    printf("Impossibile stampare la scacchiera: la scacchiera non è stata creata.\n");
  }else{
    printf("\n");

    for(int i=0; i<n; ++i){
      for(int j=0; j<n; ++j){
        printf("| %s-%s |",ritorna_oggetto(scacchiera[i*n+j].oggetto), ritorna_pericolo(scacchiera[i*n+j].pericolo));
      }
      printf("\n");
    }
    printf("\n");
    legenda();
  }
}

void termina_creazione(){
  clear();
}

void gioca(){
  int uscita = 1;
  if(scacchiera == NULL){
    printf("Non puoi giocare se non crei la scacchiera!\n");
  }else{
    ++numero_giochi;
    if(numero_giochi == 1){
    strcpy(Ciccio.nome , "Ciccio");
    strcpy(Ninja.nome , "Ninja");
    do{
    ++turno;
    if(turno%2){
      clear();
      gioca_turno(&Ciccio);
      uscita = verifica_pericolo(&Ciccio,&Ninja);
    }else{
      clear();
      gioca_turno(&Ninja);
      uscita = verifica_pericolo(&Ninja,&Ciccio);
    }

  }while(uscita);
}else{
  clear();
  printf("\nNon puoi rigiocare! Per creare una nuova partita devi terminare questa.");
}
}
}

void termina_gioco(){
  free(scacchiera);
  scacchiera = NULL;
}

void clear(){
    #if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
        system("clear");
    #endif

    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #endif
}

int verifica(){
  int num;
  do{
    scanf("%d",&num);
    if(num<0){
      printf("Errore: Inserire un numero positivo: ");
    }else{
      return num;
    }
  }while(10);
}

const char *ritorna_stato(enum Stato_giocatore stato){
  switch(stato){
    case 0:
    return "Vulnerabile";
    break;
    case 1:
    return "Scudo e Vita";
    break;
    case 2:
    return "Solo vita";
    break;
    case 3:
    return "Solo scudo";
    default:
    return "ERR";
  }

}

const char *ritorna_oggetto(enum Tipo_oggetto oggetto){
  switch(oggetto){
    case 0:
    return "Ne";
    break;
    case 1:
    return "Me";
    break;
    case 2:
    return "Po";
    break;
    case 3:
    return "Ma";
    break;
    case 4:
    return "Cl";
    default:
    return "ERR";
  }
}

const char *ritorna_pericolo(enum Tipo_pericolo pericolo){
    switch(pericolo){
      case 0:
      return "Np";
      break;
      case 1:
      return "Tr";
      break;
      case 2:
      return "Al";
      break;
      default:
      return "ERR";
    }

}

void legenda(){
  printf("Legenda:\n");
  printf("\tOggetti:\n-Ne : nessun oggetto\n-Me : medikit\n-Po : pozione\n-Ma : materiale\n-Cl : colpi lanciarazzi\n");
  printf("\n\tPericoli:\n-Ne : nessun pericolo\n-Tr : trappola\n-Al : alieno\n");
}

unsigned su(struct Giocatore* giocatore){
  if(giocatore->y == 0){
    clear();
    printf("Non puoi sportarti sopra: sei nel bordo della mappa!\n");
    return 0;
  }else{
    --giocatore->y;
    return 1;
  }
}

unsigned giu(struct Giocatore* giocatore){
  if(giocatore->y == n-1){
    clear();
    printf("Non puoi sportarti sotto: sei nel bordo della mappa!\n");
    return 0;
  }else{
    ++giocatore->y;
    return 1;
  }
}

unsigned sinistra(struct Giocatore* giocatore){
  if(giocatore->x == 0){
    clear();
    printf("Non puoi sportarti a sinistra: sei nel bordo della mappa!\n");
    return 0;
  }else{
    --giocatore->x;
    return 1;
  }
}

unsigned destra(struct Giocatore* giocatore){
  if(giocatore->x == n-1){
    clear();
    printf("Non puoi sportarti a destra: sei nel bordo della mappa!\n");
    return 0;
  }else{
    ++giocatore->x;
    return 1;
  }
}

void muovi(struct Giocatore* giocatore){
  unsigned e = 0;
  do{
  char scelta;
  printf("\n\tComandi (premere il comando e il tasto INVIO):\n-w : su\n-a : sinistra\n-s : giù\n-d : destra\n");
  scanf("%s",&scelta);
  switch(scelta){
    case 'w':
    e = su(giocatore);
    break;
    case 'a':
    e = sinistra(giocatore);
    break;
    case 's':
    e = giu(giocatore);
    break;
    case 'd':
    e = destra(giocatore);
    break;
    default:
    clear();
    printf("Opzione non valida, riprova\n");

  }
}while(e == 0);

}

void usa_oggetto(struct Giocatore giocatore){

}

void gioca_turno(struct Giocatore* giocatore){
  unsigned scelta = 0;
  unsigned e = 0;
  do{

  printf("E' il turno di: %s\t Turno: %u\n", giocatore->nome, turno);
  printf("\n\tMenu di gioco:\n1-Muoviti\n2-Usa oggetto\n");
  scanf("%u",&scelta);
  switch (scelta) {
    case 1:
    muovi(&*giocatore);
    e=1;
    break;
    case 2:
    usa_oggetto(*giocatore);
    e=1;
    break;
    default:
    clear();
    printf("Opzione non valida, riprova\n");
    break;
  }
}while(e == 0);
}

int verifica_pericolo(struct Giocatore *giocatore1, struct Giocatore *giocatore2){
  if(scacchiera[giocatore1->x*n + giocatore1->y].pericolo == 1){
    printf("%s è stato ucciso da una trappola!\n%s vince la partita!\n",giocatore1->nome,giocatore2->nome);
    return 0;
  }else{
    return 1;
  }
}
