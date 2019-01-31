#include "cxxtimer.hpp"
#include <string>

cxxtimer::Timer::Timer()
    : Timer {"Anonymous"}
{
}

cxxtimer::Timer::Timer(const std::string& name)
    : m_name {name}
    , m_started {false}
    , m_stopped {false}
    , m_reference {clock::now()}
    , m_accumulated {clock::duration::zero()}
{
}

void cxxtimer::Timer::start()
{
    if (!m_started)
    {
        m_started = true;
        m_stopped = false;
        m_reference = clock::now();
        m_accumulated = clock::duration::zero();
    }
    else if (m_stopped)
    {
        m_stopped = false;
        m_reference = clock::now();
    }
}

void cxxtimer::Timer::stop()
{
    if (m_started && !m_stopped)
    {
        m_accumulated += clock::now() - m_reference;
        m_stopped = true;
    }
}

void cxxtimer::Timer::reset()
{
    if (m_started)
    {
        m_started = false;
        m_stopped = false;
        m_reference = clock::now();
        m_accumulated = clock::duration::zero();
    }
}

std::string cxxtimer::Timer::name() const
{
    return m_name;
}
