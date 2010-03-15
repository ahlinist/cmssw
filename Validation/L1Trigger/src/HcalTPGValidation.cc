// -*- C++ -*-
//
// Package:    HcalTPGValidation
// Class:      HcalTPGValidation
// 
/**\class HcalTPGValidation HcalTPGValidation.cc Validation/HcalTPGValidation/src/HcalTPGValidation.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Ka Vang Tsang,8 R-004,+41227676479,
//         Created:  Fri Mar 12 18:25:36 CET 2010
// $Id: HcalTPGValidation.cc,v 1.2 2010/03/15 01:14:27 kvtsang Exp $
//
//


// system include files
#include "Validation/L1Trigger/interface/HcalTPGValidation.h"

#include <memory>
#include "SimDataFormats/CaloHit/interface/PCaloHitContainer.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/HcalDetId/interface/HcalTrigTowerDetId.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"

#include "FWCore/Framework/interface/ESHandle.h"                
#include "CalibFormats/CaloTPG/interface/HcalTPGCompressor.h"   
#include "CalibFormats/CaloTPG/interface/CaloTPGRecord.h"       
#include "CalibFormats/CaloTPG/interface/CaloTPGTranscoder.h"   

HcalTPGValidation::HcalTPGValidation(const edm::ParameterSet& iConfig) :
   dqm_folder_(iConfig.getParameter<std::string>("DQMFolder")),
   simhits_label_(iConfig.getParameter<edm::InputTag>("SimHitsLabel")),
   tp_label_(iConfig.getParameter<edm::InputTag>("HcalTPLabel")),
   factor_HB_(iConfig.getParameter<double>("SamplingFactorHB")),
   factor_HE_(iConfig.getParameter<double>("SamplingFactorHE")),
   factor_HF1_(iConfig.getParameter<double>("SamplingFactorHF1")),
   factor_HF2_(iConfig.getParameter<double>("SamplingFactorHF2")),
   threshold_(iConfig.getParameter<double>("EtThreshold"))
{
   //now do what ever initialization is needed
   dbe_ = edm::Service<DQMStore>().operator->();
   dbe_->setCurrentFolder(dqm_folder_);

   for (int ieta = -geometry_.nTowers(); ieta <= geometry_.nTowers(); ++ieta) {
      if (ieta == 0)
         continue;
      double eta1 = 0.0;
      double eta2 = 0.0;
      geometry_.towerEtaBounds(ieta, eta1, eta2);
      cosh_ieta_[ieta] = cosh((eta1 + eta2) / 2.0);
   }//for ieta
}


HcalTPGValidation::~HcalTPGValidation()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
HcalTPGValidation::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   Handle<PCaloHitContainer> hit_col;
   iEvent.getByLabel(simhits_label_, hit_col);

   edm::Handle<HcalTrigPrimDigiCollection> hcalTPCol;
   iEvent.getByLabel(tp_label_, hcalTPCol);

   edm::ESHandle<CaloTPGTranscoder> outTranscoder; 
   iSetup.get<CaloTPGRecord>().get(outTranscoder); 
   outTranscoder->setup(iSetup);                   

   std::map<HcalTrigTowerDetId, double> energy_from_simhits;

   if (hit_col.isValid()) {
      for (PCaloHitContainer::const_iterator hit = hit_col->begin(); hit != hit_col->end(); ++hit) {
         double sim_energy = hit->energy();

         if (sim_energy < 0)
            continue;

         HcalDetId detid(hit->id());
         // Calibration for simHits
         switch (detid.subdet()) {
            case HcalBarrel:
               sim_energy *= factor_HB_;
               break;
            case HcalEndcap:
               sim_energy *= factor_HE_;
               break;
            case HcalForward:
               sim_energy *= detid.depth() == 1 ? factor_HF1_ : factor_HF2_;
               break;
            default:
               sim_energy *= 0.0;
         }

         std::vector<HcalTrigTowerDetId> trigId = geometry_.towerIds(detid);
         double tp_energy = sim_energy / trigId.size();
         for (size_t i=0; i<trigId.size(); ++i) {
            std::map<HcalTrigTowerDetId, double>::iterator itr = energy_from_simhits.find(trigId[i]);
            if (itr == energy_from_simhits.end())
               energy_from_simhits.insert(std::make_pair(trigId[i], tp_energy));
            else
               itr->second += tp_energy;
         }//for trigId
      }//for hit_col
   }//hit_col.isValid

   if (hcalTPCol.isValid()) {
      for (HcalTrigPrimDigiCollection::const_iterator tp = hcalTPCol->begin(); tp != hcalTPCol->end(); ++tp) {
         int ieta = tp->id().ieta();
         int iphi = tp->id().iphi();

         double tp_et = outTranscoder->hcaletValue(ieta, iphi, tp->SOI_compressedEt());

         std::map<HcalTrigTowerDetId, double>::const_iterator simhit = energy_from_simhits.find(tp->id());
         double simhit_energy = simhit != energy_from_simhits.end() ? simhit->second : 0.0;
         double simhit_et = simhit_energy / cosh_ieta_[ieta];

         if (simhit_et > threshold_) {
            if (abs(ieta) < 16) {
               histo_Et_corr_HB->Fill(simhit_et, tp_et);
               histo_Et_ratio_HB->Fill(tp_et / simhit_et);
            }//HB
            else if (abs(ieta) < 29) {
               histo_Et_corr_HE->Fill(simhit_et, tp_et);
               histo_Et_ratio_HE->Fill(tp_et / simhit_et);
            }//HE
            else {
               histo_Et_corr_HF->Fill(simhit_et, tp_et);
               histo_Et_ratio_HF->Fill(tp_et / simhit_et);
            }//HF
         }//simhit_et > 2
      }//for hcalTPCol
   }//hcalTPCol.isValid()
}

// ------------ method called once each job just before starting event loop  ------------
void 
HcalTPGValidation::beginJob()
{
   histo_Et_corr_HB = dbe_->book2D("Et_corr_HB", "Et_corr_HB", 100, 0, 100, 100, 0, 100);
   histo_Et_corr_HE = dbe_->book2D("Et_corr_HE", "Et_corr_HE", 100, 0, 100, 100, 0, 100);
   histo_Et_corr_HF = dbe_->book2D("Et_corr_HF", "Et_corr_HF", 100, 0, 100, 100, 0, 100);
   histo_Et_ratio_HB = dbe_->book1D("Et_ratio_HB", "Et_ratio_HB", 50, 0.01, 3);
   histo_Et_ratio_HE = dbe_->book1D("Et_ratio_HE", "Et_ratio_HE", 50, 0.01, 3);
   histo_Et_ratio_HF = dbe_->book1D("Et_ratio_HF", "Et_ratio_HF", 50, 0.01, 3);
}

// ------------ method called once each job just after ending the event loop  ------------
void 
HcalTPGValidation::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(HcalTPGValidation);
