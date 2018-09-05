// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

#define HASH_MAX 30

node *hashtable[HASH_MAX];      // Create hashtable (array of pointers to nodes NOT data). Reserve space in memory for future addresses to datam

int index;
int wordCount;
int *wordCountPtr = &wordCount;


// Simple hash function from the example in walkthrough video
unsigned int hash(const char *str)
{
    int sum = 0;
    for (int j = 0; str[j] != '\0'; j++)
        sum = str[j] + sum;

    return abs(sum % HASH_MAX);
}


// Returns true if word is in dictionary else false
bool check(const char *word)
{
    char wordLower[strlen(word)+1];
    for (index = 0; index < strlen(word); index++)
        wordLower[index] = tolower(word[index]);
    wordLower[strlen(word)] = '\0';


    int hashNumber = hash(wordLower);

    if (hashtable[hashNumber] == NULL)
    {
        return 0;
    }
    else
    {
        node *current = hashtable[hashNumber]; // Declare pointer current

        int found;
        while (current->next != NULL)
        {
            found = strcmp(current->word, wordLower);
            if (found == 0)
                return 1;

            current = current->next;
        }

        found = strcmp(current->word, wordLower);
        if (found == 0)
            return 1;
    }

    return 0;
}


// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    for (index = 0; index < HASH_MAX; index++)
        hashtable[index] = NULL;

    FILE *dictFile = fopen(dictionary, "r");

    *wordCountPtr = 0;
    char c;
    char currWord[LENGTH];
    while ((c = fgetc(dictFile)) != EOF) // Looping through entire file
    {
        index = 0;
        while (c != '\n')
        {
            currWord[index] = c;
            index++;
            c = fgetc(dictFile);
        }
        currWord[index] = '\0';


       (*wordCountPtr)++;

        int hashNumber = hash(currWord);

        if (hashtable[hashNumber] == NULL)
        {
            hashtable[hashNumber] = malloc(sizeof(node));   // Spawn node from hash number ( = from locker box)
            if (hashtable[hashNumber] == NULL)
                return 0;

            for (index = 0; index < strlen(currWord)+1; index++)
                hashtable[hashNumber]->word[index] = currWord[index];


            hashtable[hashNumber]->next = NULL;
        }
        else
        {
            node *current = NULL;
            current = hashtable[hashNumber];                // Make current (pointer) point to the same spot as hashtable pointer

            while (current->next != NULL)                   // If you are not at end of list, move to end of list
                current = current->next;                    // Move your helper pointer from one node to another until "next" struct field is NULL (you are at end)

            // Access the struct and add info
            current->next = malloc(sizeof(node));           // Spawn node from pointer next within struct. You now have a new node with value and next field
            if (hashtable[hashNumber] == NULL)
                return 0;

            current = current->next;                        // Move helper to the new next

            for (index = 0; index < strlen(currWord)+1; index++)
                current->word[index] = currWord[index];

            current->next = NULL;
        }
    }

    fclose(dictFile);

    return 1;
}


// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return wordCount;
}


// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (index = 0; index < HASH_MAX; index++)
    {
        if (hashtable[index] != NULL)
        {
            node *current = hashtable[index];           // Declare pointer current and make it point to beginning of list
            node *prev = NULL;

            while (current->next != NULL)
            {
                prev = current;                 // Store previous position of current
                current = current->next;        // Move current to the next position after
                free(prev);                     // Free entire node that current points to
            }

            free(current);                      // Free entire node that current points to
        }

        if (index == HASH_MAX-1)
            return 1;
    }

    return 0;
}
