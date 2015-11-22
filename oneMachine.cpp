#include <iostream>
#include <random>
#include <functional>

using namespace std;

#define RANDU (rand()/(1.0 + RAND_MAX))
#define RANDE -log(RANDU)
#define RANDN (cos(2 * M_PI * RANDU) * sqrt(RANDE))

void simulateCarInBufferDuration(double numberOfCars
        , double (*getCarServiceTime)(), const char *string1);
double constantServiceTime() { return 60.0;}
double unormalizedServiceTime() {return  120 * RANDU;}
double exponentialServiceTime() { return 60 * RANDE;}
double normalizedServiceTime() { return 60 + 20 * RANDN;}

int main() {
    srand(time(0));
    double numberOfCars = 100000000;
    simulateCarInBufferDuration(numberOfCars, constantServiceTime
            , " service time always 60 sec");
    simulateCarInBufferDuration(numberOfCars, unormalizedServiceTime
            , " service time unormalized distribution 120");
    simulateCarInBufferDuration(numberOfCars, exponentialServiceTime
            , " service time exponential distribution 60");
    simulateCarInBufferDuration(numberOfCars, normalizedServiceTime
            , " service time normal distribuition 60 , 20");
    return 0;
}


void simulateCarInBufferDuration(double numberOfCars
        , double (*getCarServiceTime)(), const char *label) {

    double waitingTime = 0, summaricWaitingTime = 0,
            arrivalTime = 0, timeOut = 0, timeIn = 0;

    for (int i = 0; i < numberOfCars; i++) {
        arrivalTime = 120.0 * RANDE;
        timeIn += arrivalTime;
        waitingTime = timeOut - timeIn;
        waitingTime = waitingTime > 0 ? waitingTime : 0;
        summaricWaitingTime += waitingTime;
        timeOut = timeIn + getCarServiceTime() + waitingTime;
    }
    cout << "average waiting time: "
        << summaricWaitingTime / numberOfCars << label << endl;
}