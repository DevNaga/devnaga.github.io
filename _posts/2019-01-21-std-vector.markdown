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


