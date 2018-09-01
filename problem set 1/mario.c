#include <stdio.h>
#include <cs50.h>


int main(void)
{
    // Get the number of blocks from the user
    int height = -1;
    while (height < 0 || height > 23)
    {
        printf("Height (0-23): ");
        height = get_int();
    }

    // Large loop for each row, nested loops for characters within the row
    int i, j;
    for (i = height; i >= 1; i--)
    {
        for (j = i - 1; j > 0; j--)         // spaces before the hashes before the repeating block
            printf(" ");

        for (j = 0; j < height - i; j++)    // hashes before the repeating block
            printf("#");

        printf("#  #");                     // central, repeating block

        for (j = 0; j < height - i; j++)    // hashes after the repeating block
            printf("#");

        printf("\n");                       // newline before starting next tow
    }

    return 0;
}