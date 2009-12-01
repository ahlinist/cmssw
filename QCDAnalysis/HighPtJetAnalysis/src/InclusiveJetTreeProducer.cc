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
#include "DataFormats/JetReco/interface/JetExtendedAssociation.h"
#include "DataFormats/JetReco/interface/JetID.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/Math/interface/LorentzVector.h"

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
  mIsMCarlo               = cfg.getParameter<bool>                     ("isMCarlo");
}
//////////////////////////////////////////////////////////////////////////////////////////
void InclusiveJetTreeProducer::beginJob(EventSetup const& iSetup) 
{
  mTree = fs->make<TTree>("InclusiveJetTree","InclusiveJetTree");

  buildTree();
  
  //must be done at beginRun and not only at beginJob, because 
  //trigger names are allowed to change by run.
  if (mTriggerProcessName != "")
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

  ////////////Vertices//////////////
  Handle<reco::VertexCollection> recVtxs;
  event.getByLabel("offlinePrimaryVertices",recVtxs);
  for(unsigned int ind=0;ind<recVtxs->size();ind++) 
    {
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
      mPtHat  = hEventInfo->binningValues()[0];
      mWeight = hEventInfo->weight();
    }

  ///////// HcalNoiseCollection //////////////                                                                               
  Handle<HcalNoiseRBXCollection> rbxColl;
  event.getByLabel(mHcalNoiseTag,rbxColl);
  if(!rbxColl.isValid()) {
    throw edm::Exception(edm::errors::ProductNotFound)
      << " could not find HcalNoiseRBXCollection named " << "hcalnoise" << ".\n";
    return;
  }

  // loop over the RBXs 
  std::map<CaloTowerDetId, double> hcalNoise;
  for(HcalNoiseRBXCollection::const_iterator rit=rbxColl->begin(); rit!=rbxColl->end(); ++rit) {
    HcalNoiseRBX rbx    = (*rit);
    const std::vector<HcalNoiseHPD> hpds = rbx.HPDs();
    for(unsigned ihpd=0; ihpd<hpds.size(); ihpd++){
      const edm::RefVector<CaloTowerCollection> noiseCTowers = hpds[ihpd].caloTowers();
      for(unsigned int itow=0; itow<noiseCTowers.size(); itow++){
        hcalNoise.insert( std::pair<CaloTowerDetId, double>( noiseCTowers[itow]->id(), noiseCTowers[itow]->hadEnergy()) );
      }
    }
  }
  ////////////// Jets //////
  edm::Handle<CaloJetCollection> jets;
  event.getByLabel(mJetsName,jets);
  Handle<JetExtendedAssociation::Container> jetExtender;
  event.getByLabel(mJetExtender,jetExtender);
  edm::Handle<edm::ValueMap<reco::JetID> > jetsID;
  event.getByLabel(mJetsIDName,jetsID);
  if ((*jets).size() < 1) return;
  for(unsigned int ind=0;ind<(*jets).size();ind++) 
    {
      LorentzVector TrkCaloP4 = JetExtendedAssociation::tracksAtCaloP4(*jetExtender,(*jets)[ind]);
      LorentzVector TrkVtxP4  = JetExtendedAssociation::tracksAtVertexP4(*jetExtender, (*jets)[ind]);
      RefToBase<Jet> jetRef(Ref<CaloJetCollection>(jets,ind));
      mPt        ->push_back((*jets)[ind].pt());
      mEta       ->push_back((*jets)[ind].eta());
      mEtaD      ->push_back((*jets)[ind].detectorP4().eta());
      mY         ->push_back((*jets)[ind].y()); 
      mPhi       ->push_back((*jets)[ind].phi());
      mE         ->push_back((*jets)[ind].energy());
      mN90       ->push_back((*jets)[ind].n90());
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
      std::vector< CaloTowerPtr >jTowers = (*jets)[ind].getCaloConstituents ();
      for(unsigned int itow=0; itow<jTowers.size(); itow++) {
	std::map<CaloTowerDetId, double>::iterator thisTow = hcalNoise.find(jTowers[itow]->id());
	if( thisTow != hcalNoise.end() ) jetEneNoise += jTowers[itow]->energy();
      }
      mfHcalNoise->push_back( jetEneNoise/(*jets)[ind].energy() );
    } 

  ////////////// Trigger //////
  edm::Handle<edm::TriggerResults> triggerResultsHandle;
  event.getByLabel(mTriggerResultsTag,triggerResultsHandle); 
  if (!triggerResultsHandle.isValid())
    {
      string errorMessage = "Requested TriggerResult is not present in file! -- \n";
      throw  cms::Exception("Configuration",errorMessage);
    }
  for(unsigned int i=0;i<mTriggerNames.size();i++) 
    {
      bool accept = triggerResultsHandle->accept( mHltConfig.triggerIndex(mTriggerNames[i]) );
      if( accept ) mHLTNames->push_back( mTriggerNames[i] );
    }

  //===================== save L1 Trigger information ======================= 
  // get L1TriggerReadout records
  edm::Handle<L1GlobalTriggerReadoutRecord> gtRecord;
  event.getByLabel(mL1GTReadoutRcdSource,gtRecord);

  edm::Handle<L1GlobalTriggerObjectMapRecord> gtOMRec;
  event.getByLabel(mL1GTObjectMapRcdSource, gtOMRec);

  // sanity check on L1 Trigger Records
  if (!gtRecord.isValid()) 
    {
      std::cout << "\nL1GlobalTriggerReadoutRecord with \n \nnot found"
      "\n  --> returning false by default!\n" << std::endl;
    }
  if (!gtOMRec.isValid()) 
    {
      std::cout << "\nL1GlobalTriggerObjectMapRecord with \n \nnot found"
      "\n  --> returning false by default!\n" << std::endl;
    }
  // L1 decision word
  const DecisionWord dWord = gtRecord->decisionWord();
  std::map<std::string, int> l1map;
  const std::vector<L1GlobalTriggerObjectMap>& objMapVec =  gtOMRec->gtObjectMap();
  for (std::vector<L1GlobalTriggerObjectMap>::const_iterator itMap = objMapVec.begin();itMap != objMapVec.end(); ++itMap) 
    l1map.insert( std::pair<std::string, int> ((*itMap).algoName(), (*itMap).algoBitNumber()) );
  for(unsigned int i=0; i<mL1TriggerNames.size(); i++) 
    {
      std::map<std::string, int>::const_iterator itr = l1map.find( mL1TriggerNames[i] );
      if ( itr != l1map.end() ) 
        {
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
InclusiveJetTreeProducer::~InclusiveJetTreeProducer() 
{
}
//////////////////////////////////////////////////////////////////////////////////////////
void InclusiveJetTreeProducer::buildTree() 
{
  mPt         = new std::vector<float>();
  mEta        = new std::vector<float>();
  mEtaD       = new std::vector<float>();
  mY          = new std::vector<float>();
  mPhi        = new std::vector<float>();
  mE          = new std::vector<float>();
  mEmf        = new std::vector<float>();
  mEtaMoment  = new std::vector<float>();
  mPhiMoment  = new std::vector<float>();
  mNtrkVtx    = new std::vector<int>   ();
  mNtrkCalo   = new std::vector<int>   ();
  mTrkCaloPt  = new std::vector<float>();
  mTrkCaloEta = new std::vector<float>();
  mTrkCaloPhi = new std::vector<float>();
  mTrkVtxPt   = new std::vector<float>();
  mTrkVtxEta  = new std::vector<float>();
  mTrkVtxPhi  = new std::vector<float>();
  mN90        = new std::vector<int>   ();
  mfHPD       = new std::vector<float>();
  mfRBX       = new std::vector<float>();
  mfHcalNoise = new std::vector<float>();
  mPVx        = new std::vector<float>();
  mPVy        = new std::vector<float>();
  mPVz        = new std::vector<float>();
  mHLTNames   = new std::vector<std::string>();
  mL1Names    = new std::vector<std::string>();

  mTree->Branch("pt"         ,"vector<float>"      ,&mPt);
  mTree->Branch("eta"        ,"vector<float>"      ,&mEta);
  mTree->Branch("etaDetector","vector<float>"      ,&mEtaD);
  mTree->Branch("y"          ,"vector<float>"      ,&mY);
  mTree->Branch("phi"        ,"vector<float>"      ,&mPhi);
  mTree->Branch("e"          ,"vector<float>"      ,&mE);
  mTree->Branch("emf"        ,"vector<float>"      ,&mEmf);
  mTree->Branch("etaMoment"  ,"vector<float>"      ,&mEtaMoment);
  mTree->Branch("phiMoment"  ,"vector<float>"      ,&mPhiMoment);
  mTree->Branch("nTrkVtx"    ,"vector<int>"        ,&mNtrkVtx);
  mTree->Branch("nTrkCalo"   ,"vector<int>"        ,&mNtrkCalo);
  mTree->Branch("TrkCaloPt"  ,"vector<float>"      ,&mTrkCaloPt);
  mTree->Branch("TrkCaloEta" ,"vector<float>"      ,&mTrkCaloEta);
  mTree->Branch("TrkCaloPhi" ,"vector<float>"      ,&mTrkCaloPhi);
  mTree->Branch("TrkVtxPt"   ,"vector<float>"      ,&mTrkVtxPt);
  mTree->Branch("TrkVtxEta"  ,"vector<float>"      ,&mTrkVtxEta);
  mTree->Branch("TrkVtxPhi"  ,"vector<float>"      ,&mTrkVtxPhi);
  mTree->Branch("n90"        ,"vector<int>"        ,&mN90);
  mTree->Branch("fHPD"       ,"vector<float>"      ,&mfHPD);
  mTree->Branch("fRBX"       ,"vector<float>"      ,&mfRBX);  
  mTree->Branch("fHcalNoise" ,"vector<float>"      ,&mfHcalNoise);
  mTree->Branch("PVx"        ,"vector<float>"      ,&mPVx);
  mTree->Branch("PVy"        ,"vector<float>"      ,&mPVy);
  mTree->Branch("PVz"        ,"vector<float>"      ,&mPVz);
  mTree->Branch("hltNames"   ,"vector<string>"     ,&mHLTNames);
  mTree->Branch("l1Names"    ,"vector<string>"     ,&mL1Names);
  mTree->Branch("evtNo"      ,&mEvtNo              ,"mEvtNo/I");
  mTree->Branch("runNo"      ,&mRunNo              ,"mRunNo/I");
  mTree->Branch("lumi"       ,&mLumi               ,"mLumi/I");
  mTree->Branch("met"        ,&mMET                ,"mMET/F");
  mTree->Branch("sumet"      ,&mSumET              ,"mSumET/F");
  mTree->Branch("metNoHF"    ,&mMETnoHF            ,"mMETnoHF/F");
  mTree->Branch("sumetNoHF"  ,&mSumETnoHF          ,"mSumETnoHF/F");
  if(mIsMCarlo)
    {
      mTree->Branch("pthat"  ,&mPtHat              ,"mPtHat/F");
      mTree->Branch("weight" ,&mWeight             ,"mWeight/F");
    }
}
//////////////////////////////////////////////////////////////////////////////////////////
void InclusiveJetTreeProducer::clearTreeVectors() 
{
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
  mfHPD      ->clear();
  mfRBX      ->clear();
  mfHcalNoise->clear();
  mPVx       ->clear();
  mPVy       ->clear();
  mPVz       ->clear();
  mHLTNames  ->clear();
  mL1Names   ->clear();
}
