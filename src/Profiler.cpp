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
sort( const std::map< std::string, std::shared_ptr< TimerNode > >& nodes )
{
    std::list< std::string > sorted_keys;
    for ( const auto& node : nodes )
    {
        sorted_keys.push_back( node.first );
    }
    sorted_keys.sort( [ &nodes ]( const std::string& key1,
                                  const std::string& key2 ) {
        return ( nodes.at( key1 )->timer.count< std::chrono::nanoseconds >() >
                 nodes.at( key2 )->timer.count< std::chrono::nanoseconds >() );
    } );
    return sorted_keys;
}

void
print_impl( std::ostream& os, const TimerNode& parent, double t_root, int level,
            double threshold )
{
    using float_duration = std::chrono::duration< double >;
    auto t_parent = parent.timer.count< float_duration >();
    auto sorted_keys = sort( parent.nodes );
    sorted_keys.remove_if( [ & ]( const std::string& str ) {
        return !( ( parent.nodes.at( str )->timer.count< float_duration >() /
                    t_root * 100.0 ) > threshold );
    } );
    while ( !sorted_keys.empty() )
    {
        const auto& node = parent.nodes.at( sorted_keys.front() );
        double t_node = node->timer.count< float_duration >();
        os << std::left << std::setw( 25 )
           << std::string( level, ' ' ) + node->timer.name();
        os << std::right << std::fixed << std::setprecision( 2 );
        os << std::setw( 25 ) << t_node;
        os << std::setw( 25 ) << t_node / node->timer.num_calls();
        os << std::setw( 25 ) << t_node / t_parent * 100.0;
        os << std::setw( 25 ) << t_node / t_root * 100.0;
        os << std::endl;

        print_impl( os, *node, t_root, level + 1, threshold );

        sorted_keys.pop_front();
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

    auto t_root = root.timer.count< float_duration >();
    os << std::left << std::setw( 25 ) << root.timer.name();
    os << std::right << std::fixed << std::setprecision( 2 );
    os << std::setw( 25 ) << t_root;
    os << std::setw( 25 ) << t_root / root.timer.num_calls();
    os << std::setw( 25 ) << 100.0;
    os << std::setw( 25 ) << 100.0;
    os << std::endl;

    print_impl( os, root, t_root, 1, threshold );
}

TimerNode::TimerNode( const std::string& name )
    : timer( name )
{
}

void
Profiler::clear()
{
    m_active_timer_nodes.clear();
}

Profiler&
Profiler::push( const std::string& name )
{
    if ( m_active_timer_nodes.empty() )
    {
        m_active_timer_nodes.push_back( std::make_shared< TimerNode >( name ) );
        m_active_timer_nodes.back()->timer.start();
        m_active_timer_nodes.back()->timer.increment_num_calls();
    }
    else
    {
        auto tuple = m_active_timer_nodes.back()->nodes.emplace(
            name, std::make_shared< TimerNode >( name ) );

        // If not inserted, retrieves existing node
        m_active_timer_nodes.push_back( tuple.first->second );

        if ( m_active_timer_nodes.front()->timer.is_started() )
        {
            m_active_timer_nodes.back()->timer.start();
            m_active_timer_nodes.back()->timer.increment_num_calls();
        }
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
    std::for_each( m_active_timer_nodes.cbegin(), m_active_timer_nodes.cend(),
                   []( const std::shared_ptr< TimerNode >& node ) {
                       node->timer.start();
                   } );
}

void
Profiler::stop()
{
    std::for_each( m_active_timer_nodes.crbegin(), m_active_timer_nodes.crend(),
                   []( const std::shared_ptr< TimerNode >& node ) {
                       node->timer.stop();
                   } );
}

const std::shared_ptr< TimerNode >&
Profiler::root() const
{
    return m_active_timer_nodes.front();
}

} // namespace cxxtimer
