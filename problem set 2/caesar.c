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
    string text = get_string();


    printf("ciphertext: ");

    // Rotate every character a value of key up the alphabet, ignores special characters
    int textLength = strlen(text);
    int newText[textLength];
    for (int i = 0; i < textLength; i++)
    {
        // Encrypt characters between A-Z or a-z only
        if ((text[i] >= 'A' && text[i] <= 'Z') || (text[i] >= 'a' && text[i] <= 'z'))
        {
            newText[i] = (text[i] + key) % 'z';     // wrap around 'z'

            // if encrypted character lands on special character, shift into the accepted range
            if (newText[i] < 'A')
            {
                newText[i] = newText[i] + 96;

                // Some characters may land beyond 'z' but will loop around differently depending on where they land
                if (newText[i] > 'z' && newText[i] < 150)
                    newText[i] = (newText[i] % 'z') + 96;
                else if (newText[i] >= 150)
                    newText[i] = (newText[i] % 'z') + 70;
            }
        }
        else
        {
            newText[i] = text[i];
        }
        printf("%c", newText[i]);
    }
    printf("\n");

    return 0;
}
