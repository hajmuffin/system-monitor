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

Process::Process(int pid,string user,string command) : pid_(pid),user_(user),command_(command) {}

int Process::Pid() { return pid_; }

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

  float cpu_usage = ((total_time / hertz) / seconds); 

  return cpu_usage; 
  }

string Process::Command() { return LinuxParser::Command(pid_); }

string Process::Ram() { return LinuxParser::Ram(pid_); }

string Process::User() { return LinuxParser::User(pid_); }

long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

bool Process::operator<(Process const& a) const { 
  return (cpu_ < a.cpu_); }