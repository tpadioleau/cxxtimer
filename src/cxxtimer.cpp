#include "cxxtimer/cxxtimer.hpp"
#include <string>

namespace cxxtimer
{

Timer::Timer( const std::string& name )
    : m_name( name )
{
}

void
Timer::start()
{
    if ( m_stopped )
    {
        m_reference = clock::now();
        m_stopped = false;
    }
}

void
Timer::stop()
{
    if ( !m_stopped )
    {
        m_accumulated += clock::now() - m_reference;
        m_stopped = true;
    }
}

void
Timer::reset()
{
    m_stopped = true;
    m_reference = clock::now();
    m_accumulated = clock::duration::zero();
    m_num_calls = 0;
}

bool
Timer::is_started() const
{
    return !m_stopped;
}

void
Timer::increment_num_calls()
{
    m_num_calls++;
}

std::size_t
Timer::num_calls() const
{
    return m_num_calls;
}

std::string
Timer::name() const
{
    return m_name;
}

} // namespace cxxtimer
