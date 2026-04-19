#include "hp.h"
 
/* ─────────────────────────────────────────
   initHP
───────────────────────────────────────── */
 
void initHP(HP *hp, uint maxHP, uint shield) {
    if (hp == NULL) return;
 
    hp->maxHP     = maxHP;
    hp->currentHP = maxHP;   /* começa com HP cheio */
    hp->Shield    = shield;
}
 
/* ─────────────────────────────────────────
   takeDamage
 
   CORREÇÃO do original:
   - Usava HP (tipo) em vez de hp (parâmetro)
   - Typo "Hp->Shield" em vez de "hp->Shield"
───────────────────────────────────────── */
 
void takeDamage(uint damage, HP *hp) {
    if (hp == NULL) return;
 
    uint danoFinal = 0;
 
    if (damage > hp->Shield) {
        danoFinal = damage - hp->Shield;
    }
 
    if (danoFinal >= hp->currentHP) {
        hp->currentHP = 0;
    } else {
        hp->currentHP -= danoFinal;
    }
}
 
/* ─────────────────────────────────────────
   getHeal
 
   CORREÇÃO do original:
   - "HP->CurrentHP += heal > HP->MaxHP" avaliava
     (heal > MaxHP) como 0 ou 1 por causa da
     precedência de operadores, somando isso ao HP.
   - A cura só era aplicada no else, nunca no if.
───────────────────────────────────────── */
 
void getHeal(uint heal, HP *hp) {
    if (hp == NULL) return;
 
    if (hp->currentHP + heal >= hp->maxHP) {
        hp->currentHP = hp->maxHP;        /* teto: não passa do máximo */
    } else {
        hp->currentHP += heal;
    }
}
 
/* ─────────────────────────────────────────
   isAlive — útil para checar fim de combate
───────────────────────────────────────── */
 
int isAlive(const HP *hp) {
    if (hp == NULL) return 0;
    return hp->currentHP > 0;
}
 