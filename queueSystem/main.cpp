#include <iostream>
#include <algorithm>
#include <vector>
#include <functional>
#include "CommunicatesStack.h"
#include "MachineBlock.h"
#include "Communicate.h"
#include "Statistics.h"
#include "DestinationMachine.h"

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

#define EXP 0
#define UNORM 1
#define NORM 2
#define ACCUR 3



double getArrivalTime(double parameter, double distribution) { return parameter * distribution; }

double getServiceTime() { return 60 * RANDE; }

double exponentialDistribution(double parameter) { return parameter * RANDE; }

double unormalizedDistribution(double parameter) { return parameter * RANDU; }

double normalDistribution(double parameter) { return parameter * RANDN; }

double dokladnyDistribution(double parameter) { return parameter; }

void eventAppearedCallback(Communicate currentCommunicate);

using namespace std;

// MachineBlock(exponentialDistribution, 60, FIFO, 2, END, END, 0);
// MachineBlock(0 60 0 2 -1 -1 0 0 120);
struct MachineBlockArguments {
  int distribution;
  int distributionParameter;
  int queueType;
  int queueCapacity;
  int outputMachineNumber;
  int sourceMachineNumber;
  int id;
};

struct ArrivalDistribution {
  double (*distribution)(double);
  int parameter;
};


int main(int argc, const char * argv[]) {
  srand(time(NULL));
  double  timeIn = 0;
  int numberOfMachines = argc/7;
  MachineBlockArguments machineBlockArguments[numberOfMachines];
  MachineBlockArguments currentArguments;
  vector<MachineBlock> machineBlocks;

  for( int i = 2; i < argc; ++i ) {
    int arg = (i - 1) % 7;
    switch (arg) {
      case 0:
        currentArguments = MachineBlockArguments();
        currentArguments.distribution = (int)atol(argv[i]);
        break;
      case 1:
        currentArguments.distributionParameter = (int)atol(argv[i]);
        break;
      case 2:
        currentArguments.queueType = (int)atol(argv[i]);
        break;
      case 3:
        currentArguments.queueCapacity = (int)atol(argv[i]);
        break;
      case 4:
        currentArguments.outputMachineNumber = (int)atol(argv[i]);
        break;
      case 5:
        currentArguments.sourceMachineNumber = (int)atol(argv[i]);
        break;
      case 6:
        currentArguments.id = (int)atol(argv[i]);

        switch (currentArguments.distribution) {
          case EXP:
            machineBlocks.push_back(MachineBlock(
                    exponentialDistribution,
                    currentArguments.distributionParameter,
                    currentArguments.queueType,
                    currentArguments.queueCapacity,
                    currentArguments.outputMachineNumber,
                    currentArguments.sourceMachineNumber,
                    currentArguments.id));
            break;
          case UNORM:
            machineBlocks.push_back(MachineBlock(
                    unormalizedDistribution,
                    currentArguments.distributionParameter,
                    currentArguments.queueType,
                    currentArguments.queueCapacity,
                    currentArguments.outputMachineNumber,
                    currentArguments.sourceMachineNumber,
                    currentArguments.id));
            break;
          case NORM:
            machineBlocks.push_back(MachineBlock(
                    normalDistribution,
                    currentArguments.distributionParameter,
                    currentArguments.queueType,
                    currentArguments.queueCapacity,
                    currentArguments.outputMachineNumber,
                    currentArguments.sourceMachineNumber,
                    currentArguments.id));
            break;
          case ACCUR:
            machineBlocks.push_back(MachineBlock(
                    dokladnyDistribution,
                    currentArguments.distributionParameter,
                    currentArguments.queueType,
                    currentArguments.queueCapacity,
                    currentArguments.outputMachineNumber,
                    currentArguments.sourceMachineNumber,
                    currentArguments.id));
            break;
        }


//        machineBlockArguments[arg / 7] = currentArguments;
        break;
      default:
        break;
    }
  }

  ArrivalDistribution arrivalDistribution = ArrivalDistribution();
  switch ((int)atol(argv[argc-2])) {
    case EXP:
      arrivalDistribution.distribution = exponentialDistribution;
      break;
    case UNORM:
      arrivalDistribution.distribution = unormalizedDistribution;
      break;
    case NORM:
      arrivalDistribution.distribution = normalDistribution;
      break;
    case ACCUR:
      arrivalDistribution.distribution = dokladnyDistribution;
      break;
  }

  arrivalDistribution.parameter = ((int)atol(argv[argc-1]));

  auto statistics = Statistics();
  cout << "[";

  CommunicatesStack communicatesStack = CommunicatesStack();
  for (int i = 0; i < 100; i++) {
    timeIn += arrivalDistribution.distribution(arrivalDistribution.parameter);
    auto initialCommunicate = Communicate(i, timeIn, BLOCK, IN, 0);
    communicatesStack.add(initialCommunicate);
  }

  while (communicatesStack.communicatesExists()) {
    auto currentCommunicate = communicatesStack.getCurrentCommunicate();
    cout << "{'messageType': 'currentCommunicate', " << currentCommunicate.toString() << " }," << endl;
    if (currentCommunicate.getDestination() == END) {
      //out from system
      continue;
    }
    auto destinationMachine = machineBlocks[currentCommunicate.getDestination()];
    communicatesStack = destinationMachine.handleCommunicate(currentCommunicate, communicatesStack, machineBlocks,
                                                             statistics);
    machineBlocks[currentCommunicate.getDestination()] = destinationMachine;
    statistics.logStatistics(currentCommunicate.getTime());
  }
  cout << "{}]" << endl;

  return 0;
}
