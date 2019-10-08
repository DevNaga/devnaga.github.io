---
layout: post
title:  "Grand Central Dispatch (GCD) in C++"
date:   2019-10-01 23:26:00
categories: technology, c++, event driven
---


[ **DRAFT : I am very poor at explaining things. I am still editing this for clear description of what i did.. so come back again for more details of the big 400 line code** ]

I have been trying to make an event driven parallel (atleast with threads if not parallelism) execution layer that can be used by a software program.

The purpose of it is to provide the following methods.

1. timers
2. sockets / file descriptors
3. signals
4. any execution that required / can be allowed to be run in parallel or in a thread

A modern software program requires any or all of the above 4 patterns for it to work in a connected and communication environment. Talk about Microservices in cloud, multi process systems (which are msot common) requires any of the above patterns / methods and so this common layer came up.

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

The `Create_Signal_Event` call is different. Because, one or more functions may depend or use a common signal. So instead of one callback, we have a vector of callbacks.

The `signal_fd` trick
===================

The thing with the `signalfd` system call is that one process can have many `signalfd` file descriptors it wants - but complicates the handling and masking settings. So i came up with the following setting.


1. empty out signals
2. create `signalfd`
3. set to the fdset.

A call to the `Create_Signal_Event` is made.

1. reset the `signalfd` from the mask.
2. pass the previously got fd into the call to `signalfd` system call.
3. set `sigprocmask` to blocking.
4. set new fd to fdset.


Parallel execution
================

In the cloud or on premise devices we see always there is atleast an intel dual core or an ARM dual core machine or a single core with HT enabled. To exploit multicore some use threads, so to avoid creating threads everytime when a need arise we keep a pool of them.

So the definition of it looks as follows,


```cpp

// a process that is instantiated by the Parallel
class Parallel_Proc {
    public:
        explicit Parallel_Proc();
        ~Parallel_Proc();

        void Queue_Work(Job_Fn job);
        void Stop()
        {
            std::unique_lock<std::mutex> lock(lock_);
            stop_signal_ = true;
            printf("Gcd: stop the process\n");
            cv_.notify_one();
        }

    private:
        void Process();
        std::unique_ptr<std::thread> proc_;
        std::queue<Job_Fn> jobs_;
        std::mutex lock_;
        std::condition_variable cv_;
        bool stop_signal_ = false;
};

class Parallel {
    public:
        explicit Parallel();
        ~Parallel();

        void Queue_Work(Job_Fn job);
        void StopAll()
        {
            for (auto it : procs_) {
                it->Stop();
            }
        }
    private:
        std::vector<std::shared_ptr<Parallel_Proc>> procs_;
        int n_threads_;
};

```

See the `Parallel_Proc` define a thread of execution that is created to execute jobs requsted by the caller. So we wouldn't want the user to manage this and thus the `Parallel` class abstract away the `Parallel_Proc` managing the creation and Queueing and providing simple API that abstract the jobs of all the threads.

my system had 4 threads and thus i create that many instances of `Parallel_Proc` and set `n_threads_` to `std::thread::hardware_concurrency()`.

Each instance then starts a thread that runs the `Process` function. The main logic in this is very important.

1. wait on the condition variable
2. watch for any work being queued ?
3. if queued, deque the top and pop it.
4. execute the work.

the thread also watches if the user initiated a `Gcd` shutdown by invoking the `Terminate()` method. If the bit is being set, then the thread breaks out of the loop and stopping itself.

A sample snippet of it is below.

```cpp

void Parallel_Proc::Process()
{
    sigset_t block;

    // block termination signals ..temporarily
    sigemptyset(&block);
    sigaddset(&block, SIGINT);
    sigprocmask(SIG_BLOCK, &block, 0);
    while (1) {
        Job_Fn job_to_do = nullptr;

        {
            std::unique_lock<std::mutex> lock(lock_);
            cv_.wait(lock);
            if (stop_signal_) {
                break;
            }
            job_to_do = jobs_.front();
            jobs_.pop();
        }

        if (job_to_do)
            job_to_do();
    }
}

```

this also mean that the abstraction `Parallel` would need to schedule a job on particular thread. This can be on the following factors.


1. weights based on the time required to run this particular job
2. simple count based system where assign a counter to each thread and increment it when a work added to the thread queue

for now, we keep things simple and generate a random number between 0 and number of threads created and schedule the work on the thread. I know this results in scheduling inefficiency and so we will improve upon this Gcd implementation in the coming versions.

So my queueing would simply look below.

```cpp

// queue work on a random thread
void Parallel::Queue_Work(Job_Fn job)
{
    int thread_id = rand() % n_threads_;

    procs_[thread_id]->Queue_Work(job);
}

```



So i created a final version with all 4 of the above things in my mind.



Below is the full example of the `Gcd`: <script src="https://gist.github.com/DevNaga/07be345518682e19331e00fa1aeac9ed.js"></script>


Some more improvements can be made on the above `Gcd` implementation.


1. timer may not work if usec is multiples of sec i.e. usec = n * sec. I should do a divide and add to sec and modulo to add to usec.
2. lock guarantee in timer when a worker thread in the parallel called.
3. thread scheduling is random and thus is not cache friendly and inefficient work distribution among threads.
4. execute timer, socket and signal callbacks in the workers. Signals may not be the right ones to be executed within worker threads however.
5. block all signals within workers and only allow the main thread handle the signals.

