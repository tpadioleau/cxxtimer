#include "cxxtimer/Timer.hpp"
#include <string>

namespace cxxtimer
{

Timer::Timer( std::string name )
    : m_name( std::move( name ) )
{
}

void
Timer::start()
{
    if ( !m_is_started )
    {
        m_reference = clock::now();
        m_is_started = true;
    }
}

void
Timer::stop()
{
    if ( m_is_started )
    {
        m_accumulated += clock::now() - m_reference;
        m_is_started = false;
    }
}

void
Timer::reset()
{
    m_is_started = false;
    m_reference = clock::now();
    m_accumulated = clock::duration::zero();
    m_num_calls = 0;
}

bool
Timer::is_started() const
{
    return m_is_started;
}

Timer::clock::duration
Timer::duration() const
{
    clock::duration duration = m_accumulated;
    if ( m_is_started )
    {
        duration += clock::now() - m_reference;
    }
    return duration;
}

double
Timer::seconds() const
{
    auto duration = std::chrono::duration< double >( Timer::duration() );
    return duration.count();
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
