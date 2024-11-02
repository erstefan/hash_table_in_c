#include <stdio.h>
#include "hash_table.h"

int main() {
    ht_hash_table* table = ht_new();

    printf("table size: %i \n", table->size);
    printf("table count: %i \n", table->count);

    const char* k = "foo";
    const char* v = "bar";

    ht_insert(table, k, v);


    printf("table size: %i \n", table->size);
    printf("table count: %i \n", table->count);

    char* my_item = ht_search(table, k);


    ht_delete_hash_table(table);

}
