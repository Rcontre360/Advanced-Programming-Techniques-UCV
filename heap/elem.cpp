#include <iostream>

using namespace std;

#ifndef ELEM
#define ELEM

struct Person{
  int age;
  int knowledge;
  int experience;

  //measuring skill
  bool operator<(const Person& other) const {
    int skill = knowledge * 2 + age + experience * 5;
    int otherSkill = other.knowledge * 2 + other.age + experience * 5;
    return skill < otherSkill;
  }
};

#endif

