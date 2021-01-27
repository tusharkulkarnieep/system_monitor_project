#include <string.h>

#include "processor.h"
#include "process.h"
#include "linux_parser.h"

using namespace LinuxParser;
using namespace std;


Processor::Processor() : prevTotalTime(0.0), prevIdleTime(0.0){} // constructor

float Processor::Utilization() 
{ 
    vector<long> cpuutillong {};
    float totalcputime, idlecputime, difftotal, diffidle, cpu_percent;
    // reading the cpu utilization and converting it from string to long value.
    cpuutillong = longcpuutil(LinuxParser::CpuUtilization());
    // Adding up all the times to totalcputime
    totalcputime = cpuutillong[LinuxParser::kUser_] +
                   cpuutillong[LinuxParser::kNice_] +
                   cpuutillong[LinuxParser::kSystem_] +
                   cpuutillong[LinuxParser::kIdle_] +
                   cpuutillong[LinuxParser::kIOwait_] +
                   cpuutillong[LinuxParser::kIRQ_] +
                   cpuutillong[LinuxParser::kSoftIRQ_] +
                   cpuutillong[LinuxParser::kSteal_] +
                   cpuutillong[LinuxParser::kGuest_] +
                   cpuutillong[LinuxParser::kGuestNice_];
    // Adding up idle cpu time
    idlecputime = cpuutillong[LinuxParser::kIdle_] + cpuutillong[LinuxParser::kIOwait_];

    // Calculating difference for idle and total cpu time with respect to previous cycle
    difftotal = totalcputime - prevTotalTime;
    diffidle = idlecputime - prevIdleTime;

    // cpu_percent based on the difference.
    cpu_percent = (difftotal - diffidle)/difftotal;

    // Storing current total and idle time for the next cycle
    prevTotalTime = totalcputime; 
    prevIdleTime = idlecputime;

    return cpu_percent; 
}

//Function to convert string values to long values.
vector<long> Processor::longcpuutil(vector<string> strval)
{
    vector<long> longval{};
    for(int i = 0; i < (int)strval.size(); i++)
    {
        longval.push_back(stol(strval[i]));
    }
    return longval;
}