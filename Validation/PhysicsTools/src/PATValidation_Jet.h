#ifndef ValidationPhysicsToolsPATValidation_Jet_h
#define ValidationPhysicsToolsPATValidation_Jet_h

// Producer for validation histograms for CaloJet objects
// Author Sudhir Malik 20 May 2009
// Original F. Ratnikov, Sept. 7, 2006
// Modified by J F Novak July 10, 2008
// $Id: CaloJetTester.h,v 1.10 2009/03/28 04:57:58 hatake Exp $

#include <string>

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "DQMServices/Core/interface/MonitorElement.h"

/*namespace reco {
  class CaloJet;
  class GenJet;
} */

class MonitorElement;

class PATValidation_Jet : public edm::EDAnalyzer {
public:

  PATValidation_Jet (const edm::ParameterSet&);
  ~PATValidation_Jet();

  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void endJob() ;
 
private:
  
//  void fillMatchHists (const reco::GenJet& fGenJet, const reco::CaloJet& fCaloJet);

  DQMStore* dbe;

  edm::InputTag mInputCollection;
  edm::InputTag mInputGenCollection;
  std::string mOutputFile;
  edm::InputTag inputMETLabel_;
  std::string METType_;
  std::string inputGenMETLabel_;
  std::string inputCaloMETLabel_;


   edm::InputTag recoJet_;
   edm::InputTag patJet_;
  //bool recoJetCorr_;
  //edm::InputTag patJetCorr_;
  //edm::InputTag recoJetCorrSource_;
  std::string outputFile_;
  std::string benchmarkLabel_;


  // count number of events
  MonitorElement* numberofevents;


  // Generic RECO PARAMATERS

  MonitorElement* rmEta;
  MonitorElement* rmEtaFineBin;
  MonitorElement* rmEtaFineBin1p;
  MonitorElement* rmEtaFineBin2p;
  MonitorElement* rmEtaFineBin3p;
  MonitorElement* rmEtaFineBin1m;
  MonitorElement* rmEtaFineBin2m;
  MonitorElement* rmEtaFineBin3m;
  MonitorElement* rmPhi;
  MonitorElement* rmPhiFineBin;
  MonitorElement* rmE;
  MonitorElement* rmE_80;
  MonitorElement* rmE_3000;
  MonitorElement* rmP;
  MonitorElement* rmP_80;
  MonitorElement* rmP_3000;
  MonitorElement* rmPt;
  MonitorElement* rmPt_80;
  MonitorElement* rmPt_3000;
  MonitorElement* rmMass;
  MonitorElement* rmMass_80;
  MonitorElement* rmMass_3000;
  MonitorElement* rmConstituents;
  MonitorElement* rmConstituents_80;
  MonitorElement* rmConstituents_3000;
  MonitorElement* rmHadTiming;
  MonitorElement* rmEmTiming;

  // Leading Jet Parameters
  MonitorElement* rmEtaFirst;
  MonitorElement* rmPhiFirst;
  MonitorElement* rmEFirst;
  MonitorElement* rmEFirst_80;
  MonitorElement* rmEFirst_3000;
  MonitorElement* rmPtFirst;
  MonitorElement* rmPtFirst_80;
  MonitorElement* rmPtFirst_3000;

  MonitorElement* rmNJetsEtaC;
  MonitorElement* rmNJetsEtaF;

  MonitorElement* rmNJets1;
  MonitorElement* rmNJets2;

  // DiJet Parameters
  MonitorElement* rmMjj;
  MonitorElement* rmMjj_3000;

  // CaloJet specific
  MonitorElement* rmMaxEInEmTowers;
  MonitorElement* rmMaxEInHadTowers;
  MonitorElement* rmHadEnergyInHO;
  MonitorElement* rmHadEnergyInHB;
  MonitorElement* rmHadEnergyInHF;
  MonitorElement* rmHadEnergyInHE;
  MonitorElement* rmHadEnergyInHO_80;
  MonitorElement* rmHadEnergyInHB_80;
  MonitorElement* rmHadEnergyInHE_80;
  MonitorElement* rmHadEnergyInHO_3000;
  MonitorElement* rmHadEnergyInHB_3000;
  MonitorElement* rmHadEnergyInHE_3000;
  MonitorElement* rmEmEnergyInEB;
  MonitorElement* rmEmEnergyInEE;
  MonitorElement* rmEmEnergyInHF;
  MonitorElement* rmEmEnergyInEB_80;
  MonitorElement* rmEmEnergyInEE_80;
  MonitorElement* rmEmEnergyInEB_3000;
  MonitorElement* rmEmEnergyInEE_3000;
  MonitorElement* rmEnergyFractionHadronic;
  MonitorElement* rmEnergyFractionEm;
  MonitorElement* rmHFTotal;
  MonitorElement* rmHFTotal_80;
  MonitorElement* rmHFTotal_3000;
  MonitorElement* rmHFLong;
  MonitorElement* rmHFLong_80;
  MonitorElement* rmHFLong_3000;
  MonitorElement* rmHFShort;
  MonitorElement* rmHFShort_80;
  MonitorElement* rmHFShort_3000;
  MonitorElement* rmN90;

  // GenJet Generic Jet Parameters
  MonitorElement* rmGenEta;
  MonitorElement* rmGenPhi;
  MonitorElement* rmGenPt;
  MonitorElement* rmGenPt_80;
  MonitorElement* rmGenPt_3000;

  // GenJet Leading Jet Parameters
  MonitorElement* rmGenEtaFirst;
  MonitorElement* rmGenPhiFirst;

  // CaloJet<->GenJet matching
  MonitorElement* rmAllGenJetsPt;
  MonitorElement* rmMatchedGenJetsPt;
  MonitorElement* rmAllGenJetsEta;
  MonitorElement* rmMatchedGenJetsEta;
  MonitorElement* rmGenJetMatchEnergyFraction;
  MonitorElement* rmReverseMatchEnergyFraction;
  MonitorElement* rmRMatch;
  MonitorElement* rmDeltaEta;
  MonitorElement* rmDeltaPhi;
  MonitorElement* rmEScale;
  MonitorElement* rmlinEScale;  //new
  MonitorElement* rmDeltaE;

  MonitorElement* rmEScale_pt10;  //new
  MonitorElement* rmEScaleFineBin;  //new

  MonitorElement* rmpTScaleB;
  MonitorElement* rmpTScaleE;
  MonitorElement* rmpTScaleF;
  MonitorElement* rmpTScale_60_120;
  MonitorElement* rmpTScale_2700_3500;
  MonitorElement* rmpTScale1DB;
  MonitorElement* rmpTScale1DE;
  MonitorElement* rmpTScale1DF;
  MonitorElement* rmpTScale1D_60_120;
  MonitorElement* rmpTScale1D_2700_3500;

  MonitorElement* rmDelEta;
  MonitorElement* rmDelPhi;
  MonitorElement* rmDelPt;

  // Matching parameters
  double rmMatchGenPtThreshold;
  double rmGenEnergyFractionThreshold;
  double rmReverseEnergyFractionThreshold;
  double rmRThreshold;

  // Switch on/off unimportant histogram
  std::string  rmTurnOnEverything;

  // Energy Profiles
  MonitorElement* rmHadEnergyProfile;
  MonitorElement* rmEmEnergyProfile;
  MonitorElement* rmJetEnergyProfile;
  MonitorElement* rmHadJetEnergyProfile;
  MonitorElement* rmEMJetEnergyProfile;

  // CaloMET
  MonitorElement* rmCaloMEx;
  MonitorElement* rmCaloMEx_3000;
  MonitorElement* rmCaloMEy;
  MonitorElement* rmCaloMEy_3000;
  MonitorElement* rmCaloMET;
  MonitorElement* rmCaloMET_3000;
  MonitorElement* rmCaloMETPhi;
  MonitorElement* rmCaloSumET;
  MonitorElement* rmCaloSumET_3000;
  MonitorElement* rmCaloMETSig;
  MonitorElement* rmCaloMETSig_3000;

  // RecHits
  MonitorElement* rmHBEne;
  MonitorElement* rmHBTime;
  MonitorElement* rmHEEne;
  MonitorElement* rmHETime;
  MonitorElement* rmHOEne;
  MonitorElement* rmHOTime;
  MonitorElement* rmHFEne;
  MonitorElement* rmHFTime;
  MonitorElement* rmEBEne;
  MonitorElement* rmEBTime;
  MonitorElement* rmEEEne;
  MonitorElement* rmEETime;

///////////////////////////////////Generic PAT parameters

  MonitorElement* mEta;
  MonitorElement* mEtaFineBin;
  MonitorElement* mEtaFineBin1p;
  MonitorElement* mEtaFineBin2p;
  MonitorElement* mEtaFineBin3p;
  MonitorElement* mEtaFineBin1m;
  MonitorElement* mEtaFineBin2m;
  MonitorElement* mEtaFineBin3m;
  MonitorElement* mPhi;
  MonitorElement* mPhiFineBin;
  MonitorElement* mE;
  MonitorElement* mE_80;
  MonitorElement* mE_3000;
  MonitorElement* mP;
  MonitorElement* mP_80;
  MonitorElement* mP_3000;
  MonitorElement* mPt;
  MonitorElement* mPt_80;
  MonitorElement* mPt_3000;
  MonitorElement* mMass;   
  MonitorElement* mMass_80;
  MonitorElement* mMass_3000;
  MonitorElement* mConstituents;
  MonitorElement* mConstituents_80;
  MonitorElement* mConstituents_3000;
  MonitorElement* mHadTiming;
  MonitorElement* mEmTiming; 


  // Leading Jet Parameters 
  MonitorElement* mEtaFirst;  
  MonitorElement* mPhiFirst;
  MonitorElement* mEFirst;
  MonitorElement* mEFirst_80;   
  MonitorElement* mEFirst_3000;
  MonitorElement* mPtFirst;
  MonitorElement* mPtFirst_80;
  MonitorElement* mPtFirst_3000;

  MonitorElement* mNJetsEtaC;
  MonitorElement* mNJetsEtaF;

  MonitorElement* mNJets1;
  MonitorElement* mNJets2;

  // DiJet Parameters
  MonitorElement* mMjj;   
  MonitorElement* mMjj_3000;

  // CaloJet specific  
  MonitorElement* mMaxEInEmTowers;
  MonitorElement* mMaxEInHadTowers;
  MonitorElement* mHadEnergyInHO;
  MonitorElement* mHadEnergyInHB;
  MonitorElement* mHadEnergyInHF;
  MonitorElement* mHadEnergyInHE;
  MonitorElement* mHadEnergyInHO_80;
  MonitorElement* mHadEnergyInHB_80;
  MonitorElement* mHadEnergyInHE_80;
  MonitorElement* mHadEnergyInHO_3000;
  MonitorElement* mHadEnergyInHB_3000;
  MonitorElement* mHadEnergyInHE_3000;
  MonitorElement* mEmEnergyInEB;
  MonitorElement* mEmEnergyInEE;
  MonitorElement* mEmEnergyInHF;
  MonitorElement* mEmEnergyInEB_80;
  MonitorElement* mEmEnergyInEE_80;  
  MonitorElement* mEmEnergyInEB_3000;
  MonitorElement* mEmEnergyInEE_3000;
  MonitorElement* mEnergyFractionHadronic;
  MonitorElement* mEnergyFractionEm;
  MonitorElement* mHFTotal;   
  MonitorElement* mHFTotal_80;
  MonitorElement* mHFTotal_3000;
  MonitorElement* mHFLong;   
  MonitorElement* mHFLong_80;
  MonitorElement* mHFLong_3000;
  MonitorElement* mHFShort;
  MonitorElement* mHFShort_80;
  MonitorElement* mHFShort_3000;
  MonitorElement* mN90;

  // GenJet Generic Jet Parameters
  MonitorElement* mGenEta;
  MonitorElement* mGenPhi;   
  MonitorElement* mGenPt;
  MonitorElement* mGenPt_80;
  MonitorElement* mGenPt_3000;  
  
  // GenJet Leading Jet Parameters
  MonitorElement* mGenEtaFirst;
  MonitorElement* mGenPhiFirst;

  // CaloJet<->GenJet matching
  MonitorElement* mAllGenJetsPt;   
  MonitorElement* mMatchedGenJetsPt;
  MonitorElement* mAllGenJetsEta;
  MonitorElement* mMatchedGenJetsEta;
  MonitorElement* mGenJetMatchEnergyFraction;
  MonitorElement* mReverseMatchEnergyFraction;
  MonitorElement* mRMatch;
  MonitorElement* mDeltaEta;
  MonitorElement* mDeltaPhi;
  MonitorElement* mEScale;
  MonitorElement* mlinEScale;  //new
  MonitorElement* mDeltaE;  

  MonitorElement* mEScale_pt10;  //new
  MonitorElement* mEScaleFineBin;  //new

  MonitorElement* mpTScaleB;
  MonitorElement* mpTScaleE; 
  MonitorElement* mpTScaleF;
  MonitorElement* mpTScale_60_120;
  MonitorElement* mpTScale_2700_3500;
  MonitorElement* mpTScale1DB;
  MonitorElement* mpTScale1DE;    
  MonitorElement* mpTScale1DF; 
  MonitorElement* mpTScale1D_60_120;
  MonitorElement* mpTScale1D_2700_3500;
  
  MonitorElement* mDelEta;
  MonitorElement* mDelPhi;
  MonitorElement* mDelPt;
  
  // Matching parameters  
  double mMatchGenPtThreshold;
  double mGenEnergyFractionThreshold;
  double mReverseEnergyFractionThreshold;
  double mRThreshold;

// Energy Profiles
  MonitorElement* mHadEnergyProfile;
  MonitorElement* mEmEnergyProfile;
  MonitorElement* mJetEnergyProfile;
  MonitorElement* mHadJetEnergyProfile;
  MonitorElement* mEMJetEnergyProfile;

  // CaloMET
  MonitorElement* mCaloMEx;
  MonitorElement* mCaloMEx_3000; 
  MonitorElement* mCaloMEy;
  MonitorElement* mCaloMEy_3000;
  MonitorElement* mCaloMET; 
  MonitorElement* mCaloMET_3000;
  MonitorElement* mCaloMETPhi;
  MonitorElement* mCaloSumET; 
  MonitorElement* mCaloSumET_3000;   
  MonitorElement* mCaloMETSig;
  MonitorElement* mCaloMETSig_3000;

 
  // RecHits
  MonitorElement* mHBEne;
  MonitorElement* mHBTime;
  MonitorElement* mHEEne;
  MonitorElement* mHETime;
  MonitorElement* mHOEne;
  MonitorElement* mHOTime;
  MonitorElement* mHFEne;
  MonitorElement* mHFTime;
  MonitorElement* mEBEne;
  MonitorElement* mEBTime;
  MonitorElement* mEEEne;
  MonitorElement* mEETime;
  

};


#endif

