//
// Created by maciej on 24.11.15.
//

#ifndef KOMUNIKATY_COMMUNICATESSTACK_H
#define KOMUNIKATY_COMMUNICATESSTACK_H


#include "Communicate.h"
#include <vector>

using namespace std;

class CommunicatesStack {

private:
  vector<Communicate> _communicatesStack;

  void _eventAppearedCallback(Communicate);

public:
  CommunicatesStack();

  void add(Communicate communicate);

  Communicate getCurrentCommunicate();

  bool communicatesExists();
};


#endif //KOMUNIKATY_COMMUNICATESSTACK_H
