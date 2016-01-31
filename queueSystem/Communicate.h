//
// Created by maciej on 24.11.15.
//
#include <iostream>
#ifndef KOMUNIKATY_COMMUNICATE_H
#define KOMUNIKATY_COMMUNICATE_H

using namespace std;

class Communicate {

private:
  int _id;
  double _time;
  int _eventName;
  int _direction;
  int _destination;

public:
  Communicate(int id, double time, int eventName, int direction, int destination);

  int getDestination();

  int getDirection();

  double getTime();

  int getEventName();

  int getId();

  string toString();
};


#endif //KOMUNIKATY_COMMUNICATE_H
