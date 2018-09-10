// Note that usleep is obsolete, but it offers more granularity than sleep and is simpler to use than nanosleep; `man usleep` for more.


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
int winBoard[DIM_MAX][DIM_MAX];

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
    // Ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // Ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // Open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
        return 3;


    // Greet user with instructions
    greet();

    // Initialize the board
    init();

    // Accept moves until game is won
    while (true)
    {
        clear();                // clear the screen

        draw();                 // draw the current state of the board

        // Log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                    fprintf(file, "|");
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // Check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // Prompt for move
        printf("Tile to move: ");
        int tile = get_int();

        // Quit if user inputs 0 (for testing)
        if (tile == 0)
            break;

        // Log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // Move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // Sleep thread for animation's sake
        usleep(50000);
    }

    // Close log
    fclose(file);

    // Success
    return 0;
}


// Clears screen using ANSI escape sequences.
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}


// Greets player
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}


// Initializes the game's board with tiles numbered 1 through d*d - 1  (i.e., fills 2D array with values but does not actually print them).
void init(void)
{
    usleep(1000000);

    // Fill board with decreasing values starting with d^2
    int initVal = d*d;
    int row = 0;
    int col = -1;
    while (col < d-1)
    {
        col++;
        for (row = 0; row < d; row++)
        {
            initVal--;
            board[col][row] = initVal;
        }
    }

    if ((d*d - 1) % 2 != 0)
    {
        board[d-1][d-2] = 2;
        board[d-1][d-3] = 1;
    }

    usleep(1000000);
}


// Prints the board in its current state
void draw(void)
{
    int row = 0;      // row
    int col = -1;     // column

    while (col < d-1)
    {
        row = 0;
        col++;
        for (row = 0; row < d; row++)
            printf("%i ", board[col][row]);
        printf("\n");
    }
    printf("\n");

    usleep(200000);
}


// If tile borders empty space, moves tile and returns true, else returns false
bool move(int tile)
{
    int row = 0;      // row
    int col = -1;     // column
    int tileCol;
    int tileRow;

    while (col < d-1)             // figure out the row and column of chosen tile
    {
        row = 0;
        col++;
        for (row= 0; row < d; row++)          // look through every position in the matrix
        {
            if (board[col][row] == tile)    // once you find your number put the i and j into tileCol and tileRow
            {
                tileCol = col;
                tileRow = row;
            }
        }
    }

    // Look around your tile and see if there is a zero above, below, to the left, to the right
    if (board[tileCol-1][tileRow] == 0 && tileCol-1 >= 0)
    {
        board[tileCol-1][tileRow] = tile;
        board[tileCol][tileRow] = 0;
        return true;
    }
    else if (board[tileCol+1][tileRow] == 0 && tileCol+1 < d)
    {
        board[tileCol+1][tileRow] = tile;
        board[tileCol][tileRow] = 0;
        return true;
    }
    else if (board[tileCol][tileRow-1] == 0 && tileRow-1 >= 0)
    {
        board[tileCol][tileRow-1] = tile;
        board[tileCol][tileRow] = 0;
        return true;
    }
    else if (board[tileCol][tileRow+1] == 0 && tileRow+1 < d)
    {
        board[tileCol][tileRow+1] = tile;
        board[tileCol][tileRow] = 0;
        return true;
    }
    else
    {
        return false;       // if there is no 0 found, return false -> illegal move
    }

}


// Returns true if game is won (i.e., board is in winning configuration), else false
bool won(void)
{
    // Winboard
    int initVal = 0;
    int row = 0;
    int col = -1;
    while (col < d-1)
    {
        col++;
        for (row = 0; row < d; row++)
        {
            initVal++;
            winBoard[col][row] = initVal;
        }
    }
    winBoard[d-1][d-1] = 0;

    row = 0;
    col = -1;
    while (col < d-1)
    {
        col++;
        for (row = 0; row < d; row++)
        {
            if (winBoard[col][row] != board[col][row])
                return false;
        }
    }

    return true;
}
