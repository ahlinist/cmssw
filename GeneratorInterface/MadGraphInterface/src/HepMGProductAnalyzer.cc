#include "GeneratorInterface/MadGraphInterface/interface/HepMGProductAnalyzer.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include <iostream>
#include "TFile.h"
#include "TH1.h"

using namespace edm;
using namespace std;

HepMGProductAnalyzer::HepMGProductAnalyzer(const edm::ParameterSet& iConfig) :
label_(iConfig.getUntrackedParameter("moduleLabel",std::string("source")))
{
  // Set up histograms and output file
  fOutputFile   = new TFile( "GenLevelPlots.root", "RECREATE" ) ;
  etaPlot  = new TH1D(  "eta", "Parton level eta", 50,  -5., 5. ) ;
  etPlot  = new TH1D(  "et", "Parton level transverse energy", 10, 0, 500.) ;
  phiPlot   = new TH1D(  "phi",  "Parton level phi",   30, -3., 3. ) ; 
  nt = new TNtuple("GenTuple","Ntuple for generator stuff","HiggsEta:HiggsPhi:HiggsEt");
}


HepMGProductAnalyzer::~HepMGProductAnalyzer()
{
  // Write to file and close it
  fOutputFile->Write();
  fOutputFile->Close();
}

void HepMGProductAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   Handle<HepMCProduct> evt;
   iEvent.getByType(evt);

   HepMC::GenEvent * myGenEvent = new  HepMC::GenEvent(*(evt->GetEvent()));
  
   //To print out status code, pdg, mass, etc table uncomment below
   //   this->printTable(myGenEvent);

   //To histogram phi, eta and transverse energy for the Higgs
   for ( HepMC::GenEvent::particle_iterator p = myGenEvent->particles_begin();
	 p != myGenEvent->particles_end(); ++p) { 
     if((*p)->pdg_id()==25){
       pdgPlot->Fill((*p)->pdg_id());
       etaPlot->Fill((*p)->momentum().rapidity());
       etPlot->Fill((*p)->momentum().et());
       phiPlot->Fill((*p)->momentum().phi());
       nt->Fill((*p)->momentum().rapidity(),(*p)->momentum().phi(),(*p)->momentum().et(),1);
     }
   }
   delete myGenEvent;
}

void HepMGProductAnalyzer::printTable(HepMC::GenEvent * event)
{
  HepMC::GenEvent * myGenEvent = event;
  std::cout<<std::endl;
  std::cout<<"Pgd  status   Px   Py   Pz   E   mass"<<std::endl;   
  for ( HepMC::GenEvent::particle_iterator p = myGenEvent->particles_begin();
	p != myGenEvent->particles_end(); ++p) { 
    std::cout<<(*p)->pdg_id()<<"   "<<(*p)->status()<<"   "<<(*p)->momentum().x()<<"   "<<(*p)->momentum().y()<<"   "<<(*p)->momentum().z()<<"     "<<(*p)->momentum().t()<<"   "<<(*p)->generatedMass()<<std::endl;
  }
}
