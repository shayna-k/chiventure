#include "battle/battle_logic.h"
#include "common/utlist.h"

/* check battle_logic.h */
bool check_target(battle_t *b, char *target)
{
    combatant_t *temp;
    DL_FOREACH(b->enemy, temp)
    {
        if (strncmp(temp->name, target, MAX_NAME_LEN) == 0)
        {
            return true;
        }
    }
    return false;
}

/* check battle_logic.h */
int battle_over(combatant_t *p, combatant_t *e)
{
    if (e->stats->hp > 0 && p->stats->hp > 0)
    {
        return 0;
    }
    else if (e->stats->hp <= 0)
    {
        return 1;
    }
    else if (p->stats->hp <= 0)
    {
        return 2;
    }
    else
    {
        return -1;
    }
}

/* check battle_logic.h */
turn_t goes_first(battle_t *b)
{
    combatant_t *temp;
    DL_FOREACH(b->enemy, temp)
    {
        if (b->player->stats->speed > temp->stats->speed ||
            b->player->stats->speed == temp->stats->speed)
        {
            return PLAYER;
        }
    }
    return ENEMY;
}

/* see battle_logic.h */
item_t *find_item(item_t *inventory, int id)
{
    item_t *temp;

    DL_FOREACH(inventory, temp)
    {
        if (temp->id == id)
        {
            return temp;
        }
    }

    return NULL;
}

/* see battle_logic.h */
int consume_item(combatant_t *c, item_t *item)
{
    c->stats->hp += item->hp;
    c->stats->strength += item->attack;
    c->stats->defense += item->defense;
    return 0;
}

/* see battle_logic.h */
int player_use_item(combatant_t *c, item_t *inv, int id)
{
    if (inv == NULL)
    {
        return 1;
    }
    
    item_t *item = find_item(inv, id);
    
    if(item == NULL || item->quantity == 0)
    {
        return 2;
    }

    consume_item(c, item);
    item->quantity -= 1;
    
    return 0;
}

/* see battle_logic.h */
int award_xp(stat_t *stats, double xp)
{
    stats->xp += xp;
    return 0;
}