#include <stdio.h>
#include "gamelib.c"

int main(void){

  int scelta=0;
  do{

  printf("\n\tMenu di gioco\n1-Crea la mappa di gioco.\n2-Avvia la partita.\n3-Termina la partita\n\n");
  scanf("%d",&scelta);

  switch(scelta){
    case 1:
    crea_mappa();
    break;
    case 2:
    gioca();
    break;
    case 3:
    termina_gioco();
    break;
    default:
    printf("Errore: opzione non valida (scegliere solo tra le 3 opzioni).\n");
    break;
  }

}while(scelta > 3);


  return 0;
}
