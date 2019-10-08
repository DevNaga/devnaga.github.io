---
layout: post
title:  "Grand Central Dispatch (GCD) in C++"
date:   2019-10-01 23:26:00
categories: technology, c++, event driven
---

I have been trying to make an event driven parallel (atleast with threads if not parallelism) execution layer that can be used by a software program.

The purpose of it is to provide the following methods.

    1. timers
    2. sockets / file descriptors
    3. signals
    4. any execution that required / can be allowed to be run in parallel or in a thread

So i define a singleton class `Gcd` as follows.

```cpp

// Grand Central Dispatch main class
class Gcd {
    public:
        static Gcd *Instance()
        {
            static Gcd gcd;

            return &gcd;
        }

        ~Gcd();

        // create timer event with sec, usec and a callback
        int Create_Timer_Event(int sec, int usec, Timer_Fn timer_fn) noexcept;

        // create socket event with fd and a callback
        int Create_Socket_Event(int fd, Socket_Fn socket_fn) noexcept;

        // create signal event with signal no and a callback
        int Create_Signal_Event(int sig, Signal_Fn signal_fn) noexcept;

        // run an execution context
        void Run_Execution(Job_Fn job) noexcept;

        // run the main GCD
        void Start();

        // terminate the wheel
        void Terminate() { terminate_ = true; }
    private:
        // list of timers
        std::vector<Gcd_Timer> timers_;

        // list of sockets
        std::vector<Gcd_Socket> sockets_;

        // list of signals
        std::vector<Gcd_Signal> signals_;

        // parallel context
        Parallel p_;

        // set to true when Terminate() is called
        bool terminate_ = false;

        // fd to handle the signals
        int signal_fd_;

        // maks of all the signals
        sigset_t signal_masks_;

        // maks of all the fds
        fd_set allfd_;

        // get allmax fds
        int get_max_fd_();

        // priovate constructor .. base singleton
        explicit Gcd();
};

```


Below is the full example of the `Gcd`: <script src="https://gist.github.com/DevNaga/07be345518682e19331e00fa1aeac9ed.js"></script>


