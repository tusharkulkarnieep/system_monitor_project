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
using std::sort;

Processor& System::Cpu() { return cpu_; }

vector<Process>& System::Processes() 
{
    vector<int> pids = LinuxParser::Pids();             // Pulling in all the PIDs for all the processes
    for(int i : pids)
    {   
        Process process{i};                             // Assigning the pids to the respective processes_
        processes_.push_back(process);
    }
    sort(processes_.begin(), processes_.end());         // Sorting the processes_ vector according to decreasing order of
                                                        // their CPU utilization.
    return processes_; 
}

std::string System::Kernel() { return LinuxParser::Kernel(); }                      // pass-through for Kernal info

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }      // pass-through for MemoryUtilization

std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }    // pass-through for OperatingSystem

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }          // pass-through for RunningProcesses

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }              // pass-through for TotalProcesses

long System::SysUpTime() { return LinuxParser::SysUpTime(); }                       // pass-through for SysUpTime