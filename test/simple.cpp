/*

MIT License

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

#include <chrono>
#include <cstdlib>
#include <cxxtimer/Timer.hpp>
#include <iostream>
#include <thread>

int
main()
{
    // Instantiate cxxtimer::Timer object
    cxxtimer::Timer timer;

    std::cout << "Waiting for 50ms...";

    auto fifty_ms = std::chrono::milliseconds( 50 );

    // Start the timer
    timer.start();

    std::this_thread::sleep_for( fifty_ms );

    // Stop/pause the timer
    timer.stop();

    auto seconds = timer.seconds();

    std::cout << " done\n";
    std::cout << "Timer shows " << seconds << "s\n";

    if ( seconds > 0.051 || seconds < 0.049 )
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
