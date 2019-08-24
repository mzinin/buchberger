#pragma once

#include <iostream>


class Timer
{
public:
    void start();
    void stop();

    double userTime() const;
    double sysTime() const;
    double realTime() const;

private:
    static double sHZ_;

    double userTime_ = 0.0;
    double sysTime_ = 0.0;
    double realTime_ = 0.0;

    double userElapsed_ = 0.0;
    double sysElapsed_ = 0.0;
    double realElapsed_ = 0.0;
};

std::ostream& operator<<(std::ostream& out, const Timer& timer);
