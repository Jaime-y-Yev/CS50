#include <stdio.h>
#include <cs50.h>
#include <string.h>

#define _XOPEN_SOURCE
#include <unistd.h>

char *crypt();
_Bool compare();
int loopRecursive();
int loopNonRecursive();


int main(int argc, string argv[])
{
    // User may only input 2 command-line arguments
    if (argc != 3)
    {
        printf("Usage: ./crack hash mode (mode=0 for recursive, mode=1 for nonrecursive)\n");
        return 1;
    }

    string hash = argv[1];
    int mode = atoi(argv[2]);

    char salt[3] = {hash[0], hash[1], '\0'};            // salt is 2 characters + 1 for null byte
    //printf("salt = %s \n", salt);

    char options[52];                                   // options = {'A', 'B', ..., 'Z', 'a', 'b', ..., 'z'}
    for (int i = 0; i < 26; i++)
    {
        options[i] = 'A' + i;
        options[i+26] = 'a' + i;
    }
    //for (int i = 0; i < 52; i++)
        //printf("options[%i] = %c \n", i, options[i]);

    char key[6] = {'\0','\0','\0','\0','\0','\0'};      // key is 5 characters + 1 for null byte

    /* Loop recursively (mode=0) or non-recursive (mode=1) through all possible combinations,
     * first through all one-letter combinations (i=0), then through all two-letter passwords (i=1),
     * and so forth until looping finally through all five-letter passwords (i=4)
    */
    for (int i = 0; i < 5; i++)
    {
        if (mode == 0)
            loopRecursive(i, 0, key, options, salt, hash);
        else if (mode == 1)
            loopNonRecursive(i, key, options, salt, hash);
    }


    return 0;
}


_Bool compare(char key[], char salt[], char hash[])
{
    printf("key = %s \n", key);

    string encryptedPassword = crypt(key, salt);
    if (strcmp(encryptedPassword, hash) == 0)
    {
        printf("%s \n", key);
        exit(0);//return true;
    }

    return false;
}


int loopRecursive(int i, int keyIndex, string key, string options, string salt, string hash)
{
    printf("recursive \n");

    for (int k0 = 0; k0 < 52; k0++)
    {
        key[keyIndex] = options[k0];

        if (i < keyIndex+1)
        {
            if (compare(key, salt, hash))
                return 0;
        }
        else
        {
            loopRecursive(i, keyIndex+1, key, options, salt, hash);
        }
    }

    return 2;
}


int loopNonRecursive(int i, string key, string options, string salt, string hash)
{
    printf("nonrecursive \n");

    for (int k0 = 0; k0 < 52; k0++)
    {
        key[0] = options[k0];

        if (i < 1)
        {
            if (compare(key, salt, hash))
                return 0;
        }
        else
        {
            for (int k1 = 0; k1 < 52; k1++)
            {
                key[1] = options[k1];

                if (i < 2)
                {
                    if (compare(key, salt, hash))
                        return 0;
                }
                else
                {
                    for (int k2 = 0; k2 < 52; k2++)
                    {
                        key[2] = options[k2];

                        if (i < 3)
                        {
                            if (compare(key, salt, hash))
                                return 0;
                        }
                        else
                        {
                            for (int k3 = 0; k3 < 52; k3++)
                            {
                                key[3] = options[k3];

                                if (i < 4)
                                {
                                    if (compare(key, salt, hash))
                                        return 0;
                                }
                                else
                                {
                                    for (int k4 = 0; k4 < 52; k4++)
                                    {
                                        key[4] = options[k4];

                                        if (compare(key, salt, hash))
                                            return 0;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return 2;
}