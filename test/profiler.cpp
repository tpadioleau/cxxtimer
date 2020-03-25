#include <cxxtimer/profiler.hpp>

#include <iostream>
#include <chrono>
#include <thread>

int main(int argc, char** argv)
{
    cxxtimer::Profiler profiler ("task:0");

    profiler.start();
    {
        profiler.push("task:1");
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
        profiler.pop();

        profiler.stop();
        print(std::cout, profiler.root());
        profiler.start();

        profiler.push("task:2");
        {
            profiler.push("task:20");
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            profiler.pop().push("task:21");
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
            profiler.pop();
        }
        profiler.pop();
    }
    profiler.stop();

    print(std::cout, profiler.root());

    return 0;
}
