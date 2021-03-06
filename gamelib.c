#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "gamelib.h"

static int verifica();
static void crea_scacchiera();
static void stampa_scacchiera();
static void termina_creazione();
static void inizializza_giocatori();
static void inizializza_pericoli(unsigned int n,struct Cella *scacchiera);
static void inizializza_oggetti(unsigned int n,struct Cella *scacchiera);
static void randomizza_pericoli();
static void randomizza_oggetti();
static const char *ritorna_stato(enum Stato_giocatore stato);
static const char *ritorna_oggetto(enum Tipo_oggetto oggetto);
static const char *ritorna_oggetto2(enum Tipo_oggetto oggetto);
static const char *ritorna_pericolo(enum Tipo_pericolo pericolo);
static void dimezza_mappa();
static void legenda();
static void init();
static void muovi(struct Giocatore* giocatore);
static int usa_oggetto(struct Giocatore *giocatore);
static int gioca_turno(struct Giocatore *giocatore1, struct Giocatore *giocatore2);
static unsigned su(struct Giocatore* giocatore);
static unsigned giu(struct Giocatore* giocatore);
static unsigned destra(struct Giocatore* giocatore);
static unsigned sinistra(struct Giocatore* giocatore);
static int verifica_pericolo(struct Giocatore *giocatore1, struct Giocatore *giocatore2);
static int combatti_alieno(struct Giocatore *giocatore1, struct Giocatore *giocatore2);
static void inizializza_zaini(struct Giocatore *giocatore1, struct Giocatore *giocatore2);
static void prendi_oggetto(struct Giocatore *giocatore1);
static int zaino_pieno(struct Giocatore *giocatore);
static int scontro_finale();
static void crea_torri();
static struct Piano* crea_lista(int l);
static void stampa_lista(struct Piano* pFirst);
static void aggiorna_lista(struct Piano* pFirst);
static int gioca_finale(struct Giocatore* giocatore1, struct Giocatore* giocatore2, struct Piano* pianoG1, struct Piano** pianoG2);

static struct Cella *scacchiera = NULL;
static struct Giocatore Ninja;
static struct Giocatore Ciccio;
static unsigned int n = 0;
static unsigned int turno = 0;
static unsigned int numero_giochi = 0;
unsigned short probPericoli[3];
unsigned short probOggetti[5];
struct Piano *Piano_C = NULL;
struct Piano *Piano_N = NULL;
char nome_g1[20];
char nome_g2[20];

// La funzione contiene il codice che deve essere eseguito runtime e deve essere scritto all'interno di una funzione
static void init(){
  time_t t;
  srand((unsigned) time(&t));
}

void crea_mappa(){
  init();
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

static void crea_scacchiera(){
  if(scacchiera == NULL){
  clear();
  printf("Inserisci la dimensione della mappa: ");
  scanf("%u",&n);
  scacchiera = (struct Cella*) malloc(n*n*sizeof(struct Cella));
    if(scacchiera == NULL){
      printf("Impossibile creare la scacchiera.\n");
    }else{
      inizializza_pericoli(n,scacchiera);
      inizializza_oggetti(n,scacchiera);
      Ninja.stato = solo_vita;
      Ciccio.stato = solo_vita;
      inizializza_giocatori();
      inizializza_zaini(&Ciccio,&Ninja);
  }
}else{
  free(scacchiera);
  scacchiera = NULL;
  crea_scacchiera();
}
}

static void inizializza_giocatori(){
  Ciccio.x = rand() % n;
  Ciccio.y = rand() % n;
  Ninja.x = rand() % n;
  Ninja.y = rand() % n;
  strcpy(Ciccio.nome , nome_g1);
  strcpy(Ninja.nome ,nome_g2);
}

static void inizializza_pericoli(unsigned int n,struct Cella *scacchiera){
  int somma = 0;
  printf("\nProbabilità dei pericoli :\n\n");
  while(somma<=100){
    printf("Inserisci la probabilità che non ci sia alcun pericolo: ");
    probPericoli[0] = verifica();
    somma += probPericoli[0];
    printf("Inserisci la probabilità ci sia una trappola: ");
    probPericoli[1]= verifica();
    somma += probPericoli[1];
    printf("Inserisci la probabilità ci sia un alieno: ");
    probPericoli[2] = verifica();
    somma += probPericoli[2];
    if(somma>100 || somma <100){
      clear();
      printf("Errore: la somma delle probabilità deve essere 100, riprovare\n");
      somma = 0;
      continue;
    }else{
      break;
    }
  }
  printf("\n");
  randomizza_pericoli();
}

static void randomizza_pericoli(){
  int random = 0;
  for(int i=0; i<n;i++){
    for(int j=0; j<n;j++){
      random = rand() % 101;
      if(random <= probPericoli[0]){
        scacchiera[i*n+j].pericolo = 0;
      }else{
        if(random <= probPericoli[0]+probPericoli[1]){
          scacchiera[i*n+j].pericolo = 1;
        }else{
          scacchiera[i*n+j].pericolo = 2;
        }
      }
      }
    }
}

static void inizializza_oggetti(unsigned int n,struct Cella *scacchiera){
  int somma = 0;
  printf("\nProbabilità degli oggetti :\n\n");
  while(somma<=100){
    printf("\nInserisci la probabilità che non ci sia alcun oggetto: ");
    probOggetti[0]= verifica();
    somma+=probOggetti[0];
    printf("Inserisci la probabilità che ci sia un medikit: ");
    probOggetti[1]= verifica();
    somma+=probOggetti[1];
    printf("Inserisci la probabilità ci sia una pozione: ");
    probOggetti[2]= verifica();
    somma+=probOggetti[2];
    printf("Inserisci la probabilità ci sia un materiale: ");
    probOggetti[3]= verifica();
    somma+=probOggetti[3];
    printf("Inserisci la probabilità ci siano colpi lanciarazzi: ");
    probOggetti[4]= verifica();
    somma+=probOggetti[4];
    if(somma>100 || somma <100){
      clear();
      printf("Errore: la somma delle probabilità deve essere 100, riprovare\n");
      somma=0;
      continue;
    }else{
      break;
    }
  }
  printf("\n");
  clear();
 randomizza_oggetti();
}

static void randomizza_oggetti(){
  int random = 0;
  for(int i=0; i<n;i++){
    for(int j=0; j<n;j++){
      random = rand() % 101;
      if(random < probOggetti[0]){
        scacchiera[i*n+j].oggetto = 0;
      }else{
        if(random < probOggetti[0]+probOggetti[1]){
          scacchiera[i*n+j].oggetto = 1;
        }else{
          if(random < probOggetti[0]+probOggetti[1]+probOggetti[2]){
            scacchiera[i*n+j].oggetto = 2;
          }else{
            if(random < probOggetti[0]+probOggetti[1]+probOggetti[2]+probOggetti[3]){
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

static void stampa_scacchiera(){
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

static void termina_creazione(){
  clear();
}

void gioca(){
  int uscita = 1;

  if(scacchiera == NULL){
    printf("Non puoi giocare se non crei la scacchiera!\n");
  }else{
    do{
    ++numero_giochi;
    if(numero_giochi == 1){
      printf("\nGiocatore n1, inserisci il tuo nickname (senza spazi): \n");
      scanf("%s", Ciccio.nome);
      strcpy(nome_g1, Ciccio.nome );
      printf("\nGiocatore n2, inserisci il tuo nickname (senza spazi): \n");
      scanf("%s", Ninja.nome);
      strcpy(nome_g2, Ninja.nome );
      Ciccio.vittorie = 0;
      Ninja.vittorie = 0;
    }
    ++turno;
    if((Ciccio.x == Ninja.x) && (Ciccio.y == Ninja.y)){
      clear();
      uscita = scontro_finale();
    }else{
    if(turno%2){
      clear();
      if(turno%5 == 0){
        dimezza_mappa();
        printf("La mappa si è dimezzata! La sua nuova dimensione è %d I giocatori hanno cambiato posizione!\n",n);
      }
      uscita = gioca_turno(&Ciccio,&Ninja);
    }else{
      clear();
      if(turno%5 == 0){
        dimezza_mappa();
        printf("La mappa si è dimezzata! La sua nuova dimensione è %d I giocatori hanno cambiato posizione!\n",n);
      }
      uscita = gioca_turno(&Ninja,&Ciccio);
    }
    if(uscita == 0){
      turno = 0;
    }}
  }while(uscita);
}
}

void termina_gioco(){
  if(numero_giochi != 0){
  clear();
  printf("\nStatitistiche giocatori :\n\n\t%s\nWinrate: %.0f%%\nLoserate: %.0f%%\nAlieni uccisi: %zu\n\n\t%s\nWinrate: %.0f%%\nLoserate: %.0f%%\n", Ciccio.nome, (float)Ciccio.vittorie/numero_giochi*100, (double)(numero_giochi-Ciccio.vittorie)/numero_giochi*100, Ciccio.alieni_uccisi, Ninja.nome, (double)Ninja.vittorie/numero_giochi*100, (double)(numero_giochi-Ninja.vittorie)/numero_giochi*100);
  printf("Alieni uccisi: %zu\n\n", Ninja.alieni_uccisi);
  free(scacchiera);
  scacchiera = NULL;
}
}

void clear(){
    #if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
        system("clear");
    #endif

    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #endif
}

static void dimezza_mappa(){
  free(scacchiera);
  n = n/2;
  scacchiera = (struct Cella*) malloc(n*n*sizeof(struct Cella));
  inizializza_giocatori();
  randomizza_oggetti();
  randomizza_pericoli();
}

static int verifica(){
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

static const char *ritorna_stato(enum Stato_giocatore stato){
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

static const char *ritorna_oggetto(enum Tipo_oggetto oggetto){
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
    break;
    default:
    return "ERR";
  }
}

static const char *ritorna_oggetto2(enum Tipo_oggetto oggetto){
  switch(oggetto){
    case 0:
    return "Nessun oggetto";
    break;
    case 1:
    return "Medikit";
    break;
    case 2:
    return "Pozione";
    break;
    case 3:
    return "Materiale";
    break;
    case 4:
    return "Colpi lanciarazzi";
    break;
    default:
    return "ERR";
  }
}

static const char *ritorna_pericolo(enum Tipo_pericolo pericolo){
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

static void legenda(){
  printf("Legenda:\n");
  printf("\tOggetti:\n-Ne : nessun oggetto\n-Me : medikit\n-Po : pozione\n-Ma : materiale\n-Cl : colpi lanciarazzi\n");
  printf("\n\tPericoli:\n-Ne : nessun pericolo\n-Tr : trappola\n-Al : alieno\n");
}

static unsigned su(struct Giocatore* giocatore){
  if(giocatore->x == 0){
    clear();
    printf("Non puoi sportarti sopra: sei nel bordo della mappa!\n");
    return 0;
  }else{
    --giocatore->x;
    return 1;
  }
}

unsigned giu(struct Giocatore* giocatore){
  if(giocatore->x == n-1){
    clear();
    printf("Non puoi sportarti sotto: sei nel bordo della mappa!\n");
    return 0;
  }else{
    ++giocatore->x;
    return 1;
  }
}

static unsigned sinistra(struct Giocatore* giocatore){
  if(giocatore->y == 0){
    clear();
    printf("Non puoi sportarti a sinistra: sei nel bordo della mappa!\n");
    return 0;
  }else{
    --giocatore->y;
    return 1;
  }
}

static unsigned destra(struct Giocatore* giocatore){
  if(giocatore->y == n-1){
    clear();
    printf("Non puoi sportarti a destra: sei nel bordo della mappa!\n");
    return 0;
  }else{
    ++giocatore->y;
    return 1;
  }
}

static void muovi(struct Giocatore* giocatore){
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

/* La funzione permette di usare gli oggetti e ritorna 0 se lo zaino è vuoto(quindi non è possibile utilizzare alcun oggetto),
1 negli altri casi, in cui l'operazione è andata a buon fine. */
static int usa_oggetto(struct Giocatore *giocatore){
  unsigned scelta = 0;
  /* Variabile "e" utilizzata per ciclare nel ciclo while fino a che l'utente non usa un oggetto nella maniera corretta
  (es: usa il medikit quando è vulnerabile e non in stato solo_vita ). */
  unsigned e = 0;
  char continua;
  if(zaino_pieno(&*giocatore) == 0){
  do{
  printf("\nIl tuo zaino: \n");
  for(int i=0; i<4; ++i){
    printf("%d %s\n", giocatore->zaino[i], ritorna_oggetto(i+1));
  }
  printf("\nVita attuale: %s\n",ritorna_stato(giocatore->stato));
  printf("\nCosa vuoi usare?\n1-Medikit\n2-Pozione\n3-Materiale\n4-Colpi lanciarazzi\n5-Esci\n\n");
  scanf("%u",&scelta);
  switch(scelta){
    case 1:
    if(giocatore->zaino[0] != 0){
      if(giocatore->stato == vulnerabile){
        giocatore->stato = solo_vita;
        --giocatore->zaino[0];
        printf("Medikit utilizzato! Vita: %s\n",ritorna_stato(giocatore->stato));
        e = 1;
      }else{
        if(giocatore->stato == solo_scudo){
          giocatore->stato = scudo_vita;
          --giocatore->zaino[0];
          printf("Medikit utilizzato! Vita: %s\n",ritorna_stato(giocatore->stato));
          e = 1;
        }else{
          if(giocatore->stato == solo_vita || giocatore->stato == scudo_vita){
            clear();
            printf("\nNon puoi usare questo oggetto! Hai già ripristinato la tua vita. Usa un altro oggetto\n");
          }
        }
      }
    }else{
      printf("Non puoi usare quest'oggetto : non è nel tuo zaino!\n");
    }
    break;
    case 2:
    if(giocatore->zaino[1] != 0){
      if(giocatore->stato == vulnerabile){
        giocatore->stato = solo_scudo;
        --giocatore->zaino[1];
        printf("Pozione utilizzata! Vita: %s\n",ritorna_stato(giocatore->stato));
        e = 1;
      }else{
        if(giocatore->stato == solo_vita){
          giocatore->stato = scudo_vita;
          --giocatore->zaino[1];
          printf("Pozione utilizzata! Vita: %s\n",ritorna_stato(giocatore->stato));
          e = 1;
        }else{
          if(giocatore->stato == solo_scudo || giocatore->stato == scudo_vita){
            clear();
            printf("\nNon puoi usare questo oggetto! Hai già ripristinato il tuo scudo. Usa un altro oggetto\n");
          }
        }
      }
    }else{
      printf("Non puoi usare quest'oggetto : non è nel tuo zaino!\n");
    }
    break;
    case 3:
    clear();
    printf("Questo oggetto si può usare solo nella prima fase dello scontro finale, scegli un altro oggetto\n");
    break;
    case 4:
    clear();
    printf("Questo oggetto si può usare solo nella prima fase dello scontro finale, scegli un altro oggetto\n");
    break;
    case 5:
     e = 1;
    break;
    default:
    clear();
    printf("Errore, scegliere solo tra le opzioni proposte, riprovare\n");
    e = 0;
    break;
  }
}while(e == 0);
printf("Premi un carattere e premi invio per passare il turno... ");
scanf("%s",&continua);
return 1;
}else{
  clear();
  printf("Il tuo zaino è vuoto! Non puoi usare nessun oggetto\n");
  return 0;
}
}

/* La funzione permette di giocare il turno e ritorna 1 se il giocatore che ha giocato il turno non è morto,
altrimenti restituisce 0 (giocatore morto) */

static int gioca_turno(struct Giocatore *giocatore1, struct Giocatore *giocatore2){
  unsigned scelta = 0;
  unsigned e = 0;//variabile per uscire dal ciclo do-while
  char continua;
  int ritorna = 0;
  do{

  printf("E' il turno di: %s\t Turno: %u\nCoordinate attuali: (%d, %d)\n", giocatore1->nome, turno,  giocatore1->x, giocatore1->y);
  printf("\n\tMenu di gioco:\n1-Muoviti\n2-Usa oggetto\n");
  scanf("%u",&scelta);
  switch (scelta) {
    case 1:
    muovi(&*giocatore1);
    e=1;
    ritorna = verifica_pericolo(&*giocatore1,&*giocatore2);
    if (ritorna!=0){
    printf("Premi un carattere qualsiasi per passare il tuo turno \n");
    scanf("%s",&continua);
    }
    break;
    case 2:
    e = usa_oggetto(&*giocatore1);
    ritorna = 1;
    break;
    default:
    clear();
    printf("Opzione non valida, riprova\n");
    break;
  }
}while(e == 0);
return ritorna;
}

/* La funzione verifica_pericolo ritorna 0 se il giocatore viene ucciso da una trappola (caso in cui il campo pericolo è
uguale a 0), ritorna 1 per default,  e ritorna il valore restituito dalla funzione combatti_alieno() se il valore del pericolo
nella cella è 2, se la funzione combatti_alieno() restituisce 0 significa che il giocatore è morto nel combattimento. */

static int verifica_pericolo(struct Giocatore *giocatore1, struct Giocatore *giocatore2){
  switch(scacchiera[giocatore1->x*n + giocatore1->y].pericolo){
    case 0:
    prendi_oggetto(giocatore1);
    return 1;
    break;
    case 1:
    printf("%s è stato ucciso da una trappola!\n%s vince la partita!\n",giocatore1->nome,giocatore2->nome);
    giocatore2->vittorie++;
    free(scacchiera);
    scacchiera = NULL;
    return 0;
    case 2:
    return combatti_alieno(&*giocatore1, &*giocatore2);
    default:
    return 1;
  }
}

/* La funzione combatti_alieno restituisce 0 se il giocatore muore durante il combattimento con l'alieno, restituisce 1 se
il giocatore non muore, infatti la variabile r (variabile ritornata) viene inizializzata a 1. */

static int combatti_alieno(struct Giocatore *giocatore1, struct Giocatore *giocatore2){
  unsigned scelta = 0, e = 0,scudo = 65,random = 0;
  unsigned r = 1;//variabile di ritorno
  printf("C'è un alieno! Vuoi combatterlo? Se non lo combatti non prendi l'eventuale oggetto\n-1 Si\n-2 No\n");
  do{
    scanf("%d",&scelta);
    switch(scelta){
      case 1:
      ++giocatore1->alieni_uccisi;
      scacchiera[giocatore1->x * n + giocatore1->y].pericolo = 0;
      printf("Hai ucciso un alieno\n");
      prendi_oggetto(&*giocatore1);
      random = rand() % 101;
      if(giocatore1->stato == vulnerabile){
        r = 0;
        printf("%s è stato ucciso durante lo scontro con l'alieno!\n%s vince la partita!\n",giocatore1->nome,giocatore2->nome);
        giocatore2->vittorie++;
      }else{
      if(random < scudo){
        if(giocatore1->stato == solo_vita){
          giocatore1->stato = vulnerabile;
        }
      }else{
          if(giocatore1->stato == solo_scudo){
            giocatore1->stato = vulnerabile;
          }
      }}
      e = 1;
      break;
      case 2:
      e = 1;
      break;
      default:
      clear();
      printf("Errore: risposta non corretta, riprova\nVuoi combattere?\n-1 Si\n-2 No\n");
      break;
    }

  }while(e == 0);
  return r;
}

static void inizializza_zaini(struct Giocatore *giocatore1, struct Giocatore *giocatore2){
  for(int i = 0; i<4; i++){
    giocatore1->zaino[i]=0;
    giocatore2->zaino[i]=0;
  }
}

/* La funzione permette di prendere gli oggetti che il giocatore trova quando si sposta nella mappa e avvisa il giocatore
 che non può prendere un determinato oggetto perche possiede il numero massimo di oggetti di quel determinato tipo. */

static void prendi_oggetto(struct Giocatore *giocatore1){
  unsigned oggetto = scacchiera[giocatore1->x * n + giocatore1->y].oggetto;
  switch(oggetto){
    case 1:
    if(giocatore1->zaino[0]<3){
    ++giocatore1->zaino[0];
    scacchiera[giocatore1->x*n + giocatore1->y].oggetto = 0;
    printf("Hai ottenuto 1 %s\n",ritorna_oggetto2(oggetto));

  }else{
    printf("Impossibile prendere l'oggetto %s: hai raggiunto il numero massimo di oggetti da prendere di questo tipo\n",ritorna_oggetto2(oggetto));
  }
    break;
    case 2:
    if(giocatore1->zaino[1]<3){
      printf("Hai ottenuto 1 %s\n",ritorna_oggetto2(oggetto));
      ++giocatore1->zaino[1];
      scacchiera[giocatore1->x*n + giocatore1->y].oggetto = 0;
    }else{
      printf("Impossibile prendere l'oggetto %s: hai raggiunto il numero massimo di oggetti da prendere di questo tipo\n",ritorna_oggetto2(oggetto));
    }
    break;
    case 3:
    if(giocatore1->zaino[2]<3){
      printf("Hai ottenuto 1 %s\n",ritorna_oggetto2(oggetto));
      ++giocatore1->zaino[2];
      scacchiera[giocatore1->x*n + giocatore1->y].oggetto = 0;
    }else{
      printf("Impossibile prendere l'oggetto %s: hai raggiunto il numero massimo di oggetti da prendere di questo tipo\n",ritorna_oggetto2(oggetto));
    }
    break;
    case 4:
    if(giocatore1->zaino[3]<3){
      printf("Hai ottenuto 1 %s\n",ritorna_oggetto2(oggetto));
      ++giocatore1->zaino[3];
      scacchiera[giocatore1->x*n + giocatore1->y].oggetto = 0;
    }else{
      printf("Impossibile prendere l'oggetto %s: hai raggiunto il numero massimo di oggetti da prendere di questo tipo\n",ritorna_oggetto2(oggetto));
    }
    break;
  }
}

static int zaino_pieno(struct Giocatore *giocatore){
  if(giocatore->zaino[0] == 0 && giocatore->zaino[1]== 0 && giocatore->zaino[2]== 0 && giocatore->zaino[3]==0){
    return 1;
  }else{
    return 0;
  }
}

static struct Piano* crea_lista(int l){
  static int i = 1;
  struct Piano *pFirst = (struct Piano*) malloc(sizeof(struct Piano));
  if(l == 0){
    i = 1;
    return (NULL);
  }
  else{
    pFirst = (struct Piano*) malloc(sizeof(struct Piano));
    pFirst->piano = i;
    ++i;
    pFirst->prossimo_piano = crea_lista(l-1);
    return pFirst;
  }
}


static void stampa_lista(struct Piano* pFirst){
  if(pFirst == NULL){
    return;
  }else{
    struct Piano* pScan = pFirst;
    stampa_lista(pScan->prossimo_piano);
    printf("%d\n",pScan->piano) ;
  }
}

static void crea_torri(){
  printf("**************************SCONTRO FINALE*************************\nPrima fase:  distruzione delle torri\nTORRI PRIMA DELL'USO DEI LANCIARAZZI:\n");
  if(Ciccio.zaino[2]){
    Piano_C = crea_lista(Ciccio.zaino[2]);
    printf("Torre di %s:\n", Ciccio.nome);
    stampa_lista(Piano_C);
    printf("\n");
    for(int i=0; i<Ninja.zaino[3]; i++){
    aggiorna_lista(Piano_C);
    }
  }else{
    printf("%s non ha materiali per costruire una torre!\n", Ciccio.nome);
  }
  if(Ninja.zaino[2]){
    Piano_N = crea_lista(Ninja.zaino[2]);
    printf("Torre di %s:\n", Ninja.nome);
    stampa_lista(Piano_N);
    printf("\n");
    for(int i=0; i<Ciccio.zaino[3]; i++){
    aggiorna_lista(Piano_N);
    }
  }else{
    printf("%s non ha materiali per costruire una torre!\n", Ninja.nome);
  }

  printf("\nTORRI DOPO L'USO DEI LANCIARAZZI:\n");
  if((Ninja.zaino[3] == Ciccio.zaino[2]) && Ninja.zaino[2]!= 0){
    Piano_C = NULL;
    printf("La torre di %s è stata distrutta!\n", Ciccio.nome);
  }else{
    if(Ninja.zaino[3] == Ciccio.zaino[2]){
      printf("%s non ha torri!\n", Ciccio.nome);
    }else{
    printf("Torre di %s:\n", Ciccio.nome);
    stampa_lista(Piano_C);
    printf("\n");
  }}

  if((Ninja.zaino[2] == Ciccio.zaino[3]) && Ninja.zaino[2]!= 0){
    Piano_N = NULL;
    printf("La torre di %s è stata distrutta!\n", Ninja.nome);
  }else{
    if(Ninja.zaino[2] == Ciccio.zaino[3]){
      printf("%s non ha torri!\n", Ninja.nome);
    }else{
    printf("Torre di %s:\n", Ninja.nome);
    stampa_lista(Piano_N);
    printf("\n");
  }}
  Ninja.zaino[2] = 0;
  Ciccio.zaino[2] = 0;
  Ciccio.zaino[3]-= Ninja.zaino[2];
  Ninja.zaino[3]-= Ciccio.zaino[3];
  char x;
  printf("Premere un carattere qualsiasi e il tasto INVIO per passare alla seconda fase dello scontro finale....");
  scanf("%s",&x);
  }

static void aggiorna_lista(struct Piano* pFirst){
  if(pFirst->prossimo_piano == NULL){
    pFirst = NULL;
  }else{
  struct Piano* pScan;
  pScan = pFirst->prossimo_piano;

  while(pScan->prossimo_piano !=NULL){
    pFirst = pScan;
    pScan = pScan->prossimo_piano;
  }
  free(pFirst->prossimo_piano);
  pFirst->prossimo_piano = NULL;
}
}

static int gioca_finale(struct Giocatore* giocatore1, struct Giocatore* giocatore2, struct Piano* pianoG1, struct Piano** pianoG2){
int scelta = 0;
int esci = 0;
int r = 1;
printf("E' il turno di %s.\n\n",giocatore1->nome);
if(pianoG1 != NULL){
printf("Stato giocatore: %s\tStato torre: attiva\n", ritorna_stato(giocatore1->stato));
}else{
printf("Stato giocatore: %s\tStato torre: distrutta\n", ritorna_stato(giocatore1->stato));
}
  do{

printf("Cosa vuoi fare?\n1 - Attacca\n2 - Usa oggetto\n ");
scanf("%d",&scelta);
switch(scelta){
  case 1:
  if(*pianoG2 == NULL){
    clear();
    switch(giocatore2->stato){
      case vulnerabile:
      clear();
      printf("\n%s è morto! %s vince la partita!\n", giocatore2->nome, giocatore1->nome);
      giocatore1->vittorie++;
      r = 0;
      esci = 1;
      break;
      case solo_vita:
      giocatore2->stato = vulnerabile;
      esci = 1;
      break;
      case solo_scudo:
      giocatore2->stato = vulnerabile;
      esci = 1;
      case scudo_vita:
      giocatore2->stato = solo_vita;
      esci = 1;
      break;
      default:
      break;
  }}else{
  if((*pianoG2)->prossimo_piano == NULL){
    clear();
      printf("La torre di %s è stata colpita!\n",giocatore2->nome);
    *pianoG2 = NULL;
    esci = 1;
  }else{
  if(*pianoG2 != NULL){
  aggiorna_lista(*pianoG2);
  clear();
  printf("La torre di %s è stata colpita!\n",giocatore2->nome);
  esci = 1;
}}}
  break;
  case 2:
  usa_oggetto(giocatore1);
  clear();
  printf("E' il turno di %s.\n\n",giocatore1->nome);
  break;
  default:
  clear();
  printf("Errore: opzione non valida\n");
  break;
}
}while(esci == 0);
return r;
}

static int scontro_finale(){
  crea_torri();
  clear();
  short t = 0;
  int uscita = 1;
  char x;
  printf("**************************SCONTRO FINALE*************************\nSeconda fase: scontro all'ultimo sangue\n\n");
  printf("%s e %s stanno per combattere! Durante lo scontro è possibile utilizzare pozioni o\nmedikit per ripristinare scudo e salute ed è possibile utilizzarli senza sprecare il turno, oppure attaccare l'avversario.\nSe un giocatore possiede ancora una torre, quando viene attaccato perderà solo il livello della torre e non perderà la propria vita/scudo.\nBuona fortuna!\n\nPremere un tasto per continuare...",Ciccio.nome, Ninja.nome);
  scanf("%s",&x);
  clear();
  while(uscita){
    t++;
    if(t%2 == 0){
      uscita = gioca_finale(&Ciccio,&Ninja, Piano_C, &Piano_N);
    }else{
      uscita = gioca_finale(&Ninja,&Ciccio, Piano_N, &Piano_C);
    }
  }
  free(scacchiera);
  free(Piano_C);
  free(Piano_N);
  scacchiera = NULL;
  Piano_C = NULL;
  Piano_N = NULL;
  turno = 0;
  return 0;
}
