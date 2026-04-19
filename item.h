#ifndef ITEM_H
#define ITEM_H
 
#include "uint_type.h"
#include "hp.h"
 
/* ─────────────────────────────────────────
   Enums: identificam cada item por um ID
   Em vez de variáveis soltas (woodSword, woodBow...),
   usamos um enum — organizado e sem colisão de nomes.
───────────────────────────────────────── */
 
typedef enum {
    ARMAMENT_NONE    = 0,
    ARMAMENT_WOOD_SWORD = 1,
    ARMAMENT_WOOD_BOW   = 2,
    ARMAMENT_WOOD_ROD   = 3
} ArmamentID;
 
typedef enum {
    SHIELD_NONE       = 0,
    SHIELD_WOOD       = 1,
    BOOK_BEGGINER    = 2
} ShieldID;
 
typedef enum {
    ARMOR_NONE        = 0,
    ARMOR_STRING      = 1
} ArmorID;
 
typedef enum {
    CONSUMABLE_NONE   = 0,
    CONSUMABLE_HERB   = 1,
    CONSUMABLE_DRUG   = 2
} ConsumableID;
 
/* ─────────────────────────────────────────
   Struct: representa qualquer item equipável.
   Campos simples — sem código dentro da struct.
───────────────────────────────────────── */
 
typedef struct {
    uint id;          /* ID do item (use os enums acima)  */
    uint damage;      /* Bônus de dano (armas)            */
    uint shieldBonus; /* Bônus de escudo (escudos/armaduras) */
    uint heal;        /* Cura fornecida (consumíveis)     */
    uint durability;  /* Durabilidade atual               */
} Item;
 
/* ─────────────────────────────────────────
   Variáveis globais: item atualmente equipado.
   Declaradas com extern aqui — definidas em item.c
───────────────────────────────────────── */
 
extern Item equippedArmament;
extern Item equippedShield;
extern Item equippedArmor;
 
/* ─────────────────────────────────────────
   Funções: preenchidas em item.c
   Todas recebem ponteiros — sem modificar cópias locais.
───────────────────────────────────────── */
 
/* Equipa uma arma. Retorna 1 se equipou com sucesso, 0 se ID inválido. */
int equipArmament(ArmamentID id);
 
/* Equipa um escudo. Aplica bônus de escudo ao HP do personagem. */
int equipShield(ShieldID id, HP *hp);
 
/* Equipa uma armadura. Aplica bônus de escudo passivo. */
int equipArmor(ArmorID id, HP *hp);
 
/* Usa um consumível. Aplica cura diretamente no HP. */
int useConsumable(ConsumableID id, HP *hp);
 
/* Retorna o dano atual da arma equipada (0 se nenhuma). */
uint getArmamentDamage(void);
 
#endif /* ITEM_H */
 