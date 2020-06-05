/* Implementations of the item struct */
#include <string.h>

#include "game-state/item.h"
#include "game-state/game_action.h"


// BASIC ITEM FUNCTIONS -------------------------------------------------------
/* see common-item.h*/
int item_init(item_t *new_item, char *item_id, char *short_desc,
              char *long_desc)
{

    assert(new_item != NULL);
    strcpy(new_item->item_id, item_id);
    strcpy(new_item->short_desc, short_desc);
    strcpy(new_item->long_desc, long_desc);

    return SUCCESS;
}

/* see item.h */
item_t *item_new(char *item_id, char *short_desc, char *long_desc)
{
    item_t *new_item = malloc(sizeof(item_t));
    memset(new_item, 0, sizeof(item_t));
    new_item->item_id = malloc(MAX_ID_LEN * sizeof(char)); // tentative size allocation
    new_item->short_desc = malloc(MAX_SDESC_LEN * sizeof(char));
    new_item->long_desc = malloc(MAX_LDESC_LEN * sizeof(char));

    int check = item_init(new_item, item_id, short_desc, long_desc);

    if (new_item == NULL || new_item->item_id == NULL ||
            new_item->short_desc == NULL || new_item->long_desc == NULL)
    {

        return NULL; //item struct not properly malloced

    }

    if(check != SUCCESS)
    {
        return NULL;
    }

    return new_item;

}

/* see item.h */
char *get_sdesc_item(item_t *item)
{
    if (item == NULL)
    {
        return NULL;
    }
    return item->short_desc;
}

/* see item.h */
char *get_ldesc_item(item_t *item)
{
    if (item == NULL)
    {
        return NULL;
    }
    return item->long_desc;
}

/* See item.h */
int add_item_to_hash(item_hash_t **ht, item_t *new_item)
{
    item_t *check;
    
    HASH_FIND(hh, *ht, new_item->item_id, strnlen(new_item->item_id, MAX_ID_LEN), check);

    if (check != NULL)
    {
        return FAILURE; //this item id is already in use.
    }
    HASH_ADD_KEYPTR(hh, *ht, new_item->item_id, strnlen(new_item->item_id, MAX_ID_LEN),
                    new_item);

    return SUCCESS;
}

/* See item.h */
item_list_t *get_all_items_in_hash(item_hash_t **ht)
{
    item_list_t *head = NULL;
    item_t *ITTMP_ITEMRM, *curr_item;
    item_list_t *tmp;
    HASH_ITER(hh, *ht, curr_item, ITTMP_ITEMRM)
    {
        tmp = malloc(sizeof(item_list_t));
        tmp->item = curr_item;
        LL_APPEND(head, tmp);
    }
    return head;
}

/* See item.h */
int remove_item_from_hash(item_hash_t **ht, item_t *old_item)
{
    item_t *check;
    
    HASH_FIND(hh, *ht, old_item->item_id, strnlen(old_item->item_id, MAX_ID_LEN), check);
    
    // Only deletes if item exists in hashtable
    if (check != NULL)
    {
        HASH_DEL(*(ht), old_item);
    }
    
    return SUCCESS;
}

// ATTRIBUTE MANIPULATION FUNCTIONS -------------------------------------------
/* see common-item.h */
int add_attribute_to_hash(item_t* item, attribute_t* new_attribute)
{
    attribute_t* check;
    HASH_FIND(hh, item->attributes, new_attribute->attribute_key,
              strlen(new_attribute->attribute_key), check);
    if (check != NULL)
    {
        return FAILURE; //this attribute is already present.
    }
    HASH_ADD_KEYPTR(hh, item->attributes, new_attribute->attribute_key,
                    strlen(new_attribute->attribute_key), new_attribute);
    return SUCCESS;
}

/* see item.h */
attribute_t *get_attribute(item_t *item, char* attr_name)
{
    attribute_t* return_value;
    HASH_FIND(hh, item->attributes, attr_name, strlen(attr_name), return_value);

    return return_value;
}


/* see item.h */
int set_str_attr(item_t* item, char* attr_name, char* value)
{
    attribute_t* res = get_attribute(item, attr_name);
    if (res == NULL)
    {
        attribute_t* new_attribute = malloc(sizeof(attribute_t));
        new_attribute->attribute_tag = STRING;
        new_attribute->attribute_value.str_val = value;
        new_attribute->attribute_key = strndup(attr_name, 100);
        int rv = add_attribute_to_hash(item, new_attribute);
        return rv;
    }
    else if (res != NULL && res->attribute_tag != STRING)
    {
        return FAILURE; // skeleton for not overriding type
    }
    else
    {
        res->attribute_value.str_val = value;
        return SUCCESS;
    }
}

// TYPE-SPECIFIC SET_ATTR FUNCTIONS -------------------------------------------
/* see item.h */
int set_int_attr(item_t* item, char* attr_name, int value)
{
    attribute_t* res = get_attribute(item, attr_name);
    if (res == NULL)
    {
        attribute_t* new_attribute = malloc(sizeof(attribute_t));
        new_attribute->attribute_tag = INTEGER;
        new_attribute->attribute_value.int_val = value;
        new_attribute->attribute_key = strndup(attr_name, 100);
        int rv = add_attribute_to_hash(item, new_attribute);
        return rv;
    }
    else if (res != NULL && res->attribute_tag != INTEGER)
    {
        return FAILURE; // skeleton for not overriding type
    }
    else
    {
        res->attribute_value.int_val = value;
        return SUCCESS;
    }
}

/* see item.h */
int set_double_attr(item_t* item, char* attr_name, double value)
{
    attribute_t* res = get_attribute(item, attr_name);
    if (res == NULL)
    {
        attribute_t* new_attribute = malloc(sizeof(attribute_t));
        new_attribute->attribute_tag = DOUBLE;
        new_attribute->attribute_value.double_val = value;
        new_attribute->attribute_key = strndup(attr_name, 100);
        int rv = add_attribute_to_hash(item, new_attribute);
        return rv;
    }
    else if (res != NULL && res->attribute_tag != DOUBLE)
    {
        return FAILURE; // skeleton for not overriding type
    }
    else
    {
        res->attribute_value.double_val = value;
        return SUCCESS;
    }

}

/* see item.h */
int set_char_attr(item_t* item, char* attr_name, char value)
{
    attribute_t* res = get_attribute(item, attr_name);
    if (res == NULL)
    {
        attribute_t* new_attribute = malloc(sizeof(attribute_t));
        new_attribute->attribute_tag = CHARACTER;
        new_attribute->attribute_value.char_val = value;
        new_attribute->attribute_key = strndup(attr_name, 100);
        int rv = add_attribute_to_hash(item, new_attribute);
        return rv;
    }
    else if (res != NULL && res->attribute_tag != CHARACTER)
    {
        return FAILURE; // skeleton for not overriding type
    }

    else
    {
        res->attribute_value.char_val = value;
        return SUCCESS;
    }
}

/* see item.h */
int set_bool_attr(item_t* item, char* attr_name, bool value)
{
    attribute_t* res = get_attribute(item, attr_name);
    if (res == NULL)
    {
        attribute_t* new_attribute = malloc(sizeof(attribute_t));
        new_attribute->attribute_tag = BOOLE;
        new_attribute->attribute_value.bool_val = value;
        new_attribute->attribute_key = strndup(attr_name, 100);
        int rv = add_attribute_to_hash(item, new_attribute);
        return rv;
    }
    else if (res != NULL && res->attribute_tag != BOOLE)
    {
        return FAILURE; // skeleton for not overriding type
    }

    else
    {
        res->attribute_value.bool_val = value;
        return SUCCESS;
    }
}


// TYPE-SPECIFIC GET_ATTR FUNCTIONS -------------------------------------------
/* see item.h */
char* get_str_attr(item_t *item, char* attr_name)
{
    attribute_t* res = get_attribute(item, attr_name);
    if (res == NULL)
    {
        return NULL;
    }
    if(res->attribute_tag != STRING)
    {
        return NULL; //attribute is not type string
    }
    return res->attribute_value.str_val;
}

/* see item.h */
int get_int_attr(item_t *item, char* attr_name)
{

    attribute_t* res = get_attribute(item, attr_name);
    if (res == NULL)
    {
        // value returned if search fails, open to alternative
        return -1;
    }
    if(res->attribute_tag != INTEGER)
    {
        return -1; //attribute is not type integer
    }
    return res->attribute_value.int_val;
}

/* see item.h */
double get_double_attr(item_t *item, char* attr_name)
{

    attribute_t* res = get_attribute(item, attr_name);
    if (res == NULL)
    {
        // value returned if search fails, open to alternative
        return -1.0;
    }
    if (res->attribute_tag != DOUBLE)
    {
        return -1.0; //attribute is not type double
    }
    return res->attribute_value.double_val;
}


/* see item.h */
char get_char_attr(item_t *item, char* attr_name)
{

    attribute_t* res = get_attribute(item, attr_name);
    if (res == NULL)
    {
        return '~';
    }
    if (res->attribute_tag != CHARACTER)
    {
        return '~'; //attribute is not type character
    }
    return res->attribute_value.char_val;
}

/* see item.h */
bool get_bool_attr(item_t *item, char* attr_name)
{
    attribute_t* res = get_attribute(item, attr_name);
    if (res == NULL)
    {
        return NULL;
    }
    if (res->attribute_tag != BOOLE)
    {
        return NULL; //attribute is not type boolean
    }
    return res->attribute_value.bool_val;
}


// ---------------------------------------------------------------------------

/* see item.h */
int attributes_equal(item_t* item_1, item_t* item_2, char* attribute_name)
{
    attribute_t* attribute_1 = get_attribute(item_1, attribute_name);
    attribute_t* attribute_2 = get_attribute(item_2, attribute_name);
    if(attribute_1==NULL || attribute_2==NULL)
    {
        return -1; //attribute does not exist for one or more items
    }
    if (attribute_1->attribute_tag != attribute_2->attribute_tag)
    {

        return -1; //could not compare attributes as they are of different types
    }
    int comparison = FAILURE;
    switch(attribute_1->attribute_tag)
    {
    case(DOUBLE):
        if (attribute_1->attribute_value.double_val ==
                attribute_2->attribute_value.double_val)
        {
            comparison = SUCCESS;
        }
        break;
    case(BOOLE):
        if (attribute_1->attribute_value.bool_val ==
                attribute_2->attribute_value.bool_val)
        {
            comparison = SUCCESS;
        }
        break;
    case(CHARACTER):
        if (attribute_1->attribute_value.char_val ==
                attribute_2->attribute_value.char_val)
        {
            comparison = SUCCESS;
        }
        break;
    case(STRING):
        if (!strcmp(attribute_1->attribute_value.str_val,
                    attribute_2->attribute_value.str_val))
        {
            comparison = SUCCESS;
        }
        break;
    case(INTEGER):
        if (attribute_1->attribute_value.int_val ==
                attribute_2->attribute_value.int_val)
        {
            comparison = SUCCESS;
        }
        break;
    }
    return comparison;
}




// FREEING AND DELETION FUNCTIONS ---------------------------------------------

/* see game_action.h */
int game_action_free(game_action_t* game_action)
{
    free(game_action->action_name);
    delete_action_condition_llist(game_action->conditions);
    delete_action_effect_llist(game_action->effects);
    free(game_action->success_str);
    free(game_action->fail_str);
    free(game_action);
    return SUCCESS;
}


/* see item.h */
int attribute_free(attribute_t *attribute)
{
    free(attribute->attribute_key);

    free(attribute);
    return SUCCESS;
}


/* see common-item.h */
int delete_all_attributes(attribute_hash_t* attributes)
{
    attribute_t *current_attribute, *tmp;
    HASH_ITER(hh, attributes, current_attribute, tmp)
    {
        /* deletes (attrs advances to next) */
        HASH_DEL(attributes, current_attribute);
        attribute_free(current_attribute);             /* free it */
    }
    return SUCCESS;
}

/* See item.h */
int item_free(item_t *item)
{
    free(item->item_id);
    free(item->short_desc);
    free(item->long_desc);
    delete_all_attributes(item->attributes);
    // uthash_free(item->attributes, HASH_SIZE);
    free(item);
    return SUCCESS;
}

/* See common.h*/
int delete_all_items(item_hash_t** items)
{
    item_t *current_item, *tmp;
    HASH_ITER(hh, *items, current_item, tmp)
    {
        remove_item_from_hash(items, current_item); /* deletes (items advances to next) */
        item_free(current_item);             /* free it */
    }
    return SUCCESS;
}

/* See item.h */
attribute_list_t *get_all_attributes(item_t *item)
{
    attribute_list_t *head = NULL;
    attribute_t *ITTMP_ATTR, *curr_attribute;
    attribute_list_t *tmp;
    HASH_ITER(hh, item->attributes, curr_attribute, ITTMP_ATTR)
    {
        tmp = malloc(sizeof(attribute_list_t));
        tmp->attribute = curr_attribute;
        LL_APPEND(head, tmp);
    }
    return head;
}

/* See item.h */
int delete_attribute_llist(attribute_list_t *head)
{
    attribute_list_t *elt, *tmp;
    LL_FOREACH_SAFE(head, elt, tmp)
    {
        LL_DELETE(head, elt);
        free(elt);
    }
    return SUCCESS;
}

/* See item.h */
int delete_item_llist(item_list_t *head)
{
    item_list_t *elt, *tmp;
    LL_FOREACH_SAFE(head, elt, tmp)
    {
        LL_DELETE(head, elt);
        free(elt);
    }
    return SUCCESS;
}
