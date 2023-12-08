#include "Time.h"

struct tm* localTime() {
    time_point now = system_clock::now();
    long currentTime = system_clock::to_time_t(now);
    return std::localtime(&currentTime);
}