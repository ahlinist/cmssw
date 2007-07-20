// -*- C++ -*-
// Class:      LHJetAnalyzer
#include "TopQuarkAnalysis/LHJetAnalyzer/interface/LHJetAnalyzer.h"

LHJetAnalyzer::LHJetAnalyzer(const edm::ParameterSet& iConfig) {
  consecutiveEventNumber = 0;
  // get list of GenJets
  listOfGenJetFinders = iConfig.getParameter< std::vector<std::string> >("listOfGenJetFinders");
  std::cout<<"LHJetAnalyzer::LHJetAnalyzer - The number of Gen jetfinders analyzed is "<<listOfGenJetFinders.size()<<"."<<std::endl<<"They are: ";
   for(std::vector<std::string>::const_iterator jname = listOfGenJetFinders.begin(); jname != listOfGenJetFinders.end(); ++jname){
     std::cout<<" "<<*jname;
   }
   std::cout<<" "<<std::endl;
}

LHJetAnalyzer::~LHJetAnalyzer() {
}

void LHJetAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
 using namespace edm;
 ++consecutiveEventNumber;
 std::cout<<"Consecutive Event Number = "<<consecutiveEventNumber<<std::endl;
 std::vector<reco::Jet> genJetCollection;
 // loop over jetfinders - GenJets
 for(std::vector<std::string>::const_iterator jname = listOfGenJetFinders.begin(); jname != listOfGenJetFinders.end(); ++jname){
   edm::Handle<reco::GenJetCollection> genJetCollection; iEvent.getByLabel(*jname, genJetCollection); // get gen jets handle
   // cast the reco::GenJet into a reco::Jet
   std::vector<reco::Jet> recoJetCollection;
   // loop over jets
   for(reco::GenJetCollection::const_iterator igenjet = genJetCollection->begin(); igenjet != genJetCollection->end(); ++igenjet){
       recoJetCollection.push_back(*igenjet);
   }
   // perform analysis for this jet collection
   analyzeRecoJetCollection(recoJetCollection, *jname);
 }
}

void LHJetAnalyzer::beginJob(const edm::EventSetup&) {
}

void LHJetAnalyzer::endJob() {
}

void LHJetAnalyzer::analyzeRecoJetCollection(std::vector<reco::Jet> thisCollection, std::string thisName){
  std::cout<<"Collection "<<thisName<<" has "<<thisCollection.size()<<" jets"<<std::endl;
}
