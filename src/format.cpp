#include <string>

#include "format.h"

using std::string;
using std::to_string;

string Format::ElapsedTime(long seconds) 
{ 
    int secs;
    int hours = seconds/3600;
    int mins = seconds/60;
    string et, mm, ss, hh;

    secs = (seconds%60 != 0) ? seconds%60 : 0;                   //starting the count for secs from remainder of seconds
    if(secs < 59)                                                // check if secs is less than 60
    {
        secs++;
    }
    else
    {
        secs = 0;
        mins++;                                                 // Incrementing mins and setting secs = 0
    }

    if (mins >= 60)                                              // check if mins is greater than 60
    {
        hours = mins/60;
        mins = mins%60;
    }

    hh = (hours <=9 ) ? ("0" + to_string(hours)) : to_string(hours);
    mm = (mins <=9 ) ? ("0" + to_string(mins)) : to_string(mins);
    ss = (secs <=9 ) ? ("0" + to_string(secs)) : to_string(secs);
    
    return et = hh + ":" + mm + ":" + ss;
}