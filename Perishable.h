//
// Created by craig on 04/04/25.
//

#ifndef PERISHABLE_H
#define PERISHABLE_H

#include <iostream>
#include <limits>

#include <sys/resource.h>

#include <boost/chrono.hpp>

template <typename T>
class Perishable
{
    typedef uint64_t TTickCount; // system tick count in msecs
public:
    explicit Perishable( const boost::chrono::milliseconds &lifeTime, T initialValue = std::numeric_limits<T>::quiet_NaN() ) :
        m_LifeTime( lifeTime.count() ),
        m_LastUpdate(),
        m_Value( initialValue )
    {
        if (m_Value != std::numeric_limits<T>::quiet_NaN())
        {
            m_LastUpdate = GetTickCount();
        }
    }

    virtual ~Perishable() {};

    void Set( T newValue )
    {
        m_Value = newValue;
        m_LastUpdate = GetTickCount();
    }

    T Get() const
    {
        TTickCount age = GetTickCount() - m_LastUpdate;
        if (age > m_LifeTime)
        {
            return std::numeric_limits<T>::quiet_NaN();
        }
        return m_Value;
    }

private:

    /**
     * returns the current tick count
     *
     * NOTE: this is unaffected by ntp, unlike system clock
     *
     * NOTE: tick count may be affected by clock frequency drift,
     * although this should be insignificant in heartbeat applications.
     *
     * @return the current tick count in milliseconds
     */
    static TTickCount GetTickCount()
    {
        struct timespec now = {};
        clock_gettime( CLOCK_MONOTONIC, &now );
        TTickCount tick = now.tv_nsec / 1000000;
        tick += now.tv_sec * 1000;
        return tick;
    }

    TTickCount m_LifeTime;
    TTickCount m_LastUpdate;
    T m_Value;
};

#endif //PERISHABLE_H
