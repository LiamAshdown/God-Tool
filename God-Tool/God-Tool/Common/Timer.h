#pragma once

#include "SharedDefines.h"
#include <chrono>

class Timer
{
public:
    Timer()
    {
        Reset();
    }

    double Elasped()
    {
        auto l_Now = std::chrono::high_resolution_clock::now();
        double l_Elasped = std::chrono::duration<double, std::milli>(l_Now - m_StartTime).count();
        return l_Elasped;
    }

    void Reset()
    {
        m_StartTime = std::chrono::high_resolution_clock::now();
    }

private:
    std::chrono::time_point<std::chrono::steady_clock> m_StartTime;
};