#include <iostream>

using namespace std;

#ifndef ELEM
#define ELEM

struct Point{
  float x;
  float y;

  Point(float x, float y) : x(x), y(y) {}

  //better if further from (0,0)
  bool operator<(const Point& other) const {
    double distance = x*x + y*y;
    double otherDistance = other.x*other.x + other.y*other.y;

    return distance < otherDistance;
  }

  //better if further from (0,0)
  bool operator==(const Point& other) const {
    return this->x == other.x && this->y == other.y;
  }

  //better if further from (0,0)
  bool operator<=(const Point& other) const {
    return (*this) < other || (*this) == other;
  }

  //better if further from (0,0)
  bool operator>(const Point& other) const {
    return !(*this <= other);
  }

  //better if further from (0,0)
  bool operator>=(const Point& other) const {
    return (*this) > other || (*this) == other;
  }

};

//ostream& operator<<(ostream& os, const Point& p) {
    //os << "Point(" << p.x << ", " << p.y << ")";
    //return os;
//}

#endif

