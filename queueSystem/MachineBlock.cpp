//
// Created by maciej on 24.11.15.
//

#include <iostream>
#include <stdexcept>
#include <list>
#include <vector>
#include "MachineBlock.h"
#include "CommunicatesStack.h"
#include "Statistics.h"
#include "DestinationMachine.h"

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

CommunicatesStack MachineBlock::handleCommunicate(Communicate communicate, CommunicatesStack &stack,
                                    vector<MachineBlock> machineBlocks, Statistics &statistics) {
  totalCounter++;

  switch (communicate.getEventName()) {
    case BLOCK:
      switch (communicate.getDirection()) {
        case IN:
          tryToAccept(communicate, stack, statistics);
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
          tryToFreeChannel(communicate, stack, machineBlocks, statistics);
          break;
        default:break;
      }
      break;
    case QUEUE:
      switch (communicate.getDirection()) {
        case IN:
          addToQueue(communicate, stack, statistics);
          break;
        case OUT:
          selectNextTask(communicate, stack, statistics);
          break;
        default:break;
      }
      break;
    default:
      throw invalid_argument("unknown communicate");

  }

  return stack;

}

void MachineBlock::tryToAccept(Communicate communicate, CommunicatesStack &stack, Statistics &statistics) {
  statistics.numberAllVisits++;
  if (_channelState == FREE) {
    _channelState = WORKING;
    _channelStateChangedTime = communicate.getTime();
    stack.add(Communicate(communicate.getId(), _channelStateChangedTime, MACHINE, IN, _machineId));
//    statistics.updateWaitingTime(0.0);
//    statistics.numberWaited ++;
  }
  else if (_queueLength < _queueCapacity) {
    stack.add(Communicate(communicate.getId(), communicate.getTime(), QUEUE, IN, _machineId));
  }
  else {
    //nieobsluzone- brak miejsc
    statistics.numberAllVisits--;
    statistics.notServiced++;
    notServicedCounter += 1;
  }
}

void MachineBlock::tryToFreeChannel(Communicate communicate, CommunicatesStack &stack,
                                    vector<MachineBlock> machineBlocks, Statistics &statistics) {
  if(_channelState == FREE) {
    cout << "{'additionalInfo': 'unnecessary broadcast received from output machine'}, " << endl;
  }
  if(outputBlockIsFull(machineBlocks)) {
    _channelStateChangedTime = communicate.getTime();
    _channelState = BLOCKED;
    return;
  }
  else if (_channelState == BLOCKED) {
    double blockedTime = communicate.getTime() - _channelStateChangedTime;
    cout << "{'additionalInfo': 'time task was blocked on machine: " << blockedTime << "'}," << endl;
  }
  _channelState = FREE;
  stack.add(Communicate(communicate.getId(), communicate.getTime(), BLOCK, OUT, _machineId));
  stack.add(Communicate(communicate.getId(), communicate.getTime(), BLOCK, IN, _outputMachineNumber));
  statistics.updateAverageServiceTime(communicate.getTime() - _channelStateChangedTime);
  _channelStateChangedTime = communicate.getTime();

}

void MachineBlock::selectNextTask(Communicate communicate, CommunicatesStack &stack, Statistics &statistics) {
  Communicate selectedTask = selectTaskFromQueue();
  if(_queueLength == _queueCapacity) {
    auto notifyBlockListeners = Communicate(selectedTask.getId(), communicate.getTime(), MACHINE, OUT, _sourceMachineNumber);
    stack.add(notifyBlockListeners);
  }
  _queueLength -= 1;
  stack.add(Communicate(selectedTask.getId(), communicate.getTime(), MACHINE, IN, _machineId));
  statistics.updateWaitingTime(communicate.getTime() - selectedTask.getTime());
}


void MachineBlock::service(Communicate communicate, CommunicatesStack &stack) {
  //grab statistics
  double serviceTime = _distribution(_distributionParameter);
  summaricServiceTime += serviceTime;
  servicedCounter++;
//  _channelStateChangedTime += serviceTime;
  stack.add(Communicate(communicate.getId(), communicate.getTime() + serviceTime, MACHINE, OUT, _machineId));
}

void MachineBlock::addToQueue(Communicate communicate, CommunicatesStack &stack, Statistics &statistics) {
  _queueLength += 1;
  _queueTasks.push_back(communicate);
  statistics.numberWaited ++;
}

bool MachineBlock::outputBlockIsFull(vector<MachineBlock> machineBlocks) {
  if(_outputMachineNumber == END) {
    return false;
  }
  return  machineBlocks[_outputMachineNumber].isFull();
}

bool MachineBlock::isFull() {
  return _channelState != FREE && _queueLength == _queueCapacity;
}

Communicate MachineBlock::selectTaskFromQueue() {
  if(_queueType == LIFO) {
    auto selectedTask =  _queueTasks.front();
    _queueTasks.pop_front();
    return selectedTask;
  }
  else if (_queueType == FIFO) {
    auto selectedTask =  _queueTasks.back();
    _queueTasks.pop_back();
    return selectedTask;
  }
  else {
    throw invalid_argument("{'error': unknow queue type}");
  }

}

MachineBlock::MachineBlock() {

}
