#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>

#include "helpers.h"

const int MAX_HAYSTACK_SIZE = 65536;      // maximum amount of "hay"


int main(int argc, string argv[])
{
    // Ensure proper usage
    if (argc != 2)
    {
        printf("Usage: ./find needle\n");
        return -1;
    }

    int needle = atoi(argv[1]);     // remember needle


    // Fill haystack with ints
    int haystack[MAX_HAYSTACK_SIZE];
    for (int i = 0; i < MAX_HAYSTACK_SIZE; i++)
        haystack[i] =  get_int();


    // Sort the haystack
    sort(haystack, MAX_HAYSTACK_SIZE);

    // Try to find the needle in the haystack
    bool found = search(needle, haystack, MAX_HAYSTACK_SIZE);
    if (found)
    {
        printf("\nFound needle in haystack!\n\n");
        return 0;
    }
    else
    {
        printf("\nDidn't find needle in haystack.\n\n");
        return 1;
    }
}
