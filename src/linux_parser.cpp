 #include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "format.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::stoi;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  float memTotal,memFree,memUtilized;
  memTotal = stof(LinuxParser::ReadfsByKey((kProcDirectory + kMeminfoFilename),"MemTotal:"));
  memFree = stof(LinuxParser::ReadfsByKey((kProcDirectory + kMeminfoFilename),"MemFree:"));
  memUtilized = ((memTotal - memFree) / memTotal);
  return memUtilized;
}
  
// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  long uptime = stoi(LinuxParser::Readfs(kProcDirectory + kUptimeFilename));
  return uptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<int> LinuxParser::CpuUtilization() { 
  vector<int> cpu_values;
  string line,key;
  int value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
     while (std::getline(stream, line)) {
      std::istringstream linestream(line);
        while (linestream >> key >> value) {
          if (key == "cpu") 
            cpu_values.emplace_back(value);
        }
     }
    }
  return cpu_values;
}
// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {  
	return stoi(LinuxParser::ReadfsByKey((kProcDirectory + kStatFilename),"processes"));}
// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
	return stoi(LinuxParser::ReadfsByKey((kProcDirectory + kStatFilename),"procs_running"));}
// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string commad;
  commad= LinuxParser::Readfs(kProcDirectory + to_string(pid) + kCmdlineFilename);
  return commad;
  }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string ram;
  ram  = LinuxParser::ReadfsByKey((kProcDirectory + to_string(pid) + kStatusFilename),"VmSize:");
  ram = stof(ram) / 1000;
  return ram; 
  }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string line,uid,key;
  int value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    while(linestream >> key >> value){
      if(key == "Uid:")
        uid = to_string(value);
    }
  }
  return uid; 
  }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string user,uid,x,line,key;
  uid = LinuxParser::Uid(pid);
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> x >> key) {
        if (key == uid) 
          return user;
      }
    }
  }
  return user;
  }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function

long LinuxParser::UpTime(int pid) { 
  long piduptime;
  vector<int> pid_values;
  pid_values = LinuxParser::ReadPidStats(pid);
  piduptime = LinuxParser::UpTime() - pid_values[21];
  return piduptime;
  }

string LinuxParser::Readfs(string filename){
    string line,value;
    std::ifstream stream(filename);
    if(stream.is_open()){
        std::getline(stream,line);
        std::istringstream linestream(line);
        linestream >> value;
    }
    return value;
}
string LinuxParser::ReadfsByKey(string filename,string key){
    string line,value,input;
    std::ifstream stream(filename);
    if (stream.is_open()) {
     while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> input >> value) {
        if (input == key) 
          return value;
        }
     }
    }
    return value;
}
vector<int> LinuxParser::ReadPidStats(int pid){
  vector<int> pid_values;
  string line;
  int value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
     while (std::getline(stream, line)) {
      std::istringstream linestream(line);
        while (linestream >> value) {
            pid_values.emplace_back(value);
        }
     }
    }
  return pid_values;
}