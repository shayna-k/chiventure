#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "battle/battle_structs.h"


/** See moves.h */
<<<<<<< HEAD
int init_class(class_t *class, enum class cl, char* info, enum stats st, double bonus)
{
    assert(class != NULL);
    assert(info != NULL);

    class->cl = cl;

    class->info = (char*) malloc(sizeof(char) * 100);
    strcpy(class->info, info);

    class->st = st;

    class->bonus = bonus;

    return 0;
}


/** See moves.h */
class_t *new_class(class_type_t cl, char* info, possible_stats_t st, double bonus)
{
    class_t *class = (class_t*) malloc(sizeof(class_t));
    int rc;

    if(class == NULL)
    {
        fprintf(stderr,"Could not allocate memory\n");
        return NULL;
    }

    rc = init_class(class, cl, info, st, bonus);

    if(rc != 0)
    {
        fprintf(stderr,"Could not initialize class\n");
        return NULL;
    }

    return class;
}

<<<<<<< HEAD

/* Creates test bard class
 *
 * Parameters:
 *  none, values are hard coded in
 *
 * Returns:
 *  - A pointer to a test class
 *
 */
class_t *make_test_bard()
{
    char* bard_des = "Charismatic, always has a joke, song, or moving speech ready";
    class_t *test_bard = new_class(0, bard_des, 3, 2.0);
    return test_bard;
}


/* Creates test bard class
 *
 * Parameters:
 *  none, values are hard coded in
 *
 * Returns:
 *  - A pointer to a test class
 *
 */
class_t *make_test_cleric()
{
    char* cleric_des = "Fueled by divine inspiration, devout to the craft";
    class_t *test_cleric = new_class(1, cleric_des, 1, 2.0);
    return test_cleric;
}


/* Creates test bard class
 *
 * Parameters:
 *  none, values are hard coded in
 *
 * Returns:
 *  - A pointer to a test class
 *
 */
class_t *make_test_paladin()
{
    char* paladin_des = "Driven and committed to justice";
    class_t *test_paladin = new_class(2, paladin_des, 2, 2.0);
    return test_paladin;
}


/* Creates test bard class
 *
 * Parameters:
 *  none, values are hard coded in
 *
 * Returns:
 *  - A pointer to a test class
 *
 */
class_t *make_test_wizard()
{
    char* wizard_des = "Draws power from nature";
    class_t *test_wizard = new_class(3, wizard_des, 0, 2.0);
    return test_wizard;
}


int main()
{
    printf("Success\n");
=======
/** See moves.h */
int init_class(class_t *class, class_type_t cl, char* info, possible_stats_t st, double bonus)
{
    assert(class != NULL);
    assert(info != NULL);

    class->cl = cl;

    class->info = (char*) malloc(sizeof(char) * 100);
    strcpy(class->info, info);

    class->st = st;

    class->bonus = bonus;

    return 0;
>>>>>>> a9f5948b47b93fef3d50cdc653dc8c9bc31e558c
}
