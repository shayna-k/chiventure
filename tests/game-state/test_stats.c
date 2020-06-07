#include <criterion/criterion.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "game-state/stats.h"

/* Checks that add_stat_player correctly adds a new stat 
   to a hash table*/
Test (stats, add_stat_player)
{
    stats_hash_t *sh = NULL;
    stats_global_t g;
    g.name = "health";
    g.max = 100;

    stats_t s;
    s.key = "health";
    s.global = &g;
    s.val = 50;
    s.max = 75;
    s.modifier = 1.1;

    stats_t *stat = stats_new("health", 100);
    cr_assert_not_null(stat, "stats_new() failed. Health stat is NULL");
    cr_assert_eq(strcmp(stat-> global -> name,
        "health"), 0,
        "stats_new() failed to link the global stat pointer");
    cr_assert_eq(stat->val, 100, 
        "stats_new() failed to set the starting stat value");
    cr_assert_leq(stat->val, stat->global->max, 
        "stat base value exceeds maximal value.");
}

/* Checks that display_stat returns the correct list of stats */
Test(stats, display_stat)
{
    stats_hash_t *sh = NULL;

    stats_global_t health;
    health.name = "health";
    health.max = 100;

    stats_t s1;
    s1.key = "health";
    s1.global = &health;
    s1.val = 50;
    s1.max = 75;
    s1.modifier = 1.1;

    int rc = add_stat_player(&sh, &s1);

    cr_assert_eq(rc, SUCCESS, "add_stat_player failed");


    stats_global_t speed;
    speed.name = "speed";
    speed.max = 100;

    stats_t s2;
    s2.key = "speed";
    s2.global = &speed;
    s2.val = 25;
    s2.max = 50;
    s2.modifier = 0.9;

    rc = add_stat_player(&sh, &s2);

    cr_assert_eq(rc, SUCCESS, "add_stat_player failed");

    char *list = display_stats(sh);

    cr_assert_str_eq(list, "health, speed",
                     "expected: health, speed, but display_stat returned %s",
                     list);

    free(list);
}

/* Checks that global_effect_init correctly initializes a global effect struct */
Test (stats, effect_global_init)
{
    effects_global_t effect;

    int rc = global_effect_init(&effect, "health");

    cr_assert_eq(rc, SUCCESS, "global_effect_init failed");
    cr_assert_not_null(effect.name, "global_effect_init did not set effect name");
    cr_assert_str_eq(effect.name, "health", "global_effect_init did not set name");
}
Test(stats, init){
    stats_global_t* stat_global = stats_global_new("health", 100);
    cr_assert_not_null(stat_global, 
        "stats_global_new() failed. Health stat is NULL");

    stats_t* stat;
    int ret_val = stats_init(stat, "health", 100);
    cr_assert_eq(ret_val, SUCCESS, "stats_init() failed to return SUCCESS");

    cr_assert_not_null(stat, "stats_init() failed. Health stat is NULL");
    
    cr_assert_eq(strcmp(stat->global->name,
        "health"), 0,
        "stats_init() failed to set the starting stat name");
    cr_assert_eq(stat->val, 100, 
        "stats_new() failed to set the starting stat value");
    cr_assert_eq(stat->modifier, 0, 
        "stats_new() failed be set the modifier to 0");
    cr_assert_leq(stat->val, stat->global->max, 
        "stat base value exceeds maximal value");
}

/* Checks that global_effect_new correctly creates a new global effect struct */
Test (stats, effect_global_new)
{
    effects_global_t *effect = global_effect_new("health");

    stats_t* stat = stats_new("health", 100);
    cr_assert_not_null(stat, "stats_new() failed. Player health stat is NULL");
    
    int ret_val = free_stats(stat);
    cr_assert_eq (ret_val, SUCCESS, "free_stats() failed to return SUCCESS");
}

/* Checks that effect_init correctly initializes an effect struct */
Test (stats, stat_effect_init)
{
    effects_global_t *global = global_effect_new("speed");
    cr_assert_not_null(global, "global_effect_new failed");

    stat_effect_t effect;

    int rc = stat_effect_init(&effect, global);

    cr_assert_eq(rc, SUCCESS, "stat_effect_init failed");
    cr_assert_str_eq(effect.key, global->name, "stat_effect_init did not set key");
    cr_assert_eq(effect.global, global, "stat_effect_init did not set global pointer");
}

/* Checks that stat_effect_new correctly creates a new player effect */
Test (stats, stat_effect_new)
{
    effects_global_t *global = global_effect_new("health");
    cr_assert_not_null(global, "global_effect_new failed");

    stat_effect_t *effect = stat_effect_new(global);

    cr_assert_not_null(effect, "stat_effect_new failed");
    cr_assert_str_eq(effect->key, global->name, "stat_effect_new did not set key");
    cr_assert_eq(effect->global, global, "stat_effect_new did not set global pointer");
}
Test(stats,global_free){
    stats_global_t* stat = stats_global_new ("health",100);
    cr_assert_not_null(stat, "stats_global_new() failed. Global health stat is NULL");

    int ret_val = free_stats_global(stat);
    cr_assert_eq(ret_val, SUCCESS, "free_stats_global() failed to return SUCCESS");
}

/*test(stats, get_stat_current) {
    stats_global_t* stat_global = stats_global_new("health", 100);

    stats_t* stat = stats_new("health", 100);
    HASH_ADD_INT(stat)

    int ret_val = get_stat_current(stat_global, stat, 0.5);
    cr_assert_eq(ret_val, 100, "free_stats_global() failed to return SUCCESS");
}*/

stats_hash_t *add_stats(int type) {
    stats_hash_t *sh = NULL;

    stats_global_t g1;
    g1.name = "health";
    g1.max = 100;

    stats_global_t g2;
    g2.name = "charisma";
    g2.max = 200;

    stats_global_t g2;
    g3.name = "charisma";
    g3.max = 200;

    stats_t s1;
    s1.key = "health";
    s1.global = &g2;
    s1.val = 50;
    s1.max = 75;
    s1.modifier = 1.1;

    stats_t s2;
    s2.key = "charisma";
    s2.global = &g2;
    s2.val = 75;
    s2.max = 130;
    s2.modifier = 1;

    stats_t s3;
    s3.key = "strength";
    s3.global = &g2;
    s3.val = 20;
    s3.max = 92;
    s3.modifier = 2;

    int rc, rc1, rc2;

    if (type == 1) {
        rc = add_stat_player(&sh, &s);
    } else if  (type == 2) {
        rc = add_stat_player(&sh, &s);
        rc2 = add_stat_player(&sh, &s2);
    } else if  (type == 3) {
        rc = add_stat_player(&sh, &s);
        rc2 = add_stat_player(&sh, &s2);
        rc3 = add_stat_player(&sh, &s3);
    }

    return sh;
}

Test(stats, change_stat) {
    stats_hash_t *sh = add_stats(3);
    
    cr_assert_eq(sh->val, 50, 
        "change_stat base value not equal initially");

    change_stat(sh, "strength", 10);
    cr_assert_eq(sh->val, 63, 
        "change_stat failed to return success");
}

/*Test(stats, get_stat_max) {}

Test(stats, get_stat_mod) {}*/
