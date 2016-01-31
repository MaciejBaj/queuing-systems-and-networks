//
// Created by maciej on 24.11.15.
//

#include <iostream>
#include "CommunicatesStack.h"

bool compareCommunicateTimes (Communicate& first, Communicate& second)
{
  return first.getTime() > second.getTime();
}


CommunicatesStack::CommunicatesStack() {

}

void CommunicatesStack::add(Communicate communicate) {
  _communicatesStack.push_back(communicate);
  _communicatesStack.sort(compareCommunicateTimes);
  cout << "{ 'messageType': 'newCommunicate', " << communicate.toString() << "}," << endl;
}

Communicate CommunicatesStack::getCurrentCommunicate() {
  auto currentElement = _communicatesStack.back();
  _communicatesStack.pop_back();
  return currentElement;
}

bool CommunicatesStack::communicatesExists() {
  return !_communicatesStack.empty();
}

