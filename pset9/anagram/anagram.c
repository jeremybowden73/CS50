// Implements an anagram-creating program.

// All code and comments is the work of me, Jeremy Bowden.
// Completed 22-November-2017.





#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// default dictionary
#define DICTIONARY "dictionaries/corncob"

#define MAXLENGTH 45

// define the maximum number of letters the user can input
#define LENGTH 12

// globally define the node that will be used in a linked list to store
// all letters entered by the user (the "UI linked list")
typedef struct letter_t
{
    char Letter;
    int Flag;
    int TempFlag;
    struct letter_t *next;
} node;

// globally declare a pointer to the root of the UI linked list
// so it can be used in all functions.
node *root;
// and another for the 'current' node in the UI linked list
node *uiptr;



// globally define the node that will be used in a linked list
// to store each dictionary word found in the letters entered by the user
typedef struct rw
{
    char rwWord[LENGTH];
    int rwLength;
    struct rw *child;
} realword;

// globally declare a pointer to the root (head) of the linked list
realword *head;
// and another that we can move around freely
realword *ptr;



// function prototypes
int checkForWord(realword* word);
int checkAllFlags(void);
void storeWordString(realword* wordpos[]);
void resetAllTempFlags(void);
int iterateOverWords(realword* wordpos[], int x);
void TempFlag_to_Flag(void);
int setNewX(realword* wordpos[], int x);
void forgetWord(realword* wordpos[], int x);
void removeUIFlagsForWord(realword* wordpos[], int x);
void endOfSearch(realword* wordpos[]);
int ui_linked_list(const char *ptr_s);
int checkDictionaryWord(const char *word);
void resetAllFlags(void);




int main(void)
{
    // determine dictionary to use
    char *dictionary = DICTIONARY;

    // prompt the user for a word or phrase to make anagrams from
    printf("Please enter a word or phrase.\n");
    printf("Maximum number of alphabetic characters (including ') is %i.\n", LENGTH);
    printf("If you enter more than %i, only the first %i will be used.\n", LENGTH, LENGTH);

    // create a temporary location for the user input and
    // get the string from the user
    char s[99];
    char *ptr_s = s;
    fgets(ptr_s, 99, stdin);

    // call the function that creates the linked list of all the alpha letters
    // entered by the user ("ui linked list")
    int uiNodes = ui_linked_list(ptr_s);

    // set uiptr to the root node
    uiptr = root;

    printf("Using the %i letters you entered (", uiNodes);
    for (int j = 0 ; j < uiNodes ; j++)
    {
        printf("%c", uiptr->Letter);
        uiptr=uiptr->next;
    }
    printf("), these are the anagrams generated. (If none are shown, there were none, sorry).\n\n");





    // check every word in the dictionary to see if it can be made using only
    // the words in the UI linked list

    // open dictionary (the source file)
    FILE *ptr_dict = fopen(dictionary, "r");

    if (ptr_dict == NULL)
    {
        printf("Dictionary %s could not be opened.\n", dictionary);
        return 0;
    }

    // char array to store each word read in from the dictionary
    char word[MAXLENGTH+1];

    // int for word array position
    int index = 0;

    // int to count the number of real words found by the checkDictionaryWord() function
    // i.e. the number of nodes in the linked list
    int wordCount = 0;

    // assign heap memory for head node of the "real words" linked list
    head = malloc(sizeof(realword));

    // check pointer memory was allocated
    if (head == NULL)
    {
        printf("malloc failed to assign memory.\n");
        return 0;
    }

    // initialise the head node
    strcpy(head->rwWord, "");
    head->rwLength = 0;
    head->child = NULL;

    ptr = head;

    // loop to get each character from the dictionary in turn
    // when a newline (\n) is found, that is the end of a complete word
    for (int c = fgetc(ptr_dict) ; (!feof(ptr_dict)) ; c = fgetc(ptr_dict))
    {
        // add letter to word if alphabetical character or apostrophe
        if (isalpha(c) || (c == '\'' && index > 0))
        {
            // append character to word
            word[index] = (tolower(c));
            index++;
        }

        // if we are at the end of a dictionary word, terminate the current word array
        // and use the checkDictionaryWord() function to check if the word can be made from the
        // letters in the linked list
        else if (c == '\n')
        {
            word[index] = '\0';

            // call the function to check if the word can be made from the letters in the linked list
            int IsWord = checkDictionaryWord(word);

            if (IsWord == 1)
            {
                // if we're at the head node of the words list, we just want to populate it
                if (wordCount == 0)
                {
                    strcpy(ptr->rwWord, word);
                    ptr->rwLength = strlen(word);
                }

                // if not at the head node, we need to malloc memory for the new node
                // before populating it
                else
                {
                    // traverse to the end of the linked list
                    while (ptr->child != NULL)
                    {
                        ptr = ptr->child;
                    }

                    // now we're at the end of the list, create a node
                    // assign heap memory for new node
                    ptr->child = malloc(sizeof(realword));

                    // check pointer memory was allocated
                    if (ptr->child  == NULL)
                    {
                        printf("malloc failed to assign memory.\n");
                        return 0;
                    }

                    // move ptr to the new node, ready to populate it
                    ptr = ptr->child;

                    strcpy(ptr->rwWord, word);
                    ptr->rwLength = strlen(word);
                    ptr->child = NULL;
                }

                // iterate ready for the next real word
                wordCount++;

            }

            // prepare for next word from dictionary
            index = 0;
        }
    }


    // check whether there was an error opening the dictionary
    if (ferror(ptr_dict))
    {
        fclose(ptr_dict);
        printf("Error reading dictionary file.\n");
        return 1;
    }

    // close dictionary
    fclose(ptr_dict);


    /* prints all dictionary words found in the UI letters list
    ptr = head;
    while (ptr!=NULL)
    {
        printf("%s, ", ptr->rwWord);
        ptr=ptr->child;
    }
    printf("\n");
    */


    // create array called wordpos, consisting of "realword struct" pointers,
    // each one initially is null.
    // these will be used to point to and track the words as we find them in the user letters linked list.
    // 15 should be enough! (a bit wasteful of heap memory but it will be free'd afterwards)
    realword *wordpos[15];

    // set them all to null pointers
    for (int z = 0; z < 15; z++)
    {
        wordpos[z] = NULL;
    }





    // ********* FIND "WORD STRINGS" IN THE USER LETTERS LINKED LIST *************

    // set all the flags to 0 in the user-inputted (UI) letters linked list,
    // i.e. no letters are "used"
    resetAllFlags();

    // set pointer to root node
    uiptr = root;

    // x is used as an identifier for the x'th word in the word string
    // that makes up the complete "word string" anagram
    int x = 0;

    // starting with the 0'th word, set it to head of the linked list
    // (although it will move along the linked list in order to find
    // all combinations of words that make an anagram of the letters
    // that the user inputted)
    wordpos[x] = head;

    // continue the main loop while the child of wordpos[0] (i.e. the first
    // first word in our anagram word string) is not NULL.
    // The program should get to the the exit(0) line in function 10 before
    // this while loop returns false, but it is there as a safety net (just in case!)

    while (wordpos[0] != NULL)
    {
        if (checkForWord(wordpos[x]) == 1)              // Function 1
        {
            if (checkAllFlags() == 1)                   // Function 2
            {
                storeWordString(wordpos);               // Function 3
                resetAllTempFlags();                    // Function 4
                x = iterateOverWords(wordpos, x);       // Function 5
            }

            else
            {
                TempFlag_to_Flag();                     // Function 6
                x++;
                x = setNewX(wordpos, x);                // Function 7

            }
        }


        else        // if wordpos[x] wasn't found in the UI list
        {
            resetAllTempFlags();                        // Function 4
            x = iterateOverWords(wordpos, x);           // Function 5
        }
    }

    printf("Ooops... looks like something went wrong. Sorry!\n");

    return 0;
}










// ****************************************************************************
// ***************                    FUNCTIONS              ******************
// ****************************************************************************


// 1
// function to check if a word can be made from available letters in the
// UI linked list (i.e. letters that are not flagged)
// and if it can, set TempFlag to 1 for all the letters
int checkForWord(realword* word)
{
    uiptr = root;

    for (int y=0; y < word->rwLength; )
    {
        // if letter matches the UI linked list letter, and it's not flagged...
        if ((word->rwWord[y] == uiptr->Letter) && (uiptr->Flag == 0) && (uiptr->TempFlag == 0))
        {
            // ... set TempFlag to 1 (so it won't be re-used by another letter in this word)
            uiptr->TempFlag = 1;
            // reset uiptr to the root node
            uiptr = root;

            // iterate y so we will move on the next letter in the word
            y++;
        }


        else    // if the UI linked list letter is not matched, or it's already flagged
        {
            // if the UI node has a 'next node'...
            if (uiptr->next)
            {
                // ... move uiptr to the 'next node', so next iteration we're checking
                // the same letter against the next node in the UI linked list
                uiptr=uiptr->next;
            }

            else
            {
                // else we're at the end of the UI linked list, so we are unable to
                // find the letter in the linked list (it is flagged as having been
                // used already by a previous word, or earlier in this word).
                // So, this means the word is unable to be found, and we need to
                // reset the TempFlags and move on to the next word in
                // the 'dictionary words' linked list.
                return 0;
            }
        }
    }

    return 1;
}





// 2
// function to check if all nodes in UI linked list have
// Flag = 1 OR TempFlag = 1. If so, we've used up all the letters
// in the UI linked list.
int checkAllFlags(void)
{
    uiptr = root;
    while (uiptr)
    {
        if ((uiptr->TempFlag == 0) && (uiptr->Flag == 0))
        {
            return 0;
        }

        uiptr = uiptr->next;
    }

    // if no nodes are 0, all must have a flag
    return 1;
}




// 3
// function to store the current 'word string', because it is a perfect
// anagram of all the letters in the UI linked list.
void storeWordString(realword* wordpos[])
{
    int q = 0;
    while (wordpos[q]!=NULL)
    {
        printf("%s ", wordpos[q]->rwWord);
        q++;
    }

    printf("\n");
}




// 4
// function to set all the TempFlags to 0 in the UI linked list
void resetAllTempFlags(void)
{
    uiptr = root;
    while (uiptr)
    {
        uiptr->TempFlag = 0;
        uiptr=uiptr->next;
    }
}



// 5
// function to move a wordpos array pointer to the next node in the realwords
// linked list
int iterateOverWords(realword* wordpos[], int x)
{
    // if the current wordpos[] node has a child, move the pointer to the child node
    if (wordpos[x]->child)
    {
        //printf("XXX ");
        wordpos[x] = wordpos[x]->child;
        //printf("%i %s\n", x, wordpos[x]->rwWord);
        //printAllFlags();
        return x;
    }

    else
    // if it doesn't have a child node,
    // we're at the end of the realwords linked list, and
    // we've been unable to find a word in the UI linked list.
    // So, we have to forget about the current wordpos[x] and
    // move the previous wordpos[i.e. x-1] to the next word in the list
    {
        // BUT first... if we're on wordpos[0] and it doesn't have a child,
        // we are at the end of the search process, so end the search loop
        if (x == 0)
        {
            endOfSearch(wordpos);
        }

        // else
        forgetWord(wordpos, x);
        x--;
        removeUIFlagsForWord(wordpos, x);
        iterateOverWords(wordpos, x);
        return x;
    }

}




// 6
// function to convert TempFlags to Flags
void TempFlag_to_Flag(void)
{
    uiptr = root;
    while (uiptr)
    {
        if (uiptr->TempFlag == 1)
        {
            uiptr->Flag = 1;
            uiptr->TempFlag = 0;
        }

        uiptr = uiptr->next;
    }
}




// 7
// function to set a new wordpos[] value to the child of the previous
// wordpos[] value. This is needed so that when we need to 'forget'
// a wordpos[] pointer and go back to the previous wordpos[] value,
// we point the previous wordpos[] value to the next node in the list
int setNewX(realword* wordpos[], int x)
{
    // y is used as the previous wordpos value
    int y = x-1;

    // if the previous wordpos[] node has a child, move the pointer
    // to its child node
    if (wordpos[y]->child)
    {
        wordpos[x] = wordpos[y]->child;
        return x;
    }

    // if it doesn't, it was was the last node in the realwords list, so we
    // need to go back to the previous wordpos[] value BEFORE THAT ONE
    // and iterate it along the realwords linked list
    else
    {
        // go back to previous wordpos[] value, then 'forget' it
        x--;    // = y above

        // BUT first... if we're now on wordpos[0] and it doesn't have a child,
        // we are at the end of the search process, so end the search loop
        if (x == 0)
        {
            endOfSearch(wordpos);
        }

        //else
        // forget x
        removeUIFlagsForWord(wordpos, x);
        forgetWord(wordpos, x);

        // go back another wordpos[] value, as this is the one we need
        // to iterate to the next realwords node
        x--;
        removeUIFlagsForWord(wordpos, x);
        iterateOverWords(wordpos, x);

        return x;

    }
}




// 8
// function to 'forget' a wordpos[] pointer, i.e. remove it from the
// word string because we're at the end of the realwords linked list and
// can't use that wordpos[] word any further.
// Achieved by just setting the pointer to NULL.
void forgetWord(realword* wordpos[], int x)
{
    wordpos[x] = NULL;
}




// 9
// function to clear the Flags on the UI list nodes for a wordpos[] word that
// we are iterating to the next node in the realwords list
void removeUIFlagsForWord(realword* wordpos[], int x)
{
    uiptr = root;
    for (int c = 0; c < wordpos[x]->rwLength; )
    {
        if ((wordpos[x]->rwWord[c] == uiptr->Letter) && (uiptr->Flag == 1))
        {
            // if the letter matches the UI list node Letter, unflag it and iterate
            // to the next letter
            uiptr->Flag = 0;
            c++;
            uiptr = root;
        }

        // if the letter doesn't match, or it's already been 'unFlagged',
        // move to the next UI list node, and repeat the loop for the current letter
        else
        {
            uiptr=uiptr->next;
        }
    }
}





// 10
// function to free up malloced memory and say "Thanks" before
// the program exits
void endOfSearch(realword* wordpos[])
{
    // reset the array of pointers to null
    for (int i = 0; i < 15; i++)
    {
        wordpos[i] = NULL;
    }


    // free all the nodes in the two linked lists
    uiptr = root;
    while (uiptr)
    {
        node *uiptr2 = uiptr;
        uiptr = uiptr->next;
        free(uiptr2);
    }


    ptr = head;
    while (ptr)
    {
        realword *ptr2 = ptr;
        ptr = ptr->child;
        free(ptr2);
    }

    printf("\nThanks for using \"Anagram.\"\n\n");
    exit(0);
}




// 11
// function to create a linked list of the user input letters
int ui_linked_list(const char *ptr_s)
{
    // assign heap memory for root node
    root = malloc(sizeof(node));

    // check pointer memory was allocated
    if (root == NULL)
    {
        printf("malloc failed to assign memory.\n");
        return 0;
    }


    // initialise the root node
    root->next = NULL;
    root->Letter = 0;
    root->Flag = 0;

    // set ptr to point to the root node
    uiptr = root;

    // int to track the number of nodes in the linked list,
    // uiNodeCount = 0 is the root node
    int uiNodeCount = 0;

    // iterate over the remainder of the user input, if it's alpha add it
    // to the linked list
    for (int i = 0; i < (((int)strlen(ptr_s))-1); i++)
    {
        // only allow linked list to grow if there are < 'LENGTH' alpha characters
        if (uiNodeCount < LENGTH)
        {
            if (isalpha(ptr_s[i]))
            {
                // if we're at the root node we just want to populate it
                if (uiNodeCount == 0)
                {
                    uiptr->Letter = tolower(ptr_s[i]);
                }

                // if not at the root node, we need to malloc memory for the new node
                // before populating it
                else
                {
                    // traverse to the end of the list
                    while (uiptr->next != NULL)
                    {
                        uiptr = uiptr->next;
                    }

                    // now we're at the end of the list, create a node
                    // assign heap memory for new node
                    uiptr->next = malloc(sizeof(node));

                    // check pointer memory was allocated
                    if (uiptr->next  == NULL)
                    {
                        printf("malloc failed to assign memory.\n");
                        return 0;
                    }

                    // move ptr to the new node, ready to populate it
                    uiptr = uiptr->next;

                    // populate the node with the character info
                    // and set the next pointer to NULL
                    uiptr->Letter = tolower(ptr_s[i]);
                    uiptr->Flag = 0;
                    uiptr->next = NULL;
                }

                uiNodeCount++;
            }
        }

        else
        {
            break;
        }
    }

    return uiNodeCount;

}



// 12
// function to check if a word from the dictionary can be
// found in the UI linked list
int checkDictionaryWord(const char *word)
{
    // set uiptr pointer to point to root node
    uiptr = root;

    // clear the flags from all nodes in the linked list
    while (uiptr !=NULL)
    {
        uiptr->Flag = 0;
        uiptr=uiptr->next;
    }

    // reset pointer to point to root node
    uiptr = root;

    // count how many characters in the dictionary word to be checked
    int length = strlen(word);

    // iterate over each letter in word
    for (int i = 0 ; i < length ; i++)
    {
        // if the letter value of the node does not match the 'word' letter,
        // OR if the node is already flagged (i.e. already been used in this 'word')
        // move on to the next node in the linked list
        while ((uiptr->Letter != word[i]) || (uiptr->Flag == 1))
        {
            uiptr = uiptr->next;

            // if we're at the end of the linked list we haven't found the letter so the 'word'
            // cannot be made, therefore return 0
            if (uiptr == NULL)
            {
                return 0;
            }
        }

        // otherwise the node can be flagged to show it is a letter from 'word'
        uiptr->Flag = 1;

        // reset the pointer to point to the root node of the linked list, ready for checking the next letter
        uiptr = root;
    }


    // if we get through every letter of 'word' we can make the word, so return 1
    return 1;

}





// 13
// function to set all the flags to 0 in the UI linked list,
// i.e. no letters are "used"
void resetAllFlags(void)
{
    uiptr = root;
    while (uiptr)
    {
        uiptr->Flag = 0;
        uiptr->TempFlag = 0;
        uiptr=uiptr->next;
    }
}