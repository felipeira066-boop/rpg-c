#include "level.h"

/* ─────────────────────────────────────────
   initLevel
───────────────────────────────────────── */

void initLevel(LevelData *ld) {
    if (ld == NULL) return;

    ld->currentLevel = 1;
    ld->currentExp   = 0;
}

/* ─────────────────────────────────────────
   canLevelUp
───────────────────────────────────────── */

int canLevelUp(const LevelData *ld) {
    if (ld == NULL) return 0;
    return ld->currentLevel < MAX_LEVEL;
}

/* ─────────────────────────────────────────
   addExp

   CORREÇÕES do original:
   - "level < 0" era inútil (uint nunca é negativo)
   - A função modificava o parâmetro local em vez
     do estado real do personagem
   - Nunca incrementava currentLevel de fato

   NOVA LÓGICA:
   - Acumula EXP normalmente
   - Quando currentExp >= MAX_EXP, sobe de nível
     e guarda o excedente (pode subir vários níveis)
   - Para no MAX_LEVEL
───────────────────────────────────────── */

uint addExp(LevelData *ld, uint exp) {
    if (ld == NULL) return 0;

    uint niveisGanhos = 0;

    ld->currentExp += exp;

    while (ld->currentExp >= MAX_EXP && canLevelUp(ld)) {
        ld->currentExp   -= MAX_EXP;
        ld->currentLevel += 1;
        niveisGanhos     += 1;
    }

    /* Se chegou ao nível máximo, trava a EXP em zero */
    if (!canLevelUp(ld)) {
        ld->currentExp = 0;
    }

    return niveisGanhos;
}