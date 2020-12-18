---
layout: post
title:  "Message passing between threads or between difference classes in C++"
date:   2020-12-18 23:26:00
categories: wireless, linux, programming
---

Messaage passing is a mechanism of communicating between the threads or different processes in an OS. But in general when it comes to C++, each of the classes that are independent to each other or doing different work, must also require some message exchange between them.

This can be created with the existing c++ STL utilities in C++11 or above. Few ofwhich those ingridents needed for this are,

1. `std::queue` for storing the messages
2. `std::thread` for calling subscribers upon data reception
3. `std::function` for registering subscribers
4. templates, simpler and basic ones for generic message type

the data could be of anytype so we gonna need templates.

A simpler and easier design pattern that is singleton is used by the Message_Queue we are creating. So a  sample prototype of it will look like below,

```cpp
template <class T>
class Message_Queue {
    public:
        ~Message_Queue();
        explicit Message_Queue(const Message_Queue &) = delete;
        const Message_Queue &operator=(const Message_Queue &) = delete;
        
        /**
         * @brief - publish a value / content
         */
        int Publish(T &val);
        
        /**
         * @brief - subscribe to the data
         */
        void Subscribe(std::function<void(T)> callback);
        
        static Message_Queue<T> *Instance() {
            static Message_Queue<T> t;
            
            return &t;
        }
        
    private:
        std::queue<T> msg_q_;
        std::mutex lock_;
        std::condition_variable cond_;
        std::vector<std::function<void(T)>> sub_;
        std::unique_ptr<std::thread> callback_thr_;
        void Caller_Thread();
        
        explicit Message_Queue() { }
};

```

Above class illustrates a Message_Queue which can be shared between any classes with using the `Instance()` member function.

The `Publish` member function will set data into the queue pointed by the `std::queue` type. So the example code would,

```cpp
void Publish(T &val) {
   std::unique_lock<std::mutex> lock(lock_);
   
   msg_q_.push(val);
   cond_.notify_all();
}

```

The `Subscribe` member function will set the callbacks to the vector of callbacks.

```cpp
void Subscribe(std::function<void(T)> sub_callback) {
    sub_.emplace_back(sub_callback);
}

```

The `Caller_Thread` when initiated, will wait on the `condition_variable cond_` till the publisher publishes something.

Caller then awokens when the publisher pushes data into the queue, and for each subscriber in the subscribers, it starts calling them with the received data.

Like so,

```cpp
void Caller_Thread()
{
    while (1) {
    
        {
            std::unique_lock<std::mutex> lock(lock_);
  
            cond_.wait();
        }
        
        int q_len = msg_q_.size();
        while (q_len > 0) {
            auto elem = msg_q_.front();
            for (auto sub : sub_) {
                sub(elem);
            }
            msg_q_.pop();
            q_len = msg_q_.size();
        }
    }
}

```



a full source code example below.

<script src="https://gist.github.com/madmax440/5b23ca2bc3abe3797003c107b0fb8d93.js"></script>
