#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor& System::Cpu() { return cpu_; }

vector<Process>& System::Processes() { 
    vector<int> pids = LinuxParser::Pids();
    for(auto pid:pids) {
         std::string user = LinuxParser::User(pid);         
         std::string command = LinuxParser::Command(pid);
         Process process(pid,user,command);
         processes_.push_back(process);
     }
     std::sort(processes_.begin(),processes_.end(),[](Process &p1, Process &p2){
         return (p1 < p2);
     }); 
     return processes_;
}
std::string System::Kernel() { return LinuxParser::Kernel(); }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime();}