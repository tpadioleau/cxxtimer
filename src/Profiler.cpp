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

std::list< const TimerNode* >
sort( const std::map< std::string, TimerNode >& nodes )
{
    std::list< const TimerNode* > sorted_nodes;
    for ( const auto& node : nodes )
    {
        sorted_nodes.push_back( &node.second );
    }
    sorted_nodes.sort( []( const TimerNode* node1, const TimerNode* node2 ) {
        return ( node1->timer.duration() > node2->timer.duration() );
    } );
    return sorted_nodes;
}

void
print_impl( std::ostream& os, const TimerNode& parent, double t_root, int level,
            double threshold, const std::string& prefix )
{
    auto t_parent = parent.timer.seconds();

    auto sorted_nodes = sort( parent.nodes );
    sorted_nodes.remove_if( [ & ]( const TimerNode* node ) {
        return !( ( node->timer.seconds() / t_root * 100.0 ) > threshold );
    } );

    while ( !sorted_nodes.empty() )
    {
        const TimerNode* node = sorted_nodes.front();
        auto t_node = node->timer.seconds();
        os << std::left << std::setw( 25 )
           << prefix + ( sorted_nodes.size() > 1 ? "|- " : "\\- " ) +
                  node->timer.name();
        os << std::right << std::fixed << std::setprecision( 2 );
        os << std::setw( 25 ) << t_node;
        os << std::setw( 25 ) << t_node / node->timer.num_calls();
        os << std::setw( 25 ) << t_node / t_parent * 100.0;
        os << std::setw( 25 ) << t_node / t_root * 100.0;
        os << std::endl;

        print_impl( os, *node, t_root, level + 1, threshold,
                    prefix + ( sorted_nodes.size() > 1 ? "|  " : "   " ) );

        sorted_nodes.pop_front();
    }
}

void
print( std::ostream& os, const TimerNode& root, double threshold )
{
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

    auto t_root = root.timer.seconds();
    os << std::left << std::setw( 25 ) << root.timer.name();
    os << std::right << std::fixed << std::setprecision( 2 );
    os << std::setw( 25 ) << t_root;
    os << std::setw( 25 ) << t_root / root.timer.num_calls();
    os << std::setw( 25 ) << 100.0;
    os << std::setw( 25 ) << 100.0;
    os << std::endl;

    print_impl( os, root, t_root, 1, threshold, "" );
}

TimerNode::TimerNode( const std::string& name )
    : timer( name )
{
}

Profiler::Profiler( const Profiler& x )
    : m_root( x.m_root )
{
    TimerNode* parent = &m_root;
    for ( const TimerNode* timernode_p : x.m_active_timer_nodes )
    {
        m_active_timer_nodes.push_back(
            &( parent->nodes.at( timernode_p->timer.name() ) ) );
        parent = m_active_timer_nodes.back();
    }
}

Profiler&
Profiler::operator=( const Profiler& x )
{
    m_root = x.m_root;
    TimerNode* parent = &m_root;
    for ( const TimerNode* timernode_p : x.m_active_timer_nodes )
    {
        m_active_timer_nodes.push_back(
            &( parent->nodes.at( timernode_p->timer.name() ) ) );
        parent = m_active_timer_nodes.back();
    }
    return *this;
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

const TimerNode&
Profiler::root() const
{
    return m_root;
}

} // namespace cxxtimer
