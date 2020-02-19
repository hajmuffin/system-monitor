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

// TODO: Return this process's ID
int Process::Pid() { return 0; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
  /*long Hertz = sysconf(_SC_CLK_TCK);
  vector<int> pidcpu_values = LinuxParser::ReadPidStats(pid_);
  int utime = pidcpu_values[14];
  int stime = pidcpu_values[15];
  int cutime = pidcpu_values[16];
  int cstime = pidcpu_values[17];
  int starttime = pidcpu_values[22];

  int total_time = utime + stime +cutime + cstime;

  float cpu_usage = 100 * (total_time /Hertz) / LinuxParser::UpTime(pid_); */

  return 0;}

// TODO: Return the command that generated this process
string Process::Command() { return string();}//LinuxParser::Command(pid_); }

// TODO: Return this process's memory utilization
string Process::Ram() { return string();}//LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() { return string();}//LinuxParser::User(pid_); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return 0;}//LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }