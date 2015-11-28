#include <iostream>
#include <algorithm>
#include <vector>
#include <functional>
#include "CommunicatesStack.h"
#include "MachineBlock.h"
#include "Communicate.h"

#define RANDU (rand()/(1.0 + RAND_MAX))
#define RANDE -log(RANDU)
#define RANDN (cos(2 * M_PI * RANDU) * sqrt(RANDE))

#define FIFO 0
#define LIFO 1
#define END -1

#define BLOCK 0
#define MACHINE 1
#define QUEUE 2

#define IN 1
#define OUT 0


double getArrivalTime(double parameter, double distribution) { return parameter * distribution; }

double getServiceTime() { return 60 * RANDE; }

double exponentialDistribution(double parameter) { return parameter * RANDE; }

double unormalizedDistribution(double parameter) { return parameter * RANDU; }

double normalDistribution(double parameter) { return parameter * RANDN; }

double dokladnyDistribution(double parameter) { return parameter; }

void eventAppearedCallback(Communicate currentCommunicate);

using namespace std;


int main() {

  double arrivalParameter = 120, timeIn = 0;
  auto arrivalDistribution = exponentialDistribution;

  vector<MachineBlock> machineBlocks;
  for (int i = 0; i < 1; i++) {
    auto machineBlock = MachineBlock(exponentialDistribution, 80, FIFO, 5, END, END, i);
    machineBlocks.push_back(machineBlock);
  }

  CommunicatesStack communicatesStack = CommunicatesStack();
  for (int i = 0; i < 3; i++) {
    timeIn += arrivalDistribution(arrivalParameter);
    auto initialCommunicate = Communicate(i, timeIn, BLOCK, IN, 0);
    communicatesStack.add(initialCommunicate);
  }

  while (communicatesStack.communicatesExists()) {
    auto currentCommunicate = communicatesStack.getCurrentCommunicate();
    cout << currentCommunicate.getEventName() << endl;

    if (currentCommunicate.getDestination() == END) {
      //out from system
      continue;
    }
    auto destinationMachine = machineBlocks[currentCommunicate.getDestination()];
    communicatesStack = destinationMachine.handleCommunicate(currentCommunicate, communicatesStack, machineBlocks);
    machineBlocks[currentCommunicate.getDestination()] = destinationMachine;
  }

  cout << "simulation time: " << timeIn << endl;
  cout << "machine 0 service time: " << machineBlocks[0].summaricServiceTime << endl;
  cout << "machine 0 total counter: " << machineBlocks[0].totalCounter << endl;
  cout << "machine 0 serviced" << machineBlocks[0].servicedCounter << endl;
  return 0;
}
