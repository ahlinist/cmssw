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
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

using namespace edm;
using namespace reco;
using namespace std;
typedef math::PtEtaPhiELorentzVectorF LorentzVector;

InclusiveJetTreeProducer::InclusiveJetTreeProducer(edm::ParameterSet const& cfg) 
{
  mPFJetsName             = cfg.getParameter<std::string>              ("pfjets");
  mJetsName               = cfg.getParameter<std::string>              ("jets");
  mJetsIDName             = cfg.getParameter<std::string>              ("jetsID");
  mMetName                = cfg.getParameter<std::string>              ("met");
  mJetExtender            = cfg.getParameter<std::string>              ("jetExtender");
  mHcalNoiseTag           = cfg.getParameter<edm::InputTag>            ("hcalNoiseTag");
  mTriggerNames           = cfg.getParameter<std::vector<std::string> >("jetTriggerNames");
  mL1TriggerNames         = cfg.getParameter<std::vector<std::string> >("l1TriggerNames");   
  mTriggerResultsTag      = cfg.getParameter<edm::InputTag>            ("triggerResultsTag");               
  mL1GTReadoutRcdSource   = cfg.getParameter<edm::InputTag>            ("L1GTReadoutRcdSource");
  mL1GTObjectMapRcdSource = cfg.getParameter<edm::InputTag>            ("L1GTObjectMapRcdSource");
  mJetPtMin               = cfg.getParameter<double>                   ("minJetPt");
  mPFJetPtMin             = cfg.getParameter<double>                   ("minPFJetPt");
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

 // TRACKS
  Handle<reco::TrackCollection> tkRef;
  event.getByLabel("generalTracks",tkRef);
  const reco::TrackCollection* tkColl = tkRef.product(); 

  int numhighpurity=0;
  reco::TrackBase::TrackQuality  _trackQuality = reco::TrackBase::qualityByName("highPurity");
  reco::TrackCollection::const_iterator itk   = tkColl->begin();
  reco::TrackCollection::const_iterator itk_e = tkColl->end();
  for(;itk!=itk_e;++itk){
    if(itk->quality(_trackQuality)) numhighpurity++;
   }

  int numtrk  = tkColl->size();
  double fractrk = (float)numhighpurity/(float)tkColl->size();
  
  mbscrap=1;
  if(numtrk<10 || fractrk>0.25) mbscrap=0;
  
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
  ////////////// PF JETS //////
  Handle<PFJetCollection> pfjets;
  PFJetCollection::const_iterator pfjet;
  event.getByLabel (mPFJetsName,pfjets);
  std::vector<PFCandidatePtr> PFJetPart;
  ////////////// PF CANDIDATES //////////////  
   Handle<edm::View<PFCandidate> > pfCandidates;
   event.getByLabel("particleFlow", pfCandidates);  
  
  
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
  event.getByLabel(mL1GTReadoutRcdSource,gtRecord);
  if (!gtRecord.isValid())  m_l1GtUtils.retrieveL1EventSetup(iSetup);
  
  int bit36 =0;
  int bit37 =0;
  int bit38 =0;
  int bit39 =0;
  
  if(gtRecord->technicalTriggerWord()[36]  ==true) bit36 =1;
  if(gtRecord->technicalTriggerWord()[37]  ==true) bit37 =1;
  if(gtRecord->technicalTriggerWord()[38]  ==true) bit38 =1;
  if(gtRecord->technicalTriggerWord()[39]  ==true) bit39 =1;
 
  mbhalo=0;
  if(bit36==1&&bit37==1&&bit38==1&&bit39==1) mbhalo=1;
  
  if (mFillL1)
    {

    
   // sanity check on L1 Trigger Records
    int ErrFlag=0;
    if (!gtRecord.isValid()) 
        {
          cout << "\nL1GlobalTriggerReadoutRecord with \n \n not found"
          "\n  --> returning false by default!\n" << endl;
          ErrFlag=-1;
        }


     

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

  // LOOP OVER PF CANDIDATES FOR PF MET
   double sum_ex =0;
   double sum_ey =0;
   mPFSumET      =0;
   
  for (unsigned int pfc=0;pfc<pfCandidates->size();++pfc) {
     
    double phi        = (*pfCandidates)[pfc].phi();			 
    double theta      = (*pfCandidates)[pfc].theta();
    double e          = (*pfCandidates)[pfc].energy();
    double et         = e*sin(theta);
    
    mPFSumET += et;
    sum_ex += et*cos(phi);
    sum_ey += et*sin(phi); 
  
  }
  
   mPFMET = sqrt( sum_ex*sum_ex + sum_ey*sum_ey );
 
  // LOOP OVER PF JETS // 
  
  if((*pfjets).size() > 0){
  
    for(unsigned int ind=0;ind<(*pfjets).size();ind++){
     
      if ((*pfjets)[ind].pt() < mPFJetPtMin) continue;
       
           
	   PFJetPart     = ((*pfjets)[ind].getPFConstituents());
	
       	   mPFPt         ->push_back((*pfjets)[ind].pt());
           mPFEta        ->push_back((*pfjets)[ind].eta());
          // mPFEtaD       ->push_back((*pfjets)[ind].detectorP4().eta()); DEN UPARXEI!
           mPFY          ->push_back((*pfjets)[ind].y());
           mPFPhi        ->push_back((*pfjets)[ind].phi());
           mPFE          ->push_back((*pfjets)[ind].energy());
           
	   mPFChfJet     ->push_back((*pfjets)[ind].chargedHadronEnergyFraction());
           mPFNhfJet     ->push_back(((*pfjets)[ind].neutralHadronEnergy() + (*pfjets)[ind].HFHadronEnergy() )/(*pfjets)[ind].energy());
           mPFCemfJet    ->push_back((*pfjets)[ind].chargedEmEnergyFraction());
           mPFNemfJet    ->push_back((*pfjets)[ind].neutralEmEnergyFraction());
           mPFCmultiJet  ->push_back((*pfjets)[ind].chargedMultiplicity());
           mPFNmultiJet  ->push_back((*pfjets)[ind].neutralMultiplicity());	  
           mPFNcr        ->push_back(PFJetPart.size());
	   
        // IF MC DO THE MATHING // edw 
	    if (mIsMCarlo){
               float rmin(999);
               int indMatchedGen(-1);
               const reco::Candidate& rec = (*pfjets)[ind];
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
                   mPFGenMatchR  ->push_back(rmin);
                   mPFGenMatchPt ->push_back((*genjets)[indMatchedGen].pt());
                   mPFGenMatchEta->push_back((*genjets)[indMatchedGen].eta());
                   mPFGenMatchPhi->push_back((*genjets)[indMatchedGen].phi());
                 }
               else
                 {
                   mPFGenMatchR  ->push_back(-999);
                   mPFGenMatchPt ->push_back(-999);
                   mPFGenMatchEta->push_back(-999);
                   mPFGenMatchPhi->push_back(-999);
                 } 
             }
	     
       
    } // IS PT OF PF JET > mPFJETPTMIN
    
  } // IF PF JETS EXISTS
  
  // LOOP OVER CALO JETS // 
  
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
  mGenMatchR      = new std::vector<float>();
  mGenMatchPt     = new std::vector<float>();
  mGenMatchEta    = new std::vector<float>();
  mGenMatchPhi    = new std::vector<float>();
  mPt             = new std::vector<float>();
  mEta            = new std::vector<float>();
  mEtaD           = new std::vector<float>();
  mY              = new std::vector<float>();
  mPhi            = new std::vector<float>();
  mE              = new std::vector<float>();
  mEmf            = new std::vector<float>();
  mEtaMoment      = new std::vector<float>();
  mPhiMoment      = new std::vector<float>();
  mNtrkVtx        = new std::vector<int>   ();
  mNtrkCalo       = new std::vector<int>   ();
  mTrkCaloPt      = new std::vector<float>();
  mTrkCaloEta     = new std::vector<float>();
  mTrkCaloPhi     = new std::vector<float>();
  mTrkVtxPt       = new std::vector<float>();
  mTrkVtxEta      = new std::vector<float>();
  mTrkVtxPhi      = new std::vector<float>();
  mN90            = new std::vector<int>   ();
  mN90Hits        = new std::vector<int>   ();
  mfHPD           = new std::vector<float>();
  mfRBX           = new std::vector<float>();
  mfHcalNoise     = new std::vector<float>();
  mPVx            = new std::vector<float>();
  mPVy            = new std::vector<float>();
  mPVz            = new std::vector<float>();
  mPVchi2         = new std::vector<float>();
  mPVndof         = new std::vector<float>();
  mPVntracks      = new std::vector<int>();
  
  mPFGenMatchR    = new std::vector<float>();
  mPFGenMatchPt   = new std::vector<float>();
  mPFGenMatchEta  = new std::vector<float>();
  mPFGenMatchPhi  = new std::vector<float>();
  
  mPFPt           = new std::vector<float>();
  mPFEta          = new std::vector<float>();
  mPFEtaD         = new std::vector<float>();
  mPFY            = new std::vector<float>();
  mPFPhi          = new std::vector<float>();
  mPFE            = new std::vector<float>(); 
  mPFChfJet       = new std::vector<float>();
  mPFNhfJet   	  = new std::vector<float>();
  mPFCemfJet  	  = new std::vector<float>();
  mPFNemfJet  	  = new std::vector<float>();
  mPFCmultiJet	  = new std::vector<float>();
  mPFNmultiJet	  = new std::vector<float>();
  mPFNcr          = new std::vector<float>();
  
  mTree->Branch("pt"                 ,"vector<float>"      ,&mPt);
  mTree->Branch("eta"                ,"vector<float>"      ,&mEta);
  mTree->Branch("etaDetector"        ,"vector<float>"      ,&mEtaD);
  mTree->Branch("y"                  ,"vector<float>"      ,&mY);
  mTree->Branch("phi"                ,"vector<float>"      ,&mPhi);
  mTree->Branch("e"                  ,"vector<float>"      ,&mE);
  //
  mTree->Branch("pfpt"               ,"vector<float>"	   ,&mPFPt);
  mTree->Branch("pfeta"              ,"vector<float>"	   ,&mPFEta);
  mTree->Branch("pfetaDetector"      ,"vector<float>"	   ,&mPFEtaD);
  mTree->Branch("pfy"                ,"vector<float>"	   ,&mPFY);
  mTree->Branch("pfphi"              ,"vector<float>"	   ,&mPFPhi);
  mTree->Branch("pfe"                ,"vector<float>"	   ,&mPFE);

  mTree->Branch("pfchf"              ,"vector<float>"	    ,&mPFChfJet);	
  mTree->Branch("pfnhf"              ,"vector<float>"	    ,&mPFNhfJet);	  
  mTree->Branch("pfcem"              ,"vector<float>"	    ,&mPFCemfJet);    
  mTree->Branch("pfnem"              ,"vector<float>"	    ,&mPFNemfJet);    
  mTree->Branch("pfcml"              ,"vector<float>"	    ,&mPFCmultiJet);  
  mTree->Branch("pfnml"              ,"vector<float>"	    ,&mPFNmultiJet);  
  mTree->Branch("pfncr"              ,"vector<float>"	    ,&mPFNcr);	  

  mTree->Branch("emf"                ,"vector<float>"      ,&mEmf);
  mTree->Branch("etaMoment"          ,"vector<float>"      ,&mEtaMoment);
  mTree->Branch("phiMoment"          ,"vector<float>"      ,&mPhiMoment);
  mTree->Branch("nTrkVtx"            ,"vector<int>"        ,&mNtrkVtx);
  mTree->Branch("nTrkCalo"           ,"vector<int>"        ,&mNtrkCalo);
  mTree->Branch("TrkCaloPt"          ,"vector<float>"      ,&mTrkCaloPt);
  mTree->Branch("TrkCaloEta"         ,"vector<float>"      ,&mTrkCaloEta);
  mTree->Branch("TrkCaloPhi"         ,"vector<float>"      ,&mTrkCaloPhi);
  mTree->Branch("TrkVtxPt"           ,"vector<float>"      ,&mTrkVtxPt);
  mTree->Branch("TrkVtxEta"          ,"vector<float>"      ,&mTrkVtxEta);
  mTree->Branch("TrkVtxPhi"          ,"vector<float>"      ,&mTrkVtxPhi);
  mTree->Branch("n90"                ,"vector<int>"        ,&mN90);
  mTree->Branch("n90hits"            ,"vector<int>"        ,&mN90Hits);
  mTree->Branch("fHPD"               ,"vector<float>"      ,&mfHPD);
  mTree->Branch("fRBX"               ,"vector<float>"      ,&mfRBX);  
  mTree->Branch("fHcalNoise"         ,"vector<float>"      ,&mfHcalNoise);
  mTree->Branch("PVx"                ,"vector<float>"      ,&mPVx);
  mTree->Branch("PVy"                ,"vector<float>"      ,&mPVy);
  mTree->Branch("PVz"                ,"vector<float>"      ,&mPVz);
  mTree->Branch("PVchi2"             ,"vector<float>"      ,&mPVchi2);
  mTree->Branch("PVndof"             ,"vector<float>"      ,&mPVndof);
  mTree->Branch("PVntracks"          ,"vector<int>"         ,&mPVntracks);
  mTree->Branch("bhalo"              ,&mbhalo		    ,"mbhalo/I");
  mTree->Branch("bscrap"             ,&mbscrap		    ,"mbscrap/I");
  mTree->Branch("evtNo"              ,&mEvtNo               ,"mEvtNo/I");
  mTree->Branch("runNo"              ,&mRunNo               ,"mRunNo/I");
  mTree->Branch("lumi"               ,&mLumi                ,"mLumi/I");
  mTree->Branch("bunch"              ,&mBunch               ,"mBunch/I");
  mTree->Branch("met"                ,&mMET                 ,"mMET/F");
  mTree->Branch("sumet"              ,&mSumET               ,"mSumET/F");
  mTree->Branch("pfmet"              ,&mPFMET		    ,"mPFMET/F");
  mTree->Branch("pfsumet"            ,&mPFSumET  	    ,"mPFSumET/F");  
  mTree->Branch("passLooseHcalNoise" ,&mLooseHcalNoise      ,"mLooseHcalNoise/I"); 	 
  mTree->Branch("passTightHcalNoise" ,&mTightHcalNoise      ,"mTightHcalNoise/I");


  for (unsigned int jname=0;jname<mTriggerNames.size();jname++) {
     const char* branchname=mTriggerNames[jname].c_str();
     mTree->Branch(branchname,&mHLTTrigResults[jname],"prescale/I:fired/I");
     }

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
      mTree->Branch("genPFMatchR"    ,"vector<float>"      ,&mPFGenMatchR);
      mTree->Branch("genPFMatchPt"   ,"vector<float>"      ,&mPFGenMatchPt);
      mTree->Branch("genPFMatchEta"  ,"vector<float>"      ,&mPFGenMatchEta);
      mTree->Branch("genPFMatchPhi"  ,"vector<float>"      ,&mPFGenMatchPhi);
    }
}
///////////////////////////////////////////////////////////////////////////////

bool InclusiveJetTreeProducer::preTreeFillCut()
{
//  Just something to apply some cuts on the tree vars & friends if necessary 
//  to remove any obvious useless data from the tree.


// If there ain't any jets there isn't a lot we can do!
if (((*mPt).size() < 1) && ((*mPFPt).size() < 1)) 
  return false;

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

  mPFGenMatchR  ->clear();
  mPFGenMatchPt ->clear();
  mPFGenMatchEta->clear();
  mPFGenMatchPhi->clear();
  mPFPt         ->clear();
  mPFEta        ->clear();
  mPFEtaD       ->clear();
  mPFY          ->clear();
  mPFPhi        ->clear();
  mPFE          ->clear();
  mPFChfJet     ->clear();
  mPFNhfJet     ->clear();
  mPFCemfJet    ->clear();
  mPFNemfJet    ->clear();
  mPFCmultiJet  ->clear();
  mPFNmultiJet  ->clear();
  mPFNcr        ->clear();
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
