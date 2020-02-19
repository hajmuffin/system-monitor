#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    cpu_values = LinuxParser::CpuUtilization();
    user = cpu_values[0];
    nice = cpu_values[1];
    system = cpu_values[2];
    idle = cpu_values[3];
    iowait = cpu_values[4];
    irq = cpu_values[5];
    softirq = cpu_values[6];
    steal = cpu_values[7];

    int PrevIdle = previdle + previowait;
    int Idle = idle + iowait;

    int PrevNonIdle = prevnice + prevuser + prevsystem + previrq + prevsoftirq + prevsteal;
    int NonIdle = nice + user + system + irq + softirq + steal;

    int PrevTotal = PrevIdle + PrevNonIdle;
    int Total = Idle + NonIdle;

    int Totald = Total - PrevTotal;
    int Idled = Idle - PrevIdle;

    /*prevuser = cpu_values[0];
    prevnice = cpu_values[1];
    prevsystem = cpu_values[2];
    previdle = cpu_values[3];
    previowait = cpu_values[4];
    previrq = cpu_values[5];
    prevsoftirq = cpu_values[6];
    prevsteal = cpu_values[7]; */
    
    cpu_usage = (Totald - Idled)/Totald;
    
    return cpu_usage; 
}