#include <stdio.h>
#include <cs50.h>

void print_spaces(int height, int space_counter);
void print_hashes(int hashes);

int main(void)
{
    printf("Height: ");
    int height = get_int();
    
    while (height<0 || height>23)
    {   printf("Height: ");
        height = get_int();
    }
    
    int ctr;
    for (ctr=0; ctr < height; ctr++)
    {
        print_spaces(height, ctr);
        print_hashes(ctr);
        printf("  ");
        print_hashes(ctr);
        printf("\n");
    }
    
}


void print_spaces(int height, int space_counter)
{
    int ctr;
    for (ctr=0; ctr<(height-(space_counter+1)); ctr++)
    {
        printf(" ");
    }
}

void print_hashes(int hashes)
{
    int ctr;
    for (ctr=0; ctr<(hashes+1); ctr++)
    {
        printf("#");
    }
}