//
// Created by maciej on 24.11.15.
//

#include <iostream>
#include <stdexcept>
#include "MachineBlock.h"
#include "CommunicatesStack.h"

#define FIFO 0
#define LIFO 1
#define END -1

#define BLOCK 0
#define MACHINE 1
#define QUEUE 2

#define FREE 0
#define WORKING 1
#define BLOCKED 2

#define IN 1
#define OUT 0

using namespace std;

MachineBlock::MachineBlock(double (*distribution)(double), int distributionParameter, int queueType, int queueCapacity,
                           int outputMachineNumber, int sourceMachineNumber, int machineId) {
  _distribution = distribution;
  _distributionParameter = distributionParameter;
  _queueType = queueType;
  _queueCapacity = queueCapacity;
  _outputMachineNumber = outputMachineNumber;
  _sourceMachineNumber = sourceMachineNumber;
  _machineId = machineId;

}

CommunicatesStack MachineBlock::handleCommunicate(Communicate communicate, CommunicatesStack &stack, vector<MachineBlock> machineBlocks) {
  totalCounter++;

  switch (communicate.getEventName()) {
    case BLOCK:
      switch (communicate.getDirection()) {
        case IN:
          tryToAccept(communicate, stack);
          break;
        case OUT:
          if(_queueLength > 0) {
            stack.add(Communicate(communicate.getId(), communicate.getTime(), QUEUE, OUT, _machineId));
          }
        default:break;
      }
      break;
    case MACHINE:
      switch (communicate.getDirection()) {
        case IN:
          service(communicate, stack);
          break;
        case OUT:
          tryToFreeChannel(communicate, stack, machineBlocks);
          break;
        default:break;
      }
      break;
    case QUEUE:
      switch (communicate.getDirection()) {
        case IN:
          addToQueue(communicate, stack);
          break;
        case OUT:
          selectNextTask(communicate, stack);
          break;
        default:break;
      }
      break;
    default:
      throw invalid_argument("unknown communicate");

  }

  return stack;

}

void MachineBlock::tryToAccept(Communicate communicate, CommunicatesStack &stack) {
  if (_channelState == FREE) {
    _channelState = WORKING;
    _channelStateChangedTime = communicate.getTime();
    stack.add(Communicate(communicate.getId(), _channelStateChangedTime, MACHINE, IN, _machineId));
  }
  else if (_queueLength < _queueCapacity) {
    stack.add(Communicate(communicate.getId(), communicate.getTime(), QUEUE, IN, _machineId));
  }
  else {
    //nieobsluzone- brak miejsc
    notServicedCounter  += 1;
  }
}

void MachineBlock::tryToFreeChannel(Communicate communicate, CommunicatesStack &stack, vector<MachineBlock> machineBlocks) {
  if(_channelState == FREE) {
    cout << "unnecessary broadcast received from output machine " << endl;
  }
  if(outputBlockIsFull(machineBlocks)) {
    _channelStateChangedTime = communicate.getTime();
    _channelState = BLOCKED;
    return;
  }
  else if (_channelState == BLOCKED) {
    double blockedTime = communicate.getTime() - _channelStateChangedTime;
    cout << "time task was blocked on machine:" << blockedTime << endl;
  }
  _channelState = FREE;
  _channelStateChangedTime = communicate.getTime();
  stack.add(Communicate(communicate.getId(), communicate.getTime(), BLOCK, OUT, _machineId));
  stack.add(Communicate(communicate.getId(), communicate.getTime(), BLOCK, IN, _outputMachineNumber));

}

void MachineBlock::selectNextTask(Communicate communicate, CommunicatesStack &stack) {
  Communicate selectedTask = selectTaskFromQueue();
  if(_queueLength == _queueCapacity) {
    auto notifyBlockListeners = Communicate(selectedTask.getId(), communicate.getTime(), MACHINE, OUT, _sourceMachineNumber);
    stack.add(notifyBlockListeners);
  }
  _queueLength -= 1;
  cout << "time task was waiting in queue" << communicate.getTime() - selectedTask.getTime() << endl;
  stack.add(Communicate(selectedTask.getId(), communicate.getTime(), MACHINE, IN, _machineId));
}


void MachineBlock::service(Communicate communicate, CommunicatesStack &stack) {
  //grab statistics
  double serviceTime = _distribution(_distributionParameter);
  summaricServiceTime += serviceTime;
  servicedCounter++;
  _channelStateChangedTime += serviceTime;
  stack.add(Communicate(communicate.getId(), communicate.getTime() + serviceTime, MACHINE, OUT, _machineId));
}

void MachineBlock::addToQueue(Communicate communicate, CommunicatesStack &stack) {
  _queueLength += 1;
  _queueTasks.push_back(communicate);
}


bool MachineBlock::outputBlockIsFull(vector<MachineBlock> machineBlocks) {
  auto outputMachineBlock = machineBlocks[_outputMachineNumber];
  return outputMachineBlock.isFull();
}

bool MachineBlock::isFull() {
  return _channelState != FREE && _queueLength == _queueCapacity;
}

Communicate MachineBlock::selectTaskFromQueue() {
  auto selectedTask =  _queueTasks.back();
  _queueTasks.pop_back();
  return selectedTask;
}