// All code and comments within these function definitions is the work of me, Jeremy Bowden.

/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "dictionary.h"



    // globally define the node that will be used for all letters from the
    // dictionary that will be added to the data structure
    typedef struct node_t
    {
        bool IsWord[27];
        struct node_t *child[27];
    } node;

    // globally declare a pointer to the root of the trie, so it can be used in all functions.
    // cannot allocate memory to it yet because malloc is a run-time operation and only compile-
    // time operations can be done outside of a function
    node *root;

    // global variable for the number of words loaded in
    // from the dictionary
    int numwords_in_dict = 0;

    // prototype of the recursive function used in unload()
    bool free_nodes(node *root);


/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    node *ptr_check = root;                 // create a pointer to root node

    int t;

    int length = strlen(word);              // count how many characters in the word to be checked

    for (int i = 0 ; i < length ; i++)
    {
        t = (toupper(word[i])) - ('A');

        if (word[i] == 39)                  // if the character is an apostrophe (')
        {
            t = 26;                         // set 't' to 26 (i.e. will be the 26th element in the node arrays)
        }

        if (ptr_check->child[t] == NULL)    // if "gate" (the child array element for the value of t) is closed
        {
            return 0;                       // word 'path' is not in dictionary, so return
        }

        else                                // "gate" is open (i.e. the child array element already has a pointer)
        {
            if (i < (length-1))                // if not yet at the last letter of the word
            {
                ptr_check = ptr_check->child[t];  // move pointer to the child array element
            }

            // if we are at the last letter of the word, leave ptr_check as it is
            // so we can check IsWord to see if we have a real word
        }
    }

    if (ptr_check->IsWord[t] == 1)              // if IsWord is true, return 1
    {
        return 1;
    }

    else
    {
        return 0;
    }
}





/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{


    // open the dictinary file that was passed from speller.c
    FILE *ptr_dict = fopen(dictionary, "r");

    if (ptr_dict == NULL)
    {
        printf("Dictionary %s could not be opened.\n", dictionary);
        return 0;
    }

    root = malloc(sizeof(node));                // assign heap memory for root node

    if (root == NULL)                           // check pointer memory was allocated
    {
        printf("malloc failed to assign memory.\n");
        return 0;
    }

    for (int j = 0 ; j < 27 ; j++)              // set root node array elements to NULL
    {
        root->IsWord[j] = NULL;
        root->child[j] = NULL;
    }

    node *ptr_curr;                     // create a pointer which points to the 'current' node
    node *ptr_prev;                     // create a pointer which points to the 'previous' node

    ptr_curr = root;                    // to begin, both will point to the root node
    ptr_prev = root;

    int current = 0;                    // int value for the current character taken from the dictionary
    int previous = 0;                   // int value for the previous character taken from the dictionary, which
                                        // is needed so we can go back to it to flag its IsWord element as true
                                        // if the current character is a newline (\n)


    // loop to get a character from the dictionary and add it to the trie
    // unless its a newline (\n) in which case we move to the next character
    for (int c = fgetc(ptr_dict) ; (!feof(ptr_dict)) ; c = fgetc(ptr_dict))
    {
        current = (toupper(c)) - ('A');                 // convert letter to a number 0 - 25, so each element of the
                                                        // arrays in the node represent a letter a - z

        if (c == 39)                                     // if c is an '
        {
            current = 26;                               // set 'current' to 26 (i.e. ' will be the 26th
        }                                               // element in the node arrays)


        if (c != '\n')                                    // if the character is a letter or '
        {
            ptr_prev = ptr_curr;                                // reset the pointers

            if (ptr_curr->child[current] == NULL)                 // if "gate" is closed...
            {
                ptr_curr->child[current] = malloc(sizeof(node));  // ... tell element "current" get some memory from malloc

                if (ptr_curr->child[current] == NULL)               // check malloc assigned memory
                {
                    printf("malloc failed to assign memory.\n");
                    return 0;
                }

                ptr_curr = ptr_curr->child[current];        // move pointer to the new memory (the new node)

                for (int j = 0 ; j < 27 ; j++)              // set the new node's array elements to NULL
                {
                    ptr_curr->IsWord[j] = NULL;
                    ptr_curr->child[j] = NULL;
                }

            }

            else
            {                                                   // if the "gate" was already open, just assign it...
            ptr_curr = ptr_curr->child[current];                // move current pointer to point to what the child[current] pointer points to
            }
        }

        else                                                    // must be a \n, i.e. at the end of a word
        {
            ptr_prev->IsWord[previous] = 1;                     // set the IsWord element for the PREVIOUS letter to true
            ptr_prev = root;                                    // reset both pointers to root node
            ptr_curr = root;
            numwords_in_dict +=1;                               // add 1 to the word counter
        }

        previous = current;                             // in readiness for the next loop, copy the value of
    }                                                   // current to previous

    fclose(ptr_dict);

    //all done!
    return 1;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    if (numwords_in_dict > 0)
    {
        return numwords_in_dict;
    }

    else
    {
        return 0;
    }
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    // return 0 if cannot unload
    // return 1 if can unload

    bool returned = free_nodes(root);

    if (returned == 1)
    {
        return 1;
    }

    else
    {
        return 0;
    }

}


bool free_nodes(node *root)
{
    node *ptr = root;

    for (int j = 0; j < 27; j++)                // for each child[] element in turn,
    {                                           // check if it's a NULL pointer.
        if (ptr->child[j] != NULL)              // If not, call THIS function again but with the pointer to the child element
        {                                       // as the input instead of the root notde.
            free_nodes(ptr->child[j]);
        }
    }

    free(ptr);                                  // when all child[] elements are NULL, i.e. we are at the root node,
                                                // free the node. This frees the memory allocated to the root node and
    return 1;                                   // all the NULL children are OK.

}

