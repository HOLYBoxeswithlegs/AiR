# AiR
AiR is a really terrible attempt at making a tracker program for MS-DOS, but I suck at using the computer so I gave up on the project.

It was meant to have a really simple GUI and simple keybinds to make it so you didn't have to learn the program before making music in it.
AiR uses the PC Speaker to make tones. (THATS IT)

You need to have the programs in the DOS directory set in your path variable for AiR to work.

The first version is just the C Major scale, but the next version adds rest notes..... I also programmed some notes of Risen by Terry A Davis.

^^^^^^^^^^^^^^^

THIS STUFF UP HERE IS FOR THE OLDER VERSIONS. THE LATEST ADDITIONS WILL BE AT THE BOTTOM OF THIS SECTION.

NEW VERSION!!!!

I added text file support which means you don't need to compile from source to make the music...... except for the fact that the file name can only be song.txt.
The notes have to be enclosed with an American dollar sign and the note length has to be enclosed with an ampersand. I tried to make it as simple as possible, it's still complicated to match the notes to the note length.

It could also be that I suck at reading.

GUI UPDATE!!!!!!!!

Now you don't need to use clunky Visual Studio Code to make your music. Now you can write your music in the program.
The GUI wasn't what I wanted it to be, but it works and that's all I care about.

You can control the note selection with the arrow keys. L/R arrow keys for changing the semitone, U/D arrows keys for going to the next note in the track, tab for going to the length tab, U/D arrow keys in length to change note length by 500.

The application has scrolling so you don't have to worry about losing your notes. (Trust me I lost a lot of progress.)

Each song is still saved to song.txt.

There's also loading song.txt which means that you can share songs with your "friends."

# Compilation
### DJGPP
It should compile fine with the latest DJGPP version. I've only compiled it on Linux so I don't know if it'll compile on Windows.
### Hi-Tech Pacific C
Theres probably some stuff you've gotta change to make it work in PPD, but that's about it.
