#pragma once

#include "pch.h"
#include <stdexcept>

template <class T>
class Singleton
{
public:
    static T& getInstance()
    {
        static T _instacne;

        if (s_deleted)
        {
            // this should never happen;
            // if it does, this implementation is incorrect
            throw std::runtime_error("Call of deleted MeyerSingleton detected!");
        }

        return _instacne;
    }

    static const T& getConstInstance()
    {
        return getInstance();
    }

    ~Singleton() { s_deleted = true; };

private:
    //Singleton() = default;
    //Singleton(const Singleton&) = delete;
    //Singleton& operator = (const Singleton&) = delete;
    static bool s_deleted;
};

template <class T>
bool Singleton<T>::s_deleted(false);