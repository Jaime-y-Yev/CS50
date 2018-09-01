#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <stdlib.h>

int LUEncrypt();


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
    for (int i = 0; i < keyLength; i++)
    {
        if (key[i] < 'A' || (key[i] > 'Z' && key[i] < 'a') || key[i] > 'z')
        {
            printf("The key must contain only letters. \n");
            return 1;
        }
    }

    printf("plaintext: ");
    string plainText = get_string();

    printf("ciphertext: ");

    // Loop through key and plainText
    int plainTextLength = strlen(plainText);
    int cipherText[plainTextLength];
    int keyIndex = 0;
    for (int i = 0; i < plainTextLength; i++) // when key >= plainTextLength
    {
        if ((plainText[i] >= 'A' && plainText[i] <= 'Z'))                      // plainText[i] is uppercase
        {
            cipherText[i] = LUEncrypt(i, keyIndex, key, plainText, keyLength, plainTextLength);

            if (cipherText[i] < 'A' || cipherText[i] > 'Z')     // if encrypted not capital
            {
                cipherText[i] = cipherText[i] - 26;

                // Some characters may land beyond 'z' but will loop around differently depending on where they land
                if (cipherText[i] > 122 && cipherText[i] < 150)
                    cipherText[i] = (cipherText[i] % 122) + 96;
                else if (cipherText[i] >= 150)
                    cipherText[i] = (cipherText[i] % 122) + 70;
            }

            keyIndex++;
        }
        else if((plainText[i] >= 'a' && plainText[i] <= 'z'))                // plainText[i] is lowercase
        {
            cipherText[i] = LUEncrypt(i, keyIndex, key, plainText, keyLength, plainTextLength);

            if(cipherText[i] < 'a' || cipherText[i] > 'z') //if encrypted not lowercase
            {
                cipherText[i] = cipherText[i] - 26;

                /* some characters may land beyond the z character
                *  but will loop around differently depending on where they land
                */
                if(cipherText[i] > 122 && cipherText[i] < 150)
                    cipherText[i] = (cipherText[i] % 122) + 96;
                else if(cipherText[i] >= 150)
                    cipherText[i] = (cipherText[i] % 122) + 70;
            }

            keyIndex++;
        }
        else if(plainText[i] < 'A' || (plainText[i] > 'Z' && plainText[i] < 'a') || plainText[i] > 'z')
        {
            cipherText[i] = plainText[i];
        }

        //printf("Your plainText %c added to key %c becomes: %c \n", plainText[i], key[i], cipherText[i]);
        printf("%c", cipherText[i]);
    }
    printf("\n");

    return 0;
}


int LUEncrypt(int i, int keyIndex, string key, string plainText, int keyLength, int plainTextLength)
{
    int cipherText[plainTextLength];

    if(key[keyIndex % keyLength] >= 97 && key[keyIndex % keyLength] <= 122) //lowercase filter
    {
        cipherText[i] = plainText[i] + (key[keyIndex % keyLength] % 97);
    }
    else if(key[keyIndex % keyLength] >= 65 && key[keyIndex % keyLength] <= 90) //uppercase filter
    {
        cipherText[i] = plainText[i] + (key[keyIndex % keyLength] % 65);
    }

    return cipherText[i];
}