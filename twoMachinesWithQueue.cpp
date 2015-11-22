#include <iostream>
#include <random>
#include <functional>
#include <list>
//
//Napisać program który symuluje układ:
//
//1. M|M|2 - czas przybycia, czas obsługi, ilość maszyn, rodzaj kolejki, max poj systemu
//2. M|M|1|FIFO|3
//3. M|M|1|PS
//
//        Intensywność strumienia zgłoszeń wynosi: mi = 1/120s.
//        Intensywność obsługi systemu wynosi: lambda = 1/60s.
//
//
//        Program powinien zwrócić:
//a. Nb - średnią liczba zgłoszeń w kolejce
//        b. Tb - średni czas przebywania zgłoszenia w kolejce
//c. q  - względna zdolność obsługi systemu
//d. A  - bezwzględna zdolność obsługi systemu

using namespace std;
#define A 448
#define RAND (P * A) & A
#define RANDU (rand()/(1.0 + RAND_MAX))
#define RANDE -log(RANDU)
#define RANDN (cos(2 * M_PI * RANDU) * sqrt(RANDE))


struct SharedProcessElement {
    double timeIn;
    double remainingTime;
};

void simulateTwoMachinesWithoutQueue(double numberOfCars, double  (*getCarArrivalTime)() , double (*getCarServiceTime)());
void simulateMachineWithFifo3Queue(double numberOfCars, double  (*getCarArrivalTime)() , double (*getCarServiceTime)());
void simulateMachineWithProcessSharingQueue(double numberOfCars, double  (*getCarArrivalTime)() , double (*getCarServiceTime)());

void pushIntoQueue(double queue[2], double newElement);
bool isQueueFull(vector<double> queue, double compared);

vector<double> updateQueue(vector<double> queue, double newCarArrival);

double exponentialArrivalTime() { return 120 * RANDE;}
double exponentialServiceTime() { return 60 * RANDE;}
double chooseMinTime(double time1, double time2) {return time1 < time2 ? time1 : time2;}

int main() {
    srand(time(0));
    double numberOfCars = 10000000;
    simulateTwoMachinesWithoutQueue(numberOfCars, exponentialArrivalTime, exponentialServiceTime);
    simulateMachineWithFifo3Queue(numberOfCars, exponentialArrivalTime, exponentialServiceTime);
    simulateMachineWithProcessSharingQueue(numberOfCars, exponentialArrivalTime, exponentialServiceTime);
    return 0;
}

/*
 * C
 */

void simulateMachineWithProcessSharingQueue(double numberOfCars, double  (*getCarArrivalTime)() , double (*getCarServiceTime)()) {

    double waitingTime = 0, summaricWaitingTime = 0, serviced = numberOfCars,
            arrivalTime = 0, timeIn = 0, timeOut = 0, queueVisits = 0;
    list<SharedProcessElement> sharedProcessQueue;
    SharedProcessElement currentElement;

    for (int i = 0; i < numberOfCars; i++) {
        arrivalTime = getCarArrivalTime();
        timeIn += arrivalTime;

        list<SharedProcessElement >::iterator sharedProcessElement = sharedProcessQueue.begin();
        while(sharedProcessElement != sharedProcessQueue.end()) {
            sharedProcessElement->remainingTime -= (timeIn - sharedProcessElement->timeIn) / sharedProcessQueue.size();
            if(sharedProcessElement->remainingTime <= 0) {
                summaricWaitingTime += timeIn - sharedProcessElement->timeIn + sharedProcessElement->remainingTime;
                sharedProcessQueue.erase(sharedProcessElement++);
            }
            else {
                sharedProcessElement++;
            }
        }

        currentElement.remainingTime = getCarServiceTime();
        currentElement.timeIn = timeIn;
        sharedProcessQueue.push_front(currentElement);

        if(sharedProcessQueue.size() > 1) {
            queueVisits ++;
        }

    }
    timeOut = currentElement.timeIn + currentElement.remainingTime;
    cout << endl << "jedna maszyna, kolekja typu Process Sharing, serwis wymaga 60 sek, auto podjezdza co 120 sek" << endl;
    cout << "Srednia liczba zgloszen w kolejce: " << queueVisits / numberOfCars * 100 << "%" << endl;
    cout << "Srednia czekania w kolejce: " << summaricWaitingTime / numberOfCars<< endl;
    cout << "Względna zdoloność obsługi systemu: " << serviced / numberOfCars << endl;
    cout << "Bezwzględna zdolność obsługi systemu: " << numberOfCars / timeOut << endl;
}



/*
 * B
 */

void simulateMachineWithFifo3Queue(double numberOfCars, double  (*getCarArrivalTime)() , double (*getCarServiceTime)()) {
    double waitingTime = 0, summaricWaitingTime = 0, summaricServiceTime = 0, serviceTime = 0, summaricQueueLength = 0,
            arrivalTime = 0, timeOut = 0, timeIn = 0, queueVisits = 0, notServiced = 0, queueWiatingEnds[2] = {0.0, 0.0};
    vector<double> queue;
    for (int i = 0; i < numberOfCars; i++) {
        arrivalTime = getCarArrivalTime();
        timeIn += arrivalTime;
        queue = updateQueue(queue, timeIn);
        if(isQueueFull(queue, timeIn)) {
            notServiced ++;
            continue;
        }
        waitingTime = timeOut - timeIn;
        waitingTime = waitingTime > 0 ? waitingTime : 0;
        if (waitingTime > 0) {
            queue.push_back(waitingTime + timeIn);
            queueVisits++;
        }
        serviceTime = getCarServiceTime();
        summaricServiceTime += serviceTime;
        summaricWaitingTime += waitingTime;
        summaricQueueLength += queue.size();
        timeOut = timeIn + serviceTime + waitingTime;
    }
    cout << endl << "jedna maszyna, kolejka FIFO o pojemnosci 3, serwis srednio trwa 60 sek, auto podjezdza co 120 sek" << endl;
    cout << "Srednie wykorzystanie maszyny " << summaricServiceTime / timeOut << endl;
    cout << "Względna zdloność obsługi systemu: " << (numberOfCars - notServiced) / numberOfCars << endl;
    cout << "Bezwzględna zdloność obsługi systemu: " << (numberOfCars - notServiced) / timeOut << " zgloszen / sek" << endl;
    cout << "Srednia czekania w kolejce: " << summaricWaitingTime / (numberOfCars - notServiced)<< endl;
    cout << "Srednia spedzony w systemie: " << (summaricWaitingTime + summaricServiceTime)  / (numberOfCars - notServiced)<< endl;

}

vector<double> updateQueue(vector<double> queue, double newCarArrival) {
    if(queue.size() == 1 && queue[0] < newCarArrival) {
        queue.pop_back();
    }
    if(queue.size() == 2 ) {
        if(queue[0] < newCarArrival) {
            queue.erase(queue.begin());
            if(queue[0] < newCarArrival) {
                queue.erase(queue.begin());
            }
        }
        else if(queue[1] < newCarArrival) {
            queue.pop_back();
        }
    }
    return queue;
}

bool isQueueFull(vector<double> queue, double comparedElement) {
    return queue.size() == 2 && queue[0] > comparedElement
           && queue[1] > comparedElement;
}


void pushIntoQueue(double queue[2], double newElement) {
	queue[0] = queue[1];
	queue[1] = newElement;
}

/*
 * A
 */

void simulateTwoMachinesWithoutQueue(double numberOfCars, double  (*getCarArrivalTime)()
        , double (*getCarServiceTime)()) {

    double waitingTime = 0, summaricWaitingTime = 0, simulationEnd, serviced = numberOfCars,
            arrivalTime = 0, timeOut1 = 0, timeOut2 = 0, timeIn = 0, queueVisits = 0;

    for (int i = 0; i < numberOfCars; i++) {
        arrivalTime = getCarArrivalTime();
        timeIn += arrivalTime;
        waitingTime = chooseMinTime(timeOut1, timeOut2) - timeIn;
        waitingTime = waitingTime > 0 ? waitingTime : 0;
        if(waitingTime > 0) {
            queueVisits++;
        }
        summaricWaitingTime += waitingTime;
        if(timeOut1 < timeOut2) {
            timeOut1 = timeIn + getCarServiceTime() + waitingTime;
        }
        else {
            timeOut2 = timeIn + getCarServiceTime() + waitingTime;
        }
    }
    simulationEnd = timeOut1 > timeOut2 ? timeOut1 : timeOut2;
    cout << endl << "dwie maszyny, brak kolejki, serwis srednio trwa 60 sek, auto podjezdza co 120 sek" << endl;
    cout << "Srednia liczba zgloszen w kolejce: " << queueVisits / numberOfCars * 100 << "%" << endl;
    cout << "Srednia czekania w kolejce: " << (float) summaricWaitingTime / numberOfCars << endl;
    cout << "Względna zdloność obsługi systemu: " << serviced / numberOfCars << endl;
    cout << "Bezwzględna zdolność obsługi systemu: " << numberOfCars / simulationEnd << endl;
}

