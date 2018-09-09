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
bool search(int value, int values[], int n) // binary search
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
        x = 0; // to enter while loop
        // continue binary search until array size 2
        while (x == 0 && n > 2) 
        {
            // even # of array elements
            if (n % 2 == 0) 
            {
                // middle value coincides with needle
                if (values[n / 2] == value) 
                {
                    x = 1;
                }
                // middle value smaller or greater than needle
                else 
                {
                    int j = 0; // to rewrite array
                    // middle value greater than needle
                    if (values[n / 2] > value) 
                    {
                        for(int i = 0; i < n / 2; i++) // loop through left half of array
                        {
                            values[j] = values[i]; // copy left half to beginning of array
                            j++; // move to next element to be rewritten
                            n = n / 2; // limit next loop to rewritten portion of array
                        }
                    }
                    // middle value smaller than needle
                    else 
                    {
                        for(int i = (n / 2) + 1; i < n; i++) // loop through right half of array
                        {
                            values[j] = values[i]; // copy right half to beginning of array
                            j++; // move to next element to be rewritten
                            n = (n / 2) - 1; // limit next loop to rewritten portion of array
                        }
                    }
                }
            }   
            else // odd # of array elements; analogous to lines 33 - 61
            {
                if (values[(n + 1) / 2] == value)
                {
                    x = 1;
                }
                else
                {
                    int j = 0;
                    if (values[(n + 1) / 2] > value)
                    {
                        for(int i = 0; i < (n + 1) / 2; i++)
                        {
                            values[j] = values[i];
                            j++;
                            n = (n + 1) / 2;
                        }
                    }
                    else
                    {
                        for(int i = (n + 3) / 2; i < n; i++)
                        {
                            values[j] = values[i];
                            j++;
                            n = n - ((n + 3) / 2);
                        }
                    }
                }
            }      
        }
        
        // once array size reduced to 2, compare each element to needle
        if ((values[0] == value) || (values[1] == value))
        {
            x = 1;
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