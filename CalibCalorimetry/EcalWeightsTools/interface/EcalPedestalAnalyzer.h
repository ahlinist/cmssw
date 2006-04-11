#ifndef ECALPEDESTALANALYZER_H
#define ECALPEDESTALANALYZER_H
/**\class EcalPedestalAnalyzer TestEcal/Analyzers/src/EcalPedestalAnalyzer.h

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Renaud Bruneliere
//         Created:  Tue Aug  9 16:15:01 CEST 2005
// $Id$
//
//



// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <vector>
#include <map>

//
// class used to produce average pedestal values
// Only works with Ecal barrel data till now...
//

class EcalPedestalAnalyzer : public edm::EDAnalyzer {
 public:
  enum { kGains = 3, kFirstGainId = 1 };
  
  explicit EcalPedestalAnalyzer(edm::ParameterSet const& pSet);
  ~EcalPedestalAnalyzer();
  virtual void analyze(const edm::Event& e, const edm::EventSetup& c);
  
 private:
  unsigned int verbosity_;
  int nChannels_;
  int iEvent_; // should be removed when we can access class EventID
  std::vector<int>   nEntries_;
  std::vector<float> pedestal_;
  std::vector<float> noise_;
  std::vector< std::pair<int, int> >   indexToChannelPos_;
  std::map< std::pair<int, int>, int > channelPosToIndex_;
};
#endif
