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


#include <chrono>
#include <cstdlib>
#include <cxxtimer/cxxtimer.hpp>
#include <iostream>
#include <string>

int main()
{
    // Instantiate cxxtimer::Timer object
    cxxtimer::Timer timer {"Example"};

    // Start the timer
    timer.start();

    // Wait for the users
    std::cout << "Please, type something and press ENTER to continue: ";
    std::string input;
    std::getline(std::cin, input);

    // Stop/pause the timer
    timer.stop();

    // Get the elapsed time
    std::cout << "Timer \"" << timer.name() << "\" measured " << timer.count<std::chrono::seconds>() << " seconds." << std::endl;
    std::cout << "Timer \"" << timer.name() << "\" measured " << timer.count<std::chrono::duration<double>>() << " seconds." << std::endl;
    std::cout << "Timer \"" << timer.name() << "\" measured " << timer.count() << " milliseconds." << std::endl;
    std::cout << "Timer \"" << timer.name() << "\" measured " << timer.count<std::chrono::nanoseconds>() << " nanoseconds." << std::endl;

    return EXIT_SUCCESS;
}
