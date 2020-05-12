/*

MIT License

Copyright (c) 2020 Thomas Padioleau

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

#include "cxxtimer/Timer.hpp"

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace cxxtimer
{

class TimerNode;

void print( std::ostream& os, const TimerNode& node, double threshold );

class TimerNode
{
public:
    TimerNode() = default;

    TimerNode( const std::string& name );

    TimerNode( const TimerNode& x ) = default;

    TimerNode( TimerNode&& x ) = default;

    virtual ~TimerNode() = default;

    TimerNode& operator=( const TimerNode& x ) = default;

    TimerNode& operator=( TimerNode&& x ) = default;

    Timer timer;
    std::map< std::string, TimerNode > nodes;
};

class Profiler
{
public:
    Profiler() = default;

    Profiler( const Profiler& x );

    Profiler( Profiler&& x ) = default;

    virtual ~Profiler() = default;

    Profiler& operator=( const Profiler& x );

    Profiler& operator=( Profiler&& x ) = default;

    void reset( const std::string& name );

    Profiler& push( const std::string& name );

    Profiler& pop();

    void start();

    void stop();

    const TimerNode& root() const;

private:
    TimerNode m_root;
    std::vector< TimerNode* > m_active_timer_nodes;
};

} // namespace cxxtimer
