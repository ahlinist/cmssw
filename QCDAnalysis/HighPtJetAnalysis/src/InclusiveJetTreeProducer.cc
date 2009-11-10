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
  mFileName           = cfg.getParameter<std::string>              ("fileName");
  mJetExtender        = cfg.getParameter<std::string>              ("jetExtender");
  mTriggerNames       = cfg.getParameter<std::vector<std::string> >("jetTriggerNames");
  mTriggerProcessName = cfg.getParameter<std::string>              ("triggerProcessName"); 
  mTriggerResultsTag  = cfg.getParameter<edm::InputTag>            ("triggerResultsTag");
  mEtaMax             = cfg.getParameter<double>                   ("etaMax"); 
  mPtMin              = cfg.getParameter<double>                   ("ptMin");                 
}
//////////////////////////////////////////////////////////////////////////////////////////
void InclusiveJetTreeProducer::beginJob(EventSetup const& iSetup) 
{
  mFile = new TFile(mFileName.c_str(),"RECREATE");
  mTree = new TTree("InclusiveJetTree","InclusiveJetTree");
  
  mTree->Branch("px"       ,mPx       ,"mPx[20]/F");
  mTree->Branch("py"       ,mPy       ,"mPy[20]/F");
  mTree->Branch("pz"       ,mPz       ,"mPz[20]/F");
  mTree->Branch("pt"       ,mPt       ,"mPt[20]/F");
  mTree->Branch("eta"      ,mEta      ,"mEta[20]/F");
  mTree->Branch("phi"      ,mPhi      ,"mPhi[20]/F");
  mTree->Branch("n90"      ,mN90      ,"mN90[20]/F");
  mTree->Branch("e"        ,mE        ,"mE[20]/F");
  mTree->Branch("ctpt"     ,mCTPt     ,"mCTPt[20]/F");
  mTree->Branch("cteta"    ,mCTEta    ,"mCTEta[20]/F");
  mTree->Branch("ctphi"    ,mCTPhi    ,"mCTPhi[20]/F");
  mTree->Branch("vtpt"     ,mVTPt     ,"mVTPt[20]/F");
  mTree->Branch("vteta"    ,mVTEta    ,"mVTEta[20]/F");
  mTree->Branch("vtphi"    ,mVTPhi    ,"mVTPhi[20]/F");
  mTree->Branch("nPV"      ,&mNPV      ,"mNPV/I");
  mTree->Branch("nPVx"     ,&mPVx      ,"mPVx/F");
  mTree->Branch("nPVy"     ,&mPVy      ,"mPVy/F");
  mTree->Branch("nPVz"     ,&mPVz      ,"mPVy/F");
  mTree->Branch("emf"      ,mEmf      ,"mEmf[20]/F");
  mTree->Branch("nTrkVx"   ,mNtrkVx   ,"mNtrkVx[20]/F");
  mTree->Branch("nTrkCalo" ,mNtrkCalo ,"mNtrkCalo[20]/F");
  mTree->Branch("met"      ,&mMET     ,"mMET/F");
  mTree->Branch("sumet"    ,&mSumET   ,"mSumET/F");
  mTree->Branch("HLTBits"  ,mHLTBits  ,"mHLTBits[6]/I");
  mTree->Branch("jetSize"  ,&mJetSize ,"mJetSize/I");

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

  ////////////Vertices//////////////
  Handle<reco::VertexCollection> recVtxs;
  event.getByLabel("offlinePrimaryVertices",recVtxs);
  mNPV = 0;
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

  mJetSize = (*jets).size();

  for(int ind=0;ind<(*jets).size();ind++) 
    {
      if(((*jets)[ind].pt() > mPtMin) && (abs((*jets)[ind].eta()) < mEtaMax  && counter < 20)){
	counter++;
	mPx[ind]       = (*jets)[ind].px();
	mPy[ind]       = (*jets)[ind].py(); 
	mPz[ind]       = (*jets)[ind].pz();
	mPt[ind]       = (*jets)[ind].pt();
	mEta[ind]      = (*jets)[ind].eta();
	mPhi[ind]      = (*jets)[ind].phi();
	mE[ind]        = (*jets)[ind].energy();
	mEmf[ind]      = (*jets)[ind].emEnergyFraction(); 
	mNtrkVx[ind]   = JetExtendedAssociation::tracksAtVertexNumber(*jetExtender,(*jets)[ind]);
	mNtrkCalo[ind] = JetExtendedAssociation::tracksAtCaloNumber(*jetExtender,(*jets)[ind]); 
	reco::JetExtendedAssociation::LorentzVector TracksatCalo = JetExtendedAssociation::tracksAtCaloP4(*jetExtender,(*jets)[ind]);
	reco::JetExtendedAssociation::LorentzVector TracksatVx = JetExtendedAssociation::tracksAtVertexP4(*jetExtender, (*jets)[ind]);
	mCTPt[ind] = TracksatCalo.pt();
	mCTEta[ind] = TracksatCalo.eta();
	mCTPhi[ind] = TracksatCalo.phi();
	mVTPt[ind] = TracksatVx.pt();
	mVTEta[ind] = TracksatVx.eta();
	mVTPhi[ind] = TracksatVx.phi();
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
  //check if configuration matches trigger results:
  assert(triggerResultsHandle->size() == mHltConfig.size());
  for (unsigned int i=0;i<6;i++)
    mHLTBits[i] = 0;
  for (unsigned int i=0;i<mTriggerIndex.size();i++)
    { 
      bool result = triggerResultsHandle->accept(mTriggerIndex[i]); 
      if (result)
        mHLTBits[i] = 1;
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
  mTree->Fill();
}
//////////////////////////////////////////////////////////////////////////////////////////
InclusiveJetTreeProducer::InclusiveJetTreeProducer() 
{
  mFile = 0;
  mTree = 0;
}
