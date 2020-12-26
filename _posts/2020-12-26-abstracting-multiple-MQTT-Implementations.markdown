---
layout: post
title:  "Abstracting Multiple MQTT implementations"
date:   2020-12-18 23:26:00
categories: wireless, linux, programming
---

```cpp
/**
 * @brief - interface to various MQTT implementations
 * 
 * @copyright - 2020-present All rights reserved Devendra Naga (devendra.aaru@outlook.com)
 */
#ifndef __AUTO_OS_LIB_MQTT_IF_H__
#define __AUTO_OS_LIB_MQTT_IF_H__

#include <string>
#include <functional>
#include <memory>

namespace auto_os::lib {

enum class Mqtt_Impl : int {
    eMosquitto = 0,
    eNone,
};

typedef std::function<void(int)> connect_cb;
typedef std::function<void(std::string, uint8_t *, int)> message_rx_cb;

/**
 * @brief - implements interface to abstract most used MQTT protocols
 */
class Mqtt_Interface {
    public:
        explicit Mqtt_Interface() = default;
        ~Mqtt_Interface() = default;

        /**
         * @brief - setup a publisher
         * 
         * @param ipaddr - ip address of the MQTT broker
         * @paarm port - port of the broker
         * @param clean_session - set to true
         * @param keepalive - set to 60
         * 
         * @returns 0 on success -1 on failure
         */
        virtual int Setup_Publisher(const std::string ipaddr,
                                    int port,
                                    bool clean_session,
                                    int keepalive) = 0;

        /**
         * @brief - publish a message
         * 
         * @param topic - publishing topic
         * @param buf - messsage to publish
         * @param buf_size - message length
         * 
         * @returns 0 on success -1 on failure
         */
        virtual int Publish(const std::string topic,
                            uint8_t *buf,
                            size_t buf_size) = 0;

        /**
         * @brief - setup a subscriber
         * 
         * @param ipaddr - ip address of the MQTT broker
         * @param port - port of the broker
         * @param topic - topic to subscribe
         * @param clean_session -  set to true
         * @param keepalive - set to 60
         * @param connect_cb - connect callback to be called when connected
         * @param message_rx_cb - message callback to be called when some message from the publisher
         *
         * @returns 0 on success -1 on failure
         */
        virtual int Setup_Subscriber(const std::string ipaddr,
                                     int port,
                                     const std::string topic,
                                     bool clean_session,
                                     int keepalive,
                                     connect_cb conn,
                                     message_rx_cb msg) = 0;

        /**
         * @brief - runs the subscriber instance.. call it when in subscriber mode only in a thread
         */
        virtual void Run() = 0;

        /**
         * @brief - stop the MQTT instance
         */
        virtual void Stop() = 0;
};

/**
 * @brief - MQTT factory to create instances of Mqtt_Interfaces
 * 
 */
class Mqtt_Factory {
    public:
        /**
         * @brief - return a MQTT instance
         */
        static Mqtt_Factory *Instance() {
            static Mqtt_Factory fac;

            return &fac;
        }

        ~Mqtt_Factory() { }
        explicit Mqtt_Factory(const Mqtt_Factory &) = delete;
        const Mqtt_Factory &operator=(const Mqtt_Factory &) = delete;

        /**
         * @brief - create a mqtt interface based on the choice of MQTT lib
         * 
         * @param impl - MQTT implementation
         * 
         * @return returns pointer to mqtt_interface on success and 0 otherwise
         */
        std::shared_ptr<Mqtt_Interface> create(Mqtt_Impl impl);

    private:
        explicit Mqtt_Factory() { }
};

}

#endif

```
