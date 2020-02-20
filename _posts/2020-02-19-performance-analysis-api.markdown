---
layout: post
title:  "Performance analysis API in C++"
date:   2020-02-19 23:26:00
categories: technology,cryptography, c, yocto, android
---

Short details regarding the performance analysis of a particular code section. I have written a program under linux so i try to expolit the use of `clock_gettime` and the ability to calculate thread cpu time with `CLOCK_THREAD_CPUTIME_ID`.


```cpp
#include <time.h>
#include <sys/time.h>

int clock_gettime(clockid_t id, struct timespec *res);
```

A call to get the cpu time can look like this:

```cpp

struct timespec time_val;

clock_gettime(CLOCK_THREAD_CPUTIME_ID, &time_val);
```

A section of code exection can then be calculated by adding this above and below the code section like so,


```cpp

clock_gettime(CLOCK_THREAD_CPUTIME_ID, &start);
...
code section
...
clock_gettime(CLOCK_THREAD_CPUTIME_ID, &stop);
```

And then a delta calculation thus can be,

```cpp
double nsec = ((double)(stop.tv_sec - start.tv_sec) * 1000000000ULL) - ((double)(stop.tv_nsec - start.tv_nsec));
```


Now, the class organization can be done as follows,

1. Perf analyer class - can create as many perf objects as possible
   - can also be able to list down full stats of each of the perf object

2. Perf item class - enables the methods to start and stop performance analysis
   - can provide delta of each section


Both classes are shown as follow

```cpp
class Perf_Analyser {
    public:
        ~Perf_Analyser() { }
        static Perf_Analyser *Instance() {
            static Perf_Analyser pa;

            return &pa;
        }

        std::shared_ptr<Perf_Item> New_Perf_Item(std::string item_name, int records);

        void Dump_Analysis_Results();

    private:
        std::vector<std::shared_ptr<Perf_Item>> pi_list_;
};

class Perf_Item {
    public:
        explicit Perf_Item(const std::string item_name, int records);
        ~Perf_Item();

        void Disable();

        void Enable();

        void Start();

        void Stop();

        double Report_Avg();

        std::string Get_Name();

        int Get_Sample_count();

    private:
        bool disabled_;
        int records_;
        std::string item_name_;
        int cur_samples_;
        struct timespec start_tv_;
        struct timespec stop_tv_;
        std::vector<double> samples_;
};
```

An implementation of it would look as follows:

```cpp
#include <iostream>
#include <memory>
#include <vector>
#include <time.h>
#include <sys/time.h>

namespace Core_Middleware {

class Perf_Item {
    public:
        explicit Perf_Item(const std::string item_name, int records) :
                                            item_name_ (item_name),
                                            disabled_ (true),
                                            records_ (records) {
                                            }
        ~Perf_Item() {
        }

        void Disable() {
            disabled_ = true;
        }

        void Enable() {
            disabled_ = false;
        }

        void Start() {
            if (disabled_)
                return;
            clock_gettime(CLOCK_THREAD_CPUTIME_ID, &start_tv_);
        }

        void Stop() {
            if (disabled_)
                return;
            double nsec;

            clock_gettime(CLOCK_THREAD_CPUTIME_ID, &stop_tv_);
            nsec = ((double)(stop_tv_.tv_sec * 1000000000ULL) + stop_tv_.tv_nsec) - ((double)(start_tv_.tv_sec * 1000000000ULL) + start_tv_.tv_nsec);
            samples_.push_back(nsec);
            cur_samples_ ++;
        }

        double Report_Avg() {
            double sum = 0;
            int min = records_ < samples_.size() ? records_ : samples_.size();

            for (auto i = 0; i < min; i ++) {
                sum += samples_[i];
            }

            return sum / min;
        }

        std::string Get_Name() {
            return item_name_;
        }

        int Get_Sample_count() {
            return cur_samples_;
        }

    private:
        bool disabled_;
        int records_;
        std::string item_name_;
        int cur_samples_;
        struct timespec start_tv_;
        struct timespec stop_tv_;
        std::vector<double> samples_;
};

class Perf_Analyser {
    public:
        ~Perf_Analyser() { }
        static Perf_Analyser *Instance() {
            static Perf_Analyser pa;

            return &pa;
        }

        std::shared_ptr<Perf_Item> New_Perf_Item(std::string item_name, int records) {
            std::shared_ptr<Perf_Item> pi = std::make_shared<Perf_Item>(item_name, records);
            pi_list_.push_back(pi);
            return pi;
        }

        void Dump_Analysis_Results() {
            for (auto an : pi_list_) {
                printf("perf_item: [%s] samples [%d]  nsec: [%f]\n", an->Get_Name().c_str(), an->Get_Sample_count(), an->Report_Avg() / 1000000);
            }
        }

    private:
        std::vector<std::shared_ptr<Perf_Item>> pi_list_;
};

}
```

An example program would look as follows,


```cpp

Core_Middleware::Perf_Analyser *pa = Core_Middleware::Perf_Analyser::Instance();

void f1()
{
    int i;

    std::shared_ptr<Core_Middleware::Perf_Item> pi2 = pa->New_Perf_Item("f1", 1);

    pi2->Enable();
    pi2->Start();

    double a = 100.1;
    for (i = 0; i < 1000000000; i ++) {
        a = a * (i + 1000);
    }

    pi2->Stop();
}

int main()
{
    int i;

    std::shared_ptr<Core_Middleware::Perf_Item> pi1 = pa->New_Perf_Item("main", 1);

    pi1->Enable();
    pi1->Start();

    double a = 100.1;
    for (i = 0; i < 1000000000; i ++) {
        a = a * (i + 1000);
    }

    pi1->Stop();

    f1();

    pa->Dump_Analysis_Results();
}
```

Full example is shown below:

<script src="https://gist.github.com/DevNaga/daca125a573d2d2bffbd71ff66917253.js"></script>

