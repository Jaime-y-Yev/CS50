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
            int currentKeyChar = key[keyIndex % keyLength];         // loop through key

            // Conditionally adjust key according to upper or lower case eg. key=a or key=A means shift=0, key=B or key=B means shift=1
            int shift = 0;
            if (currentKeyChar >= 'A' && currentKeyChar <= 'Z')
                shift = currentKeyChar % 'A';
            else if (currentKeyChar >= 'a' && currentKeyChar <= 'z')
                shift = currentKeyChar % 'a';

            cipherText[i] = plainText[i] + shift;


            // Find plainText case and cipherText case
            _Bool plainLetterIsUp = false;
            _Bool cipherLetterIsUp = false;
            _Bool plainLetterIsDown = false;
            _Bool cipherLetterIsDown = false;
            if (plainText[i] >= 'A' && plainText[i] <= 'Z')
                plainLetterIsUp = true;
            if (cipherText[i] >= 'A' && cipherText[i] <= 'Z')
                cipherLetterIsUp = true;
            if (plainText[i] >= 'a' && plainText[i] <= 'z')
                plainLetterIsDown = true;
            if (cipherText[i] >= 'a' && cipherText[i] <= 'z')
                cipherLetterIsDown = true;

            // If plainText case and cipherText case don't match
            if ((plainLetterIsUp && !cipherLetterIsUp) || (plainLetterIsDown && !cipherLetterIsDown))
            {
                cipherText[i] = cipherText[i] - 26;

                // Some characters may land beyond 'z' but will loop around differently depending on where they land
                if (cipherText[i] > 'z' && cipherText[i] < 150)
                    cipherText[i] = (cipherText[i] % 122) + 96;
                else if (cipherText[i] >= 150)
                    cipherText[i] = (cipherText[i] % 122) + 70;
            }

            keyIndex++;     // pick the next key character to be used in the following loop
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
