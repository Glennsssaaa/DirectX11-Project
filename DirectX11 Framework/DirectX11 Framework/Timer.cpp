#include "Timer.h"

Timer::Timer()
{
    start = std::chrono::high_resolution_clock::now();
    stop = std::chrono::high_resolution_clock::now();
}

double Timer::GetMillisecondsElapsed()
{
    if (isrunning) {
        auto elapsed = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - start);
        return elapsed.count();
    }
    else {
        auto elapsed = std::chrono::duration<double, std::milli>(stop - start);
        return elapsed.count();
    }
    return 0.0;
}

void Timer::Restart()
{
    isrunning = true;
    start = std::chrono::high_resolution_clock::now();
}

bool Timer::Stop()
{
    if (!isrunning) {
        return false;
    }
    else {
        stop = std::chrono::high_resolution_clock::now();
        isrunning = false;
        return true;
    }
}

bool Timer::Start()
{
    if (isrunning) {
        return false;
    }
    else {
        isrunning = true;
        start = std::chrono::high_resolution_clock::now();
        return true;
    }
}
