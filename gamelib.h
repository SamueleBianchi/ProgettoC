#include <stdio.h>
#include <stdlib.h>

enum Tipo_pericolo {nessun_pericolo, trappola, alieno};

enum Stato_giocatore {vulnerabile, scudo_vita, solo_vita, solo_scudo};

enum Tipo_oggetto {nessuno, medikit, pozione, materiale, colpi_lanciarazzi};

struct Giocatore{
  char nome[20];
  enum Stato_giocatore stato;
  unsigned int vittorie;
  int x;
  int y;
  int zaino[4];
  size_t alieni_uccisi;
};

struct Cella{
  enum Tipo_pericolo pericolo;
  enum Tipo_oggetto oggetto;
};

struct Piano{
  unsigned short int piano;
  struct Piano* prossimo_piano;
};

void crea_mappa();

void gioca();

void termina_gioco();

void clear();
