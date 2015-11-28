//
// Created by maciej on 24.11.15.
//

#include "CommunicatesStack.h"


CommunicatesStack::CommunicatesStack() {

}

void CommunicatesStack::add(Communicate communicate) {
  _communicatesStack.push_back(communicate);
  //ToDo: sort stack
}

Communicate CommunicatesStack::getCurrentCommunicate() {
  auto currentElement = _communicatesStack.back();
  _communicatesStack.pop_back();
  return currentElement;
}

bool CommunicatesStack::communicatesExists() {
  return !_communicatesStack.empty();
}
