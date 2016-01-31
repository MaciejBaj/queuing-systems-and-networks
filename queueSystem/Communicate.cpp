//
// Created by maciej on 24.11.15.
//

#include <sstream>
#include "Communicate.h"


Communicate::Communicate(int id, double time, int eventName, int direction, int destination) {
  _id = id;
  _time = time;
  _eventName = eventName;
  _direction = direction;
  _destination = destination;
}

int Communicate::getDestination() {
  return _destination;
}

int Communicate::getDirection() {
  return _direction;
}

double Communicate::getTime() {
  return _time;
}

int Communicate::getEventName() {
  return _eventName;
}

int Communicate::getId() {
  return _id;
}

string Communicate::toString() {

  ostringstream oss;
  oss << "'id': " << _id << ", 'time': " << _time << ", 'eventName': " << _eventName << ", 'direction': " << _direction << ", 'destination': " << _destination;
  return  oss.str();
}
