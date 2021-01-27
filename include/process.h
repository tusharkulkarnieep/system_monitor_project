#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>

using std::vector;
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int processid);                  // constructor for process.
  int Pid();                               
  std::string User();                      
  std::string Command();                   
  float CpuUtilization();                  
  std::string Ram();                       
  long int UpTime();                       
  bool operator<(Process const& a) const;  
  vector<float> processtimes();             // Function to read and return different 
                                           // times required to calculate cpu utilization

  // TODO: Declare any necessary private members
 private:
    int processid;
    float cpuutilization;
    enum ptimes {utime = 0, stime, cutime, cstime, starttime};
};

#endif