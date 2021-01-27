#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "linux_parser.h"

#define STARTTIME (22)

using std::string;
using std::to_string;
using std::vector;
using std::cout;
using std::stof;

Process::Process(int pid) : processid(pid){}       // Defining the constructor

int Process::Pid() 
{
    return processid;
}

float Process::CpuUtilization()
{ 
    float total_time, elapsedtime;
    // Getting the process times read from processtimes function
    vector<float> protimes = Process::processtimes();
    // Assigning the starttime
    protimes[starttime] = LinuxParser::UpTime(Process::Pid());
    // Calculating the total process time
    total_time = protimes[utime] + protimes[stime] + protimes[cutime] + protimes[cstime];
    // Calculating the elapsed time since the process started
    elapsedtime = LinuxParser::SysUpTime() - protimes[starttime];
    // Calculating the CPU utilization.
    cpuutilization = total_time/elapsedtime;
    return cpuutilization; 
}

string Process::Command() { return LinuxParser::Command(Process::Pid()); }

string Process::Ram() { return LinuxParser::Ram(Process::Pid()); }

string Process::User() { return LinuxParser::User(Process::Pid()); }

long int Process::UpTime() { return LinuxParser::UpTime(Process::Pid()) ; }

 bool Process::operator<(Process const& a) const
 {
     return a.cpuutilization < cpuutilization;           // overloading operator< for comparing cpu utilization for processes 
 }

vector<float> Process::processtimes()
{
    string line, value;
    vector<float> pt {};
    float t = 0;
    string utime, stime, cutime, cstime;
    // Opening the respective stst file for respective pid
    std::ifstream filestream(LinuxParser::kProcDirectory + to_string(Process::Pid()) + LinuxParser::kStatFilename);
    if(filestream.is_open())
    {
        while (std::getline(filestream,line))
        {
            std::istringstream linestream(line);
            // looping till the 22nd value in the file that is Starttime of the process
            for (int i = 1; i <= STARTTIME ; i++) 
            {
                linestream >> value;
                // getting the values at locations 14, 15, 16 and 17 from the file
                if (i == STARTTIME-8 || i == STARTTIME-7 || i == STARTTIME-6 || i == STARTTIME-5)
                {
                    // converting the time to seconds
                    t = stof(value) / sysconf(_SC_CLK_TCK);
                    // moving the value to the vector of process times.
                    pt.push_back(t);
                }
            }
        }
    }
    return pt;
}