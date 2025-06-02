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
#define REST -100

#define MAX_NOTES 32767
#define NOTE_COUNT 37 // Number of defined notes including REST
#define MAX_VISIBLE 20 // Maximum number of notes visible on screen

// Array of note names for display and cycling
const char *note_names[] = {
    "C3", "C#3", "D3", "D#3", "E3", "F3", "F#3", "G3", "G#3", "A3", "A#3", "B3",
    "C4", "C#4", "D4", "D#4", "E4", "F4", "F#4", "G4", "G#4", "A4", "A#4", "B4",
    "C5", "C#5", "D5", "D#5", "E5", "F5", "F#5", "G5", "G#5", "A5", "A#5", "B5",
    "C6", "REST"
};

// Corresponding semitone values
const int note_values[] = {
    C3, CS3, D3, DS3, E3, F3, FS3, G3, GS3, A3, AS3, B3,
    C4, CS4, D4, DS4, E4, F4, FS4, G4, GS4, A4, AS4, B4,
    C5, CS5, D5, DS5, E5, F5, FS5, G5, GS5, A5, AS5, B5,
    C6, REST
};

// Function to play a note or a rest
void play_note(int semitone, unsigned int duration_ms) {
    if (semitone == REST) {
        unsigned int i, j;
        unsigned int loop_count = 40000;
        for (i = 0; i < duration_ms; i++) {
            for (j = 0; j < loop_count; j++) {}
        }
    } else {
        double base_frequency = 261.6256;
        double frequency = base_frequency * pow(2.0, semitone / 12.0);
        unsigned int timer_value = 1193180 / (unsigned int)frequency;

        outportb(0x43, 0xB6);
        outportb(0x42, (unsigned char)(timer_value & 0xFF));
        outportb(0x42, (unsigned char)((timer_value >> 8) & 0xFF));
        outportb(0x61, inportb(0x61) | 0x03);

        unsigned int i, j;
        unsigned int loop_count = 20000;
        for (i = 0; i < duration_ms; i++) {
            for (j = 0; j < loop_count; j++) {}
        }

        outportb(0x61, inportb(0x61) & 0xFC);
    }
}

// Function to display the song spreadsheet with scrolling
void display_song(int sequence[], unsigned int durations[], int length, int cursor_row, int cursor_col, int display_start) {
    clrscr();
    printf("00     NOTE     LENGTH\n");
    int display_end = display_start + MAX_VISIBLE;
    if (display_end > length) display_end = length;

    for (int i = display_start; i < display_end; i++) {
        printf("%02d     %-4s    %u", i + 1, note_names[sequence[i]], durations[i]);
        if (i == cursor_row) {
            if (cursor_col == 0) printf("  <- NOTE");
            else printf("  <- LENGTH");
        }
        printf("\n");
    }
    printf("\nRead the README for instructions!\n");
}

// Function to save the song to song.txt
void save_song(int sequence[], unsigned int durations[], int length) {
    FILE *file = fopen("song.txt", "w");
    if (!file) {
        printf("\nError: Could not save to song.txt\n");
        putch(7);
        delay(1000);
        return;
    }

    fprintf(file, "$");
    for (int i = 0; i < length; i++) {
        fprintf(file, "%s", note_names[sequence[i]]);
        if (i < length - 1) fprintf(file, ", ");
    }
    fprintf(file, "$\n");

    fprintf(file, "&");
    for (int i = 0; i < length; i++) {
        fprintf(file, "%u", durations[i]);
        if (i < length - 1) fprintf(file, ", ");
    }
    fprintf(file, "&\n");

    fclose(file);
    printf("\nSong saved to song.txt\n");
    play_note(-12, 75);
    play_note(-13, 75);
    play_note(-14, 75);
    delay(250);
}

// Function to trim whitespace from a string
void trim(char *str) {
    int start = 0, end = strlen(str) - 1;
    while (str[start] == ' ' || str[start] == '\t') start++;
    while (end >= 0 && (str[end] == ' ' || str[end] == '\t' || str[end] == ',' || str[end] == '\n')) end--;
    if (start > end) {
        str[0] = '\0';
        return;
    }
    memmove(str, str + start, end - start + 1);
    str[end - start + 1] = '\0';
}

// Function to load the song from song.txt
int load_song(int sequence[], unsigned int durations[], int *length) {
    FILE *file = fopen("song.txt", "r");
    if (!file) {
        display_song(sequence, durations, *length, 0, 0, 0); // Redraw current screen
        printf("\nError: Could not open song.txt");
        putch(7);
        delay(1000);
        return 0;
    }

    char line[1024];
    int note_count = 0;

    // Read notes line
    if (!fgets(line, sizeof(line), file) || line[0] != '$') {
        display_song(sequence, durations, *length, 0, 0, 0);
        printf("\nError: Invalid or missing notes line in song.txt");
        fclose(file);
        putch(7);
        delay(1000);
        return 0;
    }

    char *token = strtok(line + 1, "$,");
    if (!token || strlen(token) == 0) {
        display_song(sequence, durations, *length, 0, 0, 0);
        printf("\nError: No notes found between $...$");
        fclose(file);
        putch(7);
        delay(1000);
        return 0;
    }

    while (token && note_count < MAX_NOTES) {
        trim(token);
        if (strlen(token) == 0) {
            token = strtok(NULL, "$,");
            continue;
        }

        int found = 0;
        for (int i = 0; i < NOTE_COUNT; i++) {
            if (strcmp(token, note_names[i]) == 0) {
                sequence[note_count++] = i;
                found = 1;
                break;
            }
        }
        if (!found) {
            display_song(sequence, durations, *length, 0, 0, 0);
            printf("\nError: Unknown note '%s' in song.txt", token);
            fclose(file);
            putch(7);
            delay(2000);
            return 0;
        }
        token = strtok(NULL, "$,");
    }

    // Read durations line
    if (!fgets(line, sizeof(line), file) || line[0] != '&') {
        display_song(sequence, durations, *length, 0, 0, 0);
        printf("\nError: Invalid or missing durations line in song.txt");
        fclose(file);
        putch(7);
        delay(1000);
        return 0;
    }

    int dur_count = 0;
    token = strtok(line + 1, "&,");
    if (!token || strlen(token) == 0) {
        display_song(sequence, durations, *length, 0, 0, 0);
        printf("\nError: No durations found between &...&");
        fclose(file);
        putch(7);
        delay(1000);
        return 0;
    }

    while (token && dur_count < note_count) {
        trim(token);
        if (strlen(token) == 0) {
            token = strtok(NULL, "&,");
            continue;
        }
        durations[dur_count++] = atoi(token);
        token = strtok(NULL, "&,");
    }

    if (dur_count != note_count) {
        display_song(sequence, durations, *length, 0, 0, 0);
        printf("\nError: Mismatch between notes (%d) and durations (%d)", note_count, dur_count);
        fclose(file);
        putch(7);
        delay(1000);
        return 0;
    }

    fclose(file);
    *length = note_count;
    display_song(sequence, durations, *length, 0, 0, 0);
    printf("\nSong loaded from song.txt");
    delay(1000);
    return 1;
}

// Main function
int main() {
    int sequence[MAX_NOTES] = {0}; // Semitone indices into note_values
    unsigned int durations[MAX_NOTES] = {0};
    int sequence_length = 5; // Initial length
    int cursor_row = 0; // Current row
    int cursor_col = 0; // 0 for NOTE, 1 for LENGTH
    int display_start = 0; // Start of visible portion

    // Initial song data
    sequence[0] = 12; // C4
    durations[0] = 1000;
    sequence[1] = 13; // CS4
    durations[1] = 500;
    sequence[2] = 14; // D4
    durations[2] = 2000;
    sequence[3] = 12; // REST
    durations[3] = 500;
    sequence[4] = 15; // DS4
    durations[4] = 2000;

    display_song(sequence, durations, sequence_length, cursor_row, cursor_col, display_start);

    while (1) {
        if (kbhit()) {
            int key = getch();
            if (key == 0 || key == 224) { // Extended key (arrows)
                key = getch();
                switch (key) {
                    case 72: // Up arrow
                        if (cursor_col == 0 && cursor_row > 0) {
                            cursor_row--;
                            play_note(1, 1);
                            if (cursor_row < display_start) display_start--;
                        } else if (cursor_col == 1 && durations[cursor_row] < 65535 - 250) {
                            durations[cursor_row] += 250; // Increase LENGTH
                        }
                        break;
                    case 80: // Down arrow
                        if (cursor_col == 0 && cursor_row < sequence_length - 1) {
                            cursor_row++;
                            play_note(1, 1);
                            if (cursor_row >= display_start + MAX_VISIBLE) display_start++;
                        } else if (cursor_col == 1 && durations[cursor_row] > 250) {
                            durations[cursor_row] -= 250; // Decrease LENGTH
                        }
                        break;
                    case 75: // Left arrow
                        if (cursor_col == 0) sequence[cursor_row] = (sequence[cursor_row] - 1 + NOTE_COUNT) % NOTE_COUNT; // Previous note
                        play_note(1, 1);
                        break;
                    case 77: // Right arrow
                        if (cursor_col == 0) sequence[cursor_row] = (sequence[cursor_row] + 1) % NOTE_COUNT; // Next note
                        play_note(1, 1);
                        break;
                }
            } else {
                switch (key) {
                    case 9: // Tab key
                        cursor_col = (cursor_col + 1) % 2; // Switch between NOTE and LENGTH
                        play_note(1, 50);
                        play_note(3, 50);
                        play_note(5, 50);
                        play_note(7, 50);
                        break;
                    case 'p':
                    case 'P':
                        for (int i = 0; i < sequence_length; i++) {
                            play_note(note_values[sequence[i]], durations[i]);
                        }
                        break;
                    case 't':
                    case 'T':
                        if (sequence_length < MAX_NOTES) {
                            sequence[sequence_length] = 12; // C4 (index 12 in note_values)
                            durations[sequence_length] = 250;
                            sequence_length++;
                            cursor_row = sequence_length - 1; // Move cursor to new line
                            cursor_col = 0; // Start on NOTE column
                            play_note(-12, 75);
                            if (cursor_row >= display_start + MAX_VISIBLE) display_start = cursor_row - MAX_VISIBLE + 1;
                        } else {
                            printf("\nMax notes reached (%d)...... Dude.. Just how?\n", MAX_NOTES);
                            putch(7);
                            delay(250);
                        }
                        break;
                    case 'r':
                    case 'R':
                        if (sequence_length > 0) { // Prevent negative sequence_length
                            sequence_length--; // Remove the last note
                            cursor_row = sequence_length - 1; // Move cursor to the new last note
                            if (cursor_row < 0) cursor_row = 0; // Ensure cursor_row doesn't go negative
                            cursor_col = 0; // Reset to NOTE column (consistent with 'T' case)
                            play_note(-12, 75); // Play feedback note
                            // Adjust display to keep cursor visible
                            if (cursor_row < display_start) {
                                display_start = cursor_row; // Scroll up if cursor is above visible area
                            } else if (cursor_row >= display_start + MAX_VISIBLE) {
                                display_start = cursor_row - MAX_VISIBLE + 1; // Scroll down if needed
                            }
                        } else {
                            // Optional: Provide feedback when sequence is empty
                            printf("\nNo notes to remove!\n");
                            putch(7); // Beep
                            delay(250);
                        }
                        break;
                    case 's':
                    case 'S':
                        save_song(sequence, durations, sequence_length);
                        break;
                    case 'l':
                    case 'L':
                        load_song(sequence, durations, &sequence_length);
                        break;
                    case 'q':
                    case 'Q':
                        clrscr();
                        printf("AiR for MS-DOS by ZLH\n\n");
                        return 0;
                }
            }
            display_song(sequence, durations, sequence_length, cursor_row, cursor_col, display_start);
        }
    }
    return 0;
}
