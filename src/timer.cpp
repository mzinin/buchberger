#include "timer.h"

#ifdef _WIN32
    #ifndef _WIN32_WINNT            // Allow use of features specific to Windows 2000 or later.
        #define _WIN32_WINNT 0x0500
    #endif
    #include <windows.h>
#else
    #include <unistd.h>
    #include <sys/times.h>
#endif //_WIN32

#include <iomanip>


#ifdef _WIN32

double Timer::sHZ_ = 10000000.0;

void Timer::start()
{
    userElapsed_ = 0.0;
    sysElapsed_ = 0.0;
    realElapsed_ = 0.0;

    ULARGE_INTEGER cTime, eTime, kTime, uTime, rTime;

    ::GetProcessTimes(::GetCurrentProcess(), (LPFILETIME)&cTime, (LPFILETIME)&eTime, (LPFILETIME)&kTime, (LPFILETIME)&uTime);
    ::GetSystemTimeAsFileTime((LPFILETIME)&rTime);

    userTime_ = uTime.QuadPart/sHZ_;
    sysTime_ = kTime.QuadPart/sHZ_;
    realTime_ = rTime.QuadPart/sHZ_;
}

void Timer::resume()
{
    ULARGE_INTEGER cTime, eTime, kTime, uTime, rTime;

    ::GetProcessTimes(::GetCurrentProcess(), (LPFILETIME)&cTime, (LPFILETIME)&eTime, (LPFILETIME)&kTime, (LPFILETIME)&uTime);
    ::GetSystemTimeAsFileTime((LPFILETIME)&rTime);

    userTime_ = uTime.QuadPart/sHZ_;
    sysTime_ = kTime.QuadPart/sHZ_;
    realTime_ = rTime.QuadPart/sHZ_;
}

void Timer::stop()
{
    ULARGE_INTEGER cTime, eTime, kTime, uTime, rTime;

    ::GetProcessTimes(::GetCurrentProcess(), (LPFILETIME)&cTime, (LPFILETIME)&eTime, (LPFILETIME)&kTime, (LPFILETIME)&uTime);
    ::GetSystemTimeAsFileTime((LPFILETIME)&rTime);

    userElapsed_ += uTime.QuadPart/sHZ_ - userTime_;
    sysElapsed_ += kTime.QuadPart/sHZ_ - sysTime_;
    realElapsed_ += rTime.QuadPart/sHZ_ - realTime_;

    userTime_ = uTime.QuadPart/sHZ_;
    sysTime_ = kTime.QuadPart/sHZ_;
    realTime_ = rTime.QuadPart/sHZ_;
}

#else

double Timer::sHZ_ = double(sysconf(_SC_CLK_TCK));

void Timer::start()
{
    userElapsed_ = 0.0;
    sysElapsed_ = 0.0;
    realElapsed_ = 0.0;

    tms buffer;
    realTime_ = times(&buffer)/sHZ_;
    userTime_ = buffer.tms_utime/sHZ_;
    sysTime_ = buffer.tms_stime/sHZ_;
}

void Timer::resume()
{
    tms buffer;
    realTime_ = times(&buffer)/sHZ_;
    userTime_ = buffer.tms_utime/sHZ_;
    sysTime_ = buffer.tms_stime/sHZ_;
}

void Timer::stop()
{
    tms buffer;
    double t = times(&buffer)/sHZ_;

    userElapsed_ += buffer.tms_utime/sHZ_ - userTime_;
    sysElapsed_ += buffer.tms_stime/sHZ_ - sysTime_;
    realElapsed_ += t - realTime_;

    userTime_ = buffer.tms_utime/sHZ_;
    sysTime_ = buffer.tms_stime/sHZ_;
    realTime_ = t;
}

#endif // _WIN32

double Timer::userTime() const
{
    return userElapsed_;
}

double Timer::sysTime() const
{
    return sysElapsed_;
}

double Timer::realTime() const
{
    return realElapsed_;
}

std::ostream& operator<<(std::ostream& out, const Timer& timer)
{
    std::ios::fmtflags flags = out.flags();
    out.flags(flags | std::ios::fixed);

    out << "  user time: " << std::setprecision(2) << timer.userTime() << " sec" << std::endl;
    out << "system time: " << std::setprecision(2) << timer.sysTime()  << " sec" << std::endl;
    out << "  real time: " << std::setprecision(2) << timer.realTime() << " sec" << std::endl;

    out.flags(flags);
    return out;
}
