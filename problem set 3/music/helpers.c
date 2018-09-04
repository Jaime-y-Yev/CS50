// Helper functions for music

#include <cs50.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#include "helpers.h"

// Converts a fraction formatted as X/Y to eighths
int duration(string fraction)
{
    int top = atoi(&fraction[0]);
    int bottom = atoi(&fraction[1]);

    int eighths = 8 * (top / bottom);
    return eighths;
}

// Calculates frequency (in Hz) of a note (ej. C5, G#2, Ab3)
int frequency(string note)
{
    //printf("note = %s \n", note);

    double semitones = getSemitones(note);
    //printf("semitones = %f \n", semitones);

    double exponent = -semitones/12;
    int freq = pow(2,exponent) * 440;    // f = 2^(n/12) × 440 where n = number of semitones
    return freq;


    // int octave = 4;
    // char letter[3] = {'\0', '\0', '\0'};
    // if (strlen(note) == 2)
    // {
    //     letter[0] = note[0];
    //     octave = atoi(&note[1]);
    // }
    // else if (strlen(note) == 3)
    // {
    //     letter[0] = note[0];
    //     letter[1] = note[1];
    //     octave = atoi(&note[2]);
    // }


    // int semitones = 0;





    // int indexA = 9;
    // int indexletter = 0;
    // string notes[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
    // for (int i = 0; i < 12; i++)
    // {
    //     if (strcmp(letter, notes[i]) == 0)
    //         indexletter = i;
    // }

    // _Bool aboveItsA = false;
    // _Bool higherOctave = false;
    // if (indexletter > indexA)
    //     aboveItsA = true;
    // if (octave > 4)
    //     higherOctave = true;

    // if (!higherOctave)
    // {
    //     semitones = indexA - indexletter;
    //     semitones = semitones + 12 * abs(4 - octave);
    // }
    // else if (higherOctave && !aboveitsA)
    // {
    //     semitones = indexA - indexletter;
    //     semitones = semitones + 12 * abs(4 - octave);
    // }



}

// Determines whether a string represents a rest
bool is_rest(string s)
{
    if (!strcmp(s, ""))
        return true;
    else
        return false;
}


double getSemitones(string note)
{
    string notes[] = {"C0", "C#0", "D0", "D#0", "E0", "F0", "F#0", "G0", "G#0", "A0", "A#0", "B0",
                      "C1", "C#1", "D1", "D#1", "E1", "F1", "F#1", "G1", "G#1", "A1", "A#1", "B1",
                      "C2", "C#2", "D2", "D#2", "E2", "F2", "F#2", "G2", "G#2", "A2", "A#2", "B2",
                      "C3", "C#3", "D3", "D#3", "E3", "F3", "F#3", "G3", "G#3", "A3", "A#3", "B3",
                      "C4", "C#4", "D4", "D#4", "E4", "F4", "F#4", "G4", "G#4", "A4", "A#4", "B4",
                      "C5", "C#5", "D5", "D#5", "E5", "F5", "F#5", "G5", "G#5", "A5", "A#5", "B5",
                      "C6", "C#6", "D6", "D#6", "E6", "F6", "F#6", "G6", "G#6", "A6", "A#6", "B6",
                      "C7", "C#7", "D7", "D#7", "E7", "F7", "F#7", "G7", "G#7", "A7", "A#7", "B7",
                      "C8", "C#8", "D8", "D#8", "E8", "F8", "F#8", "G8", "G#8", "A8", "A#8", "B8"};

    int indexLetter = 0;
    int indexA4 = 57;
    for (int i = 0; i < 96; i++)
    {
        if (strcmp(note, notes[i]) == 0)
        {
            indexLetter = i;
            break;
        }
    }
    //printf("indexA4 = %i \n", indexA4);
    //printf("indexletter = %i \n", indexLetter);
    //printf("indexA4 - indexLetter = %i \n", indexA4 - indexLetter);

    double semitones = indexA4 - indexLetter;
    return semitones;
}