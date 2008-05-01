/*************************************************************************************
  Refine implements "refinement" (i.e. post-simulation ad hoc but physically motivated 
  corrections) of events
*************************************************************************************/
#ifndef __Refine
#define __Refine

#include <vector>
#include <string>
#include <map>
#include "Validation/VstQuaeroUtils/interface/QuaeroEvent.hh"
#include "Validation/VstQuaeroUtils/interface/PartitionRule.hh"

class HintSpec
{
public:
  HintSpec()
  {
    active = false;
    collider = "";
    experiment = "";
    finalState = "";
    sumPtCut = 0;
  }
  bool active;
  std::string collider, experiment;
  std::string finalState;
  double sumPtCut;
};


class Refine
{
public:
  Refine(std::string _collider, std::string _experiment, std::string partitionRuleName="", int _level=10, HintSpec _hintSpec = HintSpec());
  bool satisfiesCriteriaQ(QuaeroEvent& e);
  void setMinWt(double _minWt);
  bool passesMinWt(QuaeroEvent& e, double& runningWeight);

private:
  std::string collider, experiment;
  PartitionRule partitionRule;
  HintSpec hintSpec;
  double level;
  double minWt;
  bool passesSpecialSleuthCriteriaQ(const QuaeroEvent& e);
};

#endif
