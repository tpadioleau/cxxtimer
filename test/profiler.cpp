#include <cxxtimer/Profiler.hpp>

#include <chrono>
#include <iostream>
#include <thread>

int
main( int argc, char** argv )
{
    cxxtimer::Profiler profiler;

    profiler.push( "task:0" );
    {
        profiler.push( "task:1" );
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
        profiler.pop();

        profiler.push( "task:2" );
        for ( int i = 0; i < 4; ++i )
        {
            profiler.push( "task:21" );
            std::this_thread::sleep_for( std::chrono::milliseconds( 50 ) );
            profiler.pop().push( "task:22" ); // To have < 1% timer
            profiler.pop().push( "task:23" );
            std::this_thread::sleep_for( std::chrono::milliseconds( 30 ) );
            profiler.pop();
            std::this_thread::sleep_for( std::chrono::milliseconds( 20 ) );
            profiler.stop();
            print( std::cout, *profiler.root(), 1.0 );
            profiler.start();
        }
        profiler.pop();
    }
    profiler.stop();

    std::cout << "Full profile\n";
    print( std::cout, *profiler.root(), -1.0 );
    profiler.pop();

    return 0;
}
