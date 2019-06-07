---
layout: post
title:  "Return more than one value from a C++ function"
date:   2019-05-14 23:26:00
categories: language, c++
---

We all know that C and C++ can only return one value, unlike Golang which can do more than one. The programming practise that happens to be is that the function needs to return something because it is performing some operation on the given input data. However, along with the result it has to also return the output data if there is any.

So we generally do,

```c

int do_some_job(struct input_data *_in, struct output_data *_out)
{
    int status = -1; // fail
    ...

    // output is copied to _out
    _out->some_ouput = .. 

    status  = 0; // ok good

    ...

    return status
}

```

the above function returns the status as well has the output data to `_out`.

This is perfectly valid and holds good in cases where the object 


So, if we still wanna do, we could do with a structure that can wrap the variables in. However, this means that the structure to be allocated on heap in order to be thread safe. Having static data types mean, there is always a problem hiding behind the "state" when not managed. Add to that, the thread safety.

Another way to do is, use `std::pair` in C++. Let's have a look here.

```cpp

std::pair<int, double> return_two_values()
{
    return std::pair<int, double>(1, 1.1);
}

```

The above function is perfectly valid and compiles. See full example below.

```cpp

#include <iostream>

std::pair<int, double> return_two_values()
{
    return std::pair<int, double>(1, 1.1);
}

int main()
{
    std::pair<int, double> pair = return_two_values();

    std::cout << pair.first << " " << pair.second << std::endl;
}

```

`std::pair` is templatised, so one can return the status of the function call and output simultaneously at once.
