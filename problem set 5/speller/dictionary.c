// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

#define HASH_MAX 30

node *hashtable[HASH_MAX];      // create hashtable (= array of pointers to nodes, NOT data), ie, reserve space in memory for future addresses to data

int wordCount;      // global so that load() can count the words while it loads the dictionary and not require size() to redo the work


// Simple hash function from the example in walkthrough video
unsigned int hash(const char *str)
{
    int sum = 0;
    for (int i = 0; str[i] != '\0'; i++)
        sum = str[i] + sum;

    return abs(sum % HASH_MAX);
}


// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Convert every word to all lowercase
    int wordLength = strlen(word);
    char wordLower[wordLength+1];           // +1 for null termination byte (ie. turn wordLower into a string)
    for (int i = 0; i < wordLength; i++)
        wordLower[i] = tolower(word[i]);
    wordLower[wordLength] = '\0';


    int hashNumber = hash(wordLower);       // calculate hash number for the current word

    if (hashtable[hashNumber] == NULL)      // the word obviously doesn't exist in the dictionary if their is no hash number for it
    {
        return 0;
    }
    else
    {
        node *current = hashtable[hashNumber];          // set a pointer to the row in which the word would be stored (if it exists in the dictionary)

        // Search through all the nodes in this row for the current word
        while (current != NULL)
        {
            if (strcmp(current->word, wordLower) == 0)      // strcmp() = 0 means the word was found
                return 1;

            current = current->next;
        }
    }

    return 0;           // if it got this far, the current word is not in the dictionary
}


// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    int i = 0;

    for (i = 0; i < HASH_MAX; i++)
        hashtable[i] = NULL;

    FILE *dictFile = fopen(dictionary, "r");

    wordCount = 0;
    char c;
    char currWord[LENGTH];
    while ((c = fgetc(dictFile)) != EOF) // Looping through entire file
    {
        i = 0;
        while (c != '\n')
        {
            currWord[i] = c;
            i++;
            c = fgetc(dictFile);
        }
        currWord[i] = '\0';


        wordCount++;

        int hashNumber = hash(currWord);

        if (hashtable[hashNumber] == NULL)
        {
            hashtable[hashNumber] = malloc(sizeof(node));   // Spawn node from hash number ( = from locker box)
            if (hashtable[hashNumber] == NULL)
                return 0;

            for (i = 0; i < strlen(currWord)+1; i++)
                hashtable[hashNumber]->word[i] = currWord[i];


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

            for (i = 0; i < strlen(currWord)+1; i++)
                current->word[i] = currWord[i];

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
    for (int i = 0; i < HASH_MAX; i++)
    {
        if (hashtable[i] != NULL)
        {
            node *current = hashtable[i];           // Declare pointer current and make it point to beginning of list
            node *prev = NULL;

            while (current->next != NULL)
            {
                prev = current;                 // Store previous position of current
                current = current->next;        // Move current to the next position after
                free(prev);                     // Free entire node that current points to
            }

            free(current);                      // Free entire node that current points to
        }

        if (i == HASH_MAX-1)
            return 1;
    }

    return 0;
}
