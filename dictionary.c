#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


#include "dictionary.h"


char * input_string()
{
    char buff[BUFF];
    char * input = NULL;
    if (fgets(buff, BUFF, stdin))
    {
        buff[strcspn(buff, "\n")] = 0;
        input = (char *) malloc(strlen(buff)+1);
        if (!input) exit(1);
        strcpy(input, buff);
        return input;
    }
    return "Invalid input";
}


entry_t * new_entry(char * key, char * value)
{
    entry_t * entry = NULL;
    entry = (entry_t *)malloc(sizeof(entry_t));
    if (!entry) exit(1);

    char key_a[LENGTH];
    strcpy(key_a, key);
    entry->key = (char *)malloc(strlen(key_a)+1);
    if (!entry->key) exit(1);
    strcpy(entry->key, key_a);

    char value_a[LENGTH];
    strcpy(value_a, value);
    entry->value = (char *)malloc(strlen(value_a)+1);
    if (!entry->value) exit(1);
    strcpy(entry->value, value_a);

    entry->next = NULL;

    return entry;
}

entry_t * prompt(hash_t * set)
{
    entry_t * prompt = NULL;
    entry_t * cursor = NULL;
    for (int i = 0; i < set->size; i++)
    {
        cursor = set->table[i];

        if (cursor)
        {
            printf("%s -> ", cursor->key);
            prompt = new_entry(cursor->key, cursor->value);
            delete_kv_pair(set, cursor->key);
            break;
        }

    }
    return prompt;
}


hash_t * initialize_hashtable(int size)
{
    hash_t * hashtable = NULL;
    hashtable = (hash_t *)malloc(sizeof(hash_t));
    if (!hashtable) exit(1);

    hashtable->size = size;

    hashtable->table = (entry_t **)malloc(sizeof(entry_t *) * hashtable->size);
    if (!hashtable->table) exit(1);

    for(int i = 0; i < hashtable->size; i++)
    {
        hashtable->table[i] = NULL;
    }

    return hashtable;
}


int get_command(void)
{
    int command;
    char * buff = NULL;
    do
    {
        print_session_commands();
        buff = input_string();
        command = atoi(buff);
        free(buff);
    }
    while (command == 0);

    return command;
}


int await_command(hash_t * hashtable)
{
    int session_points = 0;
    int command;

    do
    {
        command = get_command();
        switch(command)
        {
            case 1:
                session_points += start_practice_round(hashtable);
                break;
            case 2:
                add_dict_pair(hashtable);
                break;
            case 3:
                delete_dict_pair(hashtable);
                break;
            case 4:
                readme();
                break;
            case 9:
                break;
            default:
                printf("Please enter proper command!\n");
                break;
        }
    }
    while (command != 9);
    printf("\nGreat practice! You have correctly spelled %d words in total!\n", session_points);

    return 0;
}


int load(hash_t * hashtable, const char *dictionary)
{
    FILE *fp = fopen(dictionary, "r");
    char key_a[LENGTH + 1];
    char value_a[LENGTH + 1];
    while (fscanf(fp, "%s - %[^\n]", key_a, value_a) != EOF)
    {
        add_kv_pair(hashtable, key_a, value_a);
    }
    fclose(fp);
    return 1;
}


int unload(hash_t * hashtable, const char * dictionary)
{
    FILE *fp = fopen(dictionary, "w");
    entry_t * cursor = NULL;
    for (int i = 0; i < hashtable->size; i++)
    {
        cursor = hashtable->table[i];

        while (cursor)
        {
            fputs(cursor->key, fp);
            fputs(" - ", fp);
            fputs(cursor->value, fp);
            fputs("\n", fp);
            cursor = cursor->next;
        }
    }
    fclose(fp);

    return 1;
}


int free_hashtable(hash_t * hashtable)
{
    entry_t *cursor = NULL;
    for (int i = 0; i < hashtable->size; i++)
    {
        cursor = hashtable->table[i];

        while (cursor != NULL)
        {
            entry_t *temp = cursor;
            cursor = cursor->next;
            free_entry(temp);
        }
    }
    free(hashtable->table);
    free(hashtable);

    return 0;
}


int free_entry(entry_t * entry)
{
    free(entry->value);
    free(entry->key);
    free(entry);

    return 0;
}


int print_set(hash_t * set)
{
    for (int i = 0; i < set->size; i++)
    {
        entry_t * cursor = set->table[i];

        while (cursor)
        {
            printf("%s -> %s\n", cursor->key, cursor->value);
            cursor = cursor->next;
        }
    }
    return 0;
}

int start_practice_round(hash_t * hashtable)
{
    hash_t * set = NULL;
    set = initialize_hashtable(ROUND_SET);
    prepare_practice_set(set, hashtable);
    system("@cls||clear");
    print_set(set);
    print_practice_round_commands();

    int points = practice_round(set);

    return points;
}

int practice_round(hash_t * set)
{
    char *input;
    entry_t *prompt_pair;
    int points, words;
    points = words = 0;
    input = input_string();

    while (strcasecmp(input, "-STOP") != 0 && words < set->size)
    {
        free(input);
        system("@cls||clear");
        prompt_pair = prompt(set);
        input = input_string();
        words++;
        if (strcasecmp(input, prompt_pair->value) == 0)
        {
            printf("Correct!\n");
            sleep(1);
            points++;
        }
        else
        {
            printf("Awww, you have misspelled.\n");
            printf("Correct word is %s\n", prompt_pair->value);
            sleep(3);
        }
        free_entry(prompt_pair);
    }

    printf("\nYou have correctly spelled %d out of %d words\n", points, words);
    free(input);
    free_hashtable(set);

    return points;
}

int prepare_practice_set(hash_t * set, hash_t * hashtable)
{
    entry_t * cursor = NULL;
    srand(time(NULL));

    int i = 0;
    while (i < set->size)
    {
        cursor = hashtable->table[rand() % CAPACITY];
        if(cursor)
        {
            add_kv_pair(set, cursor->key, cursor->value);
            i++;
        }

    }
    return 0;
}


int add_kv_pair(hash_t * hashtable, char * key, char * value)
{
    unsigned int hash;
    hash = DJB2Hash(key, hashtable->size);

    entry_t * entry = NULL;
    entry = new_entry(key, value);

    entry->next = hashtable->table[hash];
    hashtable->table[hash] = entry;

    return 0;
}


int add_dict_pair(hash_t * hashtable)
{
    printf("Please enter a word in language you're learning\n");
    char * key = NULL;
    key = input_string();

    printf("Please enter relevant translation in English (single word)\n");
    char * value = NULL;
    value = input_string();
    add_kv_pair(hashtable, key, value);

    free(value);
    free(key);
    printf("\nThe word pair has been added to your dictionary!\n");

    return 0;
}


int delete_dict_pair(hash_t * hashtable)
{
    printf("Please enter a word in language you're learning that you would like to remove from the dictionary\n");
    char * input = NULL;
    input = input_string();
    if (delete_kv_pair(hashtable, input))
    {
        printf("\nThe word pair has been deleted from your dictionary!\n");
    }
    else
    {
        printf("\nOoops... Something went wrong!\n");
    }
    free(input);

    return 0;
}


int delete_kv_pair(hash_t * hashtable, char * key)
{
    unsigned int hash;
    hash = DJB2Hash(key, hashtable->size);

    entry_t **address = NULL;
    entry_t * cursor = NULL;

    address = &(hashtable->table[hash]);

    while(*address)
    {
        if (strcasecmp((*address)->key, key) == 0)
        {
            cursor = *address;
            *address = cursor->next;

            free_entry(cursor);
            return 1;
        }
        *address = (*address)->next;
    }
    
    return 0;
}


int readme()
{
    char * readme = README;
    FILE *fp = fopen(readme, "r");

    char ch;

    if (!fp)
    {
        printf("Error while opening the file!\n");
        exit(1);
    }

    while ((ch = fgetc(fp)) != EOF)
    {
        printf("%c", ch);
    }
    fclose(fp);
    return 0;
}


// DJB2 hash function from http://www.cse.yorku.ca/~oz/hash.html with added functionality for case insensitivity
unsigned int DJB2Hash(const char *key, int size)
{
    unsigned int hash = 5381;
    int c;

    while ((c = tolower(*key++)))
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    // Result of hashing mod total capacity of the hashtable
    return hash % size;
}


void print_greeting()
{
    printf("\n ================== Welcome to language practice ==================\n");
    printf("|                                                                  |\n");
    printf("|  You will see 10 word pairs appear on the screen. Memorize them. |\n");
    printf("|  Once the practice begins, you will need to translate the words  |\n");
    printf("|  shown in the prompt into English!                               |\n");
    printf("|                                                                  |\n");
    printf("|  NOTE!                                                           |\n");
    printf("|  All answers are single words and do not require use of space!   |\n");
    printf("|                                                                  |\n");
    printf(" ==================================================================\n");
}


void print_session_commands()
{
    printf("\nHow would you like to proceed?\n");
    printf("1 - New practice round\n");
    printf("2 - Add new word pair\n");
    printf("3 - Delete a word pair by keyword\n");
    printf("4 - Detailed READ.ME of the Language Practice program\n");
    printf("9 - End session\n");

}


void print_practice_round_commands()
{
    printf("\nType -START to start practice\n");
    printf("Type -STOP to stop practice round at any time\n");
}