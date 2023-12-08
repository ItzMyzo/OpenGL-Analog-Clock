#include "Conversion.h"

double toRadians(double x) {
    return x * TO_RADIANS;
}

double toDegrees(double x) {
    return x * TO_DEGREES;
}

int to12Hour(int hour24) {
    return hour24 % 12;
}