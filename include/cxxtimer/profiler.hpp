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

#include "cxxtimer/cxxtimer.hpp"

#include <iostream>
#include <map>
#include <memory>
#include <list>
#include <string>

namespace cxxtimer
{

class TimerNode;

void print(std::ostream& os, const TimerNode& node, double threshold);

class TimerNode
{
public:
    TimerNode() = default;

    TimerNode(const std::string& name);

    TimerNode(const TimerNode& x) = default;

    TimerNode(TimerNode&& x) = default;

    virtual ~TimerNode() = default;

    TimerNode& operator=(const TimerNode& x) = default;

    TimerNode& operator=(TimerNode&& x) = default;

    Timer timer;
    std::map<std::string, std::shared_ptr<TimerNode>> nodes;
};

class Profiler
{
public:
    Profiler() = default;

    Profiler(const Profiler& x) = default;

    Profiler(Profiler&& x) = default;

    virtual ~Profiler() = default;

    Profiler& operator=(const Profiler& x) = default;

    Profiler& operator=(Profiler&& x) = default;

    void clear();

    Profiler& push(const std::string& name);

    Profiler& pop();

    void start();

    void stop();

    const std::list<std::shared_ptr<TimerNode>>& active_timer_nodes() const;

private:
    std::list<std::shared_ptr<TimerNode>> m_active_timer_nodes;
};

} // cxxtimer
