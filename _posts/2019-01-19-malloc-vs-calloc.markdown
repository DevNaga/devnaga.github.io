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

