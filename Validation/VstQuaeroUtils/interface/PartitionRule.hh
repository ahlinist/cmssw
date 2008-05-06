/********************
The PartitionRule class implements the rules for partitioning a set of data into exclusive final states.
These partitions are different for Quaero than for Sleuth, and different for lepton and hadron colliders.

Bruce Knuteson 2003
********************/


#ifndef __PartitionRule__
#define __PartitionRule__

#include "Validation/VstQuaeroUtils/interface/FinalState.hh"
#include "Validation/VstQuaeroUtils/interface/QuaeroEvent.hh"
#include <sstream>
#include <iostream>

class PartitionRule
{
public:
  
  /// Constructor takes a string specifying the partition rule type
  /** Legal values are:  
        tev2-exclusive:  for Quaero
	lep2-exclusive:  for Quaero
        tev2-exclusive-sansPmiss:  used for TurboSim validation
	lep2-exclusive-sansPmiss:  used for TurboSim validation
	tev2-sleuth:  for Sleuth                                 */	
  PartitionRule(std::string _ruleType="default");

  /// Determine the final state into which this event falls
  FinalState getFinalState(QuaeroEvent & event);

  /// Determine the Sleuth variables for this event
  std::vector<double> getSleuthVariables(const QuaeroEvent& event);

  /// Partition a file of events into a number of smaller files, each smaller file corresponding to a single final state
  void partition(std::string filename);

  /// Partition a stream of events into a number of smaller streams, each smaller stream corresponding to a single final state
  std::map<FinalState,std::stringstream*> partition(std::istream& metaFile);

  /// Filter out the events that may contribute to this analysis
  /// Write the events from <metaBkgFilename> that have a chance of landing in <relevantFinalStates> to the output file <relevantBkgFilename>
  void justKeepTheRelevantStuff(std::vector<std::string> relevantFinalStates, std::string metaBkgFilename, std::string relevantBkgFilename, bool keepOnlyThirdThird);

  // return pmin
  double getPmin(); 

private:
  bool globalLesserGenerationSwitch; ///< impose an equivalence relation between electrons and muons?
  bool globalChargeConjugation; ///< impose an equivalence relation between events related by global charge conjugation?
  bool quarksComeInPairs; ///< define final states to have only even numbers of jets?
  bool leptonsComeInPairs; ///< define final states to have only even numbers of leptons?
  bool identifyWeakBosons; ///< use inference of W and Z bosons when categorizing events?
  bool usePmiss; ///< use missing energy to categorize events?
  bool distinguishCharge;
  double pmin; ///< minimum momentum for an object to be considered
  double pmin_leptons; ///< minimum momentum for a lepton to be considered
  double pmin_pmiss; ///< minimum momentum for pmiss to be considered
  int maximumNumberOfJetsToIdentify; ///< lump events with this many jets or more into the same box
  std::vector<std::string> possibleObjects; ///< list of possible final state objects
  std::map<FinalState,vector<double> > sumPtFinalStateBinEdges;
  std::vector<FinalState> finalStatesToDrop;
  std::string ruleType;
};

#endif

