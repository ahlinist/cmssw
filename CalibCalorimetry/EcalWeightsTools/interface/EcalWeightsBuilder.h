#ifndef ECALWEIGHTSBUILDER_H
#define ECALWEIGHTSBUILDER_H
/**\class EcalWeightsBuilder TestEcal/Analyzers/src/EcalWeightsBuilder.h

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Renaud Bruneliere
//         Created:  Tue Aug  12 16:15:01 CEST 2005
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
#include <string>

//
// class used to produce pulse shape profiles
// Only works with Ecal barrel data till now...
//

class TFile;
class TH2F;
class TProfile;

class EcalWeightsBuilder : public edm::EDAnalyzer {
 public:

  enum { kGains = 3, kFirstGainId = 1 };


  explicit EcalWeightsBuilder(edm::ParameterSet const& pSet);
  ~EcalWeightsBuilder();
  virtual void analyze(const edm::Event& e, const edm::EventSetup& c);

 private:
  std::vector< std::vector<float> > pedestal_;
  std::map< std::pair<int, int>, int > channelPosToIndexPed_;
  std::map< std::pair<int, int>, int > channelPosToIndexHist_;
  TFile* fOut_;
  std::vector<TH2F*> hPulse_;
  std::vector<TProfile*> hPulseProf_;
  unsigned int verbosity_;
  int nBinsHisto_;
  float xMinHisto_, xMaxHisto_;
  std::string pedestalFileName_;
  int nChannels_;
  int iEvent_; // should be removed when we can access class EventID
};
#endif
