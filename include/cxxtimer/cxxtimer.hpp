/*

MIT License

Copyright (c) 2017 André L. Maravilha
Copyright (c) 2019 Thomas Padioleau

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#pragma once

#include <chrono>
#include <string>

namespace cxxtimer
{

/**
 * This class works as a stopwatch.
 */
class Timer
{
public:
    /**
     * Constructor.
     */
    Timer() = default;

    /**
     * Constructor.
     *
     * @param   name
     *          Sets the name of this timer.
     */
    Timer( const std::string& name );

    /**
     * Copy constructor.
     *
     * @param   x
     *          The object to be copied.
     */
    Timer( const Timer& x ) = default;

    /**
     * Move constructor.
     *
     * @param   x
     *          The object to be transfered.
     */
    Timer( Timer&& x ) = default;

    /**
     * Destructor.
     */
    virtual ~Timer() = default;

    /**
     * Copy assignment operator.
     *
     * @param   x
     *          The object to be copied.
     *
     * @return  A reference to this object.
     */
    Timer& operator=( const Timer& x ) = default;

    /**
     * Move assignment operator.
     *
     * @param   x
     *          The object to be transferred.
     *
     * @return  A reference to this object.
     */
    Timer& operator=( Timer&& x ) = default;

    /**
     * Starts the timer.
     */
    void start();

    /**
     * Stops the timer.
     */
    void stop();

    /**
     * Resets the timer.
     */
    void reset();

    /**
     * Returns true if started
     */
    bool is_started() const;

    /**
     * Increments number of calls
     */
    void increment_num_calls();

    /**
     * Returns number of calls
     */
    std::size_t num_calls() const;

    /**
     * Gets name of the timer
     */
    std::string name() const;

    /**
     * Returns the elapsed time.
     *
     * @param   duration_t
     *          The duration type used to return the time elapsed. If not
     *          specified, it returns the time as represented by
     *          std::chrono::milliseconds.
     *
     * @return  The elapsed time.
     */
    template < class duration_t = std::chrono::milliseconds >
    typename duration_t::rep count() const;

private:
    using clock = std::chrono::steady_clock;

    std::string m_name = "Unknown";
    bool m_stopped = true;
    clock::time_point m_reference = clock::now();
    clock::duration m_accumulated = clock::duration::zero();
    std::size_t m_num_calls = 0;
};

template < class duration_t >
typename duration_t::rep
cxxtimer::Timer::count() const
{
    clock::duration duration = m_accumulated;
    if ( !m_stopped )
    {
        duration += clock::now() - m_reference;
    }
    return std::chrono::duration_cast< duration_t >( duration ).count();
}

} // namespace cxxtimer
