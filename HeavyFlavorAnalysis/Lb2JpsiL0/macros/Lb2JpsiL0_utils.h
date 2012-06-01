#ifndef LBJPSIL_UTILS_H_GUARD
#define LBJPSIL_UTILS_H_GUARD

#include <string>
#include <sstream>

/*! Converts simple types to strings
  \param i variable to be converted
  \return string
  */
template <typename T> std::string toString(T i)
{
    std::ostringstream oss;
    oss << i;
    return oss.str();
};

/*! Strips off trailing characters from a string
  /param instring String where characters should be cut off
  /param symbol Symbol after which text should be cut off, including the symbol
  /return remaining string
  */
std::string stripOff(std::string instring, char symbol);

#endif

