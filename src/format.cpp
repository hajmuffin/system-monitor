#include <string>
#include <iostream>

#include "format.h"


using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long int seconds) { 
    int hours,minutes,secs;
    string time;
    hours = seconds / 3600;
    seconds = seconds % 3600;
    minutes = seconds / 60;
    secs = seconds % 60;
    time = std::to_string(hours) + ":" + std::to_string(minutes) + ":" + std::to_string(secs);
    return time; }
