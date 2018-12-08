#include <stdio.h>
#include <stdlib.h>
#include "gamelib.h"

static void crea_scacchiera();
static void stampa_scacchiera();
static void termina_creazione();


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
