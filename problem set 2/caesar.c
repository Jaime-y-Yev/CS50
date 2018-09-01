#include <stdio.h>
#include <cs50.h>
#include <string.h>


int main(int argc, string argv[])
{
    // User may only input 2 command-line arguments, the 2nd being a positive number
    if (argc != 2 || atoi(argv[1]) < 0)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    int key = atoi(argv[1]);        // convert the 2nd command-line argument to an integer


    // Prompt for string to encrypt
    printf("plaintext: ");
    string plainText = get_string();


    printf("ciphertext: ");

    // Rotate every character a value of key up the alphabet, ignores special characters
    int textLength = strlen(plainText);
    int cipherText[textLength];
    for (int i = 0; i < textLength; i++)
    {
        // Encrypt characters between A-Z or a-z only
        if ((plainText[i] >= 'A' && plainText[i] <= 'Z') || (plainText[i] >= 'a' && plainText[i] <= 'z'))
        {
            cipherText[i] = (plainText[i] + key) % 'z';     // wrap around 'z'

            // if encrypted character lands on special character, shift into the accepted range
            if (cipherText[i] < 'A')
            {
                cipherText[i] = cipherText[i] + 96;

                // Some characters may land beyond 'z' but will loop around differently depending on where they land
                if (cipherText[i] > 'z' && cipherText[i] < 150)
                    cipherText[i] = (cipherText[i] % 'z') + 96;
                else if (cipherText[i] >= 150)
                    cipherText[i] = (cipherText[i] % 'z') + 70;
            }
        }
        else
        {
            cipherText[i] = plainText[i];
        }
        printf("%c", cipherText[i]);
    }
    printf("\n");

    return 0;
}
