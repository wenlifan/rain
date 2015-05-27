#pragma once

#include <string>
#include <cstring>
#include <type_traits>
#include <algorithm>
#include <vector>

namespace Rn
{

inline std::size_t length(std::string const &str)
{
    return str.length();
}

inline std::size_t length(char)
{
    return 1;
}

inline std::size_t length(char const *str)
{
    return std::strlen(str);
}

template <typename T>
bool startWith(std::string const &str, T const v)
{
    return str.find(v) == str.begin();
}

template <typename T>
bool endWith(std::string const &str, T const v)
{
    return str.rfind(v) == str.length() - length(v);
}

std::string trim(std::string str)
{
    auto notspace = [] (auto c) { return !(std::isspace(c)); };

    str.erase(
            std::find_if(str.rbegin(), str.rend(), notspace).base(),
            str.end());

    str.erase(
            str.begin(),
            std::find_if(str.begin(), str.end(), notspace));

    return str;
}

std::string toUpper(std::string str)
{
    std::for_each(
            str.begin(),
            str.end(),
            [](auto c){ return std::toupper(c); });
    return str;
}

std::string toLower(std::string str)
{
    std::for_each(
            str.begin(),
            str.end(),
            [](auto c){ return std::tolower(c); });
    return str;
}

template <typename T>
std::vector<std::string> split(std::string const &str, T v)
{
    std::vector<std::string> ret;

    if (str.empty())
        return ret;

    using stype = std::string::size_type;
    stype s = 0;
    stype e = 0;

    while ((e = str.find(v, s)) != std::string::npos)
    {
        ret.push_back(str.substr(s, e-s));
        e = e + length(v);
        s = e;
    }

    ret.push_back(str.substr(s, e-s));
    ret.erase(std::remove_if(
                ret.begin(),
                ret.end(),
                [](auto const &t){ return t.empty(); }
            ), ret.end());

    return ret;
}


} // !namespace Rn
