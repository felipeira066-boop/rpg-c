#ifndef HP_H
#define HP_H

#include "uint_type.h"

/* ─────────────────────────────────────────
   Struct HP — representa a vida de qualquer
   entidade (herói, inimigo, chefe de fase).
   Sem código aqui — só declaração de dados.
───────────────────────────────────────── */

typedef struct {
    uint maxHP;      /* HP máximo                        */
    uint currentHP;  /* HP atual (nunca passa de maxHP)  */
    uint Shield;     /* absorção de dano por turno       */
} HP;

/* ─────────────────────────────────────────
   Protótipos — implementados em hp.c
───────────────────────────────────────── */

/* Inicializa um HP com valores definidos. currentHP começa igual a maxHP. */
void initHP(HP *hp, uint maxHP, uint shield);

/* Aplica dano. Desconta o Shield primeiro; o resto reduz currentHP. */
void takeDamage(uint damage, HP *hp);

/* Cura o personagem. currentHP nunca ultrapassa maxHP. */
void getHeal(uint heal, HP *hp);

/* Retorna 1 se o personagem está vivo, 0 se currentHP == 0. */
int isAlive(const HP *hp);

#endif /* HP_H */