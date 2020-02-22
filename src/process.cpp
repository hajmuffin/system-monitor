#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid,string user,string command) : pid_(pid) {}

// TODO: Return this process's ID
int Process::Pid() { return 0; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
  
  long hertz = sysconf(_SC_CLK_TCK);
  vector<string> pidcpu_values = LinuxParser::ReadPidStats(pid_);
  int utime = std::stoi(pidcpu_values[14]);
  int stime = std::stoi(pidcpu_values[15]);
  int cutime = std::stoi(pidcpu_values[16]);
  int cstime = std::stoi(pidcpu_values[17]);
  int starttime = std::stoi(pidcpu_values[21]);

  int total_time = utime + stime +cutime + cstime;

  float seconds = LinuxParser::UpTime() - (starttime / hertz);

  float cpu_usage = 100 * ((total_time / hertz) / seconds); 

  return cpu_usage; 
  }

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
  return (cpu_ < a.cpu_); }