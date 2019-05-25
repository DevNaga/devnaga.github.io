---
layout: post
title:  "C++ Namespaces"
date:   2019-05-14 23:26:00
categories: language, c++
---

This is a section of my [book](https://leanpub.com/c_cpp_refman) on C/C++ programming.

## namespaces

1. namespaces are used to organise the very large amount of source code and also to avoid name collisions (mainly).
2. a namespace can be nested within another namespace
3. namespaces can span into multiple source files / header files and the compiler combines them or extends them
4. once defined the namespace is accessed using the `using namespace` or the explicit access as `ns::var..`. the word `using` is a directive. without using the `using` directive, one should write the namespace before accessing any member of the namespace.

Below example provide an overview of the namespace usage:

**Example:**

```cpp
#include <iostream>

// namespace that's basic
namespace variable
{
    int val = 0;
}

// nested namespace
namespace dev
{
    namespace naga
    {
        int val = 1;
                //class with in namespace
        class devnaga {
            private:
                int a;
            public:
                devnaga()
                {
                    a = 2;
                }

                int getA()
                {
                    return a;
                }
        };
    }

        // declaring class object with in the namespace
        naga::devnaga p;
}

// extending namespace
namespace variable
{
    int var = 3;
}

namespace dev {
namespace naga
{
    int var = 4;
}
}

// alias of dev::naga namespace
namespace _newdev = dev::naga;

int main()
{

    // declare a class within the namespace
    dev::naga::devnaga p;

    std::cout << "val::variable:" << variable::val << "variable::var:" << variable::var << std::endl;
    std::cout << "dev::naga::val:" << dev::naga::val << "dev::naga::var:" << dev::naga::var << std::endl;
    std::cout << "getA():" << p.getA() << std::endl;
        std::cout << "getA_new():" << dev::p.getA() << std::endl;
        std::cout << "val_newdev:" << _newdev::val << std::endl;
}
```


Sometimes it gets hard to understand or access variables inside a namespace.. such as for ex defined Enums. See example below on accessing enums with in the namespaces.

**Example:**

```cpp
#include <iostream>

namespace ns {
    typedef enum {
        ENUM_1,
        ENUM_2,
    } TestEnum_t;

    TestEnum_t te;
};

int main()
{
    ns::TestEnum_t te1 = ns::ENUM_2;
    ns::te = ns::ENUM_1;
    std::cout << "Enum1: " << ns::te << std::endl;
}
```

The type `ENUM_1` is accessed with the namespace by using the scope resolution operator `::` as `ns::ENUM_1`.

in the above example the namespace `ns` defines a enum called `TestEnum_t` inside it.

This type of defining enumerators with in a namespace avoids collision of enums of similar names but conveying different meaning.

to access the defined variable `te` the `ns::te` resolution to used.

the same namespace definition can be written more than once but with in the same file or with in different files.  Writing namespaces in different files is a good idea when the namespace is sufficiently large.

Below is an example of namespace definition in the same file but with different variables.

```cpp
#include <iostream>

namespace ns {
    int var;
};

namespace ns {
    int var1;
};

int main()
{
    ns::var = 1;
    ns::var1 = 2;

    std::cout << ns::var << " " << ns::var1 << std::endl;

    return 0;
}
```

when defined in other files, the compiler automatically combines all of the definitions and references.

If the namespace is defined as 

**file1.cc**

```cpp

namespace ns {
    int var;
};

```

**file2.cc**

```cpp

namespace ns {
    int var;
}
```

and compiled as `g++ file1.cc file2.cc`, it results in a compiler error as the compiler sees the variable `var` is defined twice within the same namespace.

functions can be defined within the namespace as well. See example below.

```cpp

#include <iostream>

namespace ns {
    int a;

    void f()
    {
        std::cout << "f() is called" << std::endl;
    }
};

int main()
{
    ns::f();

    return 0;
}
```

#### code organisation with namespaces

The code organisation of multiple files can be achieved with the namespaces.

Something for example, the software stack for the networking is to be written in C++ and the definitions of the classes and member functions can be organised with in a nested namespaces.

Something like the `udp` protocol can be organised as,

```cpp

namespace networking {

namespace layer4 {

class udp {
    public:
        udp() { };
        ~udp() { };
    private:
        int format_udp();
};

}

}

```

where the `networking` is the base level namespace (defining the highest purpose of this source code), `layer4` is the next level of namespace (defining the submodule with in the networking namespace) and the `udp` class is defined within the `layer4` namespace.

We then can define the `tcp` class with in the `layer4` namespace which can be similar to the `udp`.

    



