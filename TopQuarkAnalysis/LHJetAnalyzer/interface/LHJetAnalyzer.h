// Package:    TopQuarkAnalysis
// Class:      LHJetAnalyzer
/**\class LHJetAnalyzer LHJetAnalyzer.cc LHJets/LHJetAnalyzer/src/LHJetAnalyzer.cc
 Description: Analyzer for Les Houches 2007 Jet Exercise
 Implementation: Analyze different jet collections.
*/
// Original Author:  I. Villella, D. Kcira
//         Created:  Wed Jul 18 11:07:50 CEST 2007
// $Id$
#include <memory>
#include <string>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/JetReco/interface/Jet.h"
//#include "DataFormats/JetReco/interface/CaloJet.h"
//#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetfwd.h"
class LHJetAnalyzer : public edm::EDAnalyzer {
   public:
      explicit LHJetAnalyzer(const edm::ParameterSet&);
      ~LHJetAnalyzer();
   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      void analyzeRecoJetCollection(std::vector<reco::Jet>, std::string);
   private:
      std::vector<std::string> listOfGenJetFinders;
      unsigned int consecutiveEventNumber;
};
