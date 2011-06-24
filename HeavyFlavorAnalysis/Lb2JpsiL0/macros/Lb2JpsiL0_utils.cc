#include <sstream>
#include <string>
#include <algorithm>
#include "Lb2JpsiL0_utils.h"

/*! Converts simple types to strings
  \param i variable to be converted
  \return string
  */

/*! Strips off trailing characters from a string
  /param instring String where characters should be cut off
  /param symbol Symbol after which text should be cut off, including the symbol
  /return remaining string
  */
std::string stripOff(std::string instring, char symbol)
{
    std::string::iterator iter = std::find(instring.begin(), instring.end(), symbol);
    std::string outstring;
    std::copy(instring.begin(), iter, std::back_inserter(outstring));
    return outstring;
}

