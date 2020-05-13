/* Team RPG-Openworld
 * 
 * sample_generation.c: This file. Function definitions of the functions 
 * specified in chiventure/include/sample_generation.h
 * 
 * Create a high level room generation module that pulls the lower-level 
 * room generation modules together
 * 
 * See chiventure/include/sample_generation.h header file to see function 
 * prototypes and purposes
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../../../include/openworld/sample_generation.h"

/* See sample_generation.h */
bool enter_new_room(room_t *r1, room_t *r2) {
    // Check if they have the same hash handle pointer
    return (bool) (r1->hh.key == r2->hh.key);
}

/* See sample_generation.h */
int room_generate(room_t* roomOld, room_t* roomNew, room_t* addRoom) {
    /* BELOW: implement algo from autogenerate algorithm module */
    if (enter_new_room(roomOld, roomNew)) { // Somehow modify the game states that hold these rooms, respectively
        /* BELOW: TO BE EDITED */

        // Add addRoom to the game that has roomNew
        // assert(0 == add_room_to_game(gameNew, addRoom)); // Maybe we could do something like this?
        
        // Add path from the current room to addRoom
        // Use dependency from game-state/path.h for the following function:
        path_t* path_to_room = path_new(addRoom, "to new room");
        // assert (0 == add_path_to_room(gameNew->curr_room, path_to_room)); // Maybe we could do something like this?

        /* ABOVE: TO BE EDITED */

        return 0; /* SUCCESS - room added */
    }

    return 1; /* room not added */
}

int autogen_algorithm(void) {
    /* TODO */

    /* Ideas: We should find a "random" library that pulls randomly from a 
     * given list of items (or structs). We can pull from a hard-coded list 
     * of structs randomly, only when desired. Should have a function that 
     * uses this random library to randomly pull from a given list/database/
     * hashtable (up for discussion) when the user chooses to. Does nothing 
     * when a new room is not desired */

    return 1; // Depends algorithm module to be created
}