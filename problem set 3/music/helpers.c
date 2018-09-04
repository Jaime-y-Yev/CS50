// Helper functions for music

#include <cs50.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#include "helpers.h"
// Cannot include these declarations in helpers.h because check50 uses its own version (source: https://cs50.stackexchange.com/questions/27014/pset3-find-check50-frown-helpers-c-compiles-expected-exit-code-0-not-1)
double getSemitonesBrute(string note);  // Determines the number of semitones between note and A4 by looping through all possible hardcoded notes
double getSemitones(string note);       // Determines the number of semitones between note and A4 by looping only through all the letters and then adjusting for the octave


// Converts a fraction formatted as X/Y to eighths
int duration(string fraction)
{
    double top = atoi(&fraction[0]);
    double bottom = atoi(&fraction[2]);

    double eighths = 8 * (top / bottom);
    return eighths;
}

// Calculates frequency (in Hz) of a note (eg. C5, G#2, Ab3)
int frequency(string note)
{
    double semitones = getSemitones(note);
    //double semitones = getSemitonesBrute(note);

    // The frequency f = 2^(n/12) × 440. where n = number of semitones
    double exponent = -semitones/12;            // I suppose the way the semitones are calculated requires that this exponent be negative (?)
    int freq = round(pow(2,exponent) * 440);
    return freq;
}

// Determines whether a string represents a rest
bool is_rest(string s)
{
    if (!strcmp(s, ""))
        return true;
    else
        return false;
}

// Determines the number of semitones between note and A4 by looping through all possible hardcoded notes
double getSemitonesBrute(string note)
{
    // All possible notes between C0 and B8
    string notes[] = {"C0", "C#0", "D0", "D#0", "E0", "F0", "F#0", "G0", "G#0", "A0", "A#0", "B0",
                      "C1", "C#1", "D1", "D#1", "E1", "F1", "F#1", "G1", "G#1", "A1", "A#1", "B1",
                      "C2", "C#2", "D2", "D#2", "E2", "F2", "F#2", "G2", "G#2", "A2", "A#2", "B2",
                      "C3", "C#3", "D3", "D#3", "E3", "F3", "F#3", "G3", "G#3", "A3", "A#3", "B3",
                      "C4", "C#4", "D4", "D#4", "E4", "F4", "F#4", "G4", "G#4", "A4", "A#4", "B4",
                      "C5", "C#5", "D5", "D#5", "E5", "F5", "F#5", "G5", "G#5", "A5", "A#5", "B5",
                      "C6", "C#6", "D6", "D#6", "E6", "F6", "F#6", "G6", "G#6", "A6", "A#6", "B6",
                      "C7", "C#7", "D7", "D#7", "E7", "F7", "F#7", "G7", "G#7", "A7", "A#7", "B7",
                      "C8", "C#8", "D8", "D#8", "E8", "F8", "F#8", "G8", "G#8", "A8", "A#8", "B8"};

    // Loop through all possible notes until matching with note, but first ensure any flats are converted to their equivalent sharps
    int indexA4 = 57;       // A4 always appears at notes[57]
    int indexNote = 0;      // the position in notes[] at which note appears
    for (int i = 0; i < 108; i++)
    {
        // Convert any flats to their equivalent sharps (eg. Gb3 = A#3)
        if (note[1] == 'b')
        {
            note[1] = '#';

            if (note[0] == 'B')
                note[0] = 'A';
            else if (note[0] == 'A')
                note[0] = 'G';
            else if (note[0] == 'G')
                note[0] = 'F';
            else if (note[0] == 'E')
                note[0] = 'D';
            else if (note[0] == 'D')
                note[0] = 'C';
        }

        // Find the index at which note appears in notes[]
        if (strcmp(note, notes[i]) == 0)
        {
            indexNote = i;
            break;
        }
    }

    double semitones = indexA4 - indexNote;     // the number of semitones is the difference in positions within notes[]
    return semitones;
}

// Determines the number of semitones between note and A4 by looping only through all the letters and then adjusting for the octave
double getSemitones(string note)
{
    char letter[3] = {'\0', '\0', '\0'};    // eg. if note="B2" then letter="B", if note="C#6" then letter="C#""
    int octave;                             // eg. if D#4 then octave=4, if Gb3 then octave=3
    if (strlen(note) == 2)                          // eg. if note="B2", then letter="B" and octave=2
    {
        letter[0] = note[0];
        octave = atoi(&note[1]);
    }
    else if (strlen(note) == 3)                     // eg. note="C#6", then letter="C#" and octave=6
    {
        letter[0] = note[0];
        letter[1] = note[1];
        octave = atoi(&note[2]);
    }

    // All possible letters between C and B, ignoring the octave
    string letters[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};

    // Loop through all the letters until matching with letter, but first ensure any flats are converted to their equivalent sharps
    int indexA = 9;         // A always appears at letters[9]
    int indexletter = 0;    // the position in notes[] at which letter appears
    for (int i = 0; i < 12; i++)
    {
        if (letter[1] == 'b')
        {
            // Convert any flats to their equivalent sharps (eg. Gb3 = A#3)
            letter[1] = '#';

            if (letter[0] == 'B')
                letter[0] = 'A';
            else if (letter[0] == 'A')
                letter[0] = 'G';
            else if (letter[0] == 'G')
                letter[0] = 'F';
            else if (letter[0] == 'E')
                letter[0] = 'D';
            else if (letter[0] == 'D')
                letter[0] = 'C';
        }

        // Find the index at which letter appears in letters[]
        if (strcmp(letter, letters[i]) == 0)
            indexletter = i;
    }

    // The number of semitones is the difference in positions within letters[], but the octave adjustment depends on the range (C0-A4) vs (A#4-C8)
    double semitones = indexA - indexletter;
    if (octave > 4 || strcmp(note, "A#4") == 0 || strcmp(note, "B4") == 0)      // for the range above A4 (A#4-C8)
        semitones = semitones - 12 * abs(4 - octave);
    else                                                                        // for range below and including A4 (C0-A4)
        semitones = semitones + 12 * abs(4 - octave);
    return semitones;
}