# language-practice-C
Console application in C language for memorising words from uploaded dictionaries.

Inspired by CS50's Speller assignment from 2018's pset5.
With this project I have taken a dive into implementation of linked lists, hash-tables and work with memory.

Usage: ./practice [dictionary]

Language Practice is aimed to mimic the flash-cards technique for memorizing new words.
Application has a simple and easy-to-use UI with available options such as: start new practice round (1), add (2)
or remove (3) dictionary pairs, display README (4), and end practice session (9).

Once you choose to start a practice round, 10 word pairs will appear on the screen. Please try and memorize them.
When you are ready, type -START and press ENTER (honestly, you can just press ENTER).

After the practice round starts, keywords in language, that you are learning, will be prompted on the screen.
You need to type in the translation in English and hit ENTER. If your guess is correct - you get a Schrute buck! No, you
just get a point. Otherwise the correct translation will appear on the screen.

To stop practice round at any point type in "-STOP" and press ENTER. You will return to the main menu.

If you would like to pump up your dictionary, you can add new word pairs by selecting option (2) from the main menu.
You will be prompted to enter word in the language you are learning and then its translation (so look it up in advance!)

If you need to dial down and remove some of the word pairs, just select option (3) from the main menu. You will need to enter
keyword in the language you are learning.

Should you decide to change the number of word pairs that appear during a practice round, you need to change
ROUND_SET variable in DICTIONARY.H to the desired number.

You can also upload your own dictionaries, provided that dictionary files are formatted as follows "keyword - translation",
one dictionary pair per line. Dictionaries must be saved to /dictionaries directory.
To use a non-default dictionary during practice session, you need to include file's name as a command line argument.
Recommended number of entries is 1000 key-value pairs. In case, you would like to increase
this amount, you should set CAPACITY variable in DICTIONARY.H to the desired size of the dictionary x 1.5.
