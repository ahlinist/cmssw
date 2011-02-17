// -*- C++ -*-
//
// Package:    JetTrigAnalyzer
// Class:      JetTrigAnalyzer
// 
/**\class JetTrigAnalyzer JetTrigAnalyzer.cc JetTrigger/JetTrigAnalyzer/src/JetTrigAnalyzer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  "Sertac Ozturk"
//         Created:  Mon Jul 27 11:09:40 CDT 2009
// $Id: JetTrigAnalyzer.cc,v 1.4 2010/05/31 22:24:17 ferencek Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "DataFormats/CaloTowers/interface/CaloTowerDetId.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/HcalDetId/interface/HcalSubdetector.h"

#include "CondFormats/HcalObjects/interface/HcalQIECoder.h"
#include "CalibFormats/HcalObjects/interface/HcalCalibrations.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"

// Calo Jets
#include "DataFormats/JetReco/interface/CaloJet.h"

// MET
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"

#include "DataFormats/Candidate/interface/Candidate.h"

//trigger
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuRegionalCand.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuGMTReadoutCollection.h"

//TFile Service
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

//L1 Particle jets
#include "DataFormats/L1Trigger/interface/L1JetParticle.h"
#include "DataFormats/L1Trigger/interface/L1JetParticleFwd.h"

// Delta R
#include "DataFormats/Math/interface/deltaR.h"

// Root
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TProfile.h"
#include <TROOT.h>
#include <TSystem.h>
#include "TNtuple.h"

#include "RecoHcal/HcalPromptAnalysis/interface/JetTrigAnalyzer.h"
//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
JetTrigAnalyzer::JetTrigAnalyzer(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed
  triggerTag_ = iConfig.getUntrackedParameter<edm::InputTag>("TriggerTag");
  l1CollectionsTag_= iConfig.getUntrackedParameter<edm::InputTag> ("l1collections");
  do_print_ = iConfig.getUntrackedParameter<bool>("DoPrint");

  evt_cnt=0;
}


JetTrigAnalyzer::~JetTrigAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
JetTrigAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace std;
   using namespace reco;

  // Event information
  unsigned long int  event = iEvent.id().event(); // event number
  int lumi = iEvent.luminosityBlock(); // lumi section number
  unsigned long int orbit_no = iEvent.orbitNumber(); // Orbit number
  unsigned int BX = iEvent.bunchCrossing(); // Bunch Crossing number

  ntuple_run_info->Fill(event, lumi, orbit_no, BX);

  int n_jet = 0;
  int n_jet_bit15 = 0;
  int n_jet_bit16 = 0;
  int n_jet_bit17 = 0;
  int n_jet_bit18 = 0;
  int n_jet_bit55 = 0;

  bool bit15 = false;
  bool bit16 = false;
  bool bit17 = false;
  bool bit18 = false;
  bool bit55 = false;

  // reco jets
   Handle<CaloJetCollection> caloJet_h;
   iEvent.getByLabel("iterativeCone5CaloJets",caloJet_h);
   const CaloJetCollection* cjet = caloJet_h.failedToGet () ? 0 : &*caloJet_h;


  //trigger
   Handle<L1MuGMTReadoutCollection> gmtrc_h;
   iEvent.getByLabel("l1GmtEmulDigis",gmtrc_h);
   const L1MuGMTReadoutCollection* gmtrc = gmtrc_h.failedToGet () ? 0 : &*gmtrc_h;

  // L1 global trigger
  Handle <L1GlobalTriggerReadoutRecord> gtRecord_h;
  iEvent.getByType (gtRecord_h); // assume only one L1 trigger record here
  const L1GlobalTriggerReadoutRecord* gtRecord = gtRecord_h.failedToGet () ? 0 : &*gtRecord_h;


  // MET
  Handle <CaloMETCollection> recmet_h;
  iEvent.getByLabel("met", recmet_h);
  const CaloMET* met  = recmet_h.failedToGet () ? 0 : &(recmet_h->front());

  // L1 Particles Jets in Forward
   Handle<l1extra::L1JetParticleCollection> l1jets_h;
   InputTag L1JetTag(edm::InputTag(l1CollectionsTag_.label(),"Forward"));
   iEvent.getByLabel(L1JetTag,l1jets_h);
   const l1extra::L1JetParticleCollection* l1jets = l1jets_h.failedToGet () ? 0 : &*l1jets_h;

  // L1 Particles Jets in Central
   Handle<l1extra::L1JetParticleCollection> l1jets_cent;
   InputTag L1JetTag_cent(edm::InputTag(l1CollectionsTag_.label(),"Central"));
   iEvent.getByLabel(L1JetTag_cent,l1jets_cent);
   const l1extra::L1JetParticleCollection* l1jets_central = l1jets_cent.failedToGet () ? 0 : &*l1jets_cent;


  // L1 Global trigger
   if (gtRecord) { // object is available
     for (int l1bit = 0; l1bit < 128; ++l1bit) {
       if (gtRecord->decisionWord() [l1bit]) h_l1_gtbit->Fill (l1bit);
       if(gtRecord->decisionWord() [l1bit] && l1bit == 15) bit15 = true; // Single Jet6 GeV
       if(gtRecord->decisionWord() [l1bit] && l1bit == 16) bit16 = true; // Single Jet10 GeV
       if(gtRecord->decisionWord() [l1bit] && l1bit == 17) bit17 = true; // Single Jet20 GeV
       if(gtRecord->decisionWord() [l1bit] && l1bit == 18) bit18 = true; // Single Jet30 GeV
       if(gtRecord->decisionWord() [l1bit] && l1bit == 55) bit55 = true; // Single Muon
      }
    }



  // All Offline Jets
   if (cjet) { // object is available
     for (CaloJetCollection::const_iterator jetiter=cjet->begin(); jetiter!=cjet->end(); jetiter++){
       ntuple_alljets->Fill(jetiter->energy(), jetiter->pt(), jetiter->eta(), jetiter->phi(), jetiter->emEnergyFraction(), lumi, orbit_no, BX);
    } 
  } // end of offline Jets loop


  // Leading Offline Jets
  if (cjet) { // object is available
     for (CaloJetCollection::const_iterator jetiter=cjet->begin(); jetiter!=cjet->end(); jetiter++){
       n_jet++;
       if(n_jet>1) break;
       ntuple_Leadingjets->Fill(jetiter->energy(), jetiter->pt(), jetiter->eta(), jetiter->phi(), jetiter->emEnergyFraction(), lumi, orbit_no, BX);    
    }
  } // end of offline Jets loop
   
  
  // L1 Jets 
  if (! l1jets_h.isValid()){ std::cout<<"No L1Jet Information"<<std::endl;}
   else if(l1jets){
      for(l1extra::L1JetParticleCollection::const_iterator l1 = l1jets->begin(); l1 != l1jets->end(); ++l1) {
        ntuple_L1Jets->Fill(l1->energy(), l1->pt(), l1->eta(), l1->phi(), lumi, orbit_no, BX);
      } // end of forward L1 jets

      for(l1extra::L1JetParticleCollection::const_iterator l1_cent = l1jets_central->begin(); l1_cent != l1jets_central->end(); ++l1_cent) {
         ntuple_L1Jets->Fill(l1_cent->energy(), l1_cent->pt(),l1_cent->eta(), l1_cent->phi(), lumi, orbit_no, BX);
      } // end of central L1 jets
    } // end of L1 jets loop



  //////////////////////////////////////
  // Create NTuples for each L1 Bits ///
  //////////////////////////////////////

  // *** L1 bit15 ***//
  if(bit15){ // bit15 is ON
      // All Offline Jets
   if (cjet) { // object is available
     for (CaloJetCollection::const_iterator jetiter=cjet->begin(); jetiter!=cjet->end(); jetiter++){
       ntuple_alljets_bit15->Fill(jetiter->energy(), jetiter->pt(), jetiter->eta(), jetiter->phi(), jetiter->emEnergyFraction(), lumi, orbit_no, BX);
    }
  } // end of offline Jets loop


  // Leading Offline Jets
  if (cjet) { // object is available
     for (CaloJetCollection::const_iterator jetiter=cjet->begin(); jetiter!=cjet->end(); jetiter++){
       n_jet_bit15++;
       if(n_jet_bit15>1) break;
       ntuple_Leadingjets_bit15->Fill(jetiter->energy(), jetiter->pt(), jetiter->eta(), jetiter->phi(), jetiter->emEnergyFraction(), lumi, orbit_no, BX);
    }
  } // end of offline Jets loop


  // L1 Jets
  if (! l1jets_h.isValid()){ std::cout<<"No L1Jet Information"<<std::endl;}
   else if(l1jets){
      for(l1extra::L1JetParticleCollection::const_iterator l1 = l1jets->begin(); l1 != l1jets->end(); ++l1) {
        ntuple_L1Jets_bit15->Fill(l1->energy(), l1->pt(), l1->eta(), l1->phi(), lumi, orbit_no, BX);
      } // end of forward L1 jets

      for(l1extra::L1JetParticleCollection::const_iterator l1_cent = l1jets_central->begin(); l1_cent != l1jets_central->end(); ++l1_cent) {
         ntuple_L1Jets_bit15->Fill(l1_cent->energy(), l1_cent->pt(),l1_cent->eta(), l1_cent->phi(), lumi, orbit_no, BX);
      } // end of central L1 jets
    } // end of L1 jets loop

 } //end of bit15 loop

 
 // *** L1 bit16 ***//
  if(bit16){ // bit16 is ON
      // All Offline Jets
   if (cjet) { // object is available
     for (CaloJetCollection::const_iterator jetiter=cjet->begin(); jetiter!=cjet->end(); jetiter++){
       ntuple_alljets_bit16->Fill(jetiter->energy(), jetiter->pt(), jetiter->eta(), jetiter->phi(), jetiter->emEnergyFraction(), lumi, orbit_no, BX);
    }
  } // end of offline Jets loop


  // Leading Offline Jets
  if (cjet) { // object is available
     for (CaloJetCollection::const_iterator jetiter=cjet->begin(); jetiter!=cjet->end(); jetiter++){
       n_jet_bit16++;
      if(n_jet_bit16>1) break;
       ntuple_Leadingjets_bit16->Fill(jetiter->energy(), jetiter->pt(), jetiter->eta(), jetiter->phi(), jetiter->emEnergyFraction(), lumi, orbit_no, BX);
     
   }
  } // end of offline Jets loop


  // L1 Jets
  if (! l1jets_h.isValid()){ std::cout<<"No L1Jet Information"<<std::endl;}
   else if(l1jets){
      for(l1extra::L1JetParticleCollection::const_iterator l1 = l1jets->begin(); l1 != l1jets->end(); ++l1) {
        ntuple_L1Jets_bit16->Fill(l1->energy(), l1->pt(), l1->eta(), l1->phi(), lumi, orbit_no, BX);
      } // end of forward L1 jets

      for(l1extra::L1JetParticleCollection::const_iterator l1_cent = l1jets_central->begin(); l1_cent != l1jets_central->end(); ++l1_cent) {
         ntuple_L1Jets_bit16->Fill(l1_cent->energy(), l1_cent->pt(),l1_cent->eta(), l1_cent->phi(), lumi, orbit_no, BX);
      } // end of central L1 jets
    } // end of L1 jets loop

 } //end of bit16 loop


 // *** L1 bit17 ***//
  if(bit17){ // bit17 is ON
      // All Offline Jets
   if (cjet) { // object is available
     for (CaloJetCollection::const_iterator jetiter=cjet->begin(); jetiter!=cjet->end(); jetiter++){
       ntuple_alljets_bit17->Fill(jetiter->energy(), jetiter->pt(), jetiter->eta(), jetiter->phi(), jetiter->emEnergyFraction(), lumi, orbit_no, BX);
    }
  } // end of offline Jets loop


  // Leading Offline Jets
  if (cjet) { // object is available
     for (CaloJetCollection::const_iterator jetiter=cjet->begin(); jetiter!=cjet->end(); jetiter++){
       n_jet_bit17++;
       if(n_jet_bit17>1) break;
       ntuple_Leadingjets_bit17->Fill(jetiter->energy(), jetiter->pt(), jetiter->eta(), jetiter->phi(), jetiter->emEnergyFraction(), lumi, orbit_no, BX);
     
   }
  } // end of offline Jets loop


  // L1 Jets
  if (! l1jets_h.isValid()){ std::cout<<"No L1Jet Information"<<std::endl;}
   else if(l1jets){
      for(l1extra::L1JetParticleCollection::const_iterator l1 = l1jets->begin(); l1 != l1jets->end(); ++l1) {
        ntuple_L1Jets_bit17->Fill(l1->energy(), l1->pt(), l1->eta(), l1->phi(), lumi, orbit_no, BX);
      } // end of forward L1 jets

      for(l1extra::L1JetParticleCollection::const_iterator l1_cent = l1jets_central->begin(); l1_cent != l1jets_central->end(); ++l1_cent) {
         ntuple_L1Jets_bit17->Fill(l1_cent->energy(), l1_cent->pt(),l1_cent->eta(), l1_cent->phi(), lumi, orbit_no, BX);
      } // end of central L1 jets
    } // end of L1 jets loop

 } //end of bit17 loop


  // *** L1 bit18 ***//
  if(bit18){ // bit18 is ON
      // All Offline Jets
   if (cjet) { // object is available
     for (CaloJetCollection::const_iterator jetiter=cjet->begin(); jetiter!=cjet->end(); jetiter++){
       ntuple_alljets_bit18->Fill(jetiter->energy(), jetiter->pt(), jetiter->eta(), jetiter->phi(), jetiter->emEnergyFraction(), lumi, orbit_no, BX);
    }
  } // end of offline Jets loop


  // Leading Offline Jets
  if (cjet) { // object is available
     for (CaloJetCollection::const_iterator jetiter=cjet->begin(); jetiter!=cjet->end(); jetiter++){
       n_jet_bit18++;
       if(n_jet_bit18>1) break;
       ntuple_Leadingjets_bit18->Fill(jetiter->energy(), jetiter->pt(), jetiter->eta(), jetiter->phi(), jetiter->emEnergyFraction(), lumi, orbit_no, BX);
     
    }
  } // end of offline Jets loop

  // L1 Jets
  if (! l1jets_h.isValid()){ std::cout<<"No L1Jet Information"<<std::endl;}
   else if(l1jets){
      for(l1extra::L1JetParticleCollection::const_iterator l1 = l1jets->begin(); l1 != l1jets->end(); ++l1) {
        ntuple_L1Jets_bit18->Fill(l1->energy(), l1->pt(), l1->eta(), l1->phi(), lumi, orbit_no, BX);
      } // end of forward L1 jets

      for(l1extra::L1JetParticleCollection::const_iterator l1_cent = l1jets_central->begin(); l1_cent != l1jets_central->end(); ++l1_cent) {
         ntuple_L1Jets_bit18->Fill(l1_cent->energy(), l1_cent->pt(),l1_cent->eta(), l1_cent->phi(), lumi, orbit_no, BX);
      } // end of central L1 jets
    } // end of L1 jets loop

 } //end of bit18 loop


  // *** L1 bit55 ***//
  if(bit55){ // bit55 is ON
      // All Offline Jets
   if (cjet) { // object is available
     for (CaloJetCollection::const_iterator jetiter=cjet->begin(); jetiter!=cjet->end(); jetiter++){
       ntuple_alljets_bit55->Fill(jetiter->energy(), jetiter->pt(), jetiter->eta(), jetiter->phi(), jetiter->emEnergyFraction(), lumi, orbit_no, BX);
    }
  } // end of offline Jets loop


  // Leading Offline Jets
  if (cjet) { // object is available
     for (CaloJetCollection::const_iterator jetiter=cjet->begin(); jetiter!=cjet->end(); jetiter++){
       n_jet_bit55++;
       if(n_jet_bit55>1) break;
       ntuple_Leadingjets_bit55->Fill(jetiter->energy(), jetiter->pt(), jetiter->eta(), jetiter->phi(), jetiter->emEnergyFraction(), lumi, orbit_no, BX);
     
    }
  } // end of offline Jets loop

  // L1 Jets
  if (! l1jets_h.isValid()){ std::cout<<"No L1Jet Information"<<std::endl;}
   else if(l1jets){
      for(l1extra::L1JetParticleCollection::const_iterator l1 = l1jets->begin(); l1 != l1jets->end(); ++l1) {
        ntuple_L1Jets_bit55->Fill(l1->energy(), l1->pt(), l1->eta(), l1->phi(), lumi, orbit_no, BX);
      } // end of forward L1 jets

      for(l1extra::L1JetParticleCollection::const_iterator l1_cent = l1jets_central->begin(); l1_cent != l1jets_central->end(); ++l1_cent) {
         ntuple_L1Jets_bit55->Fill(l1_cent->energy(), l1_cent->pt(),l1_cent->eta(), l1_cent->phi(), lumi, orbit_no, BX);
      } // end of central L1 jets
    } // end of L1 jets loop

 } //end of bit55 loop


   // MET
   if (met) { // object is available
     double cut = met->pt() / met->sumEt();
     h_caloMet_Met->Fill(met->pt());
     h_caloMet_Phi->Fill(met->phi());
     h_caloMet_SumEt->Fill(met->sumEt());
     h_caloMet_cut->Fill(cut);
   }


 //---HLT triggers
   Handle<TriggerResults> hltresults;
   iEvent.getByLabel(triggerTag_,hltresults);
   int ntrigs = 0;
   if (hltresults.isValid()) {
     ntrigs = hltresults->size();
     if (do_print_ || evt_cnt<10) {
       std::cout << "Nr. HLT trigger names:" <<ntrigs<< std::endl;
       if (ntrigs==0) {std::cout << "-- No trigger name given in TriggerResults of the input " << std::endl;}
     }
     //     triggerNames_.init(* hltresults); //deprecated in 3_6_X
     for (int itrig = 0; itrig != ntrigs; ++itrig){
           
       string trigName=triggerNames_.triggerName(itrig);
       bool accept = hltresults->accept(itrig);
       
       if (accept) {
	   h_hltbitOn->GetXaxis()->SetBinLabel(itrig+1,trigName.c_str());
	   h_hltbitOn->GetYaxis()->SetLabelSize(0.015);
	   h_hltbitOn->GetYaxis()->SetLabelOffset(0.);
	   h_hltbitOn->Fill(trigName.c_str(),1);
	    
	   h_hltbx->GetYaxis()->SetBinLabel(itrig+1,trigName.c_str());
	   h_hltbx->GetYaxis()->SetLabelSize(0.015);
	   h_hltbx->GetYaxis()->SetLabelOffset(0.);
	   h_hltbx->Fill(BX,trigName.c_str(),1);
	    
	   h_hltlb->GetYaxis()->SetBinLabel(itrig+1,trigName.c_str());
	   h_hltlb->GetYaxis()->SetLabelSize(0.015);
	   h_hltlb->GetYaxis()->SetLabelOffset(0.);
	   h_hltlb->Fill(lumi,trigName.c_str(),1);
	 
       }
     }
   }
   else { if (do_print_ || evt_cnt<10) std::cout << "-- No Trigger Result" << std::endl;}
   h_Nhltbits->Fill(ntrigs);


  //---L1 triggers
   int numberTriggerBits = 0;
   TString algoBitToName[128];
   Handle<L1GlobalTriggerReadoutRecord> L1GTRR;
   iEvent.getByLabel("gtDigis",L1GTRR);
   Handle<L1GlobalTriggerObjectMapRecord> L1GTOMRec;
   iEvent.getByLabel("hltL1GtObjectMap",L1GTOMRec);

   if(L1GTRR.isValid() and L1GTOMRec.isValid()) {
     DecisionWord gtDecisionWord = L1GTRR->decisionWord();
     numberTriggerBits = gtDecisionWord.size();
     if (do_print_ || evt_cnt<10) std::cout << "Nr. L1 trigger names:" <<numberTriggerBits<< std::endl;    

     const std::vector<L1GlobalTriggerObjectMap>& objMapVec =  L1GTOMRec->gtObjectMap();
     for (std::vector<L1GlobalTriggerObjectMap>::const_iterator itMap = objMapVec.begin(); itMap != objMapVec.end(); ++itMap) {
       // Get trigger bits
       int il1trig = (*itMap).algoBitNumber();
       // Get trigger names
       algoBitToName[il1trig] = TString( (*itMap).algoName() );
       if (gtDecisionWord[il1trig]) {
	   h_l1bitOn->GetXaxis()->SetBinLabel(il1trig+1,algoBitToName[il1trig]);
	   h_l1bitOn->GetXaxis()->SetLabelSize(0.015);
	   h_l1bitOn->GetXaxis()->SetLabelOffset(0.);
	   h_l1bitOn->Fill(algoBitToName[il1trig],1);
	 
	   h_l1bx->GetYaxis()->SetBinLabel(il1trig+1,algoBitToName[il1trig]);
	   h_l1bx->GetYaxis()->SetLabelSize(0.015);
	   h_l1bx->GetYaxis()->SetLabelOffset(0.);
	   h_l1bx->Fill(BX,algoBitToName[il1trig],1);
	    
	   h_l1lb->GetYaxis()->SetBinLabel(il1trig+1,algoBitToName[il1trig]);
	   h_l1lb->GetYaxis()->SetLabelSize(0.015);
	   h_l1lb->GetYaxis()->SetLabelOffset(0.);
	   h_l1lb->Fill(lumi,algoBitToName[il1trig],1);
	 
      }
     }
   }
   else{
     if (do_print_ || evt_cnt<10) {
       if (L1GTRR.isValid()) {
	 std::cout << " -- No L1 GT ObjectMapRecord" << std::endl;
       }
       else if (L1GTOMRec.isValid()) {
	 std::cout << " -- No L1 GT ReadoutRecord" << std::endl;
       }
       else {
	 std::cout << " -- No L1 GT ReadoutRecord and ObjectMapRecord" << std::endl;
       }
     }
   }
   h_Nl1bits->Fill(numberTriggerBits);

   h_bx->Fill(lumi);
   h_lb->Fill(lumi);
   h_lbbx->Fill(BX,lumi);
   evt_cnt++;


#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif
   
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
}


// ------------ method called once each job just before starting event loop  ------------
void 
JetTrigAnalyzer::beginJob()
{

  TFileDirectory JetMetDir = fs->mkdir( "JetMet" );
  TFileDirectory L1GTDir = fs->mkdir( "L1GT" );
  TFileDirectory EventDir = fs->mkdir( "Event" );

  ntuple_run_info = EventDir.make<TNtuple>("ntuple_event_info","NTuple for run information","event:lumi:orbit_no:BX");

  ntuple_alljets = JetMetDir.make<TNtuple>("ntuple_alljets","NTuple for all Offline Jets","energy:pt:eta:phi:EMF:lumi:orbit_no:BX");
  ntuple_Leadingjets = JetMetDir.make<TNtuple>("ntuple_Leadingjets","NTuple for Leading Offline Jets","energy:pt:eta:phi:EMF:lumi:orbit_no:BX");
  ntuple_L1Jets = JetMetDir.make<TNtuple>("ntuple_L1Jets","NTuple for L1 Jets","energy:pt:eta:phi:lumi:orbit_no:BX");

  //Bit15 NTuples
  ntuple_alljets_bit15 = JetMetDir.make<TNtuple>("ntuple_alljets_bit15","NTuple for all Offline Jets bit15","energy:pt:eta:phi:EMF:lumi:orbit_no:BX");
  ntuple_Leadingjets_bit15 = JetMetDir.make<TNtuple>("ntuple_Leadingjets_bit15","NTuple for Leading Offline Jets bit15","energy:pt:eta:phi:EMF:lumi:orbit_no:BX");
  ntuple_L1Jets_bit15 = JetMetDir.make<TNtuple>("ntuple_L1Jets_bit15","NTuple for L1 Jets bit15","energy:pt:eta:phi:lumi:orbit_no:BX");

  //Bit16 NTuples
  ntuple_alljets_bit16 = JetMetDir.make<TNtuple>("ntuple_alljets_bit16","NTuple for all Offline Jets bit16","energy:pt:eta:phi:EMF:lumi:orbit_no:BX");
  ntuple_Leadingjets_bit16 = JetMetDir.make<TNtuple>("ntuple_Leadingjets_bit16","NTuple for Leading Offline Jets bit16","energy:pt:eta:phi:EMF:lumi:orbit_no:BX");
  ntuple_L1Jets_bit16 = JetMetDir.make<TNtuple>("ntuple_L1Jets_bit16","NTuple for L1 Jets bit16","energy:pt:eta:phi:lumi:orbit_no:BX");

  //Bit17 NTuples
  ntuple_alljets_bit17 = JetMetDir.make<TNtuple>("ntuple_alljets_bit17","NTuple for all Offline Jets bit17","energy:pt:eta:phi:EMF:lumi:orbit_no:BX");
  ntuple_Leadingjets_bit17 = JetMetDir.make<TNtuple>("ntuple_Leadingjets_bit17","NTuple for Leading Offline Jets bit17","energy:pt:eta:phi:EMF:lumi:orbit_no:BX");
  ntuple_L1Jets_bit17 = JetMetDir.make<TNtuple>("ntuple_L1Jets_bit17","NTuple for L1 Jets bit17","energy:pt:eta:phi:lumi:orbit_no:BX");

  //Bit18 NTuples
  ntuple_alljets_bit18 = JetMetDir.make<TNtuple>("ntuple_alljets_bit18","NTuple for all Offline Jets bit18","energy:pt:eta:phi:EMF:lumi:orbit_no:BX");
  ntuple_Leadingjets_bit18 = JetMetDir.make<TNtuple>("ntuple_Leadingjets_bit18","NTuple for Leading Offline Jets bit18","energy:pt:eta:phi:EMF:lumi:orbit_no:BX");
  ntuple_L1Jets_bit18 = JetMetDir.make<TNtuple>("ntuple_L1Jets_bit18","NTuple for L1 Jets bit18","energy:pt:eta:phi:lumi:orbit_no:BX");

  //Bit55 NTuples
  ntuple_alljets_bit55 = JetMetDir.make<TNtuple>("ntuple_alljets_bit55","NTuple for all Offline Jets bit55","energy:pt:eta:phi:EMF:lumi:orbit_no:BX");
  ntuple_Leadingjets_bit55 = JetMetDir.make<TNtuple>("ntuple_Leadingjets_bit55","NTuple for Leading Offline Jets bit55","energy:pt:eta:phi:EMF:lumi:orbit_no:BX");
  ntuple_L1Jets_bit55 = JetMetDir.make<TNtuple>("ntuple_L1Jets_bit55","NTuple for L1 Jets bit55","energy:pt:eta:phi:lumi:orbit_no:BX");

  // Met Histograms
  h_caloMet_Met = JetMetDir.make<TH1F>("h_caloMet_Met", "MET from CaloTowers", 500, 0., 500.);
  h_caloMet_Phi = JetMetDir.make<TH1F>("h_caloMet_Phi", "MET #phi from CaloTowers", 100, -4., 4.);
  h_caloMet_SumEt = JetMetDir.make<TH1F>("h_caloMet_SumEt", "SumET from CaloTowers", 500, 0., 500.);
  h_caloMet_cut = JetMetDir.make<TH1F>("h_caloMet_cut","MET/SumEt",600,-1.,2.);

  // Trigger Histograms
  h_l1_gtbit = L1GTDir.make<TH1F>("h_l1_gtbit","L1 GT Bits",128, -0.5, 127.5);
  h_Nhltbits = L1GTDir.make<TH1F>("h_Nhltbits", "Number of HLT Trigger Bits", 300, -0.5, 299.5);
  h_Nl1bits = L1GTDir.make<TH1F>("h_Nl1bits", "Number of L1 Trigger Bits", 300, -0.5, 299.5);
  h_hltbitOn = L1GTDir.make<TH1F>("h_hltbitOn", "HLT Trigger Bit On", 128, -0.5, 127.5);
  h_l1bitOn = L1GTDir.make<TH1F>("h_l1bitOn", "L1 Trigger Bit On", 128, -0.5, 127.5);
  h_bx = L1GTDir.make<TH1F>("h_bx", "Bunch Crossing", 3700, -0.5, 3699.5);
  h_l1bx = L1GTDir.make<TH2F>("h_l1bx", "L1 Trigger Vs Bunch Crossing", 3700, -0.5, 3699.5, 128, -0.5, 127.5);
  h_hltbx = L1GTDir.make<TH2F>("h_hltbx", "HLT Trigger Vs Bunch Crossing", 3700, -0.5, 3699.5, 128, -0.5, 127.5);
  h_lb = L1GTDir.make<TH1F>("h_lb", "Luminosity Block", 100, -0.5, 99.5);
  h_lbbx = L1GTDir.make<TH2F>("h_lbbx", "Luminosity Block Vs Bunch Crossing", 3700, -0.5, 3699.5, 100, -0.5, 99.5);
  h_hltlb = L1GTDir.make<TH2F>("h_hltlb", "HLT Trigger Vs Luminosity Block", 100, -0.5, 99.5, 128, -0.5, 127.5);
  h_l1lb = L1GTDir.make<TH2F>("h_l1lb", "L1 Trigger Vs Luminosity Block0", 100, -0.5, 99.5, 128, -0.5, 127.5);


}

// ------------ method called once each job just after ending the event loop  ------------
void 
JetTrigAnalyzer::endJob() {
}

//define this as a plug-in
//DEFINE_FWK_MODULE(JetTrigAnalyzer);
