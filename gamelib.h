#include <stdio.h>
#include <stdlib.h>

enum Tipo_pericolo {nessun_pericolo, trappola, alieno};

enum Stato_giocatore {vulnerabile, scudo_vita, solo_vita, solo_scudo};

enum Tipo_oggetto {nessuno, medikit, pozione, materiale, colpi_lanciarazzi};

struct Giocatore{
  char nome[20];
  enum Stato_giocatore stato;
  int x;
  int y;
  int zaino[4];
  size_t alieni_uccisi;
};

struct Cella{
  enum Tipo_pericolo pericolo;
  enum Tipo_oggetto oggetto;
};

void crea_mappa();

void gioca();

void termina_gioco();

int verifica();

void crea_scacchiera();

void stampa_scacchiera();

void termina_creazione();

void randomizza_pericoli(unsigned int n,struct Cella *scacchiera);

void randomizza_oggetti(unsigned int n,struct Cella *scacchiera);

const char *ritorna_stato(enum Stato_giocatore stato);

const char *ritorna_oggetto(enum Tipo_oggetto oggetto);

const char *ritorna_pericolo(enum Tipo_pericolo pericolo);

void legenda();

void muovi(struct Giocatore* giocatore);

int usa_oggetto(struct Giocatore *giocatore);

int gioca_turno(struct Giocatore *giocatore1, struct Giocatore *giocatore2);

unsigned su(struct Giocatore* giocatore);

unsigned giu(struct Giocatore* giocatore);

unsigned destra(struct Giocatore* giocatore);

unsigned sinistra(struct Giocatore* giocatore);

int verifica_pericolo(struct Giocatore *giocatore1, struct Giocatore *giocatore2);

int combatti_alieno(struct Giocatore *giocatore1, struct Giocatore *giocatore2);

void inizializza_zaini(struct Giocatore *giocatore1, struct Giocatore *giocatore2);

void prendi_oggetto(struct Giocatore *giocatore1);

int zaino_pieno(struct Giocatore *giocatore);

void clear();
