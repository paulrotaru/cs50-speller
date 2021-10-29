// Implements a dictionary's functionality

#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cs50.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 65536; //%65536 can be translated into & 0xffff;
int word_count = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    //create a var for the low char of the word
    //create a var for the last char \0
    char low_letters[LENGTH+1];
    int last = strlen(word);
    for (int i = 0; i < last; i++)
    {
        low_letters[i] = tolower(word[i]);
    }
    low_letters[last] = '\0';

        unsigned int hashnum = hash(low_letters);

    // Checks for input word in hash table
    bool found = false;
    for (node *tmp = table[hashnum]; tmp != NULL; tmp = tmp->next)
    {
        if (strcmp(tmp->word, low_letters) == 0)
        {
            found = true;
            break;
        }
    }
    return found;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO

    unsigned long hash = 5381;
    int c;

    while ((c = tolower(*word++)))
    {
       hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    //open for reading a dictionary
    FILE *inptr = fopen (dictionary, "r");
    if (inptr == NULL)
    {
        return false;
    }
    char word[LENGTH + 1];
    char c = fgetc(inptr);
    while (c != EOF)
    {
        for (int i = 0; i < LENGTH + 1; i++)
        {
            if (c != '\n')
            {
                word[i] = c;
                c = fgetc(inptr);
            }
            else
            {
                word[i] = '\0';
                word_count++;
                c = fgetc(inptr);
                break;
            }
        }
        unsigned int hash_index = hash(word);
        node *new_word = malloc(sizeof(node));
        strcpy(new_word->word, word);
        new_word->next = NULL;

        if (table[hash_index] == NULL)
        {
            table[hash_index] = new_word;
        }
        else
        {
            new_word -> next = table[hash_index];
            table[hash_index] = new_word;
        }
    }
    fclose(inptr);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for (int i = 0; i < N; i++)
    {
        node *pointer = table[i];
        while (pointer != NULL)
        {
            node *hold = pointer;
            pointer = pointer->next;
            free(hold);
        }
        free(pointer);
    }
    return true;
}
