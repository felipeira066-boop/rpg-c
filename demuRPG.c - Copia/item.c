#include "item.h"

/* ─────────────────────────────────────────
   Definição das variáveis globais declaradas no .h
───────────────────────────────────────── */

Item equippedArmament = {ARMAMENT_NONE, 0, 0, 0, 0};
Item equippedShield   = {SHIELD_NONE,   0, 0, 0, 0};
Item equippedArmor    = {ARMOR_NONE,    0, 0, 0, 0};

/* ─────────────────────────────────────────
   Dados fixos dos itens (tabela interna)
   Assim é fácil adicionar novos itens depois.
───────────────────────────────────────── */

static Item armamentTable[] = {
    /*  id                      dmg  shld  heal  dur */
    {ARMAMENT_NONE,              0,   0,    0,    0  },
    {ARMAMENT_WOOD_SWORD,        2,   0,    0,    28 },
    {ARMAMENT_WOOD_BOW,          3,   0,    0,    20 },
    {ARMAMENT_WOOD_ROD,          2,   0,    0,    20 }
};

static Item shieldTable[] = {
    {SHIELD_NONE,       0,  0,  0,  0  },
    {SHIELD_WOOD,       0,  2,  0,  10 },
    {BOOK_BEGGINER,     2,  1,  0,  10 }
};

static Item armorTable[] = {
    {ARMOR_NONE,    0,  0,  0,  0  },
    {ARMOR_STRING,  0,  1,  0,  16 },
};

static Item consumableTable[] = {
    {CONSUMABLE_NONE,  0,  0,  0,  0 },
    {CONSUMABLE_HERB,  0,  0,  5,  1 },
    {CONSUMABLE_DRUG,  0,  2,  1,  1 }
};

/* ─────────────────────────────────────────
   Implementações
───────────────────────────────────────── */

int equipArmament(ArmamentID id) {
    if (id <= ARMAMENT_NONE || id > ARMAMENT_WOOD_ROD) {
        return 0;
    }
    equippedArmament = armamentTable[id];
    return 1;
}

int equipShield(ShieldID id, HP *hp) {
    if (id <= SHIELD_NONE || id > BOOK_BEGGINER || hp == NULL) {
        return 0;
    }
    /* Remove bônus do escudo anterior antes de equipar o novo */
    hp->Shield -= equippedShield.shieldBonus;

    equippedShield = shieldTable[id];
    hp->Shield += equippedShield.shieldBonus;
    return 1;
}

int equipArmor(ArmorID id, HP *hp) {
    if (id <= ARMOR_NONE || id > ARMOR_STRING || hp == NULL) {
        return 0;
    }
    hp->Shield -= equippedArmor.shieldBonus;

    equippedArmor = armorTable[id];
    hp->Shield += equippedArmor.shieldBonus;
    return 1;
}

int useConsumable(ConsumableID id, HP *hp) {
    if (id <= CONSUMABLE_NONE || id > CONSUMABLE_DRUG || hp == NULL) {
        return 0;
    }
    Item potion = consumableTable[id];

    /* Cura sem ultrapassar o HP máximo */
    if (hp->currentHP + potion.heal > hp->maxHP) {
        hp->currentHP = hp->maxHP;
    } else {
        hp->currentHP += potion.heal;
    }
    return 1;
}

uint getArmamentDamage(void) {
    return equippedArmament.damage;
}