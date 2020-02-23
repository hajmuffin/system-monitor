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

float LinuxParser::MemoryUtilization() {
  float memTotal,memFree,memUtilized;
  memTotal = stof(LinuxParser::ReadfsByKey((kProcDirectory + kMeminfoFilename),"MemTotal:"));
  memFree = stof(LinuxParser::ReadfsByKey((kProcDirectory + kMeminfoFilename),"MemFree:"));
  memUtilized = ((memTotal - memFree) / memTotal);
  return memUtilized;
}
  
long LinuxParser::UpTime() {
  long uptime = stol(LinuxParser::Readfs(kProcDirectory + kUptimeFilename));
  return uptime;
}

long LinuxParser::Jiffies() { return ActiveJiffies() + IdleJiffies(); }

long LinuxParser::ActiveJiffies(int pid) { 
  long hertz = sysconf(_SC_CLK_TCK);
  vector<string> pidcpu_values = LinuxParser::ReadPidStats(pid);
  int utime = std::stoi(pidcpu_values[14]);
  int stime = std::stoi(pidcpu_values[15]);
  int cutime = std::stoi(pidcpu_values[16]);
  int cstime = std::stoi(pidcpu_values[17]);
  int starttime = std::stoi(pidcpu_values[21]);

  int total_time = utime + stime +cutime + cstime;

  float seconds = LinuxParser::UpTime() - (starttime / hertz);

  return (100 * ((total_time / hertz) / seconds)); 
 }

long LinuxParser::ActiveJiffies() { 
  vector<int> cpu_values = LinuxParser::CpuUtilization();
  return cpu_values[0] + cpu_values[1] + cpu_values[2]; }

long LinuxParser::IdleJiffies() { 
  vector<int> cpu_values = LinuxParser::CpuUtilization();
  return (cpu_values[3] + cpu_values[4]);
}

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
int LinuxParser::TotalProcesses() {  
	return stoi(LinuxParser::ReadfsByKey((kProcDirectory + kStatFilename),"processes"));}

int LinuxParser::RunningProcesses() {
	return stoi(LinuxParser::ReadfsByKey((kProcDirectory + kStatFilename),"procs_running"));}


string LinuxParser::Command(int pid) { 
  string commad;
  commad= LinuxParser::Readfs(kProcDirectory + to_string(pid) + kCmdlineFilename);
  return commad;
  }

string LinuxParser::Ram(int pid) { 
  string ram;
  int temp;
  ram  = LinuxParser::ReadfsByKey((kProcDirectory + to_string(pid) + kStatusFilename),"VmSize:");
  temp = std::stoi(ram);
  ram = std::to_string((temp / 1024));
  return ram;
  }

string LinuxParser::Uid(int pid) { 
  string uid;
  uid = ReadfsByKey((kProcDirectory + to_string(pid) + kStatusFilename), "Uid:");
  return uid; 
  }

string LinuxParser::User(int pid) { 
  string user,x,line;
  int uid,key;
  uid = stoi(Uid(pid));
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':' , ' ' );
      std::istringstream linestream(line);
      while (linestream >> user >> x >> key) {
        if (key == uid){ 
          return user;
        }
      }
    }
  }
  return user;
}

long LinuxParser::UpTime(int pid) { 
  long piduptime;
  vector<string> pid_values;
  pid_values = ReadPidStats(pid);
  long starttime = std::stol(pid_values[21]);
  piduptime = UpTime() - (starttime / sysconf(_SC_CLK_TCK));
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
vector<string> LinuxParser::ReadPidStats(int pid){
  vector<string> pid_values;
  string line;
  string value;
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