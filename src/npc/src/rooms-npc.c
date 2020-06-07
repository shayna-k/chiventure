#include "npc/rooms-npc.h"

/*Object_t Description: object_t is the generic custom scripts struct 
that can hold a variety of different types including "char*".*/

/*Changes to Room_ID: We are using this struct to modify the room_id 
which will enable custom scripts and lua files to be loaded*/ 
/* See rooms-npc.h */
int npcs_in_room_init(npcs_in_room_t *npcs_in_room, object_t* room_id)
{
    assert(npcs_in_room != NULL);
    npcs_in_room->room_id = room_id;
    npcs_in_room->npc_list = NULL;
    npcs_in_room->num_of_npcs = 0;

    return SUCCESS;
}

/*Changes to Room_ID: We are using this struct to modify the room_id 
which will enable custom scripts and lua files to be loaded*/ 
/* See rooms-npc.h */
int npc_mov_init(npc_mov_t *npc_mov, object_t* npc_id, npc_mov_enum_t mov_type,
                room_t *room)
{
    object_t* room_id = room->room_id;
    assert(npc_mov != NULL);
    npc_mov->npc_id = npc_id;
    npc_mov->mov_type = mov_type;
    npc_mov->track = room_id;

    room_list_t* room_to_add = malloc(sizeof(room_list_t));
    room_to_add->next = NULL;
    room_to_add->room = room;

    room_list_t* head = NULL;    
    
    if (mov_type == NPC_MOV_DEFINITE)
    {
        npc_mov->npc_mov_type.npc_mov_definite->npc_path = head;
        LL_APPEND(head,room_to_add);
    }
    else if (mov_type == NPC_MOV_INDEFINITE )
    {
        npc_mov->npc_mov_type.npc_mov_indefinite->npc_path = head;
        LL_APPEND(head,room_to_add);
        npc_mov->npc_mov_type.npc_mov_indefinite->room_time = NULL;
    }
    
    return SUCCESS;
}

/*Changes to Room_ID: We are using this struct to modify the room_id 
which will enable custom scripts and lua files to be loaded*/ 
/* See rooms-npc.h */
npcs_in_room_t *npcs_in_room_new(object_t* room_id)
{
    npcs_in_room_t *npcs_in_room;
    npcs_in_room = malloc(sizeof(npcs_in_room_t));
    memset(npcs_in_room, 0, sizeof(npcs_in_room_t));
    npcs_in_room->room_id = malloc(MAX_ID_LEN);
    
    int check = npcs_in_room_init(npcs_in_room, room_id);

    if (npcs_in_room == NULL || npcs_in_room->room_id == 0 || check != SUCCESS)
    {
        return NULL;
    }

    return npcs_in_room;
}

/*Changes to NPC_ID: We are using this struct to modify the room_id 
which will enable custom scripts and lua files to be loaded*/ 
/* See rooms-npc.h */
npc_mov_t* npc_mov_new(object_t* npc_id, npc_mov_enum_t mov_type, room_t* room)
{
    npc_mov_t *npc_mov;
    npc_mov = malloc(sizeof(npc_mov_t));
    memset(npc_mov, 0, sizeof(npc_mov_t));
    npc_mov->npc_id = malloc(MAX_ID_LEN);
    npc_mov->track = malloc(MAX_ID_LEN);

    if (mov_type == NPC_MOV_DEFINITE)
    {
        npc_mov->npc_mov_type.npc_mov_definite = malloc(sizeof(npc_mov_definite_t));
        memset(npc_mov->npc_mov_type.npc_mov_definite, 0, sizeof(npc_mov_definite_t));

    }
    else if (mov_type == NPC_MOV_INDEFINITE )
    {
        npc_mov->npc_mov_type.npc_mov_indefinite = malloc(sizeof(npc_mov_indefinite_t));
        memset(npc_mov->npc_mov_type.npc_mov_indefinite, 0, sizeof(npc_mov_indefinite_t));
    }

    int check = npc_mov_init(npc_mov, npc_id, mov_type, room);

    if (npc_mov == NULL || npc_mov->npc_id == NULL || check != SUCCESS)
    {
        return NULL;
    }

    return npc_mov;
}


/* See rooms-npc.h */
int npcs_in_room_free(npcs_in_room_t *npcs_in_room)
{
    assert(npcs_in_room != NULL);

    //free(npcs_in_room->room_id);
    free(npcs_in_room->npc_list);
    free(npcs_in_room);

    return SUCCESS;
}


/* See rooms-npc.h */
int npc_mov_free(npc_mov_t *npc_mov) {

    assert(npc_mov != NULL);

    free(npc_mov->npc_mov_type.npc_mov_indefinite);
    //free(npc_mov->track);
    //free(npc_mov->npc_id);
    free(npc_mov);
}


/* See rooms-npc.h */
int npcs_in_room_get_number(npcs_in_room_t *npcs_in_room)
{
    return npcs_in_room->num_of_npcs;
}


/* See rooms-npc.h */
int add_npc_to_room(npcs_in_room_t *npcs_in_room, npc_t *npc)
{
    npc_t *check;
    HASH_FIND(hh, npcs_in_room->npc_list, npc->npc_id, strlen(npc->npc_id),
             check);
 
    if (check != NULL)
    {
        return FAILURE;
    }
    HASH_ADD_KEYPTR(hh, npcs_in_room->npc_list, npc->npc_id,
                    strlen(npc->npc_id), npc);
    npcs_in_room->num_of_npcs++;

    return SUCCESS;
}


/* See rooms-npc.h */
int register_npc_room_time(npc_mov_t *npc_mov, room_t *room, int time)
{
    assert(room != NULL);
    
    npc_room_time_t *return_time;

    npc_room_time_t *new_npc_room_time;
    new_npc_room_time = malloc(sizeof(npc_room_time_t));
    memset(new_npc_room_time, 0, sizeof(npc_room_time_t));
    new_npc_room_time->room_id = malloc(MAX_ID_LEN);
    strcpy(new_npc_room_time->room_id, room->room_id); 
    new_npc_room_time->time = time;

    HASH_REPLACE(hh, npc_mov->npc_mov_type.npc_mov_indefinite->room_time,
                    room_id, strlen(room->room_id), 
                    new_npc_room_time, return_time);
    
    free(return_time);

    HASH_ADD_KEYPTR(hh, npc_mov->npc_mov_type.npc_mov_indefinite->room_time,
            room->room_id, strlen(room->room_id), new_npc_room_time);
   
    return SUCCESS;
}


/* See rooms-npc.h */
int extend_path_def(npc_mov_t *npc_mov, room_t *room_to_add) 
{
    assert(room_to_add != NULL);

    room_list_t* room_to_add2 = malloc(sizeof(room_list_t));
    room_to_add2->next  = NULL;
    room_to_add2->room = room_to_add;
    
    room_list_t* head = npc_mov->npc_mov_type.npc_mov_definite->npc_path;

    LL_APPEND(head, room_to_add2);

    return SUCCESS;
}  


/* See rooms-npc.h */
int extend_path_indef(npc_mov_t *npc_mov, room_t *room_to_add, int time) 
{   
    assert(room_to_add != NULL);

    room_list_t* room_to_add2 = malloc(sizeof(room_list_t));
    room_to_add2->next  = NULL;
    room_to_add2->room = room_to_add;

    room_list_t *head = npc_mov->npc_mov_type.npc_mov_indefinite->npc_path;

    LL_APPEND(head, room_to_add2);

    int check = register_npc_room_time(npc_mov, room_to_add, time);

    if (check != SUCCESS)
        return FAILURE;
    else
        return SUCCESS;
}


/* See rooms-npc.h */
char* track_room(npc_mov_t *npc_mov) 
{
    return npc_mov->track;
}


/* See rooms-npc.h */
int reverse_path(npc_mov_t *npc_mov) 
{
    assert(npc_mov->mov_type == NPC_MOV_DEFINITE);

    room_list_t *reversed_path_head = NULL;

    room_list_t *def_path = npc_mov->npc_mov_type.npc_mov_definite->npc_path;
    room_list_t *tmp;
    LL_FOREACH(def_path, tmp) {
        LL_PREPEND(reversed_path_head, tmp);
    }
    free(def_path);
    npc_mov->npc_mov_type.npc_mov_definite->npc_path = reversed_path_head;

    return SUCCESS;
}
