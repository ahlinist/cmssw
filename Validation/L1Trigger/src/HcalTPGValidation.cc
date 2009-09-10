// -*- C++ -*-
//
// Package:    HcalTPGValidation
// Class:      HcalTPGValidation
// 
/**\class HcalTPGValidation HcalTPGValidation.cc Validation/HcalTPGValidation/src/HcalTPGValidation.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Ka Vang TSANG
//         Created:  Fri Aug 28 15:27:42 CEST 2009
// $Id$
//
//


// system include files
#include <memory>
#include <map>
#include <iostream>
#include <functional>
#include <cmath>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "Geometry/HcalTowerAlgo/interface/HcalTrigTowerGeometry.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/GeometryVector/interface/Pi.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "CalibFormats/CaloTPG/interface/HcalTPGCompressor.h"
#include "CalibFormats/CaloTPG/interface/CaloTPGRecord.h"
#include "CalibFormats/CaloTPG/interface/CaloTPGTranscoder.h"
#include "CondFormats/DataRecord/interface/L1CaloEcalScaleRcd.h"
#include "CondFormats/L1TObjects/interface/L1CaloEcalScale.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"

//TFileService
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"
#include "TProfile.h"

//
// class decleration
//

class HcalTPGValidation : public edm::EDAnalyzer {
   public:
      explicit HcalTPGValidation(const edm::ParameterSet&);
      ~HcalTPGValidation();


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // functinal for mapping eta to ieta
      struct in_the_range: public std::binary_function< std::pair<double, double>,
                                                        std::pair<double, double>, bool>{
         bool operator()(std::pair<double, double> const & lhs, std::pair<double, double> const & rhs){
            return (lhs.first < rhs.first && lhs.second <= rhs.second);
         }
      };
      typedef std::map< std::pair<double, double>, int, in_the_range> range_map_type;

      // ----------member data ---------------------------
      edm::InputTag hcalTag_;
      edm::InputTag ecalTag_;
      edm::InputTag genTag_;
      double jetPtThreshold_;

      // Map ieta to eta
      range_map_type ietaFromEtaRange;

      // Map eta to ieta
      std::map<int, double> realEta;
      std::map<int, double> realPhi;

      // Histogram for TFileService
      TProfile *mcEfficiency;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
HcalTPGValidation::HcalTPGValidation(const edm::ParameterSet& iConfig) :
   hcalTag_(iConfig.getParameter<edm::InputTag>("hcalTag")),
   ecalTag_(iConfig.getParameter<edm::InputTag>("ecalTag")),
   genTag_(iConfig.getParameter<edm::InputTag>("genTag")),
   jetPtThreshold_(iConfig.getUntrackedParameter<double>("jetPtThreshold",0))
{
   //now do what ever initialization is needed
   HcalTrigTowerGeometry theTrigTowerGeometry;
   for (int ieta=-32; ieta<=32; ++ieta){
     double eta1 = 0.0;
     double eta2 = 0.0;
     theTrigTowerGeometry.towerEtaBounds(ieta,eta1,eta2);
     realEta[ieta] = (eta1 + eta2) / 2.;

     ietaFromEtaRange[ std::pair<double, double>(eta1, eta2) ] = ieta;
   }
   for (int iphi=1; iphi <= 72; ++iphi){
      double phi = (iphi - 1) / 72. * 2 * M_PI;
      if (phi > M_PI) phi -= 2 * M_PI;
      realPhi[iphi] = phi;
   }

   // TFileService
   edm::Service<TFileService> fs;
   mcEfficiency = fs->make<TProfile>("mcEfficiency","MC Efficiency",65,-32.5,32.5,-1,1);
   mcEfficiency->SetXTitle("ieta");
   mcEfficiency->SetYTitle("(HCAL TP Et + ECAL TP Et p_{T}^{MC}) / p_{T}^{MC}");
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

   Handle<reco::GenParticleCollection> genParticles;
   iEvent.getByLabel(genTag_, genParticles);

   // Setup HCAL
   edm::Handle<HcalTrigPrimDigiCollection> hcalTPCol;
   iEvent.getByLabel(hcalTag_, hcalTPCol);

   edm::ESHandle<CaloTPGTranscoder> outTranscoder;
   iSetup.get<CaloTPGRecord>().get(outTranscoder);

   // Setup ECAL
   edm::Handle<EcalTrigPrimDigiCollection> ecalTPCol;
   iEvent.getByLabel(ecalTag_, ecalTPCol);
   ////Setup Calo Scales
   edm::ESHandle<L1CaloEcalScale> ecalScale;
   iSetup.get<L1CaloEcalScaleRcd>().get(ecalScale);
   const L1CaloEcalScale *eScale = ecalScale.product();

   for (reco::GenParticleCollection::const_iterator p = genParticles->begin(); p != genParticles->end(); ++p){

      //Choose final state quarks with pT > 20 GeV
      if (p->pdgId()<1 || p->pdgId()>6 || p->status()!=2) continue;
      if (p->pt() < jetPtThreshold_) continue;

      double mc_eta = p->eta();
      double mc_phi = p->phi();
      int mc_ieta = ietaFromEtaRange[std::pair<double, double>(mc_eta, mc_eta)];

      double mc_et = p->et();
      double hcalet = 0.0;
      double ecalet = 0.0;

      // Match MC to HCAL TP towers
      for (HcalTrigPrimDigiCollection::const_iterator tp = hcalTPCol->begin(); tp != hcalTPCol->end(); ++tp){
         int ieta = tp->id().ieta();
         int iphi = tp->id().iphi();
         double eta = realEta[ieta];
         double phi = realPhi[iphi];

         double dphi = phi - mc_phi;
         if ( dphi > M_PI ) dphi -= 2.0*M_PI;
         else if ( dphi <= -M_PI) dphi += 2.0*M_PI;
         double deta = eta - mc_eta;
         double dR = std::sqrt( deta*deta + dphi*dphi );

         double et = outTranscoder->hcaletValue(ieta, iphi, tp->SOI_compressedEt());

         if (dR < 0.5) hcalet += et;
      }

      // Match MC to ECAL TP towers
      for (EcalTrigPrimDigiCollection::const_iterator tp = ecalTPCol->begin(); tp != ecalTPCol->end(); ++tp){
         int ieta =tp->id().ieta();
         int iphi =tp->id().iphi();
         double eta = realEta[ieta];
         double phi = realPhi[iphi];

         double dphi = phi - mc_phi;
         if ( dphi > M_PI ) dphi -= 2.0*M_PI;
         else if ( dphi <= -M_PI) dphi += 2.0*M_PI;
         double deta = eta - mc_eta;
         double dR = std::sqrt( deta*deta + dphi*dphi );

         double et = eScale->et(tp->compressedEt(), abs(ieta), ieta/abs(ieta));

         if (dR < 0.5) ecalet += et;
      }

      mcEfficiency->Fill(mc_ieta, (hcalet + ecalet - mc_et)/mc_et);
   }
}


// ------------ method called once each job just before starting event loop  ------------
void 
HcalTPGValidation::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
HcalTPGValidation::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(HcalTPGValidation);
