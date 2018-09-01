#include <stdio.h>
#include <cs50.h>
#include <string.h>


int main(int argc, string argv[])
{
    // User may only input 2 command-line arguments
    if (argc != 2)
    {
        printf("Usage: ./vigenere key\n");
        return 1;
    }

    // key must be only letters
    string key = argv[1];
    int keyLength = strlen(key);
    int keyIndex = 0;
    for (keyIndex = 0; keyIndex < keyLength; keyIndex++)
    {
        if (key[keyIndex] < 'A' || (key[keyIndex] > 'Z' && key[keyIndex] < 'a') || key[keyIndex] > 'z')
        {
            printf("The key must contain only letters. \n");
            return 1;
        }
    }

    printf("plaintext: ");
    string plainText = get_string();

    printf("ciphertext: ");

    // Loop through plainText and (conditionally) the key
    int plainTextLength = strlen(plainText);
    int cipherText[plainTextLength];
    keyIndex = 0;
    for (int i = 0; i < plainTextLength; i++)
    {
        // If not a special character, encrypt plainText[i] and loop through key
        if ((plainText[i] >= 'A' && plainText[i] <= 'Z') || (plainText[i] >= 'a' && plainText[i] <= 'z'))
        {
            // Shift plaintext up according to the current key character
            int currentKeyChar = key[keyIndex % keyLength];         // loop through key

            if (currentKeyChar >= 'A' && currentKeyChar <= 'Z')         // uppercase filter
                cipherText[i] = plainText[i] + (currentKeyChar % 'A');
            else if (currentKeyChar >= 'a' && currentKeyChar <= 'z')    // lowercase filter
                cipherText[i] = plainText[i] + (currentKeyChar % 'a');


            // If plainText case and cipherText case don't match
            if (((plainText[i] >= 'A' && plainText[i] <= 'Z') && (cipherText[i] < 'A' || cipherText[i] > 'Z')) ||
                ((plainText[i] >= 'a' && plainText[i] <= 'z') && (cipherText[i] < 'a' || cipherText[i] > 'z')))
            {
                cipherText[i] = cipherText[i] - 26;

                // Some characters may land beyond 'z' but will loop around differently depending on where they land
                if (cipherText[i] > 'z' && cipherText[i] < 150)
                    cipherText[i] = (cipherText[i] % 122) + 96;
                else if (cipherText[i] >= 150)
                    cipherText[i] = (cipherText[i] % 122) + 70;
            }

            keyIndex++;
        }
        // Do not encrypt special characters, do not loop through key
        else
        {
            cipherText[i] = plainText[i];
        }

        printf("%c", cipherText[i]);
    }
    printf("\n");

    return 0;
}
