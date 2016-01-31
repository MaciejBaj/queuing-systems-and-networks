//
// Created by maciej on 24.11.15.
//

#ifndef KOMUNIKATY_COMMUNICATESSTACK_H
#define KOMUNIKATY_COMMUNICATESSTACK_H


#include "Communicate.h"
#include <list>

using namespace std;

class CommunicatesStack {

private:
  list<Communicate> _communicatesStack;

public:
  CommunicatesStack();

  void add(Communicate communicate);

  Communicate getCurrentCommunicate();

  bool communicatesExists();
};


#endif //KOMUNIKATY_COMMUNICATESSTACK_H
