#ifndef _GET_TIMESTAMP_AS_STRING_HPP_
#define _GET_TIMESTAMP_AS_STRING_HPP_

#include <iomanip>
#include <ctime>
#include <string>
#include <sstream>
#include <chrono>

std::string getTimestampAsString(std::time_t inputTime)
{
    std::ostringstream os;
    os << std::put_time(std::localtime(&inputTime), "%c");
    return os.str();
}

std::string getTimestampAsString(std::chrono::time_point<std::chrono::system_clock> chrono_inputTime)
{
    std::time_t inputTime = std::chrono::system_clock::to_time_t(chrono_inputTime);
    std::ostringstream os;
    os << std::put_time(std::localtime(&inputTime), "%c");
    return os.str();
}

#endif