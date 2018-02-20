#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main (void)
{
    // Get the string from the user
    string s = get_string();
    
    // Test that the string is not invalid.
    if (s != NULL)
    {
        // If the first character is not a space, it must be the first
        // initial, so print it (in uppercase).
        if (s[0]!=' ')
        {
            printf ("%c", toupper(s[0]));
        }
        
        // Loop through each remaining character in the string, testing if it is a space.
        // Limit is <strlen(s)-1 because of the null zero at the end of the string,
        // and if the last char entered is a space we can ignore it.
        for (int i = 0 ; i < (strlen(s)-1) ; i++)
        {
            if (s[i]==' ')
            {
                // If the character is a space, test to see if the next character is not a space.
                // If so, it must be an initial so print it (in uppercase).
                if (s[i+1]!=' ')
                {
                    printf ("%c", toupper(s[i+1]));
                }
                
            }
            
        }
        
        printf("\n");
    }
    
    return 0;
}
