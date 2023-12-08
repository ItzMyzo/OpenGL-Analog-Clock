#pragma once

#include <ctime>
#include <chrono>
#include <type_traits>

using namespace std::chrono;

struct tm* localTime();

/* dur must be derived from std::chrono::duration */
template<typename dur = milliseconds> long currentTime() {
    return duration_cast<dur>(system_clock::now().time_since_epoch()).count();
}