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

    return 0;
}

```

The above shows the iterator being assigned to the `begin()` and checked for end of vector as `it != ints.end()` and incremented with `++` operator. `end()` points to the end of the vector. Dereferencing the iterator gives the value at the index location.


The value at the iterator location can be accessed with an integer variable as,

```cpp
for (it = ints.begin(); it != ints.end(); it ++) {
   int var = *it;
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

