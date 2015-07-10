#pragma once

namespace rain
{

template <typename T>
class Singleton
{
protected:
    Singleton() = default;

    Singleton(Singleton const &) = delete;
    Singleton(Singleton &&) = delete;

    Singleton & operator = (Singleton const &) = delete;
    Singleton & operator = (Singleton &&) = delete;

public:
    static T & get_instance()
    {
        static T t;
        return t;
    }
};

} // !namespace rain