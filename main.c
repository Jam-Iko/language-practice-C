#include <stdio.h>

#include "dictionary.h"


int main(int argc, char *argv[])
{
    // Determine dictionary to use
    char *dictionary = (argc == 3) ? argv[1] : DICTIONARY;

    hash_t * hashtable = NULL;
    hashtable = initialize_hashtable(CAPACITY);

    // Load dictionary
    int loaded = load(hashtable, dictionary);

    // Exit if dictionary not loaded
    if (!loaded)
    {
        printf("Could not load %s.\n", dictionary);
        return 1;
    }

    print_greeting();
    await_command(hashtable);

    int unloaded = unload(hashtable, dictionary);
    if (!unloaded)
    {
        printf("Could not unload to file %s\n", dictionary);
        return 1;
    }
    free_hashtable(hashtable);
    return 0;
}