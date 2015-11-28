//
// Created by maciej on 24.11.15.
//

#ifndef KOMUNIKATY_MACHINEBLOCK_H
#define KOMUNIKATY_MACHINEBLOCK_H


#define FREE 0
#define WORKING 1
#define BLOCKED 2

#include "Communicate.h"
#include "CommunicatesStack.h"

class MachineBlock {

private:
  int _distributionParameter;
  int _queueType;
  int _queueCapacity;
  vector<Communicate> _queueTasks;
  int _outputMachineNumber;
  int _sourceMachineNumber;
  int _machineId;

  double (*_distribution)(double);

  int _channelState = FREE;
  int _queueLength = 0;
  double _channelStateChangedTime;

public:
  double summaricServiceTime = 0;
  double summaricQueueTime = 0;
  int servicedCounter = 0;
  int totalCounter = 0;
  int notServicedCounter = 0;

  MachineBlock(double (*distribution)(double), int distributionParameter, int queueType, int queueCapacity,
               int outputMachineNumber, int sourceMachineNumber, int machineId);

  CommunicatesStack handleCommunicate(Communicate communicate, CommunicatesStack &stack, vector<MachineBlock> machineBlocks);


  void tryToAccept(Communicate communicate, CommunicatesStack &stack);

  void tryToFreeChannel(Communicate communicate, CommunicatesStack &stack, vector<MachineBlock> machineBlocks);

  void selectNextTask(Communicate communicate, CommunicatesStack &stack);

  bool outputBlockIsFull(vector<MachineBlock> machineBlocks);

  bool isFull();

  Communicate selectTaskFromQueue();

  void service(Communicate communicate, CommunicatesStack &stack);

  void addToQueue(Communicate communicate, CommunicatesStack &stack);
};


#endif //KOMUNIKATY_MACHINEBLOCK_H
