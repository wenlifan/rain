#pragma once

#include <string>
#include <cstring>
#include <type_traits>
#include <algorithm>
#include <vector>

namespace rain
{

bool startWith(std::string const &str, std::string const &v)
{
    return str.find(v) == 0;
}

bool startWith(std::string const &str, char v)
{
    return startWith(str, std::to_string(v));
}

bool endWith(std::string const &str, std::string const &v)
{
    return str.rfind(v) == str.length() - v.length();
}

bool endWith(std::string const &str, char v)
{
    return endWith(str, std::to_string(v));
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

std::vector<std::string> split(std::string const &str, std::string const &v)
{
    std::vector<std::string> ret;

    if (str.empty())
        return ret;

    using stype = std::string::size_type;
    stype s = 0;
    stype e = -v.length();

    while ((e += v.length(), s = e, e = str.find(v, s)) != std::string::npos)
        ret.push_back(str.substr(s, e-s));

    ret.push_back(str.substr(s, e-s));
    ret.erase(std::remove_if(
                ret.begin(),
                ret.end(),
                [](auto const &t){ return t.empty(); }
            ), ret.end());

    return ret;
}

std::vector<std::string> split(std::string const &str, char v)
{
    return split(str, std::to_string(v));
}

std::string replace(std::string str, std::string const &from, std::string const &to)
{
    for (auto pos = str.find(from); pos != std::string::npos;)
    {
        str.replace(pos, from.length(), to);
        pos = str.find(from, pos);
    }
    return str;
}


} // !namespace Rn
