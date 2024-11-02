#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "hash_table.h"

static ht_item HT_DELETED_ITEM = { NULL, NULL };


static ht_item* ht_new_item(const char* k, const char* v)
{
    ht_item* i = malloc(sizeof(ht_item));

    i->key = strdup(k);
    i->value = strdup(v);

    return i;
}

ht_hash_table* ht_new() {
    ht_hash_table* ht = malloc(sizeof(ht_hash_table));

    ht->size = 53;
    ht->count = 0;
    ht->items = calloc((size_t)ht->size, sizeof(ht_item*));

    return ht;
}


static void ht_delete_item(ht_item* item) {
    free(item->key);
    free(item->value);
    free(item);
}

void ht_delete_hash_table(ht_hash_table* t) {
    for (int i = 0; i < t->size; i++) {
        ht_item* item = t->items[i];

        if (item != NULL) {
            ht_delete_item(item);
        }
    }

    free(t->items);
    free(t);
}


static int hash(const char* str, const int a, const int m)
{
    long hash = 0;
    const int str_length = strlen(str);

    for (int i = 0; i < str_length; i++) {
        hash += (long)pow(a, str_length - (i + 1)) * str[i];
        hash = hash % m;
    }

    return (int)hash;
}

static int ht_get_hash(const char* str, const int num_buckets, const int attempt)
{
    const int a = hash(str, 171, num_buckets);
    const int b = hash(str, 191, num_buckets);

    return (a + (attempt * (b + 1))) % num_buckets;
}


void ht_insert(ht_hash_table* ht, const char* key, const char* value) {
    ht_item* item = ht_new_item(key, value);
    int index = ht_get_hash(item->key, ht->size, 0);

    ht_item* cur_item = ht->items[index];
    int i = 1;

    while (cur_item != NULL) {
        if (cur_item != &HT_DELETED_ITEM) {
            if (strcmp(cur_item->key, key) == 0) {
                ht_delete_item(cur_item);
                ht->items[index] = item;
                return;
            }
        }

        index = ht_get_hash(item->key, ht->size, i);
        cur_item = ht->items[index];

        i++;
    }

    ht->items[index] = item;
    ht->count++;
}


char* ht_search(ht_hash_table* ht, const char* key) {

    int index = ht_get_hash(key, ht->size, 0);
    ht_item* item = ht->items[index];

    int i = 1;

    while (item != NULL) {
        if (item != &HT_DELETED_ITEM) {
            if (strcmp(item->key, key) == 0) {
                return item->value;
            }
        }

        index = ht_get_hash(key, ht->size, i);
        item = ht->items[index];
        i++;
    }

    return NULL;
}

void ht_delete(ht_hash_table* ht, const char* key) {
    int index = hash(key, ht->size, 0);
    ht_item* item = ht->items[index];

    int i = 1;
    while (item != NULL) {
        if (item != &HT_DELETED_ITEM) {
            if (strcmp(item->key, key) == 0) {
                ht_delete_item(item);
                ht->items[index] = &HT_DELETED_ITEM;
            }
        }

        index = hash(key, ht->size, i);
        item = ht->items[index];
        i++;
    }
    ht->count--;
}
