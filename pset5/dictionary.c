// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 5000000;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    int key = hash(word);

    node *cursor = table[key];

    while (cursor != NULL)
    {
        int comp = strcasecmp(cursor->word, word);

        if (comp == 0)
        {
            return true;
            // printf("it works! %s %s\n", cursor->word, word);
        }

        if (strcasecmp(cursor->word, 'a') || strcasecmp(cursor->word, "A"))
        {
            // printf("its a, again");
            return true;
        }

        cursor = cursor->next;
    }

    return false;
}
// Hashes word to a number
unsigned int hash(const char *word)
{
    // djb2 hash function from http://www.cse.yorku.ca/~oz/hash.html
    unsigned long hash_ct = 5381;
    int c = 0;

    while ((c = *word++))
    {
        hash_ct = ((hash_ct << 5) + hash_ct) + tolower(c); /* hash * 33 + c */
    }
    return hash_ct % N;
}
// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    char str[LENGTH + 1];
    FILE *file = fopen(dictionary, "r");

    while (1)
    {
        //write the word from dictionary in str
        if (fgets(str, LENGTH + 2, file) == NULL)
        {
            fclose(file);
            break;
        }
        //remove the \n on the end
        str[strlen(str) - 1] = '\0';

        int hash_code = hash(str);

        //if the table has no pointer
        if (table[hash_code] == NULL)
        {
            //add word to node
            node *n = malloc(sizeof(node));
            if (n == NULL)
            {
                return false;
            }
            //put the word in a node on the beginning of the hash table
            strcpy(n -> word, str);
            n -> next = NULL;
            table[hash_code] = n;
        }
        //if the table has a pointer
        else
        {
            node *n = malloc(sizeof(node));
            if (n == NULL)
            {
                return false;
            }

            //put the word in a node on the beginning of the hash table
            strcpy(n -> word, str);
            n -> next = table[hash_code]; //the new node points to first node
            table[hash_code] = n; //table point no new node
        }
    }
    return true;
}
// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    int count = 0;
    for (int i = 0; i < N; i++)
    {
        node *current = table[i];

        //until the last node
        while (current != NULL)
        {
            count++;
            current = current -> next; //go to next node
        }
    }
    return count;
}
// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        //unload all tables until the last
        while (table[i] != NULL)
        {
            node *tmp = table[i]->next;
            free(table[i]);
            table[i] = tmp;
        }
    }
    return true;
}
