#pragma once

#include <math.h>

constexpr double TO_RADIANS = M_PI / 180.0;
constexpr double TO_DEGREES = 180.0 / M_PI;

double toRadians(double x);
double toDegrees(double x);
int to12Hour(int hour24);