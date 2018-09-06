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

    // Make sure all nodes are not pointing anywhere
    for (i = 0; i < HASH_MAX; i++)
        hashtable[i] = NULL;

    FILE *dictFile = fopen(dictionary, "r");

    // Looping through entire dictionary
    wordCount = 0;
    char currWord[LENGTH];
    char c;
    while ((c = fgetc(dictFile)) != EOF)
    {
        // Turn the current dictionary word into a null-terminated string by swapping the '\n' in the dictionary with '\0'
        i = 0;
        while (c != '\n')
        {
            currWord[i] = c;
            i++;
            c = fgetc(dictFile);
        }
        currWord[i] = '\0';
        wordCount++;                // each time a newline appears, add to the word counter (avoids needing a separate function to count words in the hash table)

        int hashNumber = hash(currWord);        // so the dictionary word can be stored in its proper hash table row
        int wordLength = strlen(currWord);

        // If no row exists yet for that number, start a new row
        if (hashtable[hashNumber] == NULL)
        {
            hashtable[hashNumber] = malloc(sizeof(node));   // spawn node for this new hash number (= from locker box)
            if (hashtable[hashNumber] == NULL)
                return 0;

            // Copy every character (including null terminator) into the word of the first (and only) node in this new row
            for (i = 0; i < wordLength+1; i++)
                hashtable[hashNumber]->word[i] = currWord[i];

            hashtable[hashNumber]->next = NULL;     // make sure this first node is not pointing anywhere (since it should be the only node in this new row)
        }
        // If a row already exists of this hash number, add the dictionary word to the end of that row
        else
        {
            node *current = hashtable[hashNumber];          // make current (a pointer to a node) point to the same spot as the hashtable pointer (which already points to the first node in that row)

            // Loop until the end of the row
            while (current->next != NULL)
                current = current->next;                    // move your helper pointer from one node to another until "next" field of the node is NULL (you are at the last node)

            // Tack on a new node to the end of the row
            current->next = malloc(sizeof(node));           // first, spawn a node to point to from the current last node, effectively making it now the second-to-last node (which points to the last node)
            if (hashtable[hashNumber] == NULL)
                return 0;

            current = current->next;                        // move to this newly-created last node

            // Copy every character (including null terminator) into the word of this last node
            for (i = 0; i < wordLength+1; i++)
                current->word[i] = currWord[i];

            current->next = NULL;       // make sure this last node is not pointing anywhere (otherwise it wouldn't be the last node)
        }
    }

    // You've successfully looped through every word in the dictionary and added it to the hash table so close the dictionary and exit with code 1
    fclose(dictFile);
    return 1;
}


// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return wordCount;           // no separate function neeeded since load() counted the words as it was loading them and saved the count in this global variable
}


// Unloads dictionary from (ie, frees up the hash table), returning true if successful else false
bool unload(void)
{
    // Loop through every row and, in turn, every node in each row
    for (int i = 0; i < HASH_MAX; i++)
    {
        if (hashtable[i] != NULL)               // ignore empty rows
        {
            node *current = hashtable[i];           // declare a helper pointer and make it point to beginning of the current row
            node *next = NULL;                      // temporary pointer to not lose track of current node
            //node *prev = NULL;                      // temporary pointer to not lose track of current node

            // Loop to the end of the row
            while (current->next != NULL)
            {
                next = current->next;               // keep track of where the next node is
                free(current);                      // free the current node
                current = next;                     // move current to the next node
            }
            free(current);                          // free entire node that current points to
        }

        // You've reached and emptied the last row so exit with code 1
        if (i == HASH_MAX-1)
            return 1;
    }

    return 0;   // something must have gone wrong
}
