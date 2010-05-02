// -*- C++ -*-
//
// Package:    ElectroWeakAnalysis/MultiBosons
// Class:      CandViewCountAnalyzer
//
/**\class CandViewCountAnalyzer CandViewCountAnalyzer.cc ElectroWeakAnalysis/MultiBosons/plugins/CandViewCountAnalyzer.cc

 Description: Takes a list of product labels and produces histograms of their
  sizes / event multiplicities

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Jan Veverka,32 3-A13,+41227677936,
//         Created:  Mon Mar 22 05:44:30 CET 2010
// $Id: CandViewCountAnalyzer.cc,v 1.1 2010/04/28 09:51:51 veverka Exp $
//
//


// system include files
#include <memory>
#include <map>
#include <string>

#include "TH1.h"

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
//
// class declaration
//

class CandViewCountAnalyzer : public edm::EDAnalyzer {
public:
  explicit CandViewCountAnalyzer(const edm::ParameterSet&);
  ~CandViewCountAnalyzer();


private:
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  // ----------member data ---------------------------
  // simple map to contain all histograms;
  // histograms are booked in the beginJob()
  // method
  std::map<std::string,TH1F*> h1Container_;
  // input parameteres
  uint nbins_;
  typedef std::vector<edm::ParameterSet> VPSet;
  VPSet histograms_;
  // other member data
  std::vector<std::string> histoNames_;
  std::vector<edm::InputTag> srcTags_;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//
//
// constructors and destructor
//
CandViewCountAnalyzer::CandViewCountAnalyzer(const edm::ParameterSet& iConfig):
  h1Container_(),
  nbins_(iConfig.getUntrackedParameter<uint>("nbins")),
  histograms_(iConfig.getUntrackedParameter<VPSet>("histograms"))
{
  histoNames_.reserve(histograms_.size());
  srcTags_.reserve(histograms_.size());
}


CandViewCountAnalyzer::~CandViewCountAnalyzer()
{
}

//
// member functions
//

#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "CommonTools/CandUtils/interface/AddFourMomenta.h"
// ------------ method called to for each event  ------------
void
CandViewCountAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace reco;

  // loop over histograms
  for (uint i = 0; i < histograms_.size(); ++i) {
    Handle<CandidateView> collection;
    iEvent.getByLabel(srcTags_[i], collection);
    h1Container_[ histoNames_[i] ]->Fill( collection->size() );
  }
}

// ------------ method called once each job just before starting event loop  ------------
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
void
CandViewCountAnalyzer::beginJob()
{
  // register to the TFileService
  edm::Service<TFileService> fs;

  // book histograms:
  for (uint i=0; i < histograms_.size(); ++i) {
    srcTags_.push_back( histograms_[i].getUntrackedParameter<edm::InputTag>("src") );
    histoNames_.push_back( srcTags_[i].label() + "_" + srcTags_[i].instance() + "_Count" );
    std::string histoTitle = srcTags_[i].label() + " " + srcTags_[i].instance() + " event multiplicity";
    h1Container_[ histoNames_[i] ]=fs->make<TH1F>( histoNames_[i].c_str(), histoTitle.c_str(), nbins_, -0.5,  -0.5 + nbins_ );
  }
}

// ------------ method called once each job just after ending the event loop  ------------
void
CandViewCountAnalyzer::endJob() {
}

// define this as a plug-in
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(CandViewCountAnalyzer);
