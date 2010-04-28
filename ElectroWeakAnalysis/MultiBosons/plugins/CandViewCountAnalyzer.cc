// -*- C++ -*-
//
// Package:    GenParticleAnalyzer
// Class:      GenParticleAnalyzer
//
/**\class CandViewCountAnalyzer CandViewCountAnalyzer.cc Sherpa/CandViewCountAnalyzer/src/CandViewCountAnalyzer.cc

 Description: Analyzer GenParticles from Sherpa

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Jan Veverka,32 3-A13,+41227677936,
//         Created:  Mon Mar 22 05:44:30 CET 2010
// $Id: CandViewCountAnalyzer.cc,v 1.1 2010/03/31 23:23:46 veverka Exp $
//
//


// system include files
#include <memory>
#include <map>
#include <string>

#include "TH1.h"
#include "TH2.h"
#include "TGraph.h"

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
// #include "DataFormats/HepMCCandidate/interface/GenParticle.h"
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
  // input tags
  edm::InputTag src_;
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
  src_(iConfig.getUntrackedParameter<edm::InputTag>("src"))
{
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
  // get gen. particle collection
  Handle<View<Candidate> > collection;
  iEvent.getByLabel(src_, collection);

  std::string histoName = src_.label() + "_" + src_.instance() + "_Count";

  // std::cout << collection->size() << std::endl;

  h1Container_[histoName]->Fill( collection->size() );
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
  std::string histoName = src_.label() + "_" + src_.instance() + "_Count";
  std::string histoTitle = src_.label() + " " + src_.instance() + " event multiplicity";
  h1Container_[histoName]=fs->make<TH1F>(histoName.c_str(), histoTitle.c_str(), 101, -0.5,  100.5);
}

// ------------ method called once each job just after ending the event loop  ------------
void
CandViewCountAnalyzer::endJob() {
}

//define this as a plug-in
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(CandViewCountAnalyzer);
