/* 
 * Basic functions for custom-actions interface. 
 * 
 * Please see "interface.h" for function documentation. 
 */

#include <stdlib.h>
#include <stdio.h>
#include "custom-actions/interface.h"

/* See interface.h */
int do_custom_action(custom_action_t *action)
{
    if(action == NULL) return FAILURE;
    return run_ast_block(action->head);
}

/* PRIVATE HELPER FUNCTIONS */

int run_ast_block(AST_block_t *block) 
{
    int returnV;
    if(block == NULL) return SUCCESS;
    switch(block->block_type) 
    {
        case(CONTROL):
            return FAILURE;
            break;
        case(BRANCH):
            if(do_branch_block(block->block) == -1) return FAILURE;
            return run_ast_block(block->next[returnV]);
            break;
        case(ACTION):
            if(do_action_block(block->block) == FAILURE) return FAILURE;
            return run_ast_block(block->next[0]);
            break;
        case(CONDITIONAL):
            if(do_conditional_block(block->block)== FAILURE) return FAILURE;
            return run_ast_block(block->next[0]);
            break;   
    }
}

/* Given an branch block and its corresponding arguments, 
 * attempt to execute the given block.
 * 
 * Parameters: 
 * - block: A pointer to the branch block to be executed
 * 
 * Returns:
 * - Integer of what block to execute next
 */
int do_branch_block(branch_block_t *block)
{
    if(block->numcontrols != block->numconditionals) return FAILURE;
    // goes through each of the control blocks
    for(int i = 0; i < block->numcontrols; i++) 
    {
        // will perform the appropriate type of action
        switch(block->controls[i]->control_type) {
            case IFELSE:
                if(do_conditional_block(block->conditionals[i]) == TRUE) return i;
                break;
            case WHILEENDWHILE:
                // not implemented yet
                break;
            case FORENDFOR:
                // not implemented yet
                break;
        }
    }
    return i;
}

/* Given an conditiional block and its corresponding arguments, 
 * attempt to execute the given block.
 * 
 * Parameters: 
 * - block: A pointer to the conditional block to be evaluated
 * 
 * Returns:
 * - TRUE if condition true
 * - FALSE if condition false
 */
bool do_conditional_block(conditional_block_t *block)
{
    switch(block->conditional_type) {
        case EQ:
            return check_eq(block->left,block->right);
            break;
        case LTGT: // functioning like case LT
            return check_lt(block->left,block->right);
            break;
        /* 
        case GT // once conditional_block struct changed
            return check_gt(block->left,block->right);
            break;
        */
        case LTEGTE: // functioning like case LTE at the moment
            return check_lte(block->left,block->right);
            break;
        /*
        case GTE // once conditional_block struct changed
            return check_gte(block->left,block->right);
            break;
        */
        case IN:
            return check_in(block->left,block->right); // at the time of writing, TBI
            break;
    }
}