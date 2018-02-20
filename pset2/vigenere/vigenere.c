#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// define some constants that will be used in the maths
#define A 65
#define Z 90
#define z 122
#define alphabet 26

/*
int main(int argc, string argv[])
{

// check that only one command line argument has been entered
if (argc!=2)
{
    printf("Error. You must enter one command line argument.\n");
    return 1;
}
*/

int main(void)
{
string s = get_string("Enter keyword (e.g. ABC): ");

// ensure string was read
if (s == NULL)
{
    printf("Error. You must enter only alphabetic characters (no spaces!).\n");
    return 1;
}

    // check that each character in the keyword
    // is alphabetic, and if not print a message and exit
    int key_length=strlen(s);

    for (int i=0 ; i<key_length ; i++)
    {
        if ((isalpha(s[i])) == 0)
        {
            printf("Error. You must enter only alphabetic characters (no spaces!).\n");
            return 1;
        }
    }


    // create an integer array for the 'steps' that must be added to a character in the plaintext
    // to get the ciphertext, depending on what was entered in the key
    int steps[key_length];
    for (int k = 0 ; k < key_length ; k++)
    {
        steps[k] = toupper((s[k])) - A ;
    }


    // get the plaintext string from the user
    printf("plaintext:  ");
    string plaintext = get_string();
    int ptxt_length=strlen(plaintext);

    // test that the string is not invalid.
    if (plaintext != NULL)
    {
        // create a string to store the ciphertext, initally the same as the plaintext
        string ciphertext = plaintext;

        // loop to add the next value of 'steps' from the key to each
        // character in ciphertext, one at a time
        // but only if its an alpha character
        int steps_pos=0;    // int to keep track of how far through the characters in 'steps' we are
        for (int j = 0 ; j < ptxt_length ; j++)
        {
            // only work on plaintext[j] if is alphabetic
            if (isalpha(plaintext[j]))
            {
                // if encoding the plaintext character will cause the ciphertext character to go past
                // Z or z, loop it back round to A or a.
                if (((isupper(plaintext[j]))  &&  (((plaintext[j]) + steps[steps_pos]) > Z)) ||
                    ((islower(plaintext[j]))  &&  (((plaintext[j]) + steps[steps_pos]) > z)))
                {
                    ciphertext[j] = plaintext[j] + steps[steps_pos] - alphabet;
                }

                // if it won't, just add the value of 'steps' to the plaintext character
                else
                {
                    ciphertext[j] = plaintext[j] + steps[steps_pos];
                }



                // increase steps_pos by one to move on to the next value in the 'steps' array
                steps_pos+=1;
                // when steps_pos reaches the same value as the number of chars in the key, reset steps_pos to 0
                if (steps_pos==key_length)
                {
                    steps_pos=0;
                }
            }

            // if the ciphertext character is not alphabetic, it will not change
            else
            {
                ciphertext[j] = plaintext[j] ;
            }
        }

        printf("ciphertext: %s\n", ciphertext);

    }

    return 0;
}
