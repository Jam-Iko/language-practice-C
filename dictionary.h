// Declares a dictionary's functionality

#ifndef DICTIONARY_H
#define DICTIONARY_H

// Maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
#define LENGTH 45
#define BUFF 1024
#define CAPACITY 1500

// Default dictionary
#define DICTIONARY "dictionaries/swe"
#define README "read.me/readme"
#define ROUND_SET 10

typedef struct entry_t
{
	char *key;
    char *value;
    struct entry_t * next;
}
entry_t;

typedef struct hash_t
{
	int size;		//size of pointer table
    struct entry_t ** table;
}
hash_t;

char * input_string();
entry_t * new_entry(char * key, char * value);
entry_t * prompt_word(hash_t * set);
hash_t * initialize_hashtable(int size);
int get_command(void);
int await_command(hash_t * hashtable);
int load(hash_t * hashtable, const char *dictionary);
int unload(hash_t * hashtable, const char * dictionary);
int free_entry(entry_t * entry);
int free_hashtable(hash_t * hashtable);
int print_set(hash_t * set);
int start_practice_round(hash_t * hashtable);
int practice_round(hash_t * set);
int delete_kv_pair(hash_t * hashtable, char * key);
int prepare_practice_set(hash_t * set, hash_t * hashtable);
int add_kv_pair(hash_t * hashtable, char * key, char * value);
int add_dict_pair(hash_t * hashtable);
int delete_dict_pair(hash_t * hashtable);
int readme();
unsigned int DJB2Hash(const char *key, int size);
void print_greeting();
void print_session_commands();
void print_practice_round_commands();

#endif // DICTIONARY_H