/* 
 * A demo of playerclass functionality using x/battle-systems' battle_flow_example.c
 * This executable aims to demonstrate:
 * - Some information playerclass stores
 * - Proof-of-concept class-specific skills
 * - Class-specific item restrictions
 * - How playerclass plays a role in the battle flow
 */ 

#include <criterion/criterion.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "playerclass/class.h"
#include "playerclass/class_item_restriction.h"
#include "playerclass/class_structs.h"
#include "demo-functions.h"
#include "class-list.h"

void main()
{
    move_list_t* classlist = move_list();

    printf("Please choose a class from the the following list:\n");
    list_class_show(classlist);

    printf("You have chosen: \n");
    class_show(classlist->c);

    printf("Proceeding with battle.\n");
    
    printf("***Battle occurs here***\n");

    printf("Battle has ended. You have won.\n");
    
    item_t* wand = item_new("wand", " ", " ");
    item_t* sword = item_new("sword", " ", " ");
    item_t* guitar = item_new("guitar", " "," ");

    set_item_restriction(wand, classlist->c);
    set_item_restriction(sword, classlist->c);
    
    printf("Your enemy has dropped the following items: %s, %s, %s. You attempt to pick them up with mixed results.\n",
	    wand->item_id, sword->item_id, guitar->item_id);

    bool not_allowed_wand = get_class_restriction(wand, classlist->c);
    bool not_allowed_sword = get_class_restriction(sword, classlist->c);
    bool not_allowed_guitar = get_class_restriction(guitar, classlist->c);

    printf("You are %s to use the  %s, %s to use the %s and %s to use the %s because you are a %s \n",
	    not_allowed_wand? "not able":"able",
	    wand->item_id,
	    not_allowed_sword? "not able":"able",
	    sword->item_id,
	    not_allowed_guitar? "not_able":"able",
	    guitar->item_id,
	    classlist->c->name);
}
