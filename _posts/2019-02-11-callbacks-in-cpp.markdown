---
layout: post
title:  "calling a member function of a class in C++ via a generic callback"
date:   2019-2-11 23:26:00
categories: cryptography
---

This post is about the callbacks in C++. For TL;DR folks, this post is mostly about "How to call a member function of a class from outside the class or from another class with a function pointer".

Callbacks are a great concept in C and C++. They allow event driven programming, enables layering and abstraction when coding for multiple OS and Hardware platforms, and let the services notify for any events such as socket receive, new connecton request etc.

Callbacks are created with function pointers in C / C++.

A simple example of the callback is shown below.

```c
// callback declaration
void (*callback)(int);

void print_func(int a)
{
    printf("%d\n", a);
}

int register_print_callback()
{
    callback = print_func;
 
    // ...
    return 0;
}
    
// .. down the line in the code..

int d = 3;

register_callback();

if (callback)
    callback(d);
    
```

so let's say, we do not know what data to be passed to the callback. Such as we do not know if it takes an `int` or `float` or whatever. So we use `void *` and let the caller supply the caller's private data as a `void *`.

So an example of this would be,

```c
static void (*event_callback)(void *user_priv, uint8_t *datap, size_t datap_len);
static void *user_priv;

int register_event_callback(void *user_ptr)
{
    event_callback = my_event_callback;
    user_priv = user_ptr;

    return 0;
}

// running for ever loop
void run()
{
    for ( ; ; ) {
        if (an_event) {
            uint8_t datap[100];
        
            datap_len = event_data_get(datap);
        
            // call the callback along with user's private data pointer
            if (event_callback)
                event_callback(user_priv, datap, datap_len);
        }
    }
}


// .. caller code..


// caller's private data
struct my_struct {
    int some_data;
};

void my_event_callback(void *user_priv, uint8_t *datap, size_t datap_len)
{
    struct my_struct *my = user_priv;
}

struct my_struct *my = calloc(1, sizeof(struct my_struct));
if (!my) {
    return -1;
}

// C takes care of type conversion automatically unlike the C++

register_event_callback(my);

// let the main loop run and this calls our event callback
run();

```

So, the code waits for an event in the `run()` function and allow for calling our registered callback when there is an event. Even more so, one could register for particular type of the event to get the callback called.


However, coming to the C++ now, makes things hard when calling the callbacks, especially when they are member function of a class.


Let's see the below example,

<script src="https://gist.github.com/DevNaga/ae14e8148c462c441a495f738b6b9581.js"></script>




















