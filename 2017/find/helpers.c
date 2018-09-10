#include <cs50.h>
#include <stdio.h>

#include "helpers.h"

// Bubble sort in ascending order
void sort(int values[], int size)
{
    int changes = 1; // counts swaps, initially 1 to enter while loop

    // Sort until no changes are detected
    while (changes != 0)
    {
        changes = 0; // reset counter

        // Loop through adjacent pairs of elements recording if any changes have occured
        for (int i = 0; i < size - 1; i++)
        {
            // If the current value is larger than the next value, swap them
            if (values[i] > values[i + 1])
            {
                int aux = values[i];            // copy left's original value
                values[i] = values[i + 1];      // left takes on right's value
                values[i + 1] = aux;            // right filled with left's original value
                changes++;                      // make a note that a change has ocurred
            }
        }
    }
}


// Binary search that returns true if value is found in values[n]
bool search(int value, int values[], int n)
{
    // Return false for arrays of size zero or negative
    if (n < 1)
        return false;

    // Original start, end, and size of the array
    int start = 0;
    int end = n - 1;
    int size = n;

    // Continue binary search until array of size 2
    while (size > 2)
    {
        // Figure out middle position
        int middle;
        if (size % 2 == 0)                  // even # of array elements
            middle = size / 2;
        else                                // odd # of array elements
            middle = (size + 1) / 2;


        // Middle value coincides with needle
        if (values[middle] == value)
            return true;

        // Middle value smaller than needle, meaning that if it exists, it will be in the left half
        if (value < values[middle])
        {
            start = 0;
            end = middle;
        }
        // Middle value greater than needle, meaning that if it exists, it will be in the right half
        else if (values[middle] < value)
        {
            start = middle;
            end = middle + size;
        }


        // Overwrite array with potential half
        int j = 0;  // to overwrite array
        for (int i = start; i <= end; i++)          // copy potential array to beginning of the previous array
        {
            values[j] = values[i];
            j++;                                    // move to next element to be rewritten
        }

        size = size - middle;           // calculate new size so next loop can distinguish between even and odd
    }


    // Once array size reduced to 2, compare each element to needle
    if ((values[0] == value) || (values[1] == value))
        return true;


    return false;
}


// Linear search that returns true if value is found in values[n]
// bool search(int value, int values[], int n)
// {
//     // Return false for arrays of size zero or negative
//     if (n < 1)
//         return false;

//     // Loop through array comparing each value
//     for (int i = 0; i < n; i++)
//     {
//         if (values[i] == value)         // needle found
//             return 1;
//     }

//     return false;                       // needle not found
// }
