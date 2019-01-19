---
layout: post
title:  "malloc vs calloc"
date:   2019-1-19 23:26:00
categories: technology, c
---

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

```c
int list_add_tail(void *data)
{
	struct linked_list *t;

	t = malloc(sizeof(struct linked_list));
	if (!t) {
		return -1;
	}

	t->data = data;

	if (!head) { // no head element yet..
		head = t;
		tail = t;
	} else { // add it at the tail..
		tail->next = t;
		tail = t;
	}

	return 0;
}

```

and the list can be iterated using the `next` pointer till the `NULL` or the end of the list.

A simple list iterator is as follows

```c
void list_iter()
{
	struct linked_list *t;

	for (t = head; t; t = t->next) {
		// data..
	}

}

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

