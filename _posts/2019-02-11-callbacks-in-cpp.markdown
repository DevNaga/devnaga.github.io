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

<script src="https://gist.github.com/DevNaga/4d8329c804c70a9f05b1c8cad6e4f7e8.js"></script>




so let's say, we do not know what data to be passed to the callback. Such as we do not know if it takes an `int` or `float` or whatever. So we use `void *` and let the caller supply the caller's private data as a `void *`.

So an example of this would be,

<script src="https://gist.github.com/DevNaga/583ae347fe7b1fac89462e3a81290235.js"></script>


So, the code waits for an event in the `run()` function and allow for calling our registered callback when there is an event. Even more so, one could register for particular type of the event to get the callback called.


However, coming to the C++ now, makes things hard when calling the callbacks, especially when they are member function of a class.


Let's see the below example,

<script src="https://gist.github.com/DevNaga/ae14e8148c462c441a495f738b6b9581.js"></script>



The above example doesn't compile, and throws the following error.



```bash
g++ mem_fun.cc 
mem_fun.cc: In function ‘int main()’:
mem_fun.cc:22:16: error: cannot convert ‘mem_fun::my_public_member_f’ from type ‘void (mem_fun::)()’ to type ‘void (*)()’
     mem_fun_cb = m.my_public_member_f;

```

The main reason being, the member functions are explicitly prohibited from calling outside the class. And they are only accessible from the class object and needs to be declared.

C++-11 onwards defines a polymorphic function wrapper called `std::function`. More on the `std::function` is [here](https://en.cppreference.com/w/cpp/utility/functional/function). This allows setting a callback for any type of function. With member function, its some more work with the use of `std::bind`. More on the `std::bind` is [here](https://en.cppreference.com/w/cpp/utility/functional/bind).

Let's say if the same callback needs to be called in C++ as in C, we use `std::function` as the following.

<script src="https://gist.github.com/DevNaga/8270c9019c85b45745f0e48c8de715a3.js"></script>

Compile it with `-std=c++11` and for `std::functional` and `std::bind` requires `<functional>` header include.

The call to `std::function<void(void>` mean that the function accepts no arguments and return void.

The function then must look like,

```c
void f(void);
```

and thus our function in the above example. The `cb` gets assigned to the pointer to the function `f` and a call to the function is simply made by calling via `cb`. 

When it comes to calling the member function from the callback function we need to use `std::bind`.

this can be done in this way,

```cpp
class a_class {
    public:
        void a_mem_fun(int i) {
        }
};

std::function<void(int)> cb;
a_class o;

// bind the member function of the a_class and pass the `this` pointer. We do not have any data here.. so pass 
// std::placeholders::_1
cb = std::bind(&a_class::a_mem_fun, &o, std::placeholders::_1);

// calls member function here with value 3..
cb(3);

```

More illustrative example is below.

https://gist.github.com/DevNaga/fbafa4d15cbcf1ebc816e6f136b2790c


So coming back to the main problem, the event driven TCP server with `select` and friends that can be done with callbacks in C, can be done the same way in C++ using `std::function` and `std::bind` in the below example.

<script src="https://gist.github.com/DevNaga/293137c98ba4e913cfb242ba1e0cb932.js"></script>

I use `nc` on linux to test the server with `echo "Hello server" | nc 127.0.0.1 1244` to see if server accepts and dumps the client data on the screen.



















