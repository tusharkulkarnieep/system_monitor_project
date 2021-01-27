#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

class Processor {
 public:
    Processor(); //constructor.
    float Utilization(); 
    vector<long> longcpuutil(vector<string> strval); // Converts string vector to long vector.
  // TODO: Declare any necessary private members
  private:
    float prevTotalTime, prevIdleTime; // variables to save previous cycles' total 
                                       // and idle cpu times
};

#endif