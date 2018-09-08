#include <stdio.h>
#include <cs50.h>
#include <string.h>

#define _XOPEN_SOURCE
#include <unistd.h>

char *crypt();

// If the encrypted key equals the given hash, that key is the password (so terminate the program)
void compare(char key[], char salt[], char hash[]);    // If the encrypted key equals the given hash, that key is the password

// Non-recursively loop first through all one-letter combinations, then all two-letter combos, and so forth until all five-letter combos
void loopNonRecursive(int keyLength, string key, string options, string salt, string hash);

// Recursively loop first through all one-letter combinations, then all two-letter combos, and so forth until all five-letter combos
void loopRecursive(int keyLength, int keyIndex, string key, string options, string salt, string hash);


int main(int argc, string argv[])
{
    // User may only input 2 command-line arguments
    if (argc != 2)
    {
        printf("Usage: ./crack hash\n");
        return 1;
    }

    string hash = argv[1];

    char salt[3] = {hash[0], hash[1], '\0'};            // salt is 2 characters + 1 for null byte
    //printf("salt = %s \n", salt);

    char options[52];                                   // options = {'A', 'B', ..., 'Z', 'a', 'b', ..., 'z'}
    for (int i = 0; i < 26; i++)
    {
        options[i] = 'A' + i;
        options[i+26] = 'a' + i;
    }

    char key[6] = {'\0','\0','\0','\0','\0','\0'};      // key is 5 characters + 1 for null byte

    /* Loop through all possible combinations,
     * first through all one-letter combinations (i=0), then through all two-letter passwords (i=1),
     * and so forth until looping finally through all five-letter passwords (i=4) */
    for (int keyLength = 0; keyLength < 5; keyLength++)
    {
        loopNonRecursive(keyLength, key, options, salt, hash);          // can be done recursively or non-recursively
        //loopRecursive(keyLength, 0, key, options, salt, hash);
    }


    return 0;
}

// If the encrypted key equals the given hash, that key is the password (so terminate the program)
void compare(char key[], char salt[], char hash[])
{
    printf("key = %s \n", key);

    string encryptedPassword = crypt(key, salt);
    if (strcmp(encryptedPassword, hash) == 0)
    {
        printf("The password is: %s \n", key);
        exit(0);                // if the key is found, terminate the program with exit code 0
    }
}

// Non-recursively loop first through all one-letter combinations, then all two-letter combos, and so forth until all five-letter combos
void loopNonRecursive(int keyLength, string key, string options, string salt, string hash)
{
    for (int o0 = 0; o0 < 52; o0++)     // 52 possible options in {'A', 'B', ..., 'Z', 'a', 'b', ...,'z'}
    {
        key[0] = options[o0];

        if (keyLength < 1)                      // if in the one-letter loop, compare each and every ONE-LETTER combo
        {
            compare(key, salt, hash);
        }
        else                                    // else allow for all two-letter combos
        {
            for (int o1 = 0; o1 < 52; o1++)
            {
                key[1] = options[o1];

                if (keyLength < 2)                          // if in the two-letter loop, compare each and every TWO-LETTER combo
                {
                    compare(key, salt, hash);
                }
                else                                        // else allow for all three-letter combos
                {
                    for (int o2 = 0; o2 < 52; o2++)
                    {
                        key[2] = options[o2];

                        if (keyLength < 3)                                      // THREE-LETTER combos
                        {
                            compare(key, salt, hash);
                        }
                        else
                        {
                            for (int o3 = 0; o3 < 52; o3++)
                            {
                                key[3] = options[o3];

                                if (keyLength < 4)                                          // FOUR-LETTER combos
                                {
                                    compare(key, salt, hash);
                                }
                                else                                                        // FIVE-LETTER combos
                                {
                                    for (int o4 = 0; o4 < 52; o4++)
                                    {
                                        key[4] = options[o4];

                                        compare(key, salt, hash);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

// Recursively loop first through all one-letter combinations, then all two-letter combos, and so forth until all five-letter combos
void loopRecursive(int keyLength, int keyIndex, string key, string options, string salt, string hash)
{
    for (int o = 0; o < 52; o++)            // 52 possible options in {'A', 'B', ..., 'Z', 'a', 'b', ...,'z'}
    {
        key[keyIndex] = options[o];

        if (keyLength < keyIndex+1)             // if in the one-letter loop, compare each and every ONE-LETTER combo; if in the second-letter loop, compare each TWO-LETTER combo, etc.
            compare(key, salt, hash);
        else
            loopRecursive(keyLength, keyIndex+1, key, options, salt, hash);
    }
}
