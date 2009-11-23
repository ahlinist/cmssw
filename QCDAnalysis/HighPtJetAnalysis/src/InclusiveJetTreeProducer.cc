// contains the older method to get the HLT information
#include <iostream>
#include <sstream>
#include <istream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cmath>
#include <functional>

#include "QCDAnalysis/HighPtJetAnalysis/interface/InclusiveJetTreeProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/JetReco/interface/JetExtendedAssociation.h"


using namespace edm;
using namespace reco;
using namespace std;

InclusiveJetTreeProducer::InclusiveJetTreeProducer(edm::ParameterSet const& cfg) 
{
  mJetsName           = cfg.getParameter<std::string>              ("jets");
  mMetName            = cfg.getParameter<std::string>              ("met");
  mMetNoHFName        = cfg.getParameter<std::string>              ("metNoHF");
  mJetExtender        = cfg.getParameter<std::string>              ("jetExtender");
  mTriggerNames       = cfg.getParameter<std::vector<std::string> >("jetTriggerNames");
  mL1TriggerNames     = cfg.getParameter<std::vector<std::string> >("l1TriggerNames");
  mTriggerProcessName = cfg.getParameter<std::string>              ("triggerProcessName"); 
  mTriggerResultsTag  = cfg.getParameter<edm::InputTag>            ("triggerResultsTag");
  mEtaMax             = cfg.getParameter<double>                   ("etaMax"); 
  mPtMin              = cfg.getParameter<double>                   ("ptMin");                 
  L1GTReadoutRcdSource_  = cfg.getParameter<edm::InputTag>("L1GTReadoutRcdSource");
  L1GTObjectMapRcdSource_= cfg.getParameter<edm::InputTag>("L1GTObjectMapRcdSource");
   mIsMCarlo           = cfg.getParameter<bool>                     ("isMCarlo");

}
//////////////////////////////////////////////////////////////////////////////////////////
void InclusiveJetTreeProducer::beginJob(EventSetup const& iSetup) 
{
  mTree = fs->make<TTree>("InclusiveJetTree","InclusiveJetTree");

  buildTree();
  
  //must be done at beginRun and not only at beginJob, because 
  //trigger names are allowed to change by run.
  mHltConfig.init(mTriggerProcessName);
  for(unsigned int i=0;i<mTriggerNames.size();i++)  {
    mTriggerIndex.push_back(mHltConfig.triggerIndex(mTriggerNames[i]));
    if (mTriggerIndex[i] == mHltConfig.size())
      {
	string errorMessage = "Requested TriggerName does not exist! -- "+mTriggerNames[i]+"\n";
	throw  cms::Exception("Configuration",errorMessage);
      }
  }
}
//////////////////////////////////////////////////////////////////////////////////////////
void InclusiveJetTreeProducer::endJob() 
{
}
//////////////////////////////////////////////////////////////////////////////////////////
void InclusiveJetTreeProducer::analyze(edm::Event const& event, edm::EventSetup const& iSetup) 
{ 
  
  //Clear all the vectors stored in tree
  clearTreeVectors();

  mRun = event.id().run();
  mEvent = event.id().event();

  ////////////Vertices//////////////
  Handle<reco::VertexCollection> recVtxs;
  event.getByLabel("offlinePrimaryVertices",recVtxs);
  for(unsigned int ind=0;ind<recVtxs->size();ind++) {
    if (!((*recVtxs)[ind].isFake())) 
      {
	mPVx ->push_back( (*recVtxs)[ind].x() );
	mPVy ->push_back( (*recVtxs)[ind].y() );
	mPVz ->push_back( (*recVtxs)[ind].z() );
      }
  }

  ////////Get PtHat///////////////
  Handle<GenEventInfoProduct> hEventInfo;
  if (mIsMCarlo)
    { 
      event.getByLabel("generator", hEventInfo);
      mPtHat = hEventInfo->binningValues()[0];
    }

  ////////////// Jets //////
  edm::Handle<CaloJetCollection> jets;
  event.getByLabel(mJetsName,jets);
  Handle<JetExtendedAssociation::Container> jetExtender;
  event.getByLabel(mJetExtender,jetExtender);
  int counter = 0;

  for(unsigned int ind=0;ind<(*jets).size();ind++) 
    {
      if(((*jets)[ind].pt() > mPtMin) && (abs((*jets)[ind].eta()) < mEtaMax  && counter < 20)){
	counter++;
	mPt       ->push_back( (*jets)[ind].pt()     );
	mEta      ->push_back( (*jets)[ind].eta()    );
	mPhi      ->push_back( (*jets)[ind].phi()    );
	mE        ->push_back( (*jets)[ind].energy() );
	mN90      ->push_back( (*jets)[ind].n90()    );	
	mEmf      ->push_back( (*jets)[ind].emEnergyFraction() ); 
	mNtrkVx   ->push_back( JetExtendedAssociation::tracksAtVertexNumber(*jetExtender,(*jets)[ind]) );
	mNtrkCalo ->push_back( JetExtendedAssociation::tracksAtCaloNumber(*jetExtender,(*jets)[ind])   ); 

	reco::JetExtendedAssociation::LorentzVector TracksatCalo = JetExtendedAssociation::tracksAtCaloP4(*jetExtender,(*jets)[ind]);
	reco::JetExtendedAssociation::LorentzVector TracksatVx = JetExtendedAssociation::tracksAtVertexP4(*jetExtender, (*jets)[ind]);
	mCTPt     ->push_back( TracksatCalo.pt()     );
	mCTEta    ->push_back( TracksatCalo.eta()    );
	mCTPhi    ->push_back( TracksatCalo.phi()    );
	mVTPt     ->push_back( TracksatVx.pt()       );
	mVTEta    ->push_back( TracksatVx.eta()      );
	mVTPhi    ->push_back( TracksatVx.phi()      );
      }
    }

  ////////////// Trigger bits //////
  edm::Handle<edm::TriggerResults> triggerResultsHandle;
  event.getByLabel(mTriggerResultsTag,triggerResultsHandle); 
  if (!triggerResultsHandle.isValid())
    {
      string errorMessage = "Requested TriggerResult is not present in file! -- \n";
      throw  cms::Exception("Configuration",errorMessage);
    }

  for(unsigned int i=0;i<mTriggerNames.size();i++) {
    bool accept = triggerResultsHandle->accept( mHltConfig.triggerIndex(mTriggerNames[i]) );
    if( accept ) mHLTNames->push_back( mTriggerNames[i] );
  }


  //===================== save L1 Trigger information ======================= 
  // get L1TriggerReadout records
  edm::Handle<L1GlobalTriggerReadoutRecord>   gtRecord;
  event.getByLabel(L1GTReadoutRcdSource_,   gtRecord);

  edm::Handle<L1GlobalTriggerObjectMapRecord> gtOMRec;
  event.getByLabel(L1GTObjectMapRcdSource_, gtOMRec);

  // sanity check on L1 Trigger Records
  if (!gtRecord.isValid()) {
    std::cout << "\nL1GlobalTriggerReadoutRecord with \n \nnot found"
      "\n  --> returning false by default!\n" << std::endl;
  }
  if (!gtOMRec.isValid()) {
    std::cout << "\nL1GlobalTriggerObjectMapRecord with \n \nnot found"
      "\n  --> returning false by default!\n" << std::endl;
  }

  // L1 decision word
  const DecisionWord dWord = gtRecord->decisionWord();

  std::map<std::string, int> l1map;
  const std::vector<L1GlobalTriggerObjectMap>& objMapVec =  gtOMRec->gtObjectMap();
  for (std::vector<L1GlobalTriggerObjectMap>::const_iterator itMap = objMapVec.begin();
       itMap != objMapVec.end(); ++itMap) {
    l1map.insert( std::pair<std::string, int> ((*itMap).algoName(), (*itMap).algoBitNumber()) );

  }
  for(unsigned int i=0; i<mL1TriggerNames.size(); i++) {
    std::map<std::string, int>::const_iterator itr = l1map.find( mL1TriggerNames[i] );
    if( itr != l1map.end() ) {
      //if( dWord[itr->second] ) std::cout <<"-----> "<< itr->second << " " << itr->first << std::endl;
      if( dWord[itr->second] ) mL1Names->push_back( itr->first );
    }
  }

  ////////////// MET //////
  Handle<CaloMETCollection> met;
  event.getByLabel(mMetName,met);
  if (met->size() == 0)
    {
      mMET   = -1;
      mSumET = -1;
    }
  else
    {
      mMET   = (*met)[0].et();
      mSumET = (*met)[0].sumEt();
    }

  Handle<CaloMETCollection> metNoHF;
  event.getByLabel(mMetNoHFName,metNoHF);
  if (metNoHF->size() == 0)
    {
      mMETnoHF   = -1;
      mSumETnoHF = -1;
    }
  else
    {
      mMETnoHF   = (*metNoHF)[0].et();
      mSumETnoHF = (*metNoHF)[0].sumEt();
    }

  mTree->Fill();
}
//////////////////////////////////////////////////////////////////////////////////////////
InclusiveJetTreeProducer::InclusiveJetTreeProducer() 
{
  mTree = 0;
}

void InclusiveJetTreeProducer::buildTree() {

  mPt       = new std::vector<double>();
  mEta      = new std::vector<double>();
  mPhi      = new std::vector<double>();
  mE        = new std::vector<double>();
  mEmf      = new std::vector<double>();
  mNtrkVx   = new std::vector<double>();
  mNtrkCalo = new std::vector<double>();
  mCTPt     = new std::vector<double>();
  mCTEta    = new std::vector<double>();
  mCTPhi    = new std::vector<double>();
  mVTPt     = new std::vector<double>();
  mVTEta    = new std::vector<double>();
  mVTPhi    = new std::vector<double>();
  mN90      = new std::vector<double>();
  mPVx      = new std::vector<double>();
  mPVy      = new std::vector<double>();
  mPVz      = new std::vector<double>();
  mHLTNames = new std::vector<std::string>();
  mL1Names  = new std::vector<std::string>();

  mTree->Branch("pt"        ,"vector<double>"      ,&mPt);
  mTree->Branch("eta"       ,"vector<double>"      ,&mEta);
  mTree->Branch("phi"       ,"vector<double>"      ,&mPhi);
  mTree->Branch("e"         ,"vector<double>"      ,&mE);
  mTree->Branch("emf"       ,"vector<double>"      ,&mEmf);
  mTree->Branch("nTrkVx"    ,"vector<double>"      ,&mNtrkVx);
  mTree->Branch("nTrkCalo"  ,"vector<double>"      ,&mNtrkCalo);
  mTree->Branch("ctpt"      ,"vector<double>"      ,&mCTPt);
  mTree->Branch("cteta"     ,"vector<double>"      ,&mCTEta);
  mTree->Branch("ctphi"     ,"vector<double>"      ,&mCTPhi);
  mTree->Branch("vtpt"      ,"vector<double>"      ,&mVTPt);
  mTree->Branch("vteta"     ,"vector<double>"      ,&mVTEta);
  mTree->Branch("vtphi"     ,"vector<double>"      ,&mVTPhi);
  mTree->Branch("n90"       ,"vector<double>"      ,&mN90);
  mTree->Branch("nPVx"      ,"vector<double>"      ,&mPVx);
  mTree->Branch("nPVy"      ,"vector<double>"      ,&mPVy);
  mTree->Branch("nPVz"      ,"vector<double>"      ,&mPVz);
  mTree->Branch("hltNames"  ,"vector<string>"      ,&mHLTNames);
  mTree->Branch("l1Names"   ,"vector<string>"      ,&mL1Names);

  mTree->Branch("event"     ,&mEvent               ,"mEvent/I");
  mTree->Branch("run"       ,&mRun                 ,"mRun/I");
  mTree->Branch("met"       ,&mMET                 ,"mMET/D");
  mTree->Branch("sumet"     ,&mSumET               ,"mSumET/D");
  mTree->Branch("metNoHF"   ,&mMETnoHF             ,"mMETnoHF/D");
  mTree->Branch("sumetNoHF" ,&mSumETnoHF           ,"mSumETnoHF/D");
  if(mIsMCarlo){
    mTree->Branch("pthat"   ,&mPtHat               ,"mPtHat/D");
  }
}

void InclusiveJetTreeProducer::clearTreeVectors() {
  mPt       ->clear();
  mEta      ->clear();
  mPhi      ->clear();
  mE        ->clear();
  mEmf      ->clear();
  mNtrkVx   ->clear();
  mNtrkCalo ->clear();
  mCTPt     ->clear();
  mCTEta    ->clear();
  mCTPhi    ->clear();
  mVTPt     ->clear();
  mVTEta    ->clear();
  mVTPhi    ->clear();
  mN90      ->clear();
  mPVx      ->clear();
  mPVy      ->clear();
  mPVz      ->clear();
  mHLTNames ->clear();
  mL1Names  ->clear();
}
