#include <dos.h>
#include <conio.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#define C3 -12
#define CS3 -11
#define D3 -10
#define DS3 -9
#define E3 -8
#define F3 -7
#define FS3 -6
#define G3 -5
#define GS3 -4
#define A3 -3
#define AS3 -2
#define B3 -1
#define C4 0
#define CS4 1
#define D4 2
#define DS4 3
#define E4 4
#define F4 5
#define FS4 6
#define G4 7
#define GS4 8
#define A4 9
#define AS4 10
#define B4 11
#define C5 12
#define CS5 13
#define D5 14
#define DS5 15
#define E5 16
#define F5 17
#define FS5 18
#define G5 19
#define GS5 20
#define A5 21
#define AS5 22
#define B5 23
#define C6 24
#define REST -100 // Define a special value for a rest (pause)

// Function to play a note or a rest through the PC speaker for a specified duration
void play_note(int semitone, unsigned int duration_ms) {
    if (semitone == REST) {
        unsigned int i, j;
        unsigned int loop_count = 40000;
        for (i = 0; i < duration_ms; i++) {
            for (j = 0; j < loop_count; j++) {
                // Do nothing, just waste time
            }
        }
    } else {
        double base_frequency = 261.6256;
        double frequency = base_frequency * pow(2.0, semitone / 12.0);
        unsigned int timer_value = 1193180 / (unsigned int)frequency;

        outportb(0x43, 0xB6);
        outportb(0x42, (unsigned char)(timer_value & 0xFF));
        outportb(0x42, (unsigned char)((timer_value >> 8) & 0xFF));
        outportb(0x61, inportb(0x61) | 0x03);

        {
            unsigned int i, j;
            unsigned int loop_count = 20000;
            for (i = 0; i < duration_ms; i++) {
                for (j = 0; j < loop_count; j++) {
                    // Do nothing, just waste time
                }
            }
        }

        outportb(0x61, inportb(0x61) & 0xFC);
    }
}

// Function to convert note string to semitone value
int note_to_semitone(char *note) {
    if (strcmp(note, "REST") == 0) return REST;
    if (strcmp(note, "C3") == 0) return C3;
    if (strcmp(note, "CS3") == 0) return CS3;
    if (strcmp(note, "D3") == 0) return D3;
    if (strcmp(note, "DS3") == 0) return DS3;
    if (strcmp(note, "E3") == 0) return E3;
    if (strcmp(note, "F3") == 0) return F3;
    if (strcmp(note, "FS3") == 0) return FS3;
    if (strcmp(note, "G3") == 0) return G3;
    if (strcmp(note, "GS3") == 0) return GS3;
    if (strcmp(note, "A3") == 0) return A3;
    if (strcmp(note, "AS3") == 0) return AS3;
    if (strcmp(note, "B3") == 0) return B3;
    if (strcmp(note, "C4") == 0) return C4;
    if (strcmp(note, "CS4") == 0) return CS4;
    if (strcmp(note, "D4") == 0) return D4;
    if (strcmp(note, "DS4") == 0) return DS4;
    if (strcmp(note, "E4") == 0) return E4;
    if (strcmp(note, "F4") == 0) return F4;
    if (strcmp(note, "FS4") == 0) return FS4;
    if (strcmp(note, "G4") == 0) return G4;
    if (strcmp(note, "GS4") == 0) return GS4;
    if (strcmp(note, "A4") == 0) return A4;
    if (strcmp(note, "AS4") == 0) return AS4;
    if (strcmp(note, "B4") == 0) return B4;
    if (strcmp(note, "C5") == 0) return C5;
    if (strcmp(note, "CS5") == 0) return CS5;
    if (strcmp(note, "D5") == 0) return D5;
    if (strcmp(note, "DS5") == 0) return DS5;
    if (strcmp(note, "E5") == 0) return E5;
    if (strcmp(note, "F5") == 0) return F5;
    if (strcmp(note, "FS5") == 0) return FS5;
    if (strcmp(note, "G5") == 0) return G5;
    if (strcmp(note, "GS5") == 0) return GS5;
    if (strcmp(note, "A5") == 0) return A5;
    if (strcmp(note, "AS5") == 0) return AS5;
    if (strcmp(note, "B5") == 0) return B5;
    if (strcmp(note, "C6") == 0) return C6;
    return REST; // Default to rest if note is unrecognized
}

// Main function
int main() {
    FILE *file = fopen("song.txt", "r");
    if (!file) {
        printf("Error: Could not open song.txt\n");
        printf("Press any key to exit...\n");
        getch();
        return 1;
    }

    int sequence[100]; // Max 100 notes (adjust as needed)
    unsigned int durations[100];
    int sequence_length = 0;

    char line[256];
    char *token;

    // Read notes line
    if (fgets(line, sizeof(line), file)) {
        if (line[0] != '$') {
            printf("Error: Notes line must start with '$'\n");
            fclose(file);
            getch();
            return 1;
        }
        token = strtok(line + 1, "$, "); // Skip '$' and split by ',', '$', or space
        while (token && sequence_length < 100) {
            sequence[sequence_length++] = note_to_semitone(token);
            token = strtok(NULL, "$, ");
        }
    }

    // Read durations line
    if (fgets(line, sizeof(line), file)) {
        if (line[0] != '&') {
            printf("Error: Durations line must start with '&'\n");
            fclose(file);
            getch();
            return 1;
        }
        int i = 0;
        token = strtok(line + 1, "&, "); // Skip '&' and split by '&', ',', or space
        while (token && i < sequence_length) {
            durations[i++] = atoi(token);
            token = strtok(NULL, "&, ");
        }
        if (i != sequence_length) {
            printf("Error: Number of durations (%d) does not match number of notes (%d)\n", i, sequence_length);
            fclose(file);
            getch();
            return 1;
        }
    }

    fclose(file);

    // Play the sequence
    for (int i = 0; i < sequence_length; i++) {
        play_note(sequence[i], durations[i]);
    }

    printf("Press any key to exit...\n");
    getch();
    return 0;
}
