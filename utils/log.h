#ifndef __SR_LOG_H
#define __SR_LOG_H

#include <iostream>

class Timer {
private:
    unsigned long long m_start;
    unsigned long long m_end;
    std::string m_module;
public:
    Timer();
    Timer(const std::string& module);
    ~Timer();

    void _Tic();             // record start time
    void _Toc();             // record end time    
    float  GetElapsedTime();  // in ms
    void profile();
};

#endif