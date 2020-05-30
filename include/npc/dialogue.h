#ifndef _DIALOGUE_H
#define _DIALOGUE_H

#include "game-state/game_state_common.h"

#define MAX_DIA_LEN 500
#define MAX_KEY_LEN 30
#define MAX_QUIP_LEN 100


// BASIC PRINTING FUNCTIONS ---------------------------------------------------

/*
 * Three functions to print given string in gold, yellow, or red respectively
 */
void print_gold(char *str);

void print_yellow(char *str);

void print_red(char *str);

/*
 * Prints a string in NPC dialogue format: gold by default and yellow for
 * text surrounded by #hashes# to denote dialogue choices.
 * 
 * Parameters:
 *  - dialogue: the string to be printed in NPC format
 * 
 * Returns: None
 */
void npc_print(char *dialogue);


// DIALOGUE STRUCTURE DEFINITION ----------------------------------------------

/* Forward Declaration */
typedef struct node node_t;

/*
 * A struct to represent an edge to a node (note: does not include source node).
 * 
 * Includes:
 *  - keyword: a string that the user's input must match to enter the edge
 *  - quip: a string of what the player's character says if they enter the edge
 *  - toward: points toward which node traversing this edge leads to
 */
typedef struct edge {
    UT_hash_handle hh;
    char *keyword;
    char *quip;
    node_t *toward;
} edge_t;

/* This typedef is to distinguish between npc_t pointers which are
 * used to point to the npc_t structs in the traditional sense,
 * and those which are used to hash npc_t structs with the
 * UTHASH macros as specified in src/common/include */
typedef struct edge edge_hash_t;

/*
 * A struct to represent one scene in a conversation.
 * 
 * Includes:
 *  - node_id: a marker of which node this is
 *  - dialogue: a string of what the NPC says on arriving at the node
 *  - connection_count: the number of connections the node currently has
 *  - edges: a linked list of edge pointers (list of attached edges)
 */
typedef struct node {
    char* node_id;
    char *dialogue;
    int connection_count;
    edge_hash_t *edges;
} node_t;

/*
 * A doubly-linked list of nodes for containing all involved nodes in a convo.
 * 
 * Includes:
 *  - cur_node: Points to the location of the node currently focused on
 *  - prev: A pointer to the previous entry in the list, NULL if none left
 *  - next: A pointer to the next entry in the list, NULL if none left
 */
typedef struct node_list {
    node_t *cur_node;
    struct node_list *prev;
    struct node_list *next;
} node_list_t;

/*
 * A struct to represent a conversation.
 * 
 * Includes:
 *  - node_count: the number of nodes the convo currently has
 *  - nodes: a linked list of node pointers (list of included nodes)
 */
typedef struct convo {
    int node_count;
    node_list_t *nodes;
} convo_t;


// STRUCT FUNCTIONS -----------------------------------------------------------

/*
 * Allocates a new npc in the heap.
 * 
 * Parameters: None
 * 
 * Returns:
 *  - pointer to the new, empty convo
 */
convo_t *convo_new();

/*
 * Frees resources associated with a convo.
 *
 * Parameters:
 *  - c: the convo to be freed
 *
 * Returns:
 *  - SUCCESS if successful, FAILURE if an error occurs
 */
int convo_free(convo_t *c);

/*
 * Initializes the given node with given parameters.
 *
 * Parameters:
 *  - n: a node; must point to already allocated memory
 *  - node_id: a string referencing the node's identity
 *  - dialogue: a string of what the NPC says when the player reaches this node
 *
 * Returns:
 *  - SUCCESS on success, FAILURE if an error occurs
 */
int node_init(node_t *n, char *node_id, char *dialogue);

/*
 * Allocates a new node on the heap.
 * 
 * Parameters:
 *  - node_id: a string referencing the node's identity
 *  - dialogue: a string of what the NPC says when the player reaches this node
 * 
 * Returns:
 *  - pointer to the new node
 */
node_t *node_new(char *node_id, char *dialogue);

/*
 * Frees resources associated with a node.
 *
 * Parameters:
 *  - n: the node to be freed
 *
 * Returns:
 *  - SUCCESS if successful, FAILURE if an error occurs
 */
int node_free(node_t *n);

/*
 * Initializes the given edge with given parameters.
 *
 * Parameters:
 *  - e: an edge; must point to already allocated memory
 *  - toward: the node that traversing this edge will bring you to
 *  - keyword: a string that the user's input must match to enter the edge
 *  - quip: a string of what the player character should say
 *          when this edge is traversed
 *
 * Returns:
 *  - SUCCESS on success, FAILURE if an error occurs
 */
int edge_init(edge_t *e, node_t *toward, char *keyword, char *quip);

/*
 * Allocates a new edge struct on the heap.
 * 
 * Parameters:
 *  - toward: the node that traversing this edge will bring you to
 *  - keyword: a string that the user's input must match to enter the edge
 *  - quip: a string of what the player character should say
 *    when this edge is traversed
 * 
 * Returns:
 *  - pointer to the new edge struct
 */
edge_t *edge_new(node_t *toward, char *keyword, char *quip);

/*
 * Frees resources associated with an edge.
 *
 * Parameters:
 *  - e: the edge to be freed
 *
 * Returns:
 *  - SUCCESS if successful, FAILURE if an error occurs
 */
int edge_free(edge_t *e);


// DIALOGUE BUILDING FUNCTIONS ------------------------------------------------

/*
 * Prepends the given node before the head of the node-list of the given convo.
 * 
 * Parameters:
 *  - c: the convo that the node is to be attached to
 *  - n: the node to be attached
 * 
 * Returns:
 *  - SUCCESS if successful, FAILURE if an error occurs
 */
int prepend_node(convo_t *c, node_t *n);

/*
 * Appends the given node to the end of the node-list of the given convo.
 * 
 * Parameters:
 *  - c: the convo that the node is to be attached to
 *  - n: the node to be attached
 * 
 * Returns:
 *  - SUCCESS if successful, FAILURE if an error occurs
 */
int append_node(convo_t *c, node_t *n);

/*
 * Attaches a given edge to the edges hashtable of the given node.
 * 
 * Parameters:
 *  - n: the node that the edge is to be attached to
 *  - e: the edge to be attached
 * 
 * Returns:
 *  - SUCCESS if successful, FAILURE if an error occurs
 */
int add_edge(node_t *n, edge_t *e);

/*
 * Compares the input to the keyword and returns index of matching edge.
 * 
 * Parameters:
 *  - n: the node that player is currently on
 *  - input: the player-inputted command
 * 
 * Returns:
 *  - pointer to the matching edge on success, NULL otherwise
 */
edge_t *read_input(node_t *n, char *input);

/*
 * Asks for input and traverses to the edge specified by the input's index.
 * Prints player quip, moves to new node, prints npc's dialogue at that node.
 * 
 * Parameters:
 *  - n: the node that the player is currently on
 * 
 * Returns:
 *  - pointer to the new curent node on success, NULL otherwise
 */
node_t *traverse_edge(node_t *n);

/*
 * Ends the conversation
 * Parameters:
 *  - None
 * Returns:
 *  - Nothing
 *
 */
void end_convo();

/*
 * Runs the entire conversation until it reaches an end
 * Parameters:
 *  - c: A conversation
 * Returns:
 *  - Nothing, just runs through each node and asks for input
 *
 */
void run_convo(convo_t *c);


// LINKED LIST FUNCTIONS ------------------------------------------------------

int node_cmp(node_list_t *n1, node_list_t *n2); 

int delete_all_nodes(node_list_t *nodes);

#endif