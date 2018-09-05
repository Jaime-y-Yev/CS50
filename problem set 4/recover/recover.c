#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>


int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover card.raw\n");
        return 1;
    }

    // remember input filename
    char *infile = argv[1];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // Set pointer to the buffer
    unsigned char *buffer = (unsigned char*)malloc(512);
    if (buffer == NULL)
    {
        fprintf(stderr, "Issue with buffer pointer.\n");
        return 3;
    }


    // First file ("-01.jpg") will contain junk before first image is found
    char filename[8];                               // will contain the name "XXX.jpg\0"
    int imgCounter = -1;                            // image counter
    sprintf(filename, "%03i.jpg", imgCounter);      // create filename "-01.jpg"
    FILE *img = fopen(filename, "w");               // open the junk file "-01.jpg"

    while (fread(buffer, 1, 512, inptr) == 512)        // blocks always 512, execpt at EOF
    {
        // First 3 bytes of a new jpg are 255, 216, 255, and the MSB of the 4th byte is 1110
        if (buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            fclose(img);                                    // close the previous jpg
            imgCounter++;                                   // increase image counter
            sprintf(filename, "%03i.jpg", imgCounter);      // create the new filename
            img = fopen(filename, "w");                     // open the new jpg
        }

        // Write images "000.jpg" through "049.jph" (do not write "-01.jpg")
        if (imgCounter >= 0)
        {
            fwrite(buffer, 512, 1, img);
        }
    }

    fclose(img);                // close the last jpg
    free(buffer);               // free the alloc'd buffer

    return 0;
}