// -*- C++ -*-
// Package:    STFilter
// Class:      STFilter
/**\class STFilter STFilter.cc MyEDFilter/STFilter/src/STFilter.cc
 Description: 

 ** used for single top t-channel events generated with MadEvent 
    and matched the "Karlsruhe way"
 ** filter on 2->2 process events, where the crucial candidate (the 2nd b quark) 
    is not available until parton showering is done 
 ** filter criterion: transverse momentum of 2nd b quark "pT < pTMax" -> event accepted!
 ** How-To: include STFilter.cfg in your .cfg, replace pTMax by the desired value, 
    include module "STFilter" in outpath
 Implementation:
     <Notes on implementation>
*/
// Original Author:  Julia Weinelt
//         Created:  Wed Jan 23 15:12:46 CET 2008
// $Id: STFilter.cc,v 1.8 2008/08/07 12:37:15 saout Exp $
#include <memory>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "HepMC/GenEvent.h"
#include "TFile.h"
#include "TH1D.h"
#include "TString.h"

class STFilter : public edm::EDFilter {
   public:
      explicit STFilter(const edm::ParameterSet&);
      ~STFilter();
   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual bool filter(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
   private:
      double pTMax_;
      // debug level
      int DEBUGLVL;    
      // counters
      unsigned int input_events;
      unsigned int accepted_events;
      // histograms
      bool m_produceHistos;
      TH1D* hbPt; TH1D* hbPtFiltered;
      TH1D* hbEta; TH1D* hbEtaFiltered;
      // histogram output file
      std::string fOutputFileName ;
      TFile*  hOutputFile ;
      //
      edm::ParameterSet conf_;
};

STFilter::STFilter(const edm::ParameterSet& iConfig) {
  pTMax_ = iConfig.getParameter<double>("pTMax");
  edm::LogInfo("SingleTopMatchingFilter")<<"+++ maximum pt of associated-b  pTMax = "<<pTMax_;
  DEBUGLVL = iConfig.getUntrackedParameter<int>("debuglvl", 0); // get debug level
  input_events=0; accepted_events=0; // counters
  m_produceHistos = iConfig.getParameter<bool>("produceHistos"); // produce histograms?
  fOutputFileName = iConfig.getUntrackedParameter<std::string>("histOutFile"); // get name of output file with histograms
  conf_ = iConfig;
}


STFilter::~STFilter() {}

bool STFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  using namespace edm;
  
  bool accEvt = false;
  bool lo = false;

  int secBcount = 0;
  double pTSecB = 100;
  double etaSecB = 100;

  ++input_events;
  
  Handle<HepMCProduct> evt;
  iEvent.getByType(evt);
  const HepMC::GenEvent * myEvt = evt->GetEvent();   // GET EVENT FROM HANDLE 
  
  bool bQuarksOpposite = false;
  for(HepMC::GenEvent::particle_const_iterator i = myEvt->particles_begin();
      (*i)->status() == 3; ++i) { // abort after status 3 particles
    // logic:
    // - in 2->2 matrix elements, the incoming (top production)
    //   b quark and the outgoing (top decay) b quark have same sign,
    //   so we flip the bQuarksOpposite flag twice -> false
    //   (opposite-sign b quark comes from the shower and has status 2)
    // - in 2->3 matrix elements, we have two outgoing b quarks with status
    //   3 and opposite signs -> true
    if ((*i)->pdg_id() == -5)
      bQuarksOpposite = !bQuarksOpposite;
  }

  // ---- 22 or 23? ----
  if (!bQuarksOpposite) // 22 
    lo = true;
  else
    accEvt = true;      // 23
  
  // ---- filter only 22 events ----
  if (lo){
    for (HepMC::GenEvent::particle_const_iterator p = myEvt->particles_begin(); p!=myEvt->particles_end(); ++p){
      // ---- look in shower for 2nd b quark ----
      if ((*p)->status() == 2 && abs((*p)->pdg_id()) == 5){  
	// ---- if b quark is found, loop over its parents ----
	for (HepMC::GenVertex::particle_iterator m = (*p)->production_vertex()->particles_begin(HepMC::parents); 
	     m != (*p)->production_vertex()->particles_end(HepMC::parents); ++m){
	  // ---- found 2ndb-candidate in shower ---- // ---- check mother of this candidate ----
	  if(abs((*m)->barcode()) < 5){        
	    if(secBcount == 1) break;
	    secBcount ++;
	    pTSecB = (*p)->operator HepMC::FourVector().perp();
	    etaSecB = (*p)->operator HepMC::FourVector().eta();
	  }
	}
      }
    }
    if(pTSecB < pTMax_) accEvt = true;
    // fill histos if requested
    if(m_produceHistos){
      hbPt->Fill(pTSecB);
      hbEta->Fill(etaSecB);
      if(accEvt){
        hbPtFiltered->Fill(pTSecB);
        hbEtaFiltered->Fill(etaSecB);
      }
    }
  }
  if(accEvt) ++accepted_events;
  return accEvt;
}


void STFilter::beginJob(const edm::EventSetup&) {
  if(m_produceHistos){ // initialize histogram output file
    edm::ParameterSet Parameters;
    edm::LogInfo("SingleTopMatchingFilter)")<<"beginJob : creating histogram file: "<<fOutputFileName.c_str()<<std::endl;
    hOutputFile   = new TFile( fOutputFileName.c_str(), "RECREATE" ) ; hOutputFile->cd();
    // book histograms
    Parameters =  conf_.getParameter<edm::ParameterSet>("TH1bPt");
    hbPt = new TH1D( "bPt", "Pt of 2nd b quark", Parameters.getParameter<int32_t>("Nbinx"), Parameters.getParameter<double>("xmin"), Parameters.getParameter<double>("xmax"));
    Parameters =  conf_.getParameter<edm::ParameterSet>("TH1bEta");
    hbEta = new TH1D( "bEta", "Eta of 2nd b quark", Parameters.getParameter<int32_t>("Nbinx"), Parameters.getParameter<double>("xmin"), Parameters.getParameter<double>("xmax"));
    Parameters =  conf_.getParameter<edm::ParameterSet>("TH1bPtFiltered");
    hbPtFiltered = new TH1D( "bPtFiltered", "Pt of 2nd b quark filtered",  Parameters.getParameter<int32_t>("Nbinx"), Parameters.getParameter<double>("xmin"), Parameters.getParameter<double>("xmax"));
    Parameters =  conf_.getParameter<edm::ParameterSet>("TH1bEtaFiltered");
    hbEtaFiltered = new TH1D( "bEtaFiltered", "Eta of 2nd b quark filtered",  Parameters.getParameter<int32_t>("Nbinx"), Parameters.getParameter<double>("xmin"), Parameters.getParameter<double>("xmax"));
  }
}

void STFilter::endJob() {
 if(m_produceHistos){
   hOutputFile->cd();
   hbPt->Write(); hbEta->Write(); hbPtFiltered->Write(); hbEtaFiltered->Write();
   hOutputFile->Write() ; hOutputFile->Close() ;} // Write out histograms to file, then close it
   double fraction = (double) accepted_events / (double) input_events;
   double percent =  100. * fraction; double error =  100. * sqrt( fraction*(1-fraction) / (double) input_events );
   std::cout<<"STFilter ++ accepted_events/input_events = "<<accepted_events<<"/"<<input_events<<" = "<<fraction<<std::endl;
   std::cout<<"STFilter ++ efficiency  = "<<percent<<" % +/- "<<error<<" %"<<std::endl;
}

DEFINE_FWK_MODULE(STFilter);
