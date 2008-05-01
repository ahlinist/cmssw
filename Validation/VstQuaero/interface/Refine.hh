
#ifndef __Refine
#define __Refine

#include <vector>
#include <string>
#include <map>
#include "Validation/VstQuaeroUtils/interface/QuaeroEvent.hh"
#include "Validation/VstQuaeroUtils/interface/PartitionRule.hh"

class Refine
{
public:
  Refine(std::string _collider, std::string _experiment, int _level=10);
  bool satisfiesCriteriaQ(QuaeroEvent& e);

private:
  std::string collider, experiment;
  PartitionRule partitionRule;
  std::string finalState;
  double sumPtCut;
  double level;
};

#endif
