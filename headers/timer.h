#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

#include <chrono>
using namespace std::chrono;
class Timer {
public:
    Timer() : m_start(system_clock::time_point::min()) {}

    void Clear() {
        m_start = system_clock::time_point::min();
    }

    bool IsStarted() const {
        return (m_start.time_since_epoch() !=  system_clock::duration(0));
    }

    void Start() {
        m_start = system_clock::now();
    }

    unsigned long GetMs() {
        if (IsStarted()) {
            system_clock::duration diff;
            diff = system_clock::now() - m_start;
            return static_cast<unsigned>(duration_cast<milliseconds>(diff).count());
        }
        return 0;
    }
private :
    system_clock::time_point m_start;

};

#endif // TIMER_H_INCLUDED
