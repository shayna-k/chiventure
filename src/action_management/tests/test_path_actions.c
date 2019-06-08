#include <criterion/criterion.h>
#include <stdio.h>
#include <stdlib.h>
#include "actionmanagement.h"
#include "action_structs.h"
#include "item.h"
#include "player.h"
#include "ctx.h"

#define BUFFER_SIZE (100)
#define WRONG_KIND (1)

int execute_do_path_action(char *c_name, enum action_kind kind)
{
    chiventure_ctx_t *wrapper = chiventure_ctx_new();
    player_t *player = player_new("player", 1);
    add_player_to_game(wrapper->game, player);
    set_curr_player(wrapper->game, player);
    room_t *dest = room_new("dummyroom", "a dummy room", "a placeholder room");
    char *direction = "south";
    action_type_t *a = action_type_new(c_name, kind);
    path_t *p = path_new(dest, direction);
    char *string = malloc(BUFFER_SIZE);

    int rc = do_path_action(wrapper, a, p, &string);

    free(string);
    path_free(p);
    action_type_free(a);
    chiventure_ctx_free(wrapper);

    return rc;
}

Test(path_actions, kind_ITEM)
{
    int rc = execute_do_path_action("dummy", ITEM);

    cr_assert_eq(rc, WRONG_KIND,
                 "execute_do_path_action returned %d for wrong kind ITEM, expected WRONG_KIND (1)", rc);
}


Test(path_actions, kind_PATH)
{

    int rc = execute_do_path_action("dummy", PATH);

    cr_assert_eq(rc, SUCCESS,
                 "execute_do_path_action returned %d for correct kind PATH expected SUCCESS (0)", rc);
}


Test(path_actions, kind_ITEM_ITEM)
{
    int rc = execute_do_path_action("dummy", ITEM_ITEM);

    cr_assert_eq(rc, WRONG_KIND,
                 "execute_do_path_action returned %d for wrong kind ITEM_ITEM expected WRONG_KIND (1)");
}
