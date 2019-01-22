---
layout: post
title:  "Tutorial on std::vector"
date:   2019-1-21 23:26:00
categories: technology, c++
---

The C++ provides very powerful STL functions. One of the mostly used STL is the `std::vector`. `std::vector` is similar to arrays but is dynamically expandable. `std::vector` is a generic template with possibility to support any types.

Such as for ex:

```cpp
std::vector<int> a;
```

defines a vector of integers.

```cpp
struct obj {
   int val;
};

std::vector<obj> obj_vec;
```

defines a vector of objects of type `struct obj`.

The `std::vector` is defined in `vector` header file.

The `std::vector` has the following member functions :

| member function | defintion |
|-----------------|-----------|
| `push_back` | add an element at the end |
| `pop_back` | remove an element at the end |
| `size` | returns the size of the vector |
| `insert` | insert an element in the vector |
| `erase` | erase the elements in the vector |
| `clear` | clear the contents in the vector |
| `empty` | check if vector is empty |


Just as arrays being accessed by the `[]` operator, the vector can be accessed the same way as well.


Below is an example of a vector of ints that are set using the `push_back` method and read using the `[]` operator.


```cpp
#include <iostream>
#include <vector>

int main()
{
    std::vector<int> ints;
    int i;

    for (i = 0; i < 100; i ++) {
        ints.push_back(i);
    }

    for (i = 0 ; i < 100; i ++) {
        std::cout << "vector<int>[" << i << "] = " << ints[i] << std::endl;
    }

    return 0;
}

```

The `std::vector` provides `iterator` which is used to iterate through the elements of the vector. The `std::vector` object contains further below methods that can return the type `iterator`.

| method | description |
|--------|-------------|
| `begin` | return iterator to beginning |
| `end` | return iterator to end |
| `rbegin` | return reverse iterator to beginning |
| `rend` | return reverse iterator to end |

an iterator for a vector is declared as follows.

```cpp
std::vector<int>::iterator it;
```

An iterator is usually assigned to `begin()` member function but it can point to `end()` sometimes (cases when a search query on an `std::vector` might return `end()` if the search couldn't find the element.

Like so,

```cpp
std::vector<int>::iterator it;

it = ints.begin(); // set it to the beginning of the iterator list
```

A reverse iterator is used for traversing the list in reverse, it is called `reverse_iterator` and is declared as follows.

```cpp
std::vector<int>::reverse_iterator rit;
```

A `reverse_iterator` is assigned to `rbegin()` member function.

Like so,

```cpp
std::vector<int>::reverse_iterator rit;

rit = ints.rbegin(); // set rit to the beginning of the list that iterates in reverse
```

There is a `const_iterator` that allows constant iteration through the vector. An iterator of type `const_iterator` can be declared as,

```cpp
std::vector<int>::const_iterator cit;

cit = ints.begin();
```

As the name implies, setting a value to the cit results in a compiler error.

```cpp
std::vector<int>::const_iterator cit;

cit = ints.begin();

*cit = 4; // this line results in a compiler error

```



dereferencing the iterator gives the actual content. The same above program can be written as,

```cpp
#include <iostream>
#include <vector>

int main()
{
    std::vector<int> ints;
    int i;

    for (i = 0; i < 100; i ++) {
        ints.push_back(i);
    }

    std::vector<int>::iterator it;

    for (it = ints.begin(); it != ints.end(); it ++) {
        std::cerr << "vector<int> -> " << *it << std::endl;
    }
    
    std::vector<int>::reverse_iterator rit;
    
    for (rit = ints.rbegin(); rit != ints.rend(); rit ++) {
        std::cerr << "vector<int> (r) -> " << *rit << std::endl;
    }

    return 0;
}

```

The above shows the iterator being assigned to the `begin()` and checked for end of vector as `it != ints.end()` and incremented with `++` operator. `end()` points to the end of the vector. Dereferencing the iterator gives the value at the index location.

With C++-11 and above, the iterator can simply be defined as 

```cpp
for (auto it = ints.begin(); it != ints.end(); it ++) {
   std::cerr << *it << std::endl;
}

```


The value at the iterator location can be accessed with an integer variable as,

```cpp
for (it = ints.begin(); it != ints.end(); it ++) {
   int var = *it;
}

```

If the values at the iterator positions need change, they can be simply changed as by dereferencing the iterator and setting it a value.

```cpp
std::vector<int>::iterator it;

for (it = ints.begin(); it != ints.end(); it ++) {
    *it = 4;
}

```

The elements in the vector can be found with the `std::find` as well (declared in `<algorithm>`) as below.

```cpp
std::vector<int>::iterator it;
int search_key = 81;

it = std::find(ints.begin(), ints.end(), search_key);
if (it != ints.end()) {
    // element is found..
} else {
   // coulnd't find search element..
}

```

the `std::find` returns an iterator. An `ints.end()` mean that no elements been found.

The elements never to be deleted inside a vector iteration. Such as the following,

```cpp
std::vector<int>::iterator it;
int search_key = 81;

for (it = ints.begin(); it != ints.end(); it ++) {
    if (*it == search_key) {
        vector.erase(it);
    }
}

```

results in a crash, because the iterator is deleted once the value is found and is incremented in the next step after the deletion.

The safest way to delete an element in the vector is to search and find the element, then delete it. Both in two steps.

```cpp

//search an element

std::vector<int>::iterator it;
int search_key = 81;

it = std::find(ints.begin(), ints.end(), search_key);
if (it != ints.end()) {
    ints.erase(it); // remove the element
}

```

The above operation involve finding the element with `std::find` and once an iterator is found, it is then used for deletion with the `erase` member function.

`std::vector` erase is an overloaded member function. It can either accept a single iterator, that means a position in the vector to delete or a range of iterators in the order.

The prototype of the `erase` might look like as follows,

```cpp
iterator erase(iterator it);
iterator erase(iterator begin, iterator end);
```

Returning a vector from a function is similar to returning any other object. It can be returned as a pointer or a value. Below example shows both return types.


```cpp
#include <iostream>
#include <vector>

std::vector<int> returns_ints()
{
    std::vector<int> a;
    int i;

    for (i = 0; i < 100; i ++) {
        a.push_back(i);
    }

    return a;
}

std::vector<int>* returns_intptr()
{
    std::vector<int> *a;
    int i;

    a = new std::vector<int>;

    for (i = 0; i < 100; i ++) {
        a->push_back(i);
    }

    return a;
}

int main()
{
    std::vector<int> a, *b;

    a = returns_ints();
    b = returns_intptr();

    std::vector<int>::iterator it;

    for (it = a.begin(); it != a.end(); it ++) {
        std::cerr << *it << std::endl;
    }

    for (it = b->begin(); it != b->end(); it ++) {
        std::cerr << *it << std::endl;
    }

    delete b;
}

```

While the compiler (in this case i have tried the g++) optimises away the cost of returning a value over the pointer, it is usually not recommended to return any pointers (which may be allocated), due to the possibility of a memory leak when not freed or not defined who's job to free it (Caller's or called functions.. ).


This post isn't complete yet.. More to come .. stay tuned..



