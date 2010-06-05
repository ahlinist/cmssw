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
#include "FWCore/Common/interface/TriggerNames.h"
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
//void InclusiveJetTreeProducer::beginJob(EventSetup const& iSetup) 
void InclusiveJetTreeProducer::beginJob() 
{
  mTree = fs->make<TTree>("InclusiveJetTree","InclusiveJetTree");
  //mFirstEventFlag = true;
  buildTree();
}
//////////////////////////////////////////////////////////////////////////////////////////
void InclusiveJetTreeProducer::endJob() 
{

}
//////////////////////////////////////////////////////////////////////////////////////////
void InclusiveJetTreeProducer::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup)
{
   if (mFillHLT)
     {
       bool changed(true);
       if (mHltConfig.init(iRun,iSetup,mTriggerResultsTag.process(),changed)) 
         {
            // if init returns TRUE, initialisation has succeeded!
            mTriggerIndex.clear();
            for(unsigned int i=0;i<mTriggerNames.size();i++)
              {
                std::cout<<mTriggerNames[i]<<" "<<mHltConfig.triggerIndex(mTriggerNames[i])<<std::endl;
                mTriggerIndex.push_back(mHltConfig.triggerIndex(mTriggerNames[i]));
                if (mTriggerIndex[i] == mHltConfig.size())
                  {
                    string errorMessage="Requested TriggerName does not exist! -- "+mTriggerNames[i]+"\n";
                    //throw  cms::Exception("Configuration",errorMessage);
                  }
              }
            if (changed) 
              {
                // The HLT config has actually changed wrt the previous Run, hence rebook your
                // histograms or do anything else dependent on the revised HLT config
                std::cout<<"Run: "<<iRun.run()<<".....Changed HLTConfig"<<std::endl;
              }
         } 
       else 
         {
           // if init returns FALSE, initialisation has NOT succeeded, which indicates a problem
           // with the file and/or code and needs to be investigated!
           std::cout << " HLT config extraction failure with process name " << mTriggerResultsTag.process()<<std::endl;
           // In this case, all access methods will return empty values!
         }
     }
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
  TriggerNames triggerNames;
  if (mFillHLT)
    {
      int ErrFlag=0;
      event.getByLabel(mTriggerResultsTag,triggerResultsHandle); 
      if (!triggerResultsHandle.isValid())
        {
          string errorMessage = "Requested TriggerResult is not present in file! -- \n";
          cout << errorMessage << endl;
          ErrFlag=-1;
        //  Again we don't want to abort the entire job in this case
        //  throw  cms::Exception("Configuration",errorMessage);
        }

     // triggerNames.init (*(triggerResultsHandle.product())); //deprecated in CMSSW_3_6_X


      for(unsigned int i=0;i<mTriggerNames.size();i++) 
        {
          mHLTTrigResults[i].fired=ErrFlag;
          mHLTTrigResults[i].prescale=1;
          bool accept=false;

          if (ErrFlag>-1) {

          try {
             unsigned int trIndex=mHltConfig.triggerIndex(mTriggerNames[i]);
              if (mHltConfig.size()!=trIndex) {
                accept = triggerResultsHandle->accept(trIndex);
              }
              else {
                accept=false;
                mHLTTrigResults[i].fired=-1;
              }
            }
            catch (...) {
              accept=false;
              mHLTTrigResults[i].fired=-1;
            }

            if (accept) {
              mHLTTrigResults[i].fired=1;
            }
          }
        }
    }
  //===================== save L1 Trigger information ======================= 
  // get L1TriggerReadout records
  Handle<L1GlobalTriggerReadoutRecord> gtRecord;
  if (mFillL1)
    {

    event.getByLabel(mL1GTReadoutRcdSource,gtRecord);
   // sanity check on L1 Trigger Records
    int ErrFlag=0;
    if (!gtRecord.isValid()) 
        {
          cout << "\nL1GlobalTriggerReadoutRecord with \n \n not found"
          "\n  --> returning false by default!\n" << endl;
          ErrFlag=-1;
        }


      m_l1GtUtils.retrieveL1EventSetup(iSetup);

      for(unsigned int i=0; i<mL1TriggerNames.size(); i++) 
        {
          mL1TrigResults[i].fired=ErrFlag; 
          mL1TrigResults[i].prescale=1; 
          
          bool algResult=false;
          try { 
              int errorCode = -1;
              algResult = m_l1GtUtils.decisionAfterMask(event,mL1TriggerNames[i],errorCode);
              // leaving this in here in case somebody wants to look at unmasked bits (comment out the other guy then)
              //algResult = m_l1GtUtils.decisionBeforeMask(event,mL1TriggerNames[i],errorCode);
              mL1TrigResults[i].prescale= m_l1GtUtils.prescaleFactor(event,mL1TriggerNames[i],errorCode);

          }
          catch (...) {
             algResult=false;
             mL1TrigResults[i].fired=-1;
          }
          if (algResult) {
            
            mL1TrigResults[i].fired=1;
          }
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
               float rmin(999);
               int indMatchedGen(-1);
               const reco::Candidate& rec = (*jets)[ind];
               for(unsigned int indGen=0;indGen<(*genjets).size();indGen++)
                 {
                   const reco::Candidate& gen = (*genjets)[indGen];
                   double deltaR = reco::deltaR(rec,gen);
                   if (deltaR < rmin)
                     {
                       rmin = deltaR;
                       indMatchedGen = indGen;
                     }
                 } 
               if (indMatchedGen >= 0)
                 {
                   mGenMatchR  ->push_back(rmin);
                   mGenMatchPt ->push_back((*genjets)[indMatchedGen].pt());
                   mGenMatchEta->push_back((*genjets)[indMatchedGen].eta());
                   mGenMatchPhi->push_back((*genjets)[indMatchedGen].phi());
                 }
               else
                 {
                   mGenMatchR  ->push_back(-999);
                   mGenMatchPt ->push_back(-999);
                   mGenMatchEta->push_back(-999);
                   mGenMatchPhi->push_back(-999);
                 } 
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

  if (preTreeFillCut()) mTree->Fill();
  //mFirstEventFlag=false;
}
//////////////////////////////////////////////////////////////////////////////////////////
InclusiveJetTreeProducer::~InclusiveJetTreeProducer() 
{
}
//////////////////////////////////////////////////////////////////////////////////////////
void InclusiveJetTreeProducer::buildTree() 
{
  mGenMatchR    = new std::vector<float>();
  mGenMatchPt   = new std::vector<float>();
  mGenMatchEta  = new std::vector<float>();
  mGenMatchPhi  = new std::vector<float>();
  mPt           = new std::vector<float>();
  mEta          = new std::vector<float>();
  mEtaD         = new std::vector<float>();
  mY            = new std::vector<float>();
  mPhi          = new std::vector<float>();
  mE            = new std::vector<float>();
  mEmf          = new std::vector<float>();
  mEtaMoment    = new std::vector<float>();
  mPhiMoment    = new std::vector<float>();
  mNtrkVtx      = new std::vector<int>   ();
  mNtrkCalo     = new std::vector<int>   ();
  mTrkCaloPt    = new std::vector<float>();
  mTrkCaloEta   = new std::vector<float>();
  mTrkCaloPhi   = new std::vector<float>();
  mTrkVtxPt     = new std::vector<float>();
  mTrkVtxEta    = new std::vector<float>();
  mTrkVtxPhi    = new std::vector<float>();
  mN90          = new std::vector<int>   ();
  mN90Hits      = new std::vector<int>   ();
  mfHPD         = new std::vector<float>();
  mfRBX         = new std::vector<float>();
  mfHcalNoise   = new std::vector<float>();
  mPVx          = new std::vector<float>();
  mPVy          = new std::vector<float>();
  mPVz          = new std::vector<float>();
  mPVchi2       = new std::vector<float>();
  mPVndof       = new std::vector<float>();
  mPVntracks    = new std::vector<int>();
  
  mTree->Branch("pt"                 ,"vector<float>"      ,&mPt);
  mTree->Branch("eta"                ,"vector<float>"      ,&mEta);
  mTree->Branch("etaDetector"        ,"vector<float>"      ,&mEtaD);
  mTree->Branch("y"                  ,"vector<float>"      ,&mY);
  mTree->Branch("phi"                ,"vector<float>"      ,&mPhi);
  mTree->Branch("e"                  ,"vector<float>"      ,&mE);
  mTree->Branch("emf"                ,"vector<float>"      ,&mEmf);
  mTree->Branch("etaMoment"          ,"vector<float>"      ,&mEtaMoment);
  mTree->Branch("phiMoment"          ,"vector<float>"      ,&mPhiMoment);
  mTree->Branch("nTrkVtx"            ,"vector<int>"         ,&mNtrkVtx);
  mTree->Branch("nTrkCalo"           ,"vector<int>"         ,&mNtrkCalo);
  mTree->Branch("TrkCaloPt"          ,"vector<float>"      ,&mTrkCaloPt);
  mTree->Branch("TrkCaloEta"         ,"vector<float>"      ,&mTrkCaloEta);
  mTree->Branch("TrkCaloPhi"         ,"vector<float>"      ,&mTrkCaloPhi);
  mTree->Branch("TrkVtxPt"           ,"vector<float>"      ,&mTrkVtxPt);
  mTree->Branch("TrkVtxEta"          ,"vector<float>"      ,&mTrkVtxEta);
  mTree->Branch("TrkVtxPhi"          ,"vector<float>"      ,&mTrkVtxPhi);
  mTree->Branch("n90"                ,"vector<int>"         ,&mN90);
  mTree->Branch("n90hits"            ,"vector<int>"         ,&mN90Hits);
  mTree->Branch("fHPD"               ,"vector<float>"      ,&mfHPD);
  mTree->Branch("fRBX"               ,"vector<float>"      ,&mfRBX);  
  mTree->Branch("fHcalNoise"         ,"vector<float>"      ,&mfHcalNoise);
  mTree->Branch("PVx"                ,"vector<float>"      ,&mPVx);
  mTree->Branch("PVy"                ,"vector<float>"      ,&mPVy);
  mTree->Branch("PVz"                ,"vector<float>"      ,&mPVz);
  mTree->Branch("PVchi2"             ,"vector<float>"      ,&mPVchi2);
  mTree->Branch("PVndof"             ,"vector<float>"      ,&mPVndof);
  mTree->Branch("PVntracks"          ,"vector<int>"         ,&mPVntracks);
  mTree->Branch("evtNo"              ,&mEvtNo               ,"mEvtNo/I");
  mTree->Branch("runNo"              ,&mRunNo               ,"mRunNo/I");
  mTree->Branch("lumi"               ,&mLumi                ,"mLumi/I");
  mTree->Branch("bunch"              ,&mBunch               ,"mBunch/I");
  mTree->Branch("met"                ,&mMET                 ,"mMET/F");
  mTree->Branch("sumet"              ,&mSumET               ,"mSumET/F");
  mTree->Branch("metNoHF"            ,&mMETnoHF             ,"mMETnoHF/F");
  mTree->Branch("sumetNoHF"          ,&mSumETnoHF           ,"mSumETnoHF/F");
  mTree->Branch("passLooseHcalNoise" ,&mLooseHcalNoise      ,"mLooseHcalNoise/I"); 	 
  mTree->Branch("passTightHcalNoise" ,&mTightHcalNoise      ,"mTightHcalNoise/I");


  //cout << "mTriggerNames: " << mTriggerNames.size() << endl;
  for (unsigned int jname=0;jname<mTriggerNames.size();jname++) {
     const char* branchname=mTriggerNames[jname].c_str();
     mTree->Branch(branchname,&mHLTTrigResults[jname],"prescale/I:fired/I");
     }

  //cout << "mL1TriggerNames: " << mL1TriggerNames.size() << endl;
  for (unsigned int jname=0;jname<mL1TriggerNames.size();jname++) {
     const char* branchname=mL1TriggerNames[jname].c_str();
     mTree->Branch(branchname,&mL1TrigResults[jname],"prescale/I:fired/I");
     }


  if(mIsMCarlo)
    {
      mTree->Branch("pthat"          ,&mPtHat               ,"mPtHat/F");
      mTree->Branch("weight"         ,&mWeight              ,"mWeight/F");
      mTree->Branch("genMatchR"      ,"vector<float>"      ,&mGenMatchR);
      mTree->Branch("genMatchPt"     ,"vector<float>"      ,&mGenMatchPt);
      mTree->Branch("genMatchEta"    ,"vector<float>"      ,&mGenMatchEta);
      mTree->Branch("genMatchPhi"    ,"vector<float>"      ,&mGenMatchPhi);
    }
}
///////////////////////////////////////////////////////////////////////////////

bool InclusiveJetTreeProducer::preTreeFillCut()
{
//  Just something to apply some cuts on the tree vars & friends if necessary 
//  to remove any obvious useless data from the tree.


// If there ain't any jets there isn't a lot we can do!
if ((*mPt).size() < 1) return false;

return true;
}




//////////////////////////////////////////////////////////////////////////////////////////
void InclusiveJetTreeProducer::clearTreeVectors() 
{
  mGenMatchR  ->clear();
  mGenMatchPt ->clear();
  mGenMatchEta->clear();
  mGenMatchPhi->clear();
  mPt         ->clear();
  mEta        ->clear();
  mEtaD       ->clear();
  mY          ->clear();
  mPhi        ->clear();
  mE          ->clear();
  mEmf        ->clear();
  mEtaMoment  ->clear();
  mPhiMoment  ->clear();
  mNtrkVtx    ->clear();
  mNtrkCalo   ->clear();
  mTrkCaloPt  ->clear();
  mTrkCaloEta ->clear();
  mTrkCaloPhi ->clear();
  mTrkVtxPt   ->clear();
  mTrkVtxEta  ->clear();
  mTrkVtxPhi  ->clear();
  mN90        ->clear(); 
  mN90Hits    ->clear();
  mfHPD       ->clear();
  mfRBX       ->clear();
  mfHcalNoise ->clear();
  mPVx        ->clear();
  mPVy        ->clear();
  mPVz        ->clear();
  mPVndof     ->clear();
  mPVchi2     ->clear();
  mPVntracks  ->clear();
}
