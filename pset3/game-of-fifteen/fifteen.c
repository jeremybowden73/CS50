/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        
        // sleep thread for animation's sake
        usleep(500000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{
    // for loops for board[i] and [j], increasing the value of a on each loop
    // a is the number to minus from d*d to get the tile value
    int a=1;
    for (int i=0 ; i < d ; i++)
    {
        for (int j=0 ; j < d ; j++)
        {
            board[i][j] = ((d*d) - a);
            a+=1;
        }
    }
    
    // if d is even, swop the 2 and 1 tiles 
    if (d%2==0)
    {
        board[d-1][d-2] = 2;
        board[d-1][d-3] = 1;
    }

}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    printf("\n");
    
    for (int i=0 ; i < d ; i++)
    {
        for (int j=0 ; j < d ; j++)
        {
            // the blank tile should be shown as _ , not as d*d
            // this if condition achieves that
            if (board[i][j]==0)
            {
                printf("   _  ");
            }
            
            else
            {
                printf("  %2i  ", board[i][j]);
            }
        }
        
        printf("\n\n");
            
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    // define ints for the i (row) and j (column) position of the blank
    int blanki;
    int blankj;
    // define ints for the i (row) and j (column) position of the tile the
    // player selected for moving
    int tilei;
    int tilej;
    
    // loop through each position on the board
    // if the value is 0 i.e the blank, assign the i and j values to blanki and blankj
    // if the value is that of the tile that was selected byt he player, assign the i and j values to tilei and tilej
    for (int i=0 ; i < d ; i++)
    {
        for (int j=0 ; j < d ; j++)
        {
            if (board[i][j] == 0)
            {
                blanki=i;
                blankj=j;
            }
    
            if (board[i][j] == tile)
            {
                tilei=i;
                tilej=j;
            }
        }
    }
    
    
    // if any of the tiles bordering the blank (left, right, above or below) are selected for moving, return true
    // if any other tiles are selected for moving, return false
    if 
    ( (((tilei == blanki) && (tilej == blankj-1)) || ((tilei == blanki) && (tilej == blankj+1)))  ||
    (((tilej == blankj) && (tilei == blanki-1)) || ((tilej == blankj) && (tilei == blanki+1))) )
    
    {
        // if true, assign the value of the tile to the blank's position and the the value
        // of the blank (0) to the tile's position.
        // i.e. they swop places on the board.
        board[blanki][blankj] = tile;
        board[tilei][tilej] = 0;
        return true;
    }  
    
    else
    {
        return false;
    }
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    for (int i=0, a=1 ; i < d ; i++)
    {
        for (int j=0 ; j < d ; j++)
        {
            // check if the value of each non-zero tile is what it needs to be
            // for a winning board. Don't need to check the 'last' tile (d,d)
            // is 0 because if all the other tiles are correct then that one
            // must also be correct.
            // if any tile is not what it needs to be, return false
            if ((i!=(d-1)) || (j!=(d-1)))
            {
                if (board[i][j] == a)
                {
                    a++;
                }
                
                else
                {
                    return false;
                }
            }
        }
    }
    
    // if all tiles are correct, return true!
    return true;
}
