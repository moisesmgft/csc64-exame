#ifndef TRIE_H
#define TRIE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26
#define ID_LENGTH 5

typedef struct TrieNode
{
    struct TrieNode *children[ALPHABET_SIZE];
    float value;
    int is_end;
} TrieNode;

// Function to create a new TrieNode
TrieNode *create_node()
{
    TrieNode *node = (TrieNode *)malloc(sizeof(TrieNode));
    if (!node)
    {
        fprintf(stderr, "Erro ao alocar memória para o TrieNode.\n");
        exit(1);
    }

    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        node->children[i] = NULL;
    }
    node->is_end = 0;
    node->value = 0.0f;

    return node;
}

void insert(TrieNode *root, const char *id, float value)
{
    TrieNode *current = root;
    for (int i = 0; i < ID_LENGTH; i++)
    {
        int index = id[i] - 'A';
        if (index < 0 || index >= ALPHABET_SIZE)
        {
            fprintf(stderr, "ID inválido: %s\n", id);
            return;
        }

        if (!current->children[index])
        {
            current->children[index] = create_node();
        }
        current = current->children[index];
    }
    current->is_end = 1;
    current->value = value;
}

int find(TrieNode *root, const char *id, float *value)
{
    TrieNode *current = root;
    for (int i = 0; i < ID_LENGTH; i++)
    {
        int index = id[i] - 'A';
        if (index < 0 || index >= ALPHABET_SIZE)
        {
            fprintf(stderr, "ID inválido: %s\n", id);
            return 0;
        }

        if (!current->children[index])
        {
            return 0;
        }
        current = current->children[index];
    }

    if (current->is_end)
    {
        if (value)
        {
            *value = current->value;
        }
        return 1; // ID exists
    }
    return 0; // ID does not exist
}

// Function to free all nodes in the Trie
void free_trie(TrieNode *root)
{
    if (!root)
    {
        return;
    }

    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        free_trie(root->children[i]);
    }
    free(root);
}

#endif // TRIE_H
