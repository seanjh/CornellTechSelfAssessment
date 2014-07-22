#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "splaytree.h"

typedef enum { left, right } hand_side;

WordBTNode * create_node_WBT(char *str)
{
    WordBTNode *new_node = (WordBTNode *) malloc(sizeof(WordBTNode));
    if(!new_node) {
        fprintf(stderr, "ERROR: Out of memory (WordBTNode)\n");
        return NULL;
    }

    // Initialize the new_node pointers
    new_node->parent =  NULL;
    new_node->left  = NULL;
    new_node->right = NULL;

    // Allocate memory for the word string and terminating char
    int wordlen = strlen(str) + 1;
    new_node->word = (char *) malloc(wordlen * sizeof(char));

    if (!new_node->word) {
        fprintf(stderr, "ERROR: Out of memory (WordBTNode->word).\n");
        return NULL;
    }

    // Copy input string into the new new_node
    strcpy(new_node->word, str);

    return new_node;
}

WordSplayTree * create_WST()
{
    WordSplayTree *tree = (WordSplayTree *) malloc(sizeof(WordSplayTree));
    if (!tree) {
        fprintf(stderr, "ERROR: Out of memory! (WordSplayTree)\n");
        return NULL;
    }

    tree->root = NULL;
}

int delete_node_WST(WordBTNode **trash_node)
{
    if (*trash_node) {
        free((*trash_node)->word);
        //(*trash_node)->word = NULL;
        free(*trash_node);
        *trash_node = NULL;
        return STATUS_OK;
    } else {
        return STATUS_ERROR;
    }
}

/* Delete the tree using iterative post-order traversal.*/
void delete_all_nodes_WST_iter(WordBTNode *current_node)
{
    WordBTNode *parent_node;

    while (current_node->parent || current_node->right || current_node->left)
    {
        if (current_node->left) {
            current_node = current_node->left;
        } else if (current_node->right) {
            current_node = current_node->right;
        } else {
            parent_node = current_node->parent;
            if (current_node == parent_node->left) {
                parent_node->left = NULL;
            } else {
                parent_node->right = NULL;
            }
            delete_node_WST(&current_node);
            current_node = parent_node;
        }
    }

    delete_node_WST(&current_node);
}

/* Delete the tree using post-order traversal.

int delete_all_nodes_WST(WordBTNode *trash_node)
{
    if (trash_node->left) {
        delete_all_nodes_WST(trash_node->left);
    }
    if (trash_node->right) {
        delete_all_nodes_WST(trash_node->right);
    }
    delete_node_WST(&trash_node);
}
 */

int delete_WST(WordSplayTree **tree)
{
    // First traverse the tree, and free all the nodes
    if ((*tree)->root) {
        //delete_all_nodes_WST((*tree)->root);
        delete_all_nodes_WST_iter((*tree)->root);
    } else {
        fprintf(stderr, "ERROR: Tree is already empty.\n");
        return STATUS_ERROR;
    }

    free(*tree);
    *tree = NULL;

    return STATUS_OK;
}

/* DEBUGGING
void print_status(WordBTNode *node, char *label)
{
    printf("\t\t%s (\"%s\") is %p.\n", label, node->word, node);
    printf("\t\t%s->parent is %p ", label, node->parent);
    if (node->parent) {
        printf("(\"%s\")\n", node->parent->word);

        // Where on parent
        if (node->parent->right == node) {
            printf("\t\t%s is %s->parent->right.\n", label, label);
        } else if (node->parent->left == node) {
            printf("\t\t%s is %s->parent->left.\n", label, label);
        }

    } else {
        printf("(N/A)\n");
    }

    printf("\t\t%s->left is %p ", label, node->left);
    if (node->left)
        printf("(\"%s\").\n", node->left->word);
    else
        printf("(N/A).\n");

    printf("\t\t%s->right is %p ", label, node->right);
    if (node->right)
        printf("(\"%s\").\n", node->right->word);
    else
        printf("(N/A).\n");
}

void hold_up(int wait)
{
    printf("Halting for %d seconds\n", wait);
    clock_t ticks1, ticks2;
    ticks1=clock();
    ticks2=ticks1;
    while((ticks2/CLOCKS_PER_SEC-ticks1/CLOCKS_PER_SEC)<wait)
        ticks2=clock();
}*/

void link_nodes(WordBTNode *child, WordBTNode *new_parent, hand_side new_side)
{
    // Point new_parent's left/right node to the child
    if (new_side == right) {
        new_parent->right = child;
    } else {
        new_parent->left = child;
    }

    // Only assign the child a parent when it's not NULL.
    if (child) {
        child->parent = new_parent;
    }
}

/*
void insert_node(WordBTNode *node, WordBTNode *new_node, hand_side side)
{
    if (side == right) {
        node->right = new_node;
        new_node->parent = node;
    } else {
        node->left = new_node;
        new_node->parent = node;
    }
}
*/

WordBTNode * find_edge_iter(WordBTNode *current_node, char *input)
{
    WordBTNode *new_node = NULL;

    while (1)
    {
        int result = strcmp(input, current_node->word);

        if (result < 0) {
            if (current_node->left) {
                // current_node's left is occupied. Try to find the edge further left.
                //find_edge(current_node->left, new_node);
                current_node = current_node->left;
            } else {
                // Current_node's left is open. Insert new node in the tree there
                new_node = create_node_WBT(input);
                //insert_node(current_node, new_node, left);
                link_nodes(new_node, current_node, left);
                break;
            }
        } else if (result > 0) {
            if (current_node->right) {
                // current_node's right is occupied. Try to find the edge further right.
                //find_edge(current_node->right, new_node);
                current_node = current_node->right;
            } else {
                // Node's right is open. Insert new node in the tree there
                new_node = create_node_WBT(input);
                //insert_node(current_node, new_node, right);
                link_nodes(new_node, current_node, right);
                break;
            }
        } else {
            /* The new string is neither < nor > of the current_node.
             * This means the node is equal to the current_node. Duplicate
             * nodes are not permitted.*/
            //fprintf(stderr, "ERROR: Duplicate word (\"%s\") cannot be inserted!\n", input);
            break;
        }
    }

    return new_node;
}

/*
int find_edge(WordBTNode *current_node, WordBTNode *new_node)
{
    int result = strcmp(new_node->word, current_node->word);

    if (result < 0) {
        if (current_node->left) {
            // current_node's left is occupied. Try to find the edge further left.
            find_edge(current_node->left, new_node);
        } else {
            // Current_node's left is open. Insert new node in the tree there
            insert_node(current_node, new_node, left);
        }
    } else if (result > 0) {
        if (current_node->right) {
            // current_node's right is occupied. Try to find the edge further right.
            find_edge(current_node->right, new_node);
        } else {
            // Node's right is open. Insert new node in the tree there
            insert_node(current_node, new_node, right);
        }
    } else {*/
        /* The new string is neither < nor > of the current_node.
         * This means the node is equal to the current_node. Duplicate
         * nodes are not permitted.*/
        /*fprintf(stderr, "ERROR: Duplicate word entry in SplayTree is not permitted!\n");
        //delete_node_WST(&new_node);
        return STATUS_ERROR;
    }

    return STATUS_OK;
}
*/

/* Each insert requires finding the edge of the tree.
 * Once the new node is created at this edge, it is splayed
 * repeatedly until the new node is the tree root.
 */
int insert_WST(WordSplayTree *tree, char *input) {

    WordBTNode *new_node = NULL;

    if (tree->root) {
        //find_edge(tree->root, new_node);
        new_node = find_edge_iter(tree->root, input);
    } else {
        //tree->root = new_node;
        new_node = create_node_WBT(input);
        tree->root = new_node;
    }

    if (new_node) {
        //splay(tree, new_node);
        splay_iter(tree, new_node);
    }
}

void check_ggp(WordSplayTree *tree, WordBTNode *node, WordBTNode *p, WordBTNode *gp)
{
    bool gp_root = false;
    hand_side gp_side;
    WordBTNode *ggp = gp->parent;  // great-grandparent (ggp)

    /* Check if we find root or another node at ggp
     */
    if (!ggp) {
        // No great-grandparent located. Flag grandparent (gp) as root.
        gp_root = true;
    } else if (gp == ggp->right) {
        // Since gp is not root, determine its side from ggp.
        gp_side = right;
    } else {
        gp_side = left;
    }

    /* Assign node either to root or to the ggp->left/right node
     */
    if (gp_root) {
        tree->root = node;
        node->parent = NULL;
    } else {
        // Swap node's parent (p) with ggp
        node->parent = ggp;

        // Point ggp left/right to node
        if (gp_side == right) {
            ggp->right = node;
        }
        else {
            ggp->left = node;
        }
    }
}

void zig(WordSplayTree *tree, WordBTNode *splay_node)
{
    // Determine whether splay_node is to the left or right of root
    if (tree->root->left == splay_node) {
        // Move splay_node's right over to the old root's left
        link_nodes(splay_node->right, tree->root, left);

        // Point splay_node's right down at the old root.
        link_nodes(tree->root, splay_node, right);

    } else {
        // Move splay_node's right over to the old root's right
        link_nodes(splay_node->left, tree->root, right);

        // Point splay_node's right down at the old root.
        link_nodes(tree->root, splay_node, left);

    }

    // Make splay_node the tree's root. The root has no parent.
    tree->root = splay_node;
    splay_node->parent = NULL;
}

void zigzig(WordSplayTree *tree, WordBTNode *splay_node, WordBTNode *p, WordBTNode *gp)
{
    // Check if gp is root, in which case tree->root will be updated
    check_ggp(tree, splay_node, p, gp);

    // Determine position of p from gp
    hand_side p_side;
    if (p == gp->right)
        p_side = right;
    else
        p_side = left;

    if (p_side == right) {
        // Parent is on it's parent's right. Arrangement is: gp-right-p-right-splay_node
        link_nodes(p->left, gp, right);
        link_nodes(splay_node->left, p, right);
        link_nodes(p, splay_node, left);
        link_nodes(gp, p, left);

    } else {
        // Parent is on it's parent's left. Arrangement is: gp-left-p-left-splay_node
        link_nodes(p->right, gp, left);
        link_nodes(splay_node->right, p, left);
        link_nodes(p, splay_node, right);
        link_nodes(gp, p, right);

    }
}

void zigzag(WordSplayTree *tree, WordBTNode *splay_node, WordBTNode *p, WordBTNode *gp)
{
    // Check if gp is root, in which case tree->root will be updated
    check_ggp(tree, splay_node, p, gp);

    hand_side p_side = left;
    // Determine position of p from gp
    if (p == gp->right)
        p_side = right;

    if (p_side == right) {
        // p=gp->right and splay_node=p->left
        link_nodes(splay_node->left, gp, right);
        link_nodes(splay_node->right, p, left);
        link_nodes(p, splay_node, right);
        link_nodes(gp, splay_node, left);

    } else {
        // p=gp->left and splay_node=p->right
        link_nodes(splay_node->left, p, right);
        link_nodes(splay_node->right, gp, left);
        link_nodes(p, splay_node, left);
        link_nodes(gp, splay_node, right);

    }
}

int splay_iter(WordSplayTree *tree, WordBTNode *splay_node)
{
    if (!tree || !tree->root) {
        fprintf(stderr, "ERROR: Cannot splay an empty tree.\n");
        return STATUS_ERROR;
    }

    while (splay_node != tree->root)
    {

        if (splay_node->parent == tree->root) {
            zig(tree, splay_node);

        } else {
            WordBTNode *p = splay_node->parent;
            WordBTNode *gp = splay_node->parent->parent;

            // Determine whether ZigZig or ZigZag splay move is needed
            if (gp->left == p && p->left == splay_node) {
                zigzig(tree, splay_node, p, gp);
            } else if (gp->right == p && p->right == splay_node) {
                zigzig(tree, splay_node, p, gp);
            } else {
                zigzag(tree, splay_node, p, gp);
            }
        }

    }

    return STATUS_OK;
}

/*
void splay(WordSplayTree *tree, WordBTNode *splay_node)
{
    if (tree->root == splay_node) {
        // No splaying needed when already root
        return;
    }

    if (splay_node->parent == tree->root) {
        zig(tree, splay_node);

    } else {
        WordBTNode *p = splay_node->parent;
        WordBTNode *gp = splay_node->parent->parent;

        // Determine whether ZigZig or ZigZag splay move is needed
        if (gp->left == p && p->left == splay_node) {
            zigzig(tree, splay_node, p, gp);
        } else if (gp->right == p && p->right == splay_node) {
            zigzig(tree, splay_node, p, gp);
        } else {
            zigzag(tree, splay_node, p, gp);
        }
    }

    // Continue splaying until splay_node is the tree's root
    if (splay_node != tree->root) {
        splay(tree, splay_node);
    }
}
*/

/* Searches the SplayTree for an input string. If the string is
 * located in the tree, the corresponding node is splayed until
 * it is the tree's root.
 *
 * When no match is found, no splaying occurs.
 *
 * Returns:
 *      -1 when no match is found
 *      1 when a match is found
 */
int find_WST(WordSplayTree *tree, char *input)
{
    if (tree && tree->root)
        //return find_node(tree, tree->root, input);
        return find_node_iter(tree, tree->root, input);
    else {
        fprintf(stderr, "ERROR: Tree is empty\n");
        return -1;
    }
}

int find_node_iter(WordSplayTree *tree, WordBTNode *current_node, char *input)
{
    int result = NO_MATCH;

    while (result != 0 && current_node)
    {
        result = strcmp(input, current_node->word);

        if (result < 0) {
            current_node = current_node->left;
        } else if (result > 0) {
            current_node = current_node->right;
        }
    }

    if (current_node) {
        splay_iter(tree, current_node);
        return FOUND_MATCH;
    } else {
        return NO_MATCH;
    }
}

/*int find_node(WordSplayTree *tree, WordBTNode *current_node, char *input)
{
    int result;
    if (current_node && current_node->word) {
        result = strcmp(input, current_node->word);
    } else {
        fprintf(stderr, "ERROR: SplayTree node has NULL word.\n");
        exit(EXIT_FAILURE);
    }

    if (result < 0) {
        if (current_node->left)
            // Continue searching further left
            find_node(tree, current_node->left, input);

    } else if (result > 0) {
        if (current_node->right)
            // Continue searching further right
            find_node(tree, current_node->right, input);

    } else {
        //splay(tree, current_node);
        splay_iter(tree, current_node);
        return FOUND_MATCH;
    }
}
*/
