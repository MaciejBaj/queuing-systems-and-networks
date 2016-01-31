#include <iostream>
#include <bits/stl_numeric.h>
#include "Statistics.h"
#include <math.h>
using namespace std;

void Statistics::logStatistics(double time) {
  logCurrentAverageServiceTime(time);
  logAverageWaitingTimeInQueue(time);
  logAverageQueueLength(time);
}

void Statistics::logAverageWaitingTimeInQueue( double time ) {
  double sum = accumulate(_waitingTimes.begin(), _waitingTimes.end(), 0.0);
  double mean = numberAllVisits ? sum / numberAllVisits : 0;
  statsLogger("averageWaitingTime", time, mean);
  double squaresSum = inner_product(_waitingTimes.begin(), _waitingTimes.end(), _waitingTimes.begin(), 0.0);
  double variance = numberAllVisits ? squaresSum / numberAllVisits - mean * mean : 0;
  statsLogger("varianceWaitingTime", time, variance);
  statsLogger("standardDeviationWaitingTime", time, sqrt(variance));
}

void Statistics::logCurrentAverageServiceTime( double time ) {
  double sum = accumulate(_serviceTimes.begin(), _serviceTimes.end(), 0.0);
  double mean = _serviceTimes.size() ? ( sum / _serviceTimes.size() ): 0;
  statsLogger("averageServiceTime", time, mean);
  double squaresSum = inner_product(_serviceTimes.begin(), _serviceTimes.end(), _serviceTimes.begin(), 0.0);
  double variance = _serviceTimes.size() ? squaresSum / _serviceTimes.size() - mean * mean : 0;
  statsLogger("varianceServiceTime", time, variance);
  statsLogger("standardDeviationServiceTime", time, sqrt(variance));
}

void Statistics::logAverageQueueLength( double time ) {
  statsLogger("averageQueueLength", time, numberAllVisits ? numberWaited / numberAllVisits : 0);
}

void Statistics::statsLogger(const char *type, double time, double value) {
  cout << "{'messageType': '" << type << "', 'time': "<< time << ", 'value': " << value << " }," << endl;
}

void Statistics::updateAverageServiceTime(double serviceTime) {
  _serviceTimes.push_back(serviceTime);
  numberServiced++;
}

void Statistics::updateWaitingTime(double waitingTime) {
  _waitingTimes.push_back(waitingTime);
}
