#include <stdio.h>
#include <cs50.h>
#include <string.h>

#define _XOPEN_SOURCE
#include <unistd.h>

char *crypt();
_Bool compare();


int main(int argc, string argv[])
{
    // User may only input 2 command-line arguments
    if (argc != 2)
    {
        printf("Usage: ./crack hash\n");
        return 1;
    }

    string hash = argv[1];

    char salt[3];       // salt is 2 characters + 1 for null byte
    salt[0] = hash[0];
    salt[1] = hash[1];
    salt[2] = '\0';
    printf("salt = %s \n", salt);

    char options[52];
    for (int i = 0; i < 26; i++)
    {
        options[i] = 'A' + i;
        options[i+26] = 'a' + i;
    }
    //for (int i = 0; i < 52; i++)
        //printf("options[%i] = %c \n", i, options[i]);

    char key[6] = {'\0','\0','\0','\0','\0','\0'};        // key is 5 characters + 1 for null byte

    _Bool found = false;
    for (int i = 0; i < 5; i++)
    {
        for (int k0 = 0; k0 < 52; k0++)
        {
            key[0] = options[k0];

            if (i < 1)
            {
                found = compare(key, salt, hash);

                if (found)
                    return 0;
            }
            else
            {
                for (int k1 = 0; k1 < 52; k1++)
                {
                    key[1] = options[k1];

                    if (i < 2)
                    {
                        found = compare(key, salt, hash);

                        if (found)
                            return 0;
                    }
                    else
                    {
                        for (int k2 = 0; k2 < 52; k2++)
                        {
                            key[2] = options[k2];

                            if (i < 3)
                            {
                                found = compare(key, salt, hash);

                                if (found)
                                    return 0;
                            }
                            else
                            {
                                for (int k3 = 0; k3 < 52; k3++)
                                {
                                    key[3] = options[k3];

                                    if (i < 4)
                                    {
                                        found = compare(key, salt, hash);

                                        if (found)
                                            return 0;
                                    }
                                    else
                                    {
                                        for (int k4 = 0; k4 < 52; k4++)
                                        {
                                            key[4] = options[k4];

                                            found = compare(key, salt, hash);

                                            if (found)
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
        return true;
    }

    return false;
}