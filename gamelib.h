#include <stdio.h>
#include <stdlib.h>

void crea_mappa();

void gioca();

void termina_gioco();

enum Tipo_pericolo {nessun_pericolo, trappola, alieno};

enum Stato_giocatore {vulnerabile, scudo_vita, solo_vita, solo_scudo};

enum Tipo_oggetto {nessuno, medikit, pozione, materiale, colpi_lanciarazzi};

struct Giocatore{
  enum Stato_giocatore stato;
  int x;
  int y;
  int zaino[4];
  int alieni_uccisi;
};

struct Cella{
  enum Tipo_pericolo pericolo;
  enum Tipo_oggetto oggetto;
};
