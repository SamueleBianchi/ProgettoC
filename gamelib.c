#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gamelib.h"

static void crea_scacchiera();
static void stampa_scacchiera();
static void termina_creazione();
int verifica();
void randomizza_pericoli(int n,struct Cella *scacchiera);
void randomizza_oggetti(int n,struct Cella *scacchiera);


struct Cella *scacchiera = NULL;
static struct Giocatore Ninja;
static struct Giocatore Ciccio;

void crea_mappa(){

  system("clear");
  int scelta;
  do{

  printf("\n\tMenu per la creazione della mappa\n1-Crea la scacchiera.\n2-Stampa la scacchiera.\n3-Termina la creazione\n\n");
  scanf("%d",&scelta);

  switch(scelta){
    case 1:
    crea_scacchiera();
    scelta=4;
    break;
    case 2:
    stampa_scacchiera();
    scelta=4;
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

static void crea_scacchiera(){
  system("clear");
  int n;
  printf("Inserisci la dimensione della mappa: ");
  scanf("%d",&n);
  printf("Creazione della mappa in corso\n");
  scacchiera = malloc(n*n*sizeof(struct Cella));
  if(scacchiera == NULL){
    printf("Impossibile creare la scacchiera.\n");
  }else{
    randomizza_pericoli(n,scacchiera);
    randomizza_oggetti(n,scacchiera);
  }
}

void randomizza_pericoli(int n,struct Cella *scacchiera){
  int prob[3];
  int somma=0;

  while(somma<=100){
    printf("Inserisci la probabilità che non ci sia alcun pericolo: ");
    prob[0]= verifica();
    somma+=prob[0];
    printf("Inserisci la probabilità ci sia una trappola: ");
    prob[1]= verifica();
    somma+=prob[1];
    printf("Inserisci la probabilità ci sia un alieno: ");
    prob[2]= verifica();
    somma+=prob[2];
    if(somma>100 || somma <100){
      printf("Errore: la somma delle probabilità deve essere 100, riprovare\n");
      somma=0;
      continue;
    }else{
      break;
    }
  }
  time_t t;
  srand((unsigned) time(&t));
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
        printf("random: %d pericolo %d:\n",random,scacchiera[i*n+j].pericolo);
      }


    }
}

void randomizza_oggetti(int n,struct Cella *scacchiera){
  int prob[5];
  int somma=0;

  while(somma<=100){
    printf("Inserisci la probabilità che non ci sia alcun oggetto: ");
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
  time_t t;
  srand((unsigned) time(&t));
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
        printf("random: %d pericolo %d:\n",random,scacchiera[i*n+j].oggetto);
      }


    }
}

static void stampa_scacchiera(){

}

static void termina_creazione(){

}

void gioca(){

}

void termina_gioco(){
  free(scacchiera);
  scacchiera = NULL;
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
