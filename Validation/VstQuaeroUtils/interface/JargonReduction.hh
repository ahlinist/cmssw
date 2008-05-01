/***********************
This class implements jargon reduction
***********************/

#include <string>
#include <map>

#ifndef __JargonReduction__
#define __JargonReduction__

class JargonReduction
{
public:

  // Constructor
  JargonReduction();

  void load(std::string jargonReductionFileName="");
  std::string interpret(std::string jargon);

private:
  std::map<std::string,std::string> jargonReductionMap;

};

#endif











