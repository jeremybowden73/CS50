#define _XOPEN_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <cs50.h>
#include <crypt.h>
#include <stdlib.h>

void check_for_match(int length, string cla_password, string password);

int main(int argc, string argv[])
{
    // check that only one command line argument has been entered
    if (argc!=2)
    {
        printf("Error. You must enter one command line argument.\n");
        return 1;
    }


    // create int variable and string variable to enable using the command line argument
    int length = (strlen(argv[1]));
    string cla_password = (argv[1]);
    
    // create a char array for the password "attempts",
    // capable of taking up to (4 + \0) = 5 chars
    char password[5];
    
    
    // loop to create 1-char password "attempts" between ASCII characters A and z
    // which admittedly includes 6 non-alpha characters (i.e. an ineffiency in this code).
    for (password[0] = 'A' ; password[0] <= 'z' ; password[0]++)
    {
        // for each iteration of the password, call this function which creates the hashed
        // password by using crypt() and compares it to the command line argument (cla) password.
        check_for_match(length, cla_password, password);
    }
    
    
    // similar loop to create 2-char password "attempts"
    for (password[0] = 'A' ; password[0] <= 'z' ; password[0]++)
    {
        for (password[1] = 'A' ; password[1] <= 'z' ; password[1]++)
        {
            check_for_match(length, cla_password, password);
        }
        
    }
    
    
    // similar loop to create 3-char password "attempts"
    for (password[0] = 'A' ; password[0] <= 'z' ; password[0]++)
    {
        for (password[1] = 'A' ; password[1] <= 'z' ; password[1]++)
        {
            for (password[2] = 'A' ; password[2] <= 'z' ; password[2]++)
            {
                check_for_match(length, cla_password, password);
            }
        }
    }

       
    // similar loop to create 4-char password "attempts"
    for (password[0] = 'A' ; password[0] <= 'z' ; password[0]++)
    {
        for (password[1] = 'A' ; password[1] <= 'z' ; password[1]++)
        {
            for (password[2] = 'A' ; password[2] <= 'z' ; password[2]++)
            {
                for (password[3] = 'A' ; password[3] <= 'z' ; password[3]++)
                {
                    check_for_match(length, cla_password, password);
                }
                
            }
        }
    }
    
}




// function to create the hashed password for each password "attempt" and
// check each element of it against the corresponding element of the
// known (command line argument) password
void check_for_match(int length, string cla_password, string password)
{
    // make 'hashed_password' by running crypt on the password "attempt" with salt as 50
    string hashed_password = crypt(password, "50");
        
    int i=0;
    for (i=0 ; i<length ; i++)
    {
        // compare the i'th element of the known hashed password against the i'th element of the
        // hashed password "attempt"
        if (cla_password[i] != hashed_password[i])
        {
            // terminate the loop as soon as the first unmatched element is found
            break;;
        }
        
    }  
    
    // check if the loop above ran all "length" times, if it did then we have found the password!
    // if it didn't, we go back to main() for the next iteration of password "attempt"
    if (i == length)
        {
            printf("%s\n", password);
            // as soon as the password if found, exit the program with return value 0
            exit(0);
        }
}