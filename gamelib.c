#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gamelib.h"

static struct Cella *scacchiera = NULL;
static struct Giocatore Ninja;
static struct Giocatore Ciccio;
static unsigned int n = 0;
static unsigned int turno = 0;

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
    printf("Errore: opzione non valida (scegliere solo tra le 3 opzioni).\n");
    break;
  }
}while(scelta > 3);
}

void crea_scacchiera(){
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
        printf("%d ",scacchiera[i*n+j].oggetto);
      }
      printf("\n");
    }
  }
}

void termina_creazione(){
  clear();
}

void gioca(){
  if(scacchiera == NULL){
    printf("Non puoi giocare se non crei la scacchiera!\n");
  }else{
    printf("Puoi giocare\n");
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
    return "Scudo e vita";
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
