#include <iostream>
#include <sstream>
#include <istream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cmath>
#include <functional>

#include "JetMETCorrections/DijetBalance/interface/RelJECTreeProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/JetReco/interface/JPTJet.h"
#include "DataFormats/JetReco/interface/JPTJetCollection.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

using namespace edm;
using namespace reco;
using namespace std;

RelJECTreeProducer::RelJECTreeProducer(edm::ParameterSet const& cfg) 
{
  mCaloJetsName = cfg.getParameter<std::string>   ("calojets");
  mPFJetsName   = cfg.getParameter<std::string>   ("pfjets");
  mJPTJetsName  = cfg.getParameter<std::string>   ("jptjets");
  mJetPtMin     = cfg.getParameter<double>        ("minJetPt");
  mJetEtaMax    = cfg.getParameter<double>        ("maxJetEta");
  mIsMCarlo     = cfg.getUntrackedParameter<bool> ("IsMCarlo",false);
}
////////////////////////////////////////////////////////////////////////////////////////// 
void RelJECTreeProducer::beginJob() 
{
  mTree = fs->make<TTree>("RelJECTree","RelJECTree");
  buildTree();
}
//////////////////////////////////////////////////////////////////////////////////////////
void RelJECTreeProducer::endJob() 
{

}
//////////////////////////////////////////////////////////////////////////////////////////
void RelJECTreeProducer::analyze(edm::Event const& event, edm::EventSetup const& iSetup) 
{ 
  clearTreeVectors();

  mRunNo = event.id().run();
  mEvtNo = event.id().event();
  mLumi  = event.luminosityBlock();
  mBunch = event.bunchCrossing();

  ////////Get PtHat///////////////
  Handle<GenEventInfoProduct> hEventInfo;
  if (mIsMCarlo)
    { 
      event.getByLabel("generator", hEventInfo);
      mPtHat  = hEventInfo->binningValues()[0];
      mWeight = hEventInfo->weight();
    }

  ////////////// Jets //////
  Handle<CaloJetCollection> calojets;
  event.getByLabel(mCaloJetsName,calojets);
  Handle<PFJetCollection> pfjets;
  event.getByLabel(mPFJetsName,pfjets);
  Handle<JPTJetCollection> jptjets;
  event.getByLabel(mJPTJetsName,jptjets);
  for(unsigned int ind=0;ind<(*pfjets).size();ind++)
    {      
      //cout<<ind<<" "<<(*pfjets)[ind].pt()<<" "<<(*pfjets)[ind].eta()<<endl; 
      if ((*pfjets)[ind].pt() < mJetPtMin || fabs((*pfjets)[ind].eta()) > mJetEtaMax) continue;
      const reco::Candidate& rec = (*pfjets)[ind];
      //----matching with CaloJets --------
      float rmin(999);
      int indMatchedCalo(-1);
      for(unsigned int indCalo=0;indCalo<(*calojets).size();indCalo++)
        {
          const reco::Candidate& calo = (*calojets)[indCalo];
          double deltaR = reco::deltaR(rec,calo);
          if (deltaR < rmin)
            {
              rmin = deltaR;
              indMatchedCalo = indCalo;
            }
        } 
      if (indMatchedCalo >= 0)
        {
          mCaloMatchR  ->push_back(rmin);
          mCaloMatchE  ->push_back((*calojets)[indMatchedCalo].energy());
          mCaloMatchPt ->push_back((*calojets)[indMatchedCalo].pt());
          mCaloMatchEta->push_back((*calojets)[indMatchedCalo].eta());
          mCaloMatchPhi->push_back((*calojets)[indMatchedCalo].phi());
          mCaloResponse->push_back((*calojets)[indMatchedCalo].pt()/(*pfjets)[ind].pt());
        }
      else
        {
          mCaloMatchR  ->push_back(-999);
          mCaloMatchE  ->push_back(-999);
          mCaloMatchPt ->push_back(-999);
          mCaloMatchEta->push_back(-999);
          mCaloMatchPhi->push_back(-999);
          mCaloResponse->push_back(-999);
        }
      //----matching with JPTJets --------
      rmin = 999;
      int indMatchedJPT(-1);
      for(unsigned int indJPT=0;indJPT<(*jptjets).size();indJPT++)
        {
          const reco::Candidate& jpt = (*jptjets)[indJPT];
          double deltaR = reco::deltaR(rec,jpt);
          if (deltaR < rmin)
            {
              rmin = deltaR;
              indMatchedJPT = indJPT;
            }
        }    
      if (indMatchedJPT >= 0)
        {
          mJPTMatchR  ->push_back(rmin);
          mJPTMatchE  ->push_back((*jptjets)[indMatchedJPT].energy());
          mJPTMatchPt ->push_back((*jptjets)[indMatchedJPT].pt());
          mJPTMatchEta->push_back((*jptjets)[indMatchedJPT].eta());
          mJPTMatchPhi->push_back((*jptjets)[indMatchedJPT].phi());
          mJPTResponse->push_back((*jptjets)[indMatchedJPT].pt()/(*pfjets)[ind].pt());
        }
      else
        {
          mJPTMatchR  ->push_back(-999);
          mJPTMatchE  ->push_back(-999); 
          mJPTMatchPt ->push_back(-999);
          mJPTMatchEta->push_back(-999);
          mJPTMatchPhi->push_back(-999);
          mJPTResponse->push_back(-999);
        } 
      mE  ->push_back((*pfjets)[ind].energy());
      mPt ->push_back((*pfjets)[ind].pt());
      mEta->push_back((*pfjets)[ind].eta());
      mPhi->push_back((*pfjets)[ind].phi());
    }// jet loop
  if (preTreeFillCut())
    mTree->Fill();
}
//////////////////////////////////////////////////////////////////////////////////////////
RelJECTreeProducer::~RelJECTreeProducer() 
{
}
//////////////////////////////////////////////////////////////////////////////////////////
void RelJECTreeProducer::buildTree() 
{
  mCaloMatchR   = new std::vector<float>();
  mCaloMatchE   = new std::vector<float>();
  mCaloMatchPt  = new std::vector<float>();
  mCaloMatchEta = new std::vector<float>();
  mCaloMatchPhi = new std::vector<float>();
  mCaloResponse = new std::vector<float>();
  mJPTMatchR    = new std::vector<float>();
  mJPTMatchE    = new std::vector<float>();
  mJPTMatchPt   = new std::vector<float>();
  mJPTMatchEta  = new std::vector<float>();
  mJPTMatchPhi  = new std::vector<float>();
  mJPTResponse  = new std::vector<float>();
  mE            = new std::vector<float>();
  mPt           = new std::vector<float>();
  mEta          = new std::vector<float>();
  mPhi          = new std::vector<float>();
  
  mTree->Branch("energy"          ,"vector<float>" ,&mE);
  mTree->Branch("pt"              ,"vector<float>" ,&mPt);
  mTree->Branch("eta"             ,"vector<float>" ,&mEta);
  mTree->Branch("phi"             ,"vector<float>" ,&mPhi);
  mTree->Branch("evtNo"           ,&mEvtNo         ,"mEvtNo/I");
  mTree->Branch("runNo"           ,&mRunNo         ,"mRunNo/I");
  mTree->Branch("lumi"            ,&mLumi          ,"mLumi/I");
  mTree->Branch("bunch"           ,&mBunch         ,"mBunch/I");
  mTree->Branch("caloMatchR"      ,"vector<float>" ,&mCaloMatchR);
  mTree->Branch("caloMatchE"      ,"vector<float>" ,&mCaloMatchE);
  mTree->Branch("caloMatchPt"     ,"vector<float>" ,&mCaloMatchPt);
  mTree->Branch("caloMatchEta"    ,"vector<float>" ,&mCaloMatchEta);
  mTree->Branch("caloMatchPhi"    ,"vector<float>" ,&mCaloMatchPhi);
  mTree->Branch("caloResponse"    ,"vector<float>" ,&mCaloResponse);
  mTree->Branch("jptMatchR"       ,"vector<float>" ,&mJPTMatchR);
  mTree->Branch("jptMatchE"       ,"vector<float>" ,&mJPTMatchE);
  mTree->Branch("jptMatchPt"      ,"vector<float>" ,&mJPTMatchPt);
  mTree->Branch("jptMatchEta"     ,"vector<float>" ,&mJPTMatchEta);
  mTree->Branch("jptMatchPhi"     ,"vector<float>" ,&mJPTMatchPhi);
  mTree->Branch("jptResponse"     ,"vector<float>" ,&mJPTResponse);
  if(mIsMCarlo)
    {
      mTree->Branch("pthat"          ,&mPtHat               ,"mPtHat/F");
      mTree->Branch("weight"         ,&mWeight              ,"mWeight/F");
    }
}
///////////////////////////////////////////////////////////////////////////////
bool RelJECTreeProducer::preTreeFillCut()
{
  if ((*mPt).size() < 1) return false;
  return true;
}
//////////////////////////////////////////////////////////////////////////////////////////
void RelJECTreeProducer::clearTreeVectors() 
{
  mCaloMatchR  ->clear();
  mCaloMatchE  ->clear();
  mCaloMatchPt ->clear();
  mCaloMatchEta->clear();
  mCaloMatchPhi->clear();
  mCaloResponse->clear();
  mJPTMatchR  ->clear();
  mJPTMatchE  ->clear();
  mJPTMatchPt ->clear();
  mJPTMatchEta->clear();
  mJPTMatchPhi->clear();  
  mJPTResponse->clear();
  mE          ->clear();
  mPt         ->clear();
  mEta        ->clear();
  mPhi        ->clear();
}




