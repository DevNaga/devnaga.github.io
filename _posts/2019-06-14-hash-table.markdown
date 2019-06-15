---
layout: post
title:  "Implementing Hashtable in C"
date:   2019-06-14 23:26:00
categories: language, c
---

Hashtables are one of the important and most used data structures. If the program is doing a serious work, it probably using a hashtable type of structure for storage or even more powerful algorithm for storing and lookups.

Hashtables, from a very high level view, is a series of arrays packed together in rows. This means that the memory that is reserved, is fixed and cannot be expanded. If we need to store an element (add an element) to the table, we need to "hash in" the unique identifier corresponding the element. Remember that, This "hash"'ing is not same as that of the hash function in cryptography however, they share similar traits.

A novice and a Naive hash function will be as similar as the following,

```c

// assume string is != NULL
int hash_function(char *string)
{
    int hash = 0;

    while (*string != '\0') {
        hash += *string;
        string ++;
    }

    return hash;
}

```

The above function returns a value called "hash". This value is usually unique to each input or the key that is passed in to the Hash function. since the value could go out of the range of the table size, that results again in a possible memory violation, we generally modulo the value with the table size. That is

```c

const int table_size = 3;

// hash in the input and then get the index to this input
int hash = hash_function(input) % table_size;

// now set the value in the table
table[hash].set(input);

```

This seem so easy and the operations took are in the order O(1). However, there are places where more than one input hashes to same hash. This phenomenon is called "collision".

To comeover this problem, there are two solutions.

1. implement a better hash function that results in less collisions
2. use linked lists in place of arrays as rows.

This means, in case of a collision, the element could be simply added to the end of the list, while the hash function balances the distributed hash nature.

A commonly used hash function is the DJ Bernstien's Hash function. Example here,

```c
uint32_t hash_djb(uint8_t *key, size_t keysize)
{
    unsigned int hash = 5381;
    unsigned int i = 0;

    for (i = 0; i < keysize; i ++) {
        hash = ((hash << 5) + hash) + buf[i];
    }
    return hash;
}
```

Below is an example of the Hashtable datastructure,

```c
struct linked_list {
    void *data;
    struct linked_list *next;
};

struct linked_list_base {
    struct linked_list *head;
    struct linked_list *tail;
};

struct hash_table {
    struct linked_list_base **table;
    int n_items;
};
```

`hash_table` implements the main data structure, that encloses the linkedlist base structure. This base structure is used to avoid traversal in adding an element in the list, reducing the insertion to just 1.

The list base contains `head` and `tail` pointers which can allow to add an element at the `head` or at `tail`.

The `linked_list` implements the generic data structure, thus any element (integer, strings, or even structures) can be inserted.

The `hash_table` holds the double pointer to the list base and the number of rows in the table.

Here's an implementation of initialization of this `hash_table`.

```c
struct hash_table *table;

int hash_table_init(int items)
{
    // initalize the hash_table
    table = calloc(1, sizeof(struct hash_table));
    if (!table) {
        return -1;
    }

    // allocate now the table
    table->table = calloc(items, sizeof(struct linked_list_base));
    if (!table->table) {
        return -1;
    }

    int i;

    for (i = 0; i < items; i ++) {
        table->table[i] = calloc(1, sizeof(struct linked_list_base));
        if (!table->table[i]) {
            return -1;
        }
    }
    // set number of rows
    table->n_items = items;
    return 0;
}
```

To add an element in the list, the following is the tasks.

1. hash the input key
2. find the table row by modulo the hash
3. allocate a linked list element and set the item
4. if there is a collision, add the element to the tail
5. if there is no collision, add the element to the head

Below is an example of adding an element into the hashtable.

```c
int hash_table_add_item(void *item, uint32_t hash)
{
    struct linked_list *node;
    int hash_val;

    // modulo the hash in, for index
    hash_val = hash % table->n_items;

    // allocate a node
    printf("item %p hash_val %d\n", item, hash_val);
    node = calloc(1, sizeof(struct linked_list));
    if (!node) {
        return -1;
    }

    // set the data
    node->data = item;

    // if there is already an element - we are in collision 
    if (table->table[hash_val]->tail) {
        table->table[hash_val]->tail->next = node;
        table->table[hash_val]->tail = node;
    } else { // we aren't in collision - so lets add it to the head element
        table->table[hash_val]->head = node;
        table->table[hash_val]->tail = node;
    }
    return 0;
}
```

Finding an element has the following tasks.

1. hash in the input key
2. find the table row by modulo the hash
3. for each item in the corresponding row, match the input to the item stored at the entry.
4. return item if matched
5. return failure if not matched

Below is an example of finding an element into the hashtable.

```c
void *hash_table_find_item(void *item, uint32_t hash)
{
    struct linked_list *node;
    int hash_val;
    int iterations = 0;

    // get the table index
    hash_val = hash % table->n_items;

    // for each element in this particular "row" governed by the table index
    for (node = table->table[hash_val]->head; node != NULL; node = node->next) {

        // count number of iterations we took for lookup
        iterations ++;

        // if the item matches then return
        if (node->data == item) {
            printf("took %d iterations to find\n", iterations);
            return item;
        }
    }
    return NULL;
}
```

Now lets put it all together in, Here's the implementation, use it freely in your application. Just keep my name as original implementor as,

```
Original Implementor: Devendra Naga <devnaga@tuta.io>
```


<script src="https://gist.github.com/DevNaga/59a51004cdd48ff058149c738f1dccae.js"></script>










