#include <stdio.h>
#include <stdlib.h>
#include "gamelib.h"

static void crea_scacchiera();
static void stampa_scacchiera();
static void termina_creazione();
int verifica();


static struct Cella *scacchiera = NULL;
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

  int prob[3];
  int somma=0;

  while(somma<=100){
    printf("Inserisci la probabilità che non ci sia alcun pericolo: ");
    prob[0]= verifica();
    somma+=prob[0];
    printf("Inserisci la probabilità che non ci sia una trappola: ");
    prob[1]= verifica();
    somma+=prob[1];
    printf("Inserisci la probabilità che non ci sia un alieno: ");
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
  printf("Mappa creata con successo.\n");
}

static void stampa_scacchiera(){

}

static void termina_creazione(){

}

void gioca(){

}

void termina_gioco(){

}

int verifica(){
  int num;
  do{
    scanf("%d",&num);
    if(num<=0){
      printf("Errore: Inserire un numero positivo: ");
    }else{
      return num;
    }
  }while(10);
}
