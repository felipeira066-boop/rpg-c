#include "class.h"
#include <stdlib.h>
#include <time.h>

/* ─────────────────────────────────────────
   Tabela de moves
   Cada linha: { moveID, classeExigida, nivelMinimo, danoBase }
   Para adicionar um move novo: adicione uma linha aqui + um valor no enum.
───────────────────────────────────────── */

typedef struct {
    MoveID   id;
    ClasseID classeExigida;
    uint     nivelMinimo;
    uint     danoBase;
} MoveInfo;

static const MoveInfo movesTable[] = {
    { MOVE_PORRADA,         CLASSE_GUERREIRO,  1,   5  },
    { MOVE_FURIA,           CLASSE_GUERREIRO,  5,   10 },
    { MOVE_ECLIPSE_MORTAL,  CLASSE_GUERREIRO,  15,  28 },
    { MOVE_TIRO,            CLASSE_ARQUEIRO,   1,   8  },
    { MOVE_FLECHA_DE_OURO,  CLASSE_ARQUEIRO,   5,   18 },
    { MOVE_MIRA_DA_MORTE,   CLASSE_ARQUEIRO,   15,  35 },
    { MOVE_EVIDENCIA,       CLASSE_MAGO,       1,   9  },
    { MOVE_CONJURACAO,      CLASSE_MAGO,       5,   16 },
    { MOVE_BOLA_DE_FOGO,    CLASSE_MAGO,       15,  35 },
};

#define NUM_MOVES (sizeof(movesTable) / sizeof(movesTable[0]))

/* ─────────────────────────────────────────
   Valores base por classe
───────────────────────────────────────── */

static void setBasesPorClasse(Personagem *p) {
    switch (p->classe) {
        case CLASSE_GUERREIRO:
            p->hp.maxHP     = 10;
            p->hp.currentHP = 10;
            p->hpGrowth     = 8.0f;
            break;

        case CLASSE_ARQUEIRO:
            p->hp.maxHP     = 6;
            p->hp.currentHP = 6;
            p->hpGrowth     = 5.0f;
            break;

        case CLASSE_MAGO:
            p->hp.maxHP     = 6;
            p->hp.currentHP = 6;
            p->hpGrowth     = 4.0f;
            break;

        default:
            p->hp.maxHP     = 1;
            p->hp.currentHP = 1;
            p->hpGrowth     = 1.0f;
            break;
    }
}

/* ─────────────────────────────────────────
   initPersonagem
───────────────────────────────────────── */

void initPersonagem(Personagem *p, ClasseID classe) {
    if (p == NULL) return;

    p->classe        = classe;
    p->currentLevel  = 1;
    p->hp.Shield     = 0;
    setBasesPorClasse(p);
}

/* ─────────────────────────────────────────
   growthStats — chamado ao subir de nível
───────────────────────────────────────── */

void growthStats(Personagem *p) {
    if (p == NULL) return;

    p->currentLevel++;
    p->hp.maxHP    += (uint)p->hpGrowth;
    p->hp.currentHP = p->hp.maxHP;   /* restaura HP ao subir de nível */
}

/* ─────────────────────────────────────────
   moveDisponivel — checagem separada da execução
───────────────────────────────────────── */

int moveDisponivel(const Personagem *p, MoveID move) {
    if (p == NULL) return 0;

    uint i;
    for (i = 0; i < NUM_MOVES; i++) {
        if (movesTable[i].id == move) {
            return (movesTable[i].classeExigida == p->classe &&
                    p->currentLevel >= movesTable[i].nivelMinimo);
        }
    }
    return 0;
} 

/* ─────────────────────────────────────────
   geradores de Críticos
─────────────────────────────────────────*/


static uint Critical(uint limiteMin){
    limiteMin;
    uint critical_rate = (rand()%21);
    printf("  [d20: %u]", critical_rate);
    return critical_rate >= limiteMin;
}

/* ─────────────────────────────────────────
   usarMove — coração do sistema de moves
───────────────────────────────────────── */

uint usarMove(Personagem *p, MoveID move, HP *alvo) {
    if (p == NULL || alvo == NULL) return 0;
    if (!moveDisponivel(p, move))  return 0;



    uint dano = 0;

    switch (move) {

        /* ── Moves do Guerreiro ─────────────── */

        case MOVE_PORRADA:
            dano = 5;
            break;

        case MOVE_FURIA:
            dano = 10;
            break;

        case MOVE_ECLIPSE_MORTAL:
            dano = 28;
            break;

        /* ── Moves do Arqueiro ──────────────── */

        case MOVE_TIRO:
            dano = 8;
            break;

        case MOVE_FLECHA_DE_OURO:
            dano = 18;
            break;

        case MOVE_MIRA_DA_MORTE:
            dano = 35;
            break;
        
        /* ── Moves do Mago ──────────────────── */
        
        case MOVE_EVIDENCIA:
            dano = 9;
            break;
        
        case MOVE_CONJURACAO:
            dano = 16;
            break;

        case MOVE_BOLA_DE_FOGO:
            dano = 33;
            break;

        default:
            return 0;
    }

    if(Critical(20)){
        dano *= 2;
        printf("****CRÍTICO****\n");
    }

    /* Aplica o dano ao alvo (respeitando escudo) */
    uint danoFinal = (dano > alvo->Shield) ? (dano - alvo->Shield) : 0;

    if (danoFinal >= alvo->currentHP) {
        alvo->currentHP = 0;
    } else {
        alvo->currentHP -= danoFinal;
    }

    return danoFinal;
}