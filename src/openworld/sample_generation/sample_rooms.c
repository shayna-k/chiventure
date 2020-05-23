#include <stdlib.h>
#include <string.h>

#include "../../../include/common/utlist.h"
#include "../../../include/openworld/sample_rooms.h" 

/* see sample_rooms.h */
roomspec_t **get_allowed_room(char *bucket, char *sh_desc, char *l_desc,
	item_list_t *llist) {

	if (llist == NULL) {
		llist = item_list_new();
	}

	//these types of rooms are shared by more than one themed room group
	roomspec_t *closet = roomspec_new("closet", "A broom closet",
		"A small broom closet with supplies",
		get_allowed_items("closet", llist), NULL, NULL);
	roomspec_t *hallway = roomspec_new("hallway", "A well-lit hallway",
		"A sterile, white hallway with no windows",
		get_allowed_items("hallway", llist), NULL, NULL);
	roomspec_t *library = roomspec_new("library", "This is a library room with resources",
		"An old, dusty library with skill-boosting resources like books and potions",
		get_allowed_items("library", llist), NULL, NULL);

	assert(bucket != NULL);

	if (!strcmp(bucket, "school")) {
		roomspec_t **school_rooms = calloc(5, sizeof(roomspec_t*));
		school_rooms[0] = roomspec_new("cafeteria", "A grungy cafeteria",
			"A messy high school cafeteria with trays and tables out",
			get_allowed_items("cafeteria", llist), NULL, NULL);
		school_rooms[1] = roomspec_new("classroom",
			"A medium-sized classroom with 30 desks",
			"A geography teacher's classroom with 30 desks",
			get_allowed_items("classroom", llist), NULL, NULL);
		school_rooms[2] = closet;
		school_rooms[3] = hallway;
		school_rooms[4] = library;
		return school_rooms;
	}
	else if (!strcmp(bucket, "farmhouse")) {
		roomspec_t **farm_rooms = calloc(5, sizeof(roomspec_t*));
		farm_rooms[0] = roomspec_new("barn", "A red barn",
			"A red barn with stables inside",
			get_allowed_items("barn", llist), NULL, NULL);
		farm_rooms[1] = roomspec_new("open field",
			"An open field outside",
			"An open field with grass and a clear view",
			get_allowed_items("open field", llist), NULL, NULL);
		farm_rooms[2] = roomspec_new("kitchen", "A 60s era (outdated) kitchen",
			"An outdated kitchen with obvious wear-and-tear",
			get_allowed_items("kitchen", llist), NULL, NULL);
		farm_rooms[3] = hallway;
		farm_rooms[4] = roomspec_new("living room", "A living room with basic items",
			"A plain, unremarkable living room",
			get_allowed_items("living room", llist), NULL, NULL);
		roomspec_free(closet);
		roomspec_free(library);
		return farm_rooms;
	} else if(!strcmp(bucket, "castle")) {	
		roomspec_t **castle_rooms = calloc(5, sizeof(roomspec_t*));
		castle_rooms[0] = closet;
		castle_rooms[1] = roomspec_new("dungeon", "A dark dungeon",
			"A dank, dark dungeon with traps and enemies to battle",
			get_allowed_items("dungeon", llist), NULL, NULL);
		castle_rooms[2] = library;
		castle_rooms[3] = hallway;
		castle_rooms[4] = roomspec_new("throne room", "This is a throne room",
			"A regal throne room decked out with lavish items",
			get_allowed_items("throne room", llist), NULL, NULL);
		return castle_rooms;
	} else{
		roomspec_t **rooms = calloc(1, sizeof(roomspec_t*));
		rooms[0] = roomspec_new(bucket, sh_desc, l_desc,
			get_allowed_items(bucket, llist), NULL, NULL);
		return rooms;
	}
}

/* see sample_rooms.h */
roomspec_t *make_default_room(char *bucket, char *sh_desc, char *l_desc,
	item_list_t *items) {
	roomspec_t *hash = NULL;

	//get allowed rooms and defined descriptions
	roomspec_t **rooms = get_allowed_room(bucket, sh_desc, l_desc, items);

	int i = 0;

	while (rooms[i] != NULL) {
		int counter;
		item_list_t *allowed = get_allowed_items(rooms[i]->room_name, items);
		LL_COUNT(allowed, allowed, counter);
		//generate actual item list from allowed items
//		for (unsigned int i = 0; i < counter; i++) {
			//append this to item_hash_t for this room spec
			LL_APPEND(rooms[i]->allowed_items, allowed);
			HASH_ADD_STR(hash, room_name, rooms[i]);
			//iterate to next allowed item
//			rooms[i]->allowed_items = rooms[i]->allowed_items->next;
//			allowed = allowed->next;
//		}
//		i++;
	}
	return hash;
}
