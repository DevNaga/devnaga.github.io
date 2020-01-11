---
layout: post
title:  "malloc vs calloc"
date:   2019-1-19 23:26:00
categories: technology, c
---

**Devendra Naga**

**Email: [devendra.aaru@gmail.com](devendra.aaru@gmail.com)**

**Dated: 1/19/2019**


The C / C++ programming languages provide the memory allocation functions such as `malloc`, `calloc` and `realloc`. Most of the programs does use `malloc` and most of them **never** resets the memory that they allocate.

Example of allocation with `malloc`

```c
int *array_ptr;

array_ptr = malloc(sizeof(int) * 10);
if (!array_ptr) {
    return FAIL;
}

// use array_ptr here..
```

Notice the above, `malloc` returns a void pointer and C takes care of the type conversion. So no need of typecasting. For C++, this means a typecast must be required and without it, it results in a compiler error.

`malloc` returns a pointer to the virtual memory that may have contents uninitialised or garbage.

Further, the pointer returned by the `malloc` is not a physical page that is allocated. This means if the memory to be allocated is sufficiently huge, `malloc` may simply return `true` as long as this is with in the addressable range of the CPU.

Now, how this is a problem, i describe it with a simple naive example, 

Let's take an example of linked list. A linked list is shown below.

```c
struct linked_list {
    void *data; // the actual data in the list
    struct linked_list *next; // pointer to the next element in the chain
};

static struct linked_list *head, *tail;
```

Below function shows the addition of an element at the end of the list.

<script src="https://gist.github.com/DevNaga/65f16a921006d61a81384724f5be87d8.js"></script>


and the list can be iterated using the `next` pointer till the `NULL` or the end of the list.

A simple list iterator is as follows

```c
void list_iter()
{
    struct linked_list *t;

    for (t = head; t; t = t->next) {
        //..
    }

}

```

In the above example, the `next` pointer may never be `NULL`. See `list_add_tail` and the newly allocated element is only setting the data pointer but never setting the next pointer to `NULL`.

This means that the `next` pointer may contain uninitialised value or a garbage that results in undefined behavior (or in sometimes results in a crash).

Having the program crash in such kind of situations is better than having the program keep executing. The resolution is to set the `next` pointer <b>explicitly</b> to `NULL`.

such as the following,

```c
t->next = NULL;
```


Example of allocation with `calloc`.

```c
int *array_ptr;

array_ptr = calloc(10, sizeof(int));
if (!array_ptr) {
    return FAIL;
}

// use array_ptr here..
```

The `calloc` returns a pointer to the memory that again points to a page in the kernel that is cleared (or in programming world called 0s).

`calloc` does get the page that is allocated by the kernel that maps to the physical page. This means, if the caller tries to allocate memory thats bigger than the physical, calloc returns a `NULL` pointer. 

This means that `calloc` takes some amount of time more than that of `malloc` to allocate a memory, and thus slower.

Let's take the same example of linked list. There replacing the calls of `malloc` will `calloc` results in pointer `next` to point to `NULL` and thus do not require any explicit setting.


It is always safe to use memory that is cleared to work with, the main reason being that there is no chance of unknown or uninitialised values to work with that may result in unknown issues.


When using the memory allocation functions in embedded systems, it is always best to pre-allocate them before into what is called a pool and then re-use them often than allocate them at runtime. This has added advantage of using `calloc` (although one might say its slower compared to malloc).

Once the memory that has been used, it must be returned back to the pool. Any new call to the pool must reset the memory so that the previous user's data will be erased and can be started fresh.

