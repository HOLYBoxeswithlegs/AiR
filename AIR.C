#include <dos.h>
#include <conio.h>
#include <stdio.h>
#include <math.h>

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

// Function to play a note through the PC speaker for a specified duration
void play_note(int semitone, unsigned int duration_ms) {
    // Define the base frequency (A4 = 440 Hz)
    double base_frequency = 256.0; // 256.0 is C4

    // Calculate the frequency for the given semitone
    double frequency = base_frequency * pow(2.0, semitone / 12.0);

    // Calculate the timer value for the PC speaker
    unsigned int timer_value = 1193180 / (unsigned int)frequency;

    // Send the timer value to the PC speaker
    outportb(0x43, 0xB6); // Set up the timer (channel 2, mode 3)
    outportb(0x42, (unsigned char)(timer_value & 0xFF)); // Low byte
    outportb(0x42, (unsigned char)((timer_value >> 8) & 0xFF)); // High byte

    // Turn on the speaker
    outportb(0x61, inportb(0x61) | 0x03);

    // Delay for the specified duration
    {
        unsigned int i, j;
        unsigned int loop_count = 16000; // Adjust this value for timing

        // Outer loop for milliseconds
        for (i = 0; i < duration_ms; i++) {
            // Inner loop for busy-wait
            for (j = 0; j < loop_count; j++) {
                // Do nothing, just waste time
            }
        }
    }

    // Turn off the speaker
    outportb(0x61, inportb(0x61) & 0xFC);
}

// Main function
void main() {
    // Declare all variables at the top
    int sequence[] = {C4, CS4, D4, DS4, E4, F4, FS4, G4}; // Semitones for C major scale
    unsigned int durations[] = {10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000}; // Durations in milliseconds
    int sequence_length = sizeof(sequence) / sizeof(sequence[0]);
    int i; // Loop variable

    // Play the sequence
    for (i = 0; i < sequence_length; i++) {
        play_note(sequence[i], durations[i]);
    }

    // Wait for a key press before exiting
    printf("Press any key to exit...\n");
    getch();
}
