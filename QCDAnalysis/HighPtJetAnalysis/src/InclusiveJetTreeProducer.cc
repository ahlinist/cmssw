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
  mFileName           = cfg.getParameter<std::string>              ("fileName");
  mJetExtender        = cfg.getParameter<std::string>              ("jetExtender");
  mTriggerNames       = cfg.getParameter<std::vector<std::string> >("jetTriggerNames");
  mL1TriggerNames     = cfg.getParameter<std::vector<std::string> >("l1TriggerNames");
  mTriggerProcessName = cfg.getParameter<std::string>              ("triggerProcessName"); 
  mTriggerResultsTag  = cfg.getParameter<edm::InputTag>            ("triggerResultsTag");
  mEtaMax             = cfg.getParameter<double>                   ("etaMax"); 
  mPtMin              = cfg.getParameter<double>                   ("ptMin");     
  mIsMCarlo           = cfg.getParameter<bool>                     ("isMCarlo");   
}
//////////////////////////////////////////////////////////////////////////////////////////
void InclusiveJetTreeProducer::beginJob(EventSetup const& iSetup) 
{
  mFile = new TFile(mFileName.c_str(),"RECREATE");
  mTree = new TTree("InclusiveJetTree","InclusiveJetTree");
  
  mTree->Branch("pt"       ,&mPt       ,"mPt[20]/F");
  mTree->Branch("eta"      ,&mEta      ,"mEta[20]/F");
  mTree->Branch("phi"      ,&mPhi      ,"mPhi[20]/F");
  mTree->Branch("n90"      ,&mN90      ,"mN90[20]/F");
  mTree->Branch("e"        ,&mE        ,"mE[20]/F");
  mTree->Branch("ctpt"     ,&mCTPt     ,"mCTPt[20]/F");
  mTree->Branch("cteta"    ,&mCTEta    ,"mCTEta[20]/F");
  mTree->Branch("ctphi"    ,&mCTPhi    ,"mCTPhi[20]/F");
  mTree->Branch("vtpt"     ,&mVTPt     ,"mVTPt[20]/F");
  mTree->Branch("vteta"    ,&mVTEta    ,"mVTEta[20]/F");
  mTree->Branch("vtphi"    ,&mVTPhi    ,"mVTPhi[20]/F");
  mTree->Branch("nPV"      ,&mNPV      ,"mNPV/I");
  mTree->Branch("nPVx"     ,&mPVx      ,"mPVx/F");
  mTree->Branch("nPVy"     ,&mPVy      ,"mPVy/F");
  mTree->Branch("nPVz"     ,&mPVz      ,"mPVy/F");
  mTree->Branch("emf"      ,&mEmf      ,"mEmf[20]/F");
  mTree->Branch("nTrkVx"   ,&mNtrkVx   ,"mNtrkVx[20]/I");
  mTree->Branch("nTrkCalo" ,&mNtrkCalo ,"mNtrkCalo[20]/I");
  mTree->Branch("met"      ,&mMET     ,"mMET[2]/F");
  mTree->Branch("sumet"    ,&mSumET   ,"mSumET[2]/F");
  mTree->Branch("HLTBits"  ,&mHLTBits  ,"mHLTBits[6]/I");
  mTree->Branch("L1Bits"   ,&mL1Bits   ,"mL1Bits[5]/I");
  mTree->Branch("jetSize"  ,&mJetSize ,"mJetSize/I");
  mTree->Branch("event"    ,&mEvent   ,"mEvent/I");
  mTree->Branch("run"      ,&mRun     ,"mRun/I");
  
  if (mIsMCarlo)
    {
      mTree->Branch("pthat"     ,&mPtHat       ,"mPtHat/F"); 

    }
  
  //must be done at beginRun and not only at beginJob, because 
    //trigger names are allowed to change by run.
  mHltConfig.init(mTriggerProcessName);

  for(unsigned int i=0;i<mTriggerNames.size();i++)
    {
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

  if (mFile != 0) 
    {
      mFile->cd();
      mTree->Write();
      delete mFile;
      mFile = 0;      
    }

}
//////////////////////////////////////////////////////////////////////////////////////////
void InclusiveJetTreeProducer::analyze(edm::Event const& event, edm::EventSetup const& iSetup) 
{ 
  Handle<GenEventInfoProduct> hEventInfo;

  //add pt Hat if Monte Carlo
    if (mIsMCarlo)
    { 
      event.getByLabel("generator", hEventInfo);
      mPtHat = hEventInfo->binningValues()[0];
    }
    // get Run and Event info
  mRun = event.id().run();
  mEvent = event.id().event();

  // get L1 Trigger menu record
  edm::ESHandle<L1GtTriggerMenu> menuRcd;
  iSetup.get<L1GtTriggerMenuRcd>().get(menuRcd);
  const L1GtTriggerMenu* menu = menuRcd.product();
  // get L1 Trigger Readout Record
  edm::Handle< L1GlobalTriggerReadoutRecord > gtReadoutRecord;
  event.getByLabel( edm::InputTag("gtDigis"), gtReadoutRecord);
  const  DecisionWord& gtDecisionWordBeforeMask = gtReadoutRecord->decisionWord();
  
  int L1Tsize = mL1TriggerNames.size();
  // fill L1 bits
  for(int i = 0; i < 5; i++){
	mL1Bits[i] = 0;}
   for(int j = 0; j < L1Tsize; j++){
    bool result = menu->gtAlgorithmResult( mL1TriggerNames[j], gtDecisionWordBeforeMask);
    if(result) {mL1Bits[j] = 1;}
    }
  

  
   
  ////////////Vertices//////////////
  Handle<reco::VertexCollection> recVtxs;
  event.getByLabel("offlinePrimaryVertices",recVtxs);
  mNPV = 0;
  mPVx = 0;
  mPVy = 0;
  mPVz = 0;
  for(unsigned int ind=0;ind<recVtxs->size();ind++)
    if (!((*recVtxs)[ind].isFake()))
     mNPV++; 
  if (mNPV>0)
    {
      mPVx = (*recVtxs)[0].x();
      mPVy = (*recVtxs)[0].y(); 
      mPVz = (*recVtxs)[0].z();  
    }

  ////////////// Jets //////
  edm::Handle<CaloJetCollection> jets;
  event.getByLabel(mJetsName,jets);
  Handle<JetExtendedAssociation::Container> jetExtender;
  event.getByLabel(mJetExtender,jetExtender);
  int counter = 0;

  // get Jet Size (need to know this since we're stopping at 20 jets)
  mJetSize = (*jets).size();

  // Initialize jet variables with unphysical non-zero values
  for(int jin = 0; jin < 20; jin++){
	mPt[jin] = -1;
	mEta[jin] = -100;
	mPhi[jin] = -100;
	mE[jin] = -1;
	mEmf[jin] = -100;
	mNtrkVx[jin] = -1;
	mNtrkCalo[jin] = -1;
	mCTPt[jin] = -1;
	mCTEta[jin] = -100;
	mCTPhi[jin] = -100;
	mVTPt[jin] = -1;
	mVTEta[jin] = -100;
	mVTPhi[jin] = -100;
	mN90[jin] = -1;
  }
  
  // fill jet variables
  for(int ind=0; ind < mJetSize; ind++) 
    {
      // cut on pt and eta (specified in config file)
      if(((*jets)[ind].pt() > mPtMin) && (abs((*jets)[ind].eta()) < mEtaMax)  && (counter < 20)){
	// fill Energy 4-vector, jet ID quantities
	mPt[counter]       = (*jets)[ind].pt();
	mEta[counter]      = (*jets)[ind].eta();
	mPhi[counter]      = (*jets)[ind].phi();
	mE[counter]        = (*jets)[ind].energy();
	mEmf[counter]      = (*jets)[ind].emEnergyFraction();
        mN90[counter]      = (*jets)[ind].n90();
	mNtrkVx[counter]   = JetExtendedAssociation::tracksAtVertexNumber(*jetExtender,(*jets)[ind]);
	mNtrkCalo[counter] = JetExtendedAssociation::tracksAtCaloNumber(*jetExtender,(*jets)[ind]);
	// get Tracks at Calorimeter and Vertex
	reco::JetExtendedAssociation::LorentzVector TracksatCalo = JetExtendedAssociation::tracksAtCaloP4(*jetExtender,(*jets)[ind]);
	reco::JetExtendedAssociation::LorentzVector TracksatVx = JetExtendedAssociation::tracksAtVertexP4(*jetExtender, (*jets)[ind]);
	mCTPt[counter] = TracksatCalo.pt();
	mCTEta[counter] = TracksatCalo.eta();
	mCTPhi[counter] = TracksatCalo.phi();
	mVTPt[counter] = TracksatVx.pt();
	mVTEta[counter] = TracksatVx.eta();
	mVTPhi[counter] = TracksatVx.phi();
	// increment counter (stop at 20 jets)
	counter++;
      }
    }
  counter = 0;
  ////////////// Trigger bits //////
  edm::Handle<edm::TriggerResults> triggerResultsHandle;
  event.getByLabel(mTriggerResultsTag,triggerResultsHandle); 
  if (!triggerResultsHandle.isValid())
    {
      string errorMessage = "Requested TriggerResult is not present in file! -- \n";
      throw  cms::Exception("Configuration",errorMessage);
    }
  //check if configuration matches trigger results:
  assert(triggerResultsHandle->size() == mHltConfig.size());
  for (unsigned int i=0;i<6;i++)  mHLTBits[i] = 0;
  for (unsigned int i=0;i<mTriggerIndex.size();i++)
    { 
      bool result = triggerResultsHandle->accept(mTriggerIndex[i]); 
      if (result)
        mHLTBits[i] = 1;
    }



  ////////////// MET //////
  Handle<CaloMETCollection> met;
  event.getByLabel(mMetName,met);
  mMET[0]   = -1;
  mSumET[0] = -1;
  if (met->size() == 0) 
    {
      mMET[0]   = (*met)[0].et();
      mSumET[0] = (*met)[0].sumEt();
    }
  // 2nd flavor of MET
  Handle<CaloMETCollection> metNoHF;
  event.getByLabel(mMetNoHFName,metNoHF);
  mMET[1]   = -1;
  mSumET[1] = -1;    
  if (metNoHF->size() != 0)
    {
      mMET[1]   = (*metNoHF)[0].et();
      mSumET[1] = (*metNoHF)[0].sumEt();
    }
//  mTree->Fill();
}
//////////////////////////////////////////////////////////////////////////////////////////
InclusiveJetTreeProducer::~InclusiveJetTreeProducer()
{
  // close the file and tree
  if(mFile != 0){
    mFile = 0;}
  if(mTree != 0){
    mTree = 0;}
}
