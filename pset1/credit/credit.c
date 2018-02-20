#include <stdio.h>
#include <cs50.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
int values(long long num, int counter);
int bankname(long long num, int counter);
int values_calculator(long long num, int counter);

int main(void)
{
    printf("Number: ");
    long long number = get_long_long();
    
    // set up variables to track the total of the values of the digits in the card number
    // and a 'score' to determine which bank it is
    int digits_total=0;
    int bank_total=0;
    int returned_values;
    int bank_returned_vals;
    int ctr;
    
    // loop to add up the values of the digits in the card number
    for (ctr=1; ctr <=16; ctr++)
    {
        returned_values = values(number, ctr);
        digits_total = digits_total + returned_values;
    }
    
    
    // loop to check which bank, based on a 'score' for each digit in the card number
    for (ctr=1; ctr <=16; ctr++)
    {
        bank_returned_vals = bankname(number, ctr);
        bank_total = bank_total + bank_returned_vals;
    }
    
    // check if the number is genuine --- is the digits total divisible by 10?
    // if so, use the bank 'score' to check which bank
    if (digits_total % 10 == 0)
    {
        if (bank_total==1 || bank_total==2 || bank_total==12 || bank_total==13)
        {
            printf("VISA\n");
        }
        
        else if (bank_total==5 || bank_total==15 || bank_total==16)
        {
            printf("MASTERCARD\n");
        }
        
        else if (bank_total==10 || bank_total==11)
        {
            printf("AMEX\n");
        }
        
        else
        {
            printf("INVALID\n");
        }
        
    }
    
    else
    {
        printf("INVALID\n");
    }
    
    return 0;
}



// function to return the values of the digits in the card number, using a secondary calculator function
int values(long long num, int counter)
{
    int return_value;
    return_value = values_calculator(num, counter);
    return(return_value);
}



// function to 'score' each card number to determine which bank it is, using the secondary calculator function
int bankname(long long num, int counter)
{
    int return_value;
    int bank_score=0;
    
    return_value = values_calculator(num, counter);
    
    // known data about visa, amex, mastercard numbers is used to give a 'score' to the number depending on
    // its position in the number and it's numeric value
    (counter==13 && return_value==4) ? (bank_score+=1) : (bank_score+=0) ;
    (counter==15 && return_value==3) ? (bank_score+=10) : (bank_score+=0) ;
    (counter==16 && return_value==8) ? (bank_score+=2) : (bank_score+=0) ;
    (counter==16 && return_value==1) ? (bank_score+=5) : (bank_score+=0) ;
    
    return(bank_score);      
}



// function to calculate the numerical value of each digit in the card number
// multiply by 2, or don't, depending on its position in the number (which is
// determined by the value of the variable 'counter')
int values_calculator(long long num, int counter)
{
    long long mod = pow(10,counter);
    long long divi = pow(10,counter-1);
    int return_value;
    
    if (counter % 2 == 0)
    {
    return_value = 2*((num % mod)/divi);
    (return_value>=10) ? (return_value = 1 + (return_value % 10)) : (return_value = return_value+0);
    }
    
    else
    {
        return_value = ((num % mod)/divi);
    }
    
    return(return_value);
}