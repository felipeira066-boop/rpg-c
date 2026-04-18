#ifndef LEVEL_H
#define LEVEL_H

#include "uint_type.h"

/* ─────────────────────────────────────────
   Constantes de nível
───────────────────────────────────────── */

#define MAX_LEVEL    20u
#define MAX_EXP      1000u   /* EXP necessária para subir 1 nível */

/* ─────────────────────────────────────────
   Struct — agrupa nível e experiência juntos.
   Antes eram variáveis globais soltas; agora
   cada personagem carrega o próprio progresso.
───────────────────────────────────────── */

typedef struct {
    uint currentLevel;   /* nível atual (1 a MAX_LEVEL)        */
    uint currentExp;     /* EXP acumulada no nível atual        */
} LevelData;

/* ─────────────────────────────────────────
   Protótipos — implementados em level.c
───────────────────────────────────────── */

/* Inicializa com nível 1 e 0 EXP. */
void initLevel(LevelData *ld);

/* Adiciona EXP e sobe de nível automaticamente se atingir MAX_EXP.
   Retorna quantos níveis foram ganhos (pode ganhar mais de 1 de vez). */
uint addExp(LevelData *ld, uint exp);

/* Retorna 1 se o personagem pode ainda subir de nível. */
int canLevelUp(const LevelData *ld);

#endif /* LEVEL_H */