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
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/JetReco/interface/JetExtendedAssociation.h"
#include "DataFormats/JetReco/interface/JetID.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "L1Trigger/GlobalTriggerAnalyzer/interface/L1GtUtils.h"

using namespace edm;
using namespace reco;
using namespace std;
typedef math::PtEtaPhiELorentzVectorF LorentzVector;
InclusiveJetTreeProducer::InclusiveJetTreeProducer(edm::ParameterSet const& cfg) 
{
  mJetsName               = cfg.getParameter<std::string>              ("jets");
  mJetsIDName             = cfg.getParameter<std::string>              ("jetsID");
  mMetName                = cfg.getParameter<std::string>              ("met");
  mMetNoHFName            = cfg.getParameter<std::string>              ("metNoHF");
  mJetExtender            = cfg.getParameter<std::string>              ("jetExtender");
  mHcalNoiseTag           = cfg.getParameter<edm::InputTag>            ("hcalNoiseTag");
  mTriggerNames           = cfg.getParameter<std::vector<std::string> >("jetTriggerNames");
  mL1TriggerNames         = cfg.getParameter<std::vector<std::string> >("l1TriggerNames");   
  mTriggerProcessName     = cfg.getParameter<std::string>              ("triggerProcessName"); 
  mTriggerResultsTag      = cfg.getParameter<edm::InputTag>            ("triggerResultsTag");               
  mL1GTReadoutRcdSource   = cfg.getParameter<edm::InputTag>            ("L1GTReadoutRcdSource");
  mL1GTObjectMapRcdSource = cfg.getParameter<edm::InputTag>            ("L1GTObjectMapRcdSource");
  mJetPtMin               = cfg.getParameter<double>                   ("minJetPt");
  mIsMCarlo               = cfg.getUntrackedParameter<bool>            ("isMCarlo",false);
  mGenJetsName            = cfg.getUntrackedParameter<std::string>     ("genjets","");

  mFillHLT = (!mTriggerNames.empty());
  mFillL1  = (!mL1TriggerNames.empty());
}
//////////////////////////////////////////////////////////////////////////////////////////
void InclusiveJetTreeProducer::beginJob(EventSetup const& iSetup) 
{
  mTree = fs->make<TTree>("InclusiveJetTree","InclusiveJetTree");

  buildTree();
  
  //must be done at beginRun and not only at beginJob, because 
  //trigger names are allowed to change by run.
  if (mFillHLT)
    {
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

  mRunNo = event.id().run();
  mEvtNo = event.id().event();
  mLumi  = event.luminosityBlock();
  mBunch = event.bunchCrossing();

  ////////////Vertices//////////////
  Handle<reco::VertexCollection> recVtxs;
  event.getByLabel("offlinePrimaryVertices",recVtxs);
  for(unsigned int ind=0;ind<recVtxs->size();ind++) 
    {
      if (!((*recVtxs)[ind].isFake())) 
        {
	  mPVx      ->push_back((*recVtxs)[ind].x());
	  mPVy      ->push_back((*recVtxs)[ind].y());
	  mPVz      ->push_back((*recVtxs)[ind].z());
          mPVchi2   ->push_back((*recVtxs)[ind].chi2());
          mPVndof   ->push_back((*recVtxs)[ind].ndof());
          mPVntracks->push_back((*recVtxs)[ind].tracksSize());
        }
    }

  ////////Get PtHat///////////////
  Handle<GenEventInfoProduct> hEventInfo;
  if (mIsMCarlo)
    { 
      event.getByLabel("generator", hEventInfo);
      mPtHat  = hEventInfo->binningValues()[0];
      mWeight = hEventInfo->weight();
    }
  ///////// HcalNoiseCollection //////////////                                                                               
  Handle<HcalNoiseRBXCollection> rbxColl;
  event.getByLabel(mHcalNoiseTag,rbxColl);
  if (!rbxColl.isValid()) 
    {
      throw edm::Exception(edm::errors::ProductNotFound)
      << " could not find HcalNoiseRBXCollection named " << "hcalnoise" << ".\n";
      return;
    }
  // loop over the RBXs 
  map<CaloTowerDetId, double> hcalNoise;
  for(HcalNoiseRBXCollection::const_iterator rit=rbxColl->begin(); rit!=rbxColl->end(); ++rit) 
    {
      HcalNoiseRBX rbx = (*rit);
      const vector<HcalNoiseHPD> hpds = rbx.HPDs();
      for(unsigned ihpd=0; ihpd<hpds.size(); ihpd++)
        {
          const RefVector<CaloTowerCollection> noiseCTowers = hpds[ihpd].caloTowers();
          for(unsigned int itow=0; itow<noiseCTowers.size(); itow++)
            hcalNoise.insert(pair<CaloTowerDetId, double>(noiseCTowers[itow]->id(),noiseCTowers[itow]->hadEnergy()));
        }
    }
  Handle<HcalNoiseSummary> noiseSummary; 	 
  event.getByLabel("hcalnoise", noiseSummary); 	 
  const HcalNoiseSummary summary = *noiseSummary; 	 
  mLooseHcalNoise = summary.passLooseNoiseFilter(); 	 
  mTightHcalNoise = summary.passTightNoiseFilter();
  ////////////// Jets //////
  Handle<CaloJetCollection> jets;
  event.getByLabel(mJetsName,jets);
  Handle<JetExtendedAssociation::Container> jetExtender;
  event.getByLabel(mJetExtender,jetExtender);
  Handle<ValueMap<reco::JetID> > jetsID;
  event.getByLabel(mJetsIDName,jetsID);
  Handle<GenJetCollection> genjets;
  if (mIsMCarlo)
    event.getByLabel(mGenJetsName,genjets);
  ////////////// Trigger //////
  //===================== save HLT Trigger information =======================
  Handle<TriggerResults> triggerResultsHandle;
  if (mFillHLT)
    {
      event.getByLabel(mTriggerResultsTag,triggerResultsHandle); 
      if (!triggerResultsHandle.isValid())
        {
          string errorMessage = "Requested TriggerResult is not present in file! -- \n";
          throw  cms::Exception("Configuration",errorMessage);
        }
      for(unsigned int i=0;i<mTriggerNames.size();i++) 
        {
          bool accept = triggerResultsHandle->accept(mHltConfig.triggerIndex(mTriggerNames[i]));
          if (accept) 
            mHLTNames->push_back(mTriggerNames[i]);
        }
    }
  //===================== save L1 Trigger information ======================= 
  // get L1TriggerReadout records
  Handle<L1GlobalTriggerReadoutRecord> gtRecord;
  Handle<L1GlobalTriggerObjectMapRecord> gtOMRec;
  if (mFillL1)
    {
      event.getByLabel(mL1GTObjectMapRcdSource, gtOMRec);
      event.getByLabel(mL1GTReadoutRcdSource,gtRecord);
      // sanity check on L1 Trigger Records
      if (!gtRecord.isValid()) 
        {
          cout << "\nL1GlobalTriggerReadoutRecord with \n \n not found"
          "\n  --> returning false by default!\n" << endl;
        }
      // L1 decision word
      const DecisionWord dWord = gtRecord->decisionWord();
      //map<string, int> l1map;
      for(unsigned int i=0; i<mL1TriggerNames.size(); i++) 
        {
          //map<string, int>::const_iterator itr = l1map.find(mL1TriggerNames[i]);
          bool algResult = l1AlgorithmResult(event, iSetup, mL1TriggerNames[i] );
          if (algResult) 
            mL1Names->push_back(mL1TriggerNames[i]);
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
  
  if ((*jets).size() > 0);
     {
       for(unsigned int ind=0;ind<(*jets).size();ind++)
         {       
           if ((*jets)[ind].pt() < mJetPtMin) continue;
           if (mIsMCarlo)
             {
               float rmin(99);
               const reco::Candidate& rec = (*jets)[ind];
               for(unsigned int indGen=0;indGen<(*genjets).size();indGen++)
                 {
                   const reco::Candidate& gen = (*genjets)[indGen];
                   double deltaR = reco::deltaR(rec,gen);
                   if (deltaR < rmin)
                     rmin = deltaR;
                 }
               mGenMatchR ->push_back(rmin);
             }
           LorentzVector TrkCaloP4 = JetExtendedAssociation::tracksAtCaloP4(*jetExtender,(*jets)[ind]);
           LorentzVector TrkVtxP4  = JetExtendedAssociation::tracksAtVertexP4(*jetExtender,(*jets)[ind]);
           RefToBase<Jet> jetRef(Ref<CaloJetCollection>(jets,ind));
           mPt        ->push_back((*jets)[ind].pt());
           mEta       ->push_back((*jets)[ind].eta());
           mEtaD      ->push_back((*jets)[ind].detectorP4().eta());
           mY         ->push_back((*jets)[ind].y());
           mPhi       ->push_back((*jets)[ind].phi());
           mE         ->push_back((*jets)[ind].energy());
           mN90       ->push_back((*jets)[ind].n90());
           mN90Hits   ->push_back(int((*jetsID)[jetRef].n90Hits));
           mfHPD      ->push_back((*jetsID)[jetRef].fHPD);
           mfRBX      ->push_back((*jetsID)[jetRef].fRBX);
           mEmf       ->push_back((*jets)[ind].emEnergyFraction());
           mEtaMoment ->push_back((*jets)[ind].etaetaMoment());
           mPhiMoment ->push_back((*jets)[ind].phiphiMoment());
           mNtrkVtx   ->push_back(JetExtendedAssociation::tracksAtVertexNumber(*jetExtender,(*jets)[ind]));
           mNtrkCalo  ->push_back(JetExtendedAssociation::tracksAtCaloNumber(*jetExtender,(*jets)[ind]));
           mTrkCaloPt ->push_back(TrkCaloP4.pt());
           mTrkCaloEta->push_back(TrkCaloP4.eta());
           mTrkCaloPhi->push_back(TrkCaloP4.phi());
           mTrkVtxPt  ->push_back(TrkVtxP4.pt());
           mTrkVtxEta ->push_back(TrkVtxP4.eta());
           mTrkVtxPhi ->push_back(TrkVtxP4.phi());
           double jetEneNoise=0.0;
           vector< CaloTowerPtr >jTowers = (*jets)[ind].getCaloConstituents();
           for (unsigned int itow=0; itow<jTowers.size(); itow++)
             {
               map<CaloTowerDetId, double>::iterator thisTow = hcalNoise.find(jTowers[itow]->id());
               if (thisTow != hcalNoise.end()) 
                 jetEneNoise += jTowers[itow]->energy();
             }
           mfHcalNoise->push_back(jetEneNoise/(*jets)[ind].energy());
         }
    }

  mTree->Fill();
}
//////////////////////////////////////////////////////////////////////////////////////////
InclusiveJetTreeProducer::~InclusiveJetTreeProducer() 
{
}
//////////////////////////////////////////////////////////////////////////////////////////
void InclusiveJetTreeProducer::buildTree() 
{
  mGenMatchR    = new std::vector<double>();
  mPt           = new std::vector<double>();
  mEta          = new std::vector<double>();
  mEtaD         = new std::vector<double>();
  mY            = new std::vector<double>();
  mPhi          = new std::vector<double>();
  mE            = new std::vector<double>();
  mEmf          = new std::vector<double>();
  mEtaMoment    = new std::vector<double>();
  mPhiMoment    = new std::vector<double>();
  mNtrkVtx      = new std::vector<int>   ();
  mNtrkCalo     = new std::vector<int>   ();
  mTrkCaloPt    = new std::vector<double>();
  mTrkCaloEta   = new std::vector<double>();
  mTrkCaloPhi   = new std::vector<double>();
  mTrkVtxPt     = new std::vector<double>();
  mTrkVtxEta    = new std::vector<double>();
  mTrkVtxPhi    = new std::vector<double>();
  mN90          = new std::vector<int>   ();
  mN90Hits      = new std::vector<int>   ();
  mfHPD         = new std::vector<double>();
  mfRBX         = new std::vector<double>();
  mfHcalNoise   = new std::vector<double>();
  mPVx          = new std::vector<double>();
  mPVy          = new std::vector<double>();
  mPVz          = new std::vector<double>();
  mPVchi2       = new std::vector<double>();
  mPVndof       = new std::vector<double>();
  mPVntracks    = new std::vector<int>();
  mHLTNames     = new std::vector<std::string>();
  mL1Names      = new std::vector<std::string>();
  
  mTree->Branch("pt"                 ,"vector<double>"      ,&mPt);
  mTree->Branch("eta"                ,"vector<double>"      ,&mEta);
  mTree->Branch("etaDetector"        ,"vector<double>"      ,&mEtaD);
  mTree->Branch("y"                  ,"vector<double>"      ,&mY);
  mTree->Branch("phi"                ,"vector<double>"      ,&mPhi);
  mTree->Branch("e"                  ,"vector<double>"      ,&mE);
  mTree->Branch("emf"                ,"vector<double>"      ,&mEmf);
  mTree->Branch("etaMoment"          ,"vector<double>"      ,&mEtaMoment);
  mTree->Branch("phiMoment"          ,"vector<double>"      ,&mPhiMoment);
  mTree->Branch("nTrkVtx"            ,"vector<int>"         ,&mNtrkVtx);
  mTree->Branch("nTrkCalo"           ,"vector<int>"         ,&mNtrkCalo);
  mTree->Branch("TrkCaloPt"          ,"vector<double>"      ,&mTrkCaloPt);
  mTree->Branch("TrkCaloEta"         ,"vector<double>"      ,&mTrkCaloEta);
  mTree->Branch("TrkCaloPhi"         ,"vector<double>"      ,&mTrkCaloPhi);
  mTree->Branch("TrkVtxPt"           ,"vector<double>"      ,&mTrkVtxPt);
  mTree->Branch("TrkVtxEta"          ,"vector<double>"      ,&mTrkVtxEta);
  mTree->Branch("TrkVtxPhi"          ,"vector<double>"      ,&mTrkVtxPhi);
  mTree->Branch("n90"                ,"vector<int>"         ,&mN90);
  mTree->Branch("n90hits"            ,"vector<int>"         ,&mN90Hits);
  mTree->Branch("fHPD"               ,"vector<double>"      ,&mfHPD);
  mTree->Branch("fRBX"               ,"vector<double>"      ,&mfRBX);  
  mTree->Branch("fHcalNoise"         ,"vector<double>"      ,&mfHcalNoise);
  mTree->Branch("PVx"                ,"vector<double>"      ,&mPVx);
  mTree->Branch("PVy"                ,"vector<double>"      ,&mPVy);
  mTree->Branch("PVz"                ,"vector<double>"      ,&mPVz);
  mTree->Branch("PVchi2"             ,"vector<double>"      ,&mPVchi2);
  mTree->Branch("PVndof"             ,"vector<double>"      ,&mPVndof);
  mTree->Branch("PVntracks"          ,"vector<int>"         ,&mPVntracks);
  mTree->Branch("hltNames"           ,"vector<string>"      ,&mHLTNames);
  mTree->Branch("l1Names"            ,"vector<string>"      ,&mL1Names);
  mTree->Branch("evtNo"              ,&mEvtNo               ,"mEvtNo/I");
  mTree->Branch("runNo"              ,&mRunNo               ,"mRunNo/I");
  mTree->Branch("lumi"               ,&mLumi                ,"mLumi/I");
  mTree->Branch("bunch"              ,&mBunch               ,"mBunch/I");
  mTree->Branch("met"                ,&mMET                 ,"mMET/D");
  mTree->Branch("sumet"              ,&mSumET               ,"mSumET/D");
  mTree->Branch("metNoHF"            ,&mMETnoHF             ,"mMETnoHF/D");
  mTree->Branch("sumetNoHF"          ,&mSumETnoHF           ,"mSumETnoHF/D");
  mTree->Branch("passLooseHcalNoise" ,&mLooseHcalNoise      ,"mLooseHcalNoise/I"); 	 
  mTree->Branch("passTightHcalNoise" ,&mTightHcalNoise      ,"mTightHcalNoise/I");
  if(mIsMCarlo)
    {
      mTree->Branch("pthat"          ,&mPtHat               ,"mPtHat/D");
      mTree->Branch("weight"         ,&mWeight              ,"mWeight/D");
      mTree->Branch("genMatchR"      ,"vector<double>"      ,&mGenMatchR);
    }
}
//////////////////////////////////////////////////////////////////////////////////////////
void InclusiveJetTreeProducer::clearTreeVectors() 
{
  mGenMatchR  ->clear();
  mPt        ->clear();
  mEta       ->clear();
  mEtaD      ->clear();
  mY         ->clear();
  mPhi       ->clear();
  mE         ->clear();
  mEmf       ->clear();
  mEtaMoment ->clear();
  mPhiMoment ->clear();
  mNtrkVtx   ->clear();
  mNtrkCalo  ->clear();
  mTrkCaloPt ->clear();
  mTrkCaloEta->clear();
  mTrkCaloPhi->clear();
  mTrkVtxPt  ->clear();
  mTrkVtxEta ->clear();
  mTrkVtxPhi ->clear();
  mN90       ->clear(); 
  mN90Hits   ->clear();
  mfHPD      ->clear();
  mfRBX      ->clear();
  mfHcalNoise->clear();
  mPVx       ->clear();
  mPVy       ->clear();
  mPVz       ->clear();
  mPVndof    ->clear();
  mPVchi2    ->clear();
  mPVntracks ->clear();
  mHLTNames  ->clear();
  mL1Names   ->clear();
}
