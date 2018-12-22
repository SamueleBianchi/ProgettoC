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

void usa_oggetto(struct Giocatore giocatore);

void turni(struct Giocatore* Giocatore);

void su(struct Giocatore* giocatore);

void giu(struct Giocatore* giocatore);

void destra(struct Giocatore* giocatore);

void sinistra(struct Giocatore* giocatore);

void clear();
