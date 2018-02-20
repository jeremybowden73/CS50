#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main (void)
{
    printf("O hai! How much change is owed?\n");
    float dollars = get_float();
    
    while (dollars<0)
    {
        printf("How much change is owed?\n");
        dollars = get_float();
    }
    
    int cents = round(100*dollars);
    int coins=0;
    
    // How many 25c coins can be given?
    coins = cents/25;
    cents = cents % 25;
    
    // How many 10c coins can be given?
    coins = coins + (cents/10);
    cents = cents % 10;
    
    // How many 5c coins can be given?
    coins = coins + (cents/5);
    cents = cents % 5;
    
    // How many 1c coins can be given?
    coins = coins + (cents/1);
    cents = cents % 1;
    
    //How many coins in total?
    printf("%d\n", coins);
    
}