#include <iostream>
#include <algorithm>
#include <vector>
#include <functional>

#define RANDU (rand()/(1.0 + RAND_MAX))
#define RANDE -log(RANDU)
#define RANDN (cos(2 * M_PI * RANDU) * sqrt(RANDE))

double getArrivalTime(double parameter, double distribution) { return parameter * distribution;}
double getServiceTime() { return 60 * RANDE;}

double exponentialDistribution(double parameter) { return parameter * RANDE;}
double unormalizedDistribution(double parameter) { return parameter * RANDU;}
double normalDistribution(double parameter) { return parameter * RANDN;}
double dokladnyDistribution(double parameter) { return parameter;}


using namespace std;
struct type {
    string event;
    bool in;
};

struct communicate {
    int time;
    type eventType;
};

struct machineBlock {
    char distribution;
    int distributionParameter;
    string queueType;
    int queueCapacity;
};

void pushToCommunicatesQueue(communicate communicate);

int main() {

    double arrivalParameter = 120;
    auto arrivalDistribution = exponentialDistribution;



    communicate initialCommunicate;
    type initialType;
    initialType.event = 'system';
    initialType.in = true;
    initialCommunicate.time = arrivalDistribution(arrivalParameter);
    initialCommunicate.eventType = initialType;
    pushToCommunicatesQueue(initialCommunicate);
    return 0;
}

void pushToCommunicatesQueue(communicate communicate){

}