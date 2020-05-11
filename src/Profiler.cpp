#include "cxxtimer/Profiler.hpp"

#include <algorithm>
#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <thread>
#include <vector>

namespace cxxtimer
{

std::list< std::string >
sort( const std::map< std::string, TimerNode >& nodes )
{
    std::list< std::string > sorted_keys;
    for ( const auto& node : nodes )
    {
        sorted_keys.push_back( node.first );
    }
    sorted_keys.sort( [ &nodes ]( const std::string& key1,
                                  const std::string& key2 ) {
        return ( nodes.at( key1 ).timer.count< std::chrono::nanoseconds >() >
                 nodes.at( key2 ).timer.count< std::chrono::nanoseconds >() );
    } );
    return sorted_keys;
}

void
print_impl( std::ostream& os, const TimerNode& node, double t_root,
            double t_parent, int level, double threshold,
            const std::string& prefix, const std::string& prefix2 )
{
    using float_duration = std::chrono::duration< double >;
    double t_node = node.timer.count< float_duration >();
    if ( t_node / t_root * 100.0 > threshold )
    {
        os << std::left << std::setw( 25 )
           << ( prefix + prefix2 + node.timer.name() );
        os << std::right << std::fixed << std::setprecision( 2 );
        os << std::setw( 25 ) << t_node;
        os << std::setw( 25 ) << t_node / node.timer.num_calls();
        os << std::setw( 25 ) << t_node / t_parent * 100.0;
        os << std::setw( 25 ) << t_node / t_root * 100.0;
        os << std::endl;

        auto sorted_keys = sort( node.nodes );
        while ( !sorted_keys.empty() )
        {
            std::string prefix_child = prefix + ( level > 0 ? "|   " : "" );
            std::string prefix2_child = ( sorted_keys.size() > 1 ? "|-- " : "\\-- " );
            print_impl( os, node.nodes.at( sorted_keys.front() ), t_root,
                        t_node, level + 1, threshold, prefix_child,
                        prefix2_child );
            sorted_keys.pop_front();
        }
    }
}

void
print( std::ostream& os, const TimerNode& root, double threshold )
{
    using float_duration = std::chrono::duration< double >;
    os << std::left << std::setw( 25 );
    if ( threshold > 0.0 )
    {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision( 1 );
        oss << "Timer name [> " << threshold << "%]";
        os << oss.str();
    }
    else
    {
        os << "Timer name";
    }

    os << std::right;
    os << std::setw( 25 ) << "Time (s)";
    os << std::setw( 25 ) << "Mean time per call (s)";
    os << std::setw( 25 ) << "Rel. to parent (%)";
    os << std::setw( 25 ) << "Rel. to \'" + root.timer.name() + "\' (%)";
    os << std::endl;
    print_impl( os, root, root.timer.count< float_duration >(),
                root.timer.count< float_duration >(), 0, threshold, "", "" );
}

TimerNode::TimerNode( const std::string& name )
    : timer( name )
{
}

void
Profiler::reset( const std::string& name )
{
    m_root = TimerNode( name );
    m_root.timer.increment_num_calls();
}

Profiler&
Profiler::push( const std::string& name )
{
    TimerNode* parent;
    if ( m_active_timer_nodes.empty() )
    {
        parent = &m_root;
    }
    else
    {
        parent = m_active_timer_nodes.back();
    }

    auto tuple = parent->nodes.emplace( name, name );
    auto child = &tuple.first->second;

    // If not inserted, retrieves existing node
    m_active_timer_nodes.push_back( child );

    if ( parent->timer.is_started() )
    {
        child->timer.start();
        child->timer.increment_num_calls();
    }

    return *this;
}

Profiler&
Profiler::pop()
{
    if ( !m_active_timer_nodes.empty() )
    {
        m_active_timer_nodes.back()->timer.stop();
        m_active_timer_nodes.pop_back();
    }
    return *this;
}

void
Profiler::start()
{
    m_root.timer.start();
    std::for_each( m_active_timer_nodes.cbegin(), m_active_timer_nodes.cend(),
                   []( TimerNode* node ) { node->timer.start(); } );
}

void
Profiler::stop()
{
    std::for_each( m_active_timer_nodes.crbegin(), m_active_timer_nodes.crend(),
                   []( TimerNode* node ) { node->timer.stop(); } );
    m_root.timer.stop();
}

} // namespace cxxtimer
