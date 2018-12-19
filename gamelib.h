#include <stdio.h>
#include <stdlib.h>

enum Tipo_pericolo {nessun_pericolo, trappola, alieno};

enum Stato_giocatore {vulnerabile, scudo_vita, solo_vita, solo_scudo};

enum Tipo_oggetto {nessuno, medikit, pozione, materiale, colpi_lanciarazzi};

struct Giocatore{
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

void randomizza_pericoli(int n,struct Cella *scacchiera);

void randomizza_oggetti(int n,struct Cella *scacchiera);

void clear();
