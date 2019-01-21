#include <iostream>
#include <vector>
#include <algorithm>

int main()
{
    std::vector<int> ints;
    int i;

    for (i = 0; i < 100; i ++) {
        ints.push_back(i);
    }

    std::vector<int>::iterator it;

    for (it = ints.begin(); it != ints.end(); it ++) {
        int var = *it;

        std::cerr << "vector<int> -> " << var << std::endl;
    }

    int number = 81;

    it = std::find(ints.begin(), ints.end(), number);
    if (it != ints.end()) {
        std::cerr << "element " << *it << " found" << std::endl;
    }
    return 0;
}

