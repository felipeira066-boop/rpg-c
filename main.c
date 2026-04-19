#include <stdio.h>
#include <string.h>
#include "hp.h"
#include "level.h"
#include "item.h"
#include "class.h"
#include <time.h>
#include <stdlib.h>

#define NOME_MAX 32

/* ─────────────────────────────────────────
   Inimigo
───────────────────────────────────────── */

typedef struct {
    char nome[32];
    HP   hp;
    uint danoBase;
    uint expRecompensa;
} Inimigo;

/* ─────────────────────────────────────────
   Arena progressiva de inimigos
───────────────────────────────────────── */

static Inimigo arenaInimigos[] = {
    { "Goblin",             { 14, 14, 0 }, 3,  40  },
    { "Goblin Arqueiro",    { 10, 10, 0 }, 3,  44  },
    { "Goblin Guerreiro",   { 14, 14, 1 }, 3,  44  },
    { "Goblin de Elite",    { 18, 18, 1 }, 4,  55  },
    { "Lobo de Alcateia",   { 28, 28, 2 }, 5,  60  },
    { "Lobo de Alcateia",   { 28, 28, 2 }, 5,  67  },
    { "Lobo de elite",      { 32, 32, 2 }, 8,  78  },
    { "Ogro",               { 45, 45, 3 }, 8,  95  },
    { "Ogro Chefe",         { 60, 60, 4 }, 12, 150 },
    { "Dragao",             { 80, 80, 5 }, 14, 200 }
};
#define NUM_INIMIGOS (sizeof(arenaInimigos) / sizeof(arenaInimigos[0]))

/* ─────────────────────────────────────────
   Tabela de moves exibida no menu
───────────────────────────────────────── */

typedef struct {
    MoveID      id;
    const char *nome;
    uint        nivelMin;
    uint        dano;
} MoveMenu;

static MoveMenu movesGuerreiro[] = {
    { MOVE_PORRADA,        "Porrada",        1,  5  },
    { MOVE_FURIA,          "Furia",          5,  10 },
    { MOVE_ECLIPSE_MORTAL, "Eclipse Mortal", 15, 28 }
};

static MoveMenu movesArqueiro[] = {
    { MOVE_TIRO,           "Tiro",           1,  8  },
    { MOVE_FLECHA_DE_OURO, "Flecha de Ouro", 5,  18 },
    { MOVE_MIRA_DA_MORTE,  "Mira da Morte",  15, 35 }
};

static MoveMenu movesMago[] = {
    { MOVE_EVIDENCIA,      "Evidencia",      1,  9  },
    { MOVE_CONJURACAO,     "Conjuracao",     5,  16 },
    { MOVE_BOLA_DE_FOGO,   "Bola de fogo",   15, 35 }
};

#define NUM_MOVES_CLASSE 3

/* ─────────────────────────────────────────
   Utilitários
───────────────────────────────────────── */

void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void printSep(void) {
    printf("----------------------------------------\n");
}

void printBarraHP(const char *nome, const HP *hp) {
    uint total  = 20;
    uint cheios = (hp->maxHP > 0) ? (hp->currentHP * total / hp->maxHP) : 0;
    uint i;
    printf("  %-12s [", nome);
    for (i = 0; i < total; i++) printf(i < cheios ? "#" : ".");
    printf("] %u/%u", hp->currentHP, hp->maxHP);
    if (hp->Shield > 0) printf("  Shield:%u", hp->Shield);
    printf("\n");
}

void printStatus(const char *nome, const Personagem *p, const LevelData *ld) {
    printSep();
    printf("  %s  |  Nivel %u  |  EXP %u/%u\n",
           nome, ld->currentLevel, ld->currentExp, MAX_EXP);
    printBarraHP(nome, &p->hp);
    printSep();
}

/* ─────────────────────────────────────────
   Seleção de classe
───────────────────────────────────────── */

ClasseID escolherClasse(void) {
    int op = 0;
    printf("\nEscolha sua classe:\n");
    printf("  1. Guerreiro    (HP: 10 | Crescimento: +8hp/nivel | Ataques corpo a corpo)\n");
    printf("  2. Arqueiro     (HP:  6 | Crescimento: +2hp/nivel | Ataques a distancia)\n");
    printf("  3. Mago Arcano  (HP:  6 | Crescimento: +1hp/nivel | Ataques magicos)\n");
    printf("> ");
    scanf("%d", &op);
    limparBuffer();
    switch (op){
        case 1: return CLASSE_GUERREIRO;
        case 2: return CLASSE_ARQUEIRO;
        case 3: return CLASSE_MAGO;

        default: return CLASSE_GUERREIRO;
        
    }
}

/* ─────────────────────────────────────────
   Menu de ataque — mostra desbloqueados e bloqueados
───────────────────────────────────────── */

MoveID escolherMove(const Personagem *heroi, const LevelData *ld) {
    MoveMenu *lista = NULL;
    switch (heroi->classe){
        case CLASSE_GUERREIRO:  
            lista = movesGuerreiro;
            break;
        case CLASSE_ARQUEIRO:   
            lista = movesArqueiro;
            break;
        case CLASSE_MAGO:       
            lista = movesMago;
            break;
        default: 
            lista = movesGuerreiro;
            break;
    }
    int i, op;

    printf("\n  Escolha seu ataque:\n");
    for (i = 0; i < NUM_MOVES_CLASSE; i++) {
        if (ld->currentLevel >= lista[i].nivelMin) {
            printf("    %d. %-18s  dano: %u\n",
                   i + 1, lista[i].nome, lista[i].dano);
        } else {
            printf("    %d. %-18s  (bloqueado - nivel %u)\n",
                   i + 1, lista[i].nome, lista[i].nivelMin);
        }
    }
    printf("  > ");
    scanf("%d", &op);
    limparBuffer();

    op--;
    if (op < 0 || op >= NUM_MOVES_CLASSE) op = 0;

    if (ld->currentLevel < lista[op].nivelMin) {
        printf("  Move ainda bloqueado! Usando o primeiro disponivel.\n");
        op = 0;
    }

    return lista[op].id;
}

/* ─────────────────────────────────────────
   Loop de batalha
───────────────────────────────────────── */

int batalha(const char *nomeHeroi, Personagem *heroi,
            LevelData *ld, Inimigo *ini) {

    ini->hp.currentHP = ini->hp.maxHP;  /* reseta o inimigo */

    int turno = 1;
    printf("\n");
    printSep();
    printf("  BATALHA: %s  vs  %s\n", nomeHeroi, ini->nome);
    printSep();

    while (isAlive(&heroi->hp) && isAlive(&ini->hp)) {
        
        printf("\n  -- Turno %d --\n", turno);
        printBarraHP(nomeHeroi, &heroi->hp);
        printBarraHP(ini->nome, &ini->hp);

        /* Turno do jogador */
        MoveID move = escolherMove(heroi, ld);
        uint dano   = usarMove(heroi, move, &ini->hp);

        if (dano == 0)
            printf("  > Ataque totalmente absorvido pelo escudo!\n");
        else
            printf("  > Você causou %u de dano em %s!\n", dano, ini->nome);

        if (!isAlive(&ini->hp)) break;

        /* Turno do inimigo */
        printf("  > %s ataca! Causa %u de dano.\n", ini->nome, ini->danoBase);
        takeDamage(ini->danoBase, &heroi->hp);

        turno++;
    }

    printf("\n");
    printBarraHP(nomeHeroi, &heroi->hp);
    printf("\n");

    if (!isAlive(&heroi->hp)) {
        printf("  Você foi derrotado por %s. Game Over.\n", ini->nome);
        return 0;
    }

    printf("  Você derrotou %s!\n", ini->nome);
    printf("  + %u EXP\n", ini->expRecompensa);

    uint niveis = addExp(ld, ini->expRecompensa);
    uint i;
    for (i = 0; i < niveis; i++) {
        growthStats(heroi);
        printf("\n  *** LEVEL UP! Agora você é nivel %u ***\n", ld->currentLevel);
        printf("  HP maximo: %u\n", heroi->hp.maxHP);

        /* Notifica moves desbloqueados */
        MoveMenu *lista = NULL;
        switch (heroi->classe){
            case CLASSE_GUERREIRO:  lista = movesGuerreiro;
            case CLASSE_ARQUEIRO:   lista = movesArqueiro;
            case CLASSE_MAGO:       lista = movesMago;
            default: lista = movesGuerreiro;
        }

        int j;
        for (j = 0; j < NUM_MOVES_CLASSE; j++) {
            if (ld->currentLevel == lista[j].nivelMin)
                printf("  --> Novo move desbloqueado: %s!\n", lista[j].nome);
        }
    }

    return 1;
}

/* ─────────────────────────────────────────
   main
───────────────────────────────────────── */

int main(void) {
    char       nome[NOME_MAX];
    Personagem heroi;
    LevelData  ld;

    srand(time(NULL));
    
    printSep();
    printf("         BEM-VINDO AO RPG\n");
    printSep();

    /* Nome */
    printf("\nNome do personagem: ");
    fgets(nome, NOME_MAX, stdin);
    nome[strcspn(nome, "\n")] = '\0';
    if (strlen(nome) == 0) strcpy(nome, "Heroi");

    /* Classe e inicialização */
    ClasseID classe = escolherClasse();
    initPersonagem(&heroi, classe);
    initLevel(&ld);

    /* Equipa itens iniciais conforme classe */
    if (classe == CLASSE_GUERREIRO) {
        equipArmament(ARMAMENT_WOOD_SWORD);
        equipArmor(ARMOR_STRING, &heroi.hp);
        printf("\nEquipado: Espada de Madeira (dano +2) + Armadura de Couro (shield +1)\n");
    } else if (classe == CLASSE_ARQUEIRO) {
        equipArmament(ARMAMENT_WOOD_BOW);
        equipShield(SHIELD_WOOD, &heroi.hp);
        printf("\nEquipado: Arco de Madeira (dano +3) + Escudo de Madeira (shield +2)\n");
    } else {
        equipArmament(ARMAMENT_WOOD_ROD);
        equipShield(BOOK_BEGGINER, &heroi.hp);
        printf("\nEquipado: Vara de Madeira (dano +2) + livro do iniciante (shield +1, dano +1)\n");
    }

    printf("\nBoa sorte, %s!\n", nome);
    printStatus(nome, &heroi, &ld);

    /* Arena */
    int i;
    for (i = 0; i < NUM_INIMIGOS; i++) {
        
        printf("\nProximo desafio: %s  (HP: %u | Shield: %u | Dano: %u)\n",
               arenaInimigos[i].nome,
               arenaInimigos[i].hp.maxHP,
               arenaInimigos[i].hp.Shield,
               arenaInimigos[i].danoBase);

        printf("O que deseja fazer?\n");
        printf("  1. Entrar em batalha\n");
        printf("  2. Usar Ervas (cura +5 HP) e batalhar\n");
        printf("  3. Usar Drogas (cura +1, escudo +1)\n");
        printf("> ");
        int esc = 1;
        scanf("%d", &esc);
        limparBuffer();

        if (esc == 2) {
            uint antes = heroi.hp.currentHP;
            useConsumable(CONSUMABLE_HERB, &heroi.hp);
            printf("Ervas usadas: HP %u -> %u\n", antes, heroi.hp.currentHP);
        }else if(esc == 3) {
            uint antes = heroi.hp.currentHP;
            useConsumable(CONSUMABLE_DRUG, &heroi.hp);
            printf("Drogas usadas: HP %u -> %u, escudo %u -> %u\n", antes, heroi.hp.currentHP, antes, heroi.hp.Shield);
        }

        if (!batalha(nome, &heroi, &ld, &arenaInimigos[i])) {
            return 0;
        }

        printStatus(nome, &heroi, &ld);
    }

    printSep();
    printf("  %s completou toda a arena!\n", nome);
    printf("  Nivel final: %u | EXP: %u/%u\n",
           ld.currentLevel, ld.currentExp, MAX_EXP);
    printSep();

    return 0;
}