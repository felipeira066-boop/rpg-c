#ifndef CLASS_H
#define CLASS_H

#include "uint_type.h"
#include "hp.h"
#include "level.h"



/* ─────────────────────────────────────────
   Enum de classes jogáveis
───────────────────────────────────────── */

typedef enum {
    CLASSE_GUERREIRO = 0,
    CLASSE_ARQUEIRO  = 1,
    CLASSE_MAGO      = 2
} ClasseID;

/* ─────────────────────────────────────────
   Enum de moves — cada move tem um ID único.
   Guerreiro e arqueiro compartilham o mesmo enum;
   a função valida se o move pertence à classe certa.
───────────────────────────────────────── */

typedef enum {
    /* Guerreiro */
    MOVE_PORRADA        = 0,   /* disponível a partir do nível 1  */
    MOVE_FURIA          = 1,   /* disponível a partir do nível 5  */
    MOVE_ECLIPSE_MORTAL = 2,   /* disponível a partir do nível 15 */

    /* Arqueiro */
    MOVE_TIRO           = 3,   /* disponível a partir do nível 1  */
    MOVE_FLECHA_DE_OURO = 4,   /* disponível a partir do nível 5  */
    MOVE_MIRA_DA_MORTE  = 5,   /* disponível a partir do nível 15 */

    /*Mago*/
    MOVE_EVIDENCIA      = 6,   /* disponível a partir do nivel 1  */
    MOVE_CONJURACAO     = 7,   /* disponível a partir do nivel 5  */
    MOVE_BOLA_DE_FOGO   = 8,   /* disponível a partir do nivel 15 */

    MOVE_INVALIDO       = 99
} MoveID;

/* ─────────────────────────────────────────
   Struct: dados de um personagem
───────────────────────────────────────── */

typedef struct {
    ClasseID classe;
    uint     currentLevel;
    HP       hp;
    float  hpGrowth;     /* quanto o MaxHP cresce por nível */
} Personagem;

/* ─────────────────────────────────────────
   Protótipos
───────────────────────────────────────── */

/* Inicializa um personagem com valores base da classe escolhida. */
void initPersonagem(Personagem *p, ClasseID classe);

/* Executa um level up: aumenta MaxHP e restaura HP atual. */
void growthStats(Personagem *p);

/* Usa um move. Retorna o dano causado, ou 0 se o move for inválido/bloqueado. */
uint usarMove(Personagem *p, MoveID move, HP *alvo);

/* Retorna 1 se o personagem pode usar o move (nível e classe corretos). */
int moveDisponivel(const Personagem *p, MoveID move);

#endif /* CLASS_H */