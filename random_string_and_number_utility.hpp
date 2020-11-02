#ifndef _RANDOM_STRING_AND_NUMBER_UTILITY_HPP_
#define _RANDOM_STRING_AND_NUMBER_UTILITY_HPP_

#include <random>
#include <limits>
#include <algorithm>
#include <cctype>

double getRandomDouble(double min = std::numeric_limits<double>::lowest(), double max = std::numeric_limits<double>::max())
{
    thread_local static std::random_device rd;
    thread_local static std::mt19937 mt(rd());
    thread_local static std::uniform_real_distribution<double> dist(min, max);
    return dist(mt);
}

std::int64_t getRandomInt(std::int64_t min = std::numeric_limits<std::int64_t>::lowest(), std::int64_t max = std::numeric_limits<std::int64_t>::max())
{
    thread_local static std::random_device rd;
    thread_local static std::mt19937 mt(rd());
    thread_local static std::uniform_int_distribution<std::int64_t> dist(min, max);
    return dist(mt);
}

std::string getRandomString(std::string::size_type length)
{
    static auto& chrs = "0123456789"
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    thread_local static std::mt19937 rg{std::random_device{}()};
    thread_local static std::uniform_int_distribution<std::string::size_type> pick(0, sizeof(chrs) - 2);

    std::string s;

    s.reserve(length);

    while(length--)
        s += chrs[pick(rg)];

    return s;
}

#endif