/**
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */

#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n) // linear search
{
    int x; // x = 0 (false) or x = 1 (true)

    // array size zero or negative --> false
    if (n < 1)
    {
        x = 0;
    }
    // array of size 1 at least --> begin search
    else
    {
        for (int i = 0; i < n; i++) // loop through array
        {
            if (values[i] == value) // needle found
            {
                x = 1;
                break; // avoids next element reseting x to 0
            }
            else // needle not found
            {
                x = 0;
            }
        }
    }

    if (x == 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n) // bubble sort
{
    int c = 1; // counts swaps, initially 1 to enter while loop
    // sort until no changes are counted
    while (c != 0)
    {
        c = 0; // reset counter
        // loop through adjacent pairs of elements
        for (int i = 0; i < n - 1; i++)
        {
            if (values[i] > values[i + 1]) // left value is bigger
            {
                int aux = values[i]; // copy left value
                values[i] = values[i + 1]; // left takes on right´s value
                values[i + 1] = aux; // right filled with left's original value
                c++; // swap recorded
            }
        }
    }
}
