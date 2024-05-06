#include <iostream>

using namespace std;

#ifndef ELEM
#define ELEM

struct Point{
  float x;
  float y;

  //better if further from (0,0)
  bool operator<(const Point& other) const {
    double distance = x*x + y*y;
    double otherDistance = other.x*other.x + other.y*other.y;

    return distance < otherDistance;
  }
};

#endif

