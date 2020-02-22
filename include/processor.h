#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
    int user;
    int nice;
    int system;
    int idle;
    int iowait;
    int irq;
    int softirq;
    int steal;
    int prevuser;
    int prevnice;
    int prevsystem;
    int previdle;
    int previowait;
    int previrq;
    int prevsoftirq;
    int prevsteal;

    float cpu_usage;
    std::vector<int> cpu_values;
    std::vector<int> cpu_values1;

};

#endif