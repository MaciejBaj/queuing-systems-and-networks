//
// Created by maciej on 10.12.15.
//

#ifndef KOMUNIKATY_STATISTICS_H
#define KOMUNIKATY_STATISTICS_H
#include <vector>

class Statistics {

public:
  void logStatistics( double time );

  void logAverageWaitingTimeInQueue( double time );

  void logCurrentAverageServiceTime( double time );

  void logAverageQueueLength( double time );

  void updateWaitingTime(double waitingTime);

  double numberWaited = 0;
  double numberServiced = 0;
  double numberAllVisits = 0;
  double notServiced = 0;
  std::vector<double> _serviceTimes;
  std::vector<double> _waitingTimes;

  void statsLogger(const char *string, double time, double value);

  void updateAverageServiceTime(double serviceTime);

  double countVariance(double average, double probe, double currentVariance, double n);

};


#endif //KOMUNIKATY_STATISTICS_H
