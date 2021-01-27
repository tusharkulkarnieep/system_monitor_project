#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>

#include "linux_parser.h"

#define PID_UPTIME (22)

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::stol;

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
  string os, kernel;
  string line;
  string version;
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

float LinuxParser::MemoryUtilization() 
{
  string line;
  string key;
  int value;
  float memfree, memtotal, mempercent;
  std::ifstream filestream (kProcDirectory + kMeminfoFilename);   // Open the file
  if(filestream.is_open())
  {
    while(std::getline(filestream, line))
    {
      std::replace(line.begin(), line.end(), ':', ' ');   // Replace unwanted data with spaces
      std::replace(line.begin(), line.end(), 'k', ' ');
      std::replace(line.begin(), line.end(), 'B', ' ');
      std::istringstream linestream(line);
      while(linestream >> key >> value)
      {
        if (key == "MemTotal") memtotal = value;          // Get the values for total and free memory
        if (key == "MemFree") memfree = value;
      }
    }
  }
  mempercent = (memtotal-memfree)/memtotal;
  return mempercent;                                      // returning percent memory utilization
}

long LinuxParser::SysUpTime() 
{ 
  long uptime;
  long idletime;
  string line;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open())
  {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idletime;
  }
  return uptime;
}

vector<string> LinuxParser::CpuUtilization() 
{
  vector<string> cpuutil;
  string line;
  string cpu, user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open())
  {
    std::getline (filestream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq
                      >> softirq >> steal >> guest >> guest_nice;             // reading everything needed for populating
                                                                              // cpu utilization vector.
    cpuutil.push_back(user);
    cpuutil.push_back(nice);
    cpuutil.push_back(system);
    cpuutil.push_back(idle);
    cpuutil.push_back(iowait);
    cpuutil.push_back(irq);
    cpuutil.push_back(softirq);
    cpuutil.push_back(steal);
    cpuutil.push_back(guest);
    cpuutil.push_back(guest_nice);
  } 
  return cpuutil;
  
}

int LinuxParser::TotalProcesses() 
{
  string key;
  int value;
  string line;
  int tot_proc;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open())
  {
    while(std::getline(filestream,line))
    {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "processes")
      {
        tot_proc = value;
      }
      else value = 0;
    }
  }
  return tot_proc;
}

int LinuxParser::RunningProcesses() 
{ 
  string key;
  int value;
  int run_proc;
  string line;
  std::ifstream filestream (kProcDirectory + kStatFilename);
  if(filestream.is_open())
  {
    while(std::getline(filestream, line))
    {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == "procs_running")
      {
        run_proc = value; 
      }
      else value = 0;
    }  
  }
  return run_proc;
}

string LinuxParser::Command(int pid) 
{
  string command;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open())
  {
    std::getline (filestream, command);
  }
  return command; 
}

string LinuxParser::Ram(int pid) 
{
  string line, ram;
  string key;
  string value;
  std::ifstream memutildir(kProcDirectory + to_string(pid) + kStatusFilename);
  if(memutildir.is_open())
  {
    while(std::getline(memutildir, line))
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::replace(line.begin(), line.end(), 'k', ' ');
      std::replace(line.begin(), line.end(), 'B', ' ');
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == "VmSize")
      {
        ram = to_string(stoi(value) / 1000);  // converting RAM to Mb
      }
    }
  }
  return ram; 
}

string LinuxParser::Uid(int pid) 
{
  string line, uid, key, value;
  std::ifstream filestream (kProcDirectory + to_string(pid) + kStatusFilename);
  if(filestream.is_open())
  {
    while (std::getline(filestream, line))
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == "Uid")
      {
        uid = value;
      }
    }
  }
  return uid; 
}

string LinuxParser::User(int pid) 
{ 
  string line, username, x, uid;
  std::ifstream filestream(kPasswordPath);
  if(filestream.is_open())
  {
    while (std::getline(filestream, line))
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> username >> x >> uid;
      if(uid == Uid(pid))
      {
        return username;
      } 
    }
  }
  return username; 
}

long LinuxParser::UpTime(int pid) 
{
  string line;
  long puptime;
  string starttime;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if(filestream.is_open())
  {
    while (std::getline(filestream,line))
    {
      std::istringstream linestream(line);
      for (int i = 0; i <= PID_UPTIME-1 ; i++)
      {
        linestream >> starttime;
      }
    }
    // Change made to calculate uptime of the process.
    puptime = LinuxParser::SysUpTime() - (stol(starttime) / sysconf(_SC_CLK_TCK));
  }
  return puptime;
}