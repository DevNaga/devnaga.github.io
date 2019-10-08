---
layout: post
title:  "Grand Central Dispatch (GCD) in C++"
date:   2019-10-01 23:26:00
categories: technology, c++, event driven
---


[ DRAFT : I am very poor at explaining things. I am still editing this for clear description of what i did.. so come back again for more details of the big 400 line code]

I have been trying to make an event driven parallel (atleast with threads if not parallelism) execution layer that can be used by a software program.

The purpose of it is to provide the following methods.

    1. timers
    2. sockets / file descriptors
    3. signals
    4. any execution that required / can be allowed to be run in parallel or in a thread

So i define a singleton class `Gcd` as follows.

```cpp

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
        explicit Gcd();
};

```

now i have defined the uses of the Gcd as the member function of the above class. Since its a singleton, it can be used anywhere in the program. Thus, if there are mulitple files, the base can be directly used by the single instance.

Lets look at the `Create_Timer_Event`. It accepts 3 parameters namely `sec`, `usec` and `Timer_Fn`.

The first two are the timeouts that caller sets, the `Timer_Fn` is the callback set by the caller, it is to be called when the timer event occur.

Since this is a type `std::function<>`, the caller is flexible to register a callback in a regular function call or a member function can be registered as a callback.

```cpp

class my_class {
    public:
        void my_member() { }

        void register_timer() {
            Timer_Fn f = std::bind(&my_class::my_member, this);

            Gcd::Instance()->Create_Timer_Event(1, 0, f);
        }
};
```


For All the timers ,sockets, signals i am using file descriptors. Here's the advantage i have seen with them.

    1. it can be wait on `select` or `pselect` or `poll` or `epoll`
    2. all events are queued and delivered one by one
    3. one base function can be used to wait for all the above


so, i've chosen `timerfd` calls which are **LINUX only**. Your operating system might have something or other of these, which can be allowed to wait on `select`.

Each `Create_Timer_Event` call calls `timerfd_create` and returns a file descriptor referring to the timer, each call of `Create_Timer_Event` also has a callback that is to be called upon an expiry. So we need to store all this into a structure and store it in a vector. So we have a vector of timers.

Same apply to `Create_Socket_Event` call.

The `Create_Signal_Event` call is different. Because, one or more functions may require one signal


So i created a final version with above things in my mind.

Although my version does not take care of the following, but it fairly works without deep runs.


    1. timer may not work if usec is multiples of sec i.e. usec = n * sec. I should do a divide and add to sec and modulo to add to usec.
    2. lock guarantee in timer when a worker thread in the parallel called.
    3. thread scheduling is random and thus is not cache friendly and inefficient work distribution among threads.


Below is the full example of the `Gcd`: <script src="https://gist.github.com/DevNaga/07be345518682e19331e00fa1aeac9ed.js"></script>


