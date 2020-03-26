#include "cxxtimer/profiler.hpp"

#include <algorithm>
#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <thread>
#include <vector>

namespace cxxtimer
{

std::list<std::string> sort(std::map<std::string, std::shared_ptr<TimerNode>>& nodes)
{
    std::list<std::string> sorted_keys;
    for (const auto& node : nodes)
    {
        sorted_keys.push_back(node.first);
    }
    sorted_keys.sort([&nodes](const std::string& key1, const std::string& key2)
                     {
                         return (nodes.at(key1)->timer.count<std::chrono::nanoseconds>() >
                                 nodes.at(key2)->timer.count<std::chrono::nanoseconds>());
                     });
    return sorted_keys;
}

void print_impl(std::ostream& os, TimerNode& node, double t_root, double t_parent,
                int level, double threshold)
{
    using float_duration = std::chrono::duration<double>;
    double t_node = node.timer.count<float_duration>();
    if (t_node/t_root*100.0 > threshold)
    {
        os << std::left << std::setw(25) << std::string(level, ' ')+node.timer.name();
        os << std::right << std::fixed << std::setprecision(2);
        os << std::setw(25) << t_node;
        os << std::setw(25) << t_node/t_parent*100.0;
        os << std::setw(25) << t_node/t_root*100.0;
        os << std::endl;

        auto sorted_keys = sort(node.nodes);
        for (const auto& key : sorted_keys)
        {
            print_impl(os, *node.nodes.at(key), t_root, t_node, level+1, threshold);
        }
    }
}

void print(std::ostream& os, TimerNode& root, double threshold)
{
    using float_duration = std::chrono::duration<double>;
    os << std::left << std::setw(25);
    if (threshold > 0.0)
    {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1);
        oss << "Timer name [> " << threshold << "%]";
        os << oss.str();
    }
    else
    {
        os << "Timer name";
    }

    os << std::right;
    os << std::setw(25) << "Time (s)";
    os << std::setw(25) << "Rel. to parent (%)";
    os << std::setw(25) << "Rel. to \'"+root.timer.name()+"\' (%)";
    os << std::endl;
    print_impl(os, root, root.timer.count<float_duration>(), root.timer.count<float_duration>(),
               0, threshold);
}


TimerNode::TimerNode(const std::string& name)
    : timer ( name )
{
}


Profiler::Profiler(const std::string& name)
    : m_root ( std::make_shared<TimerNode> (name) )
{
}

void Profiler::reset(const std::string &name)
{
    m_root = std::make_shared<TimerNode>(name);
    m_active_nodes.clear();
    m_active_nodes.push_back(m_root.get());
}

Profiler& Profiler::push(const std::string& name)
{
    auto node = std::make_shared<TimerNode>(name);

    auto tuple = m_active_nodes.back()->nodes.insert(std::make_pair(name, node));
    // If not inserted, retrieves existing node
    if (!tuple.second)
    {
        node = tuple.first->second;
    }

    m_active_nodes.push_back(node.get());

    if (m_root->timer.is_started())
    {
        node->timer.start();
    }
    return *this;
}

Profiler& Profiler::pop()
{
    if (m_active_nodes.back() != m_root.get())
    {
        m_active_nodes.back()->timer.stop();
        m_active_nodes.pop_back();
    }
    return *this;
}

void Profiler::start()
{
    std::for_each(m_active_nodes.cbegin(), m_active_nodes.cend(),
                  [] (TimerNode* node)
                  {
                      node->timer.start();
                  });
}

void Profiler::stop()
{
    std::for_each(m_active_nodes.crbegin(), m_active_nodes.crend(),
                  [] (TimerNode* node)
                  {
                      node->timer.stop();
                  });
}

TimerNode& Profiler::root() const
{
    return *m_root;
}

}  // cxxtimer
