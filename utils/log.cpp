#include "log.h"
#include <memory.h>
#include <sys/time.h>

unsigned long long GetTime(void) {
    struct timeval tv;
    struct timezone tz;

    gettimeofday(&tv, &tz);

    return tv.tv_sec * 1000000ULL + tv.tv_usec;
}

Timer::Timer() {
    memset(this, 0, sizeof(Timer));
}

Timer::Timer(const std::string& module) {
    m_module = module;
}

Timer::~Timer() {
    
}

void Timer::_Tic() {
    m_start = GetTime();
    
}

void Timer::_Toc() {
    m_end = GetTime();
}

// in ms
float Timer::GetElapsedTime() {
    return ( (float) (m_end - m_start) / 1000.0f );
}

void Timer::profile() {
    std::cout << "Module: (" << m_module << ") " << "runtime: " << this->GetElapsedTime() << "(ms)" << std::endl;
}