#include <stdlib.h>
#include <stdio.h>
#include <criterion/criterion.h>
#include <stdbool.h>
#include <string.h>
#include "battle/battle_flow.h"
#include "battle/battle_flow_structs.h"
#include "battle/battle_print.h"
#include "battle/battle_structs.h"

/* Tests print_start_battle() */
Test(battle_print, print_start_battle)
{
    // Setting up a battle with set_battle
    stat_t *player_stats = calloc(1,sizeof(stat_t));
    stat_t *enemy_stats = calloc(1,sizeof(stat_t));
    player_t *ctx_player = new_ctx_player("player_name", NULL, player_stats, NULL, NULL);
    npc_enemy_t *npc_enemy = make_npc_enemy("Bob", NULL, enemy_stats, NULL, NULL);
    environment_t env = ENV_DESERT;
    battle_t *b = set_battle(ctx_player, npc_enemy, env);
    cr_assert_not_null(b, "set_battle() failed");
    b->player->stats->hp = 100;
    b->enemy->stats->hp = 100;

    char* string = print_start_battle(b);
    cr_assert_not_null(string, "print_start_battle() failed");

    char *expected_string = "You have encountered Bob!\n\n"
                            "Let the battle begin!\n"
                            "-- Your HP: 100\n"
                            "ENEMY HP\n"
                            "-- Bob's HP: 100\n";
    cr_expect_str_eq(string, expected_string, "print_start_battle() failed to set string");

    free(string);
}

/* Tests print_hp() for battle against one enemy */
Test(battle_print, print_hp_one_enemy)
{
    /* Setting up a battle with set_battle */
    stat_t *player_stats = calloc(1,sizeof(stat_t));
    stat_t *enemy_stats = calloc(1,sizeof(stat_t));
    player_t *ctx_player = new_ctx_player("player_name", NULL, player_stats, NULL, NULL);
    npc_enemy_t *npc_enemy = make_npc_enemy("Bob", NULL, enemy_stats, NULL, NULL);
    environment_t env = ENV_DESERT;
    battle_t *b = set_battle(ctx_player, npc_enemy, env);
    cr_assert_not_null(b, "set_battle() failed");
    b->player->stats->hp = 89;
    b->enemy->stats->hp = 64;

    /* Set up string to store message in */
    char* string = calloc(BATTLE_BUFFER_SIZE + 1, sizeof(char));

    /* Test print_hp() */
    int rc = print_hp(b, string);
    cr_assert_eq(rc, SUCCESS, "print_hp() failed");

    char *expected_string = "-- Your HP: 89\n"
                            "ENEMY HP\n"
                            "-- Bob's HP: 64\n";

    cr_expect_str_eq(string, expected_string, "print_hp() failed to set string");

    free(string);
}

/* Tests print_hp() for battle against two enemies */
Test(battle_print, print_hp_two_enemies)
{
    /* Setting up a battle with set_battle */
    stat_t *player_stats = calloc(1,sizeof(stat_t));
    stat_t *e1_stats = calloc(1,sizeof(stat_t));
    stat_t *e2_stats = calloc(1,sizeof(stat_t));
    player_t *ctx_player = new_ctx_player("player_name", NULL, player_stats, NULL, NULL);
    npc_enemy_t *head = NULL;
    npc_enemy_t *e1 = make_npc_enemy("Bob", NULL, e1_stats, NULL, NULL);
    npc_enemy_t *e2 = make_npc_enemy("Annie", NULL, e2_stats, NULL, NULL);
    DL_APPEND(head, e1);
    DL_APPEND(head, e2);
    cr_assert_not_null(e1, "make_npc_enemy() failed");
    cr_assert_not_null(e2, "make_npc_enemy() failed");
    environment_t env = ENV_DESERT;
    battle_t *b = set_battle(ctx_player, head, env);
    cr_assert_not_null(b, "set_battle() failed");
    b->player->stats->hp = 89;
    b->enemy->stats->hp = 64;
    b->enemy->next->stats->hp = 75;

    /* Set up string to store message in */
    char* string = calloc(BATTLE_BUFFER_SIZE + 1, sizeof(char));

    /* Test print_hp() */
    int rc = print_hp(b, string);
    cr_assert_eq(rc, SUCCESS, "print_hp() failed");

    char *expected_string = "-- Your HP: 89\n"
                            "ENEMY HP\n"
                            "-- Bob's HP: 64\n"
                            "-- Annie's HP: 75\n";

    cr_expect_str_eq(string, expected_string, "print_hp() failed to set string");

    free(string);
}

/* Tests print_battle_move() on a player move */
Test(battle_print, print_player_move)
{
    // Setting up a battle with set_battle
    stat_t *player_stats = calloc(1,sizeof(stat_t));
    stat_t *enemy_stats = calloc(1,sizeof(stat_t));
    player_t *ctx_player = new_ctx_player("player_name", NULL, player_stats, NULL, NULL);
    npc_enemy_t *npc_enemy = make_npc_enemy("Bob", NULL, enemy_stats, NULL, NULL);
    environment_t env = ENV_DESERT;
    battle_t *b = set_battle(ctx_player, npc_enemy, env);
    cr_assert_not_null(b, "set_battle() failed");
    b->player->stats->hp = 100;
    b->enemy->stats->hp = 98;

    // Set up a move
    move_t *move = calloc(1,sizeof(move_t));
    move->damage = 2;
    move->info = "Punch";
    b->player->moves = move;

    char* string = print_battle_move(b, PLAYER, move);
    cr_assert_not_null(string, "print_start_battle() failed");

    char *expected_string = "You used Punch! It did 2 damage.\n"
                            "-- Your HP: 100\n"
                            "ENEMY HP\n"
                            "-- Bob's HP: 98\n";

    cr_expect_str_eq(string, expected_string, "print_player_move() failed to set string");

    free(string);
}

/* Tests print_battle_move() on an enemy move */
Test(battle_print, print_enemy_move)
{
    // Setting up a battle with set_battle
    stat_t *player_stats = calloc(1,sizeof(stat_t));
    stat_t *enemy_stats = calloc(1,sizeof(stat_t));
    player_t *ctx_player = new_ctx_player("player_name", NULL, player_stats, NULL, NULL);
    npc_enemy_t *npc_enemy = make_npc_enemy("Bob", NULL, enemy_stats, NULL, NULL);
    environment_t env = ENV_WATER;
    battle_t *b = set_battle(ctx_player, npc_enemy, env);
    cr_assert_not_null(b, "set_battle() failed");
    b->player->stats->hp = 1;
    b->enemy->stats->hp = 100;

    // Set up a move
    move_t *move = calloc(1,sizeof(move_t));
    move->damage = 99;
    move->info = "Laugh";
    b->player->moves = move;

    char* string = print_battle_move(b, ENEMY, move);
    cr_assert_not_null(string, "print_start_battle() failed");

    char *expected_string = "Bob used Laugh! It did 99 damage.\n"
                            "-- Your HP: 1\n"
                            "ENEMY HP\n"
                            "-- Bob's HP: 100\n";

    cr_expect_str_eq(string, expected_string, "print_enemy_move() failed to set string");

    free(string);
}


/* Tests print_battle_winner() when player wins */
Test(battle_print, print_player_winner)
{
    battle_status_t status = BATTLE_VICTOR_PLAYER;
    int xp = 2;

    char* string = print_battle_winner(status, xp);
    cr_assert_not_null(string, "print_start_battle() failed");

    char *expected_string = "You've won! You gain 2 XP!\n";
    cr_expect_str_eq(string, expected_string, "print_player_winner() failed to set string");

    free(string);
}

/* Tests print_battle_winner() when enemy wins */
Test(battle_print, print_enemy_winner)
{
    battle_status_t status = BATTLE_VICTOR_ENEMY;
    int xp = 2;

    char* string = print_battle_winner(status, xp);
    cr_assert_not_null(string, "print_start_battle() failed");

    char *expected_string = "You have been defeated!\n";
    cr_expect_str_eq(string, expected_string, "print_enemy_winner() failed to set string");

    free(string);
}
