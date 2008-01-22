// Authors: F. Ambroglini, L. Fano'
#include <iostream>

#include "AnalysisExamples/MinimumBiasUnderlyingEvent/test/MBUEAnalysisRootpleProducerOnlyMC.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
 
 
using namespace edm;
using namespace std;
using namespace reco;


class GreaterPt{
public:
  bool operator()( const math::XYZTLorentzVector& a, const math::XYZTLorentzVector& b) {
    return a.pt() > b.pt();
  }
};

class GenJetSort{
public:
  bool operator()(const GenJet& a, const GenJet& b) {
    return a.pt() > b.pt();
  }
};


void MBUEAnalysisRootpleProducerOnlyMC::store(){
  MBUEAnalysisTree->Fill();
  NumberMCParticles=0;
  NumberInclusiveJet=0;
  NumberChargedJet=0;
}

void MBUEAnalysisRootpleProducerOnlyMC::fillEventInfo(int e){
  EventKind = e;
}

void MBUEAnalysisRootpleProducerOnlyMC::fillMCParticles(float p, float pt, float eta, float phi){
  MomentumMC[NumberMCParticles]=p;
  TransverseMomentumMC[NumberMCParticles]=pt;
  EtaMC[NumberMCParticles]=eta;
  PhiMC[NumberMCParticles]=phi;
  NumberMCParticles++;
}

void MBUEAnalysisRootpleProducerOnlyMC::fillInclusiveJet(float p, float pt, float eta, float phi){
  MomentumIJ[NumberInclusiveJet]=p;
  TransverseMomentumIJ[NumberInclusiveJet]=pt;
  EtaIJ[NumberInclusiveJet]=eta;
  PhiIJ[NumberInclusiveJet]=phi;
  NumberInclusiveJet++;
}

void MBUEAnalysisRootpleProducerOnlyMC::fillChargedJet(float p, float pt, float eta, float phi){
  MomentumCJ[NumberChargedJet]=p;
  TransverseMomentumCJ[NumberChargedJet]=pt;
  EtaCJ[NumberChargedJet]=eta;
  PhiCJ[NumberChargedJet]=phi;
  NumberChargedJet++;
}

MBUEAnalysisRootpleProducerOnlyMC::MBUEAnalysisRootpleProducerOnlyMC( const ParameterSet& pset )
  : fOutputFileName( pset.getUntrackedParameter<string>("HistOutFile",std::string("TestHiggsMass.root")) ),
    mcEvent( pset.getUntrackedParameter<string>("MCEvent",std::string(""))),
    genJetCollName( pset.getUntrackedParameter<string>("GenJetCollectionName",std::string(""))),
    chgJetCollName( pset.getUntrackedParameter<string>("ChgGenJetCollectionName",std::string(""))),
    chgGenPartCollName( pset.getUntrackedParameter<string>("ChgGenPartCollectionName",std::string("")))
{
  piG = acos(-1.);
  NumberMCParticles=0;
  NumberInclusiveJet=0;
  NumberChargedJet=0;
}


void MBUEAnalysisRootpleProducerOnlyMC::beginJob( const EventSetup& )
{
  
  hFile = new TFile ( fOutputFileName.c_str(), "RECREATE" );
  MBUEAnalysisTree = new TTree("MBUEAnalysisTree","MBUE Analysis Tree ");
  
  MBUEAnalysisTree->Branch("EventKind",&EventKind,"EventKind/I");
  
  MBUEAnalysisTree->Branch("NumberMCParticles",&NumberMCParticles,"NumberMCParticles/I");
  MBUEAnalysisTree->Branch("MomentumMC",MomentumMC,"MomentumMC[NumberMCParticles]/F");
  MBUEAnalysisTree->Branch("TransverseMomentumMC",TransverseMomentumMC,"TransverseMomentumMC[NumberMCParticles]/F");
  MBUEAnalysisTree->Branch("EtaMC",EtaMC,"EtaMC[NumberMCParticles]/F");
  MBUEAnalysisTree->Branch("PhiMC",PhiMC,"PhiMC[NumberMCParticles]/F");
  
  MBUEAnalysisTree->Branch("NumberTracks",&NumberTracks,"NumberTracks/I");
  MBUEAnalysisTree->Branch("MomentumTK",MomentumTK,"MomentumTK[NumberTracks]/F");
  MBUEAnalysisTree->Branch("TrasverseMomentumTK",TransverseMomentumTK,"TransverseMomentumTK[NumberTracks]/F");
  MBUEAnalysisTree->Branch("EtaTK",EtaTK,"EtaTK[NumberTracks]/F");
  MBUEAnalysisTree->Branch("PhiTK",PhiTK,"PhiTK[NumberTracks]/F");
  
  MBUEAnalysisTree->Branch("NumberInclusiveJet",&NumberInclusiveJet,"NumberInclusiveJet/I");
  MBUEAnalysisTree->Branch("MomentumIJ",MomentumIJ,"MomentumIJ[NumberInclusiveJet]/F");
  MBUEAnalysisTree->Branch("TrasverseMomentumIJ",TransverseMomentumIJ,"TransverseMomentumIJ[NumberInclusiveJet]/F");
  MBUEAnalysisTree->Branch("EtaIJ",EtaIJ,"EtaIJ[NumberInclusiveJet]/F");
  MBUEAnalysisTree->Branch("PhiIJ",PhiIJ,"PhiIJ[NumberInclusiveJet]/F");
  
  MBUEAnalysisTree->Branch("NumberChargedJet",&NumberChargedJet,"NumberChargedJet/I");
  MBUEAnalysisTree->Branch("MomentumCJ",MomentumCJ,"MomentumCJ[NumberChargedJet]/F");
  MBUEAnalysisTree->Branch("TrasverseMomentumCJ",TransverseMomentumCJ,"TransverseMomentumCJ[NumberChargedJet]/F");
  MBUEAnalysisTree->Branch("EtaCJ",EtaCJ,"EtaCJ[NumberChargedJet]/F");
  MBUEAnalysisTree->Branch("PhiCJ",PhiCJ,"PhiCJ[NumberChargedJet]/F");
  
  MBUEAnalysisTree->Branch("NumberTracksJet",&NumberTracksJet,"NumberTracksJet/I");
  MBUEAnalysisTree->Branch("MomentumTJ",MomentumTJ,"MomentumTJ[NumberTracksJet]/F");
  MBUEAnalysisTree->Branch("TrasverseMomentumTJ",TransverseMomentumTJ,"TransverseMomentumTJ[NumberTracksJet]/F");
  MBUEAnalysisTree->Branch("EtaTJ",EtaTJ,"EtaTJ[NumberTracksJet]/F");
  MBUEAnalysisTree->Branch("PhiTJ",PhiTJ,"PhiTJ[NumberTracksJet]/F");
  
  MBUEAnalysisTree->Branch("NumberCaloJet",&NumberCaloJet,"NumberCaloJet/I");
  MBUEAnalysisTree->Branch("MomentumEHJ",MomentumEHJ,"MomentumEHJ[NumberCaloJet]/F");
  MBUEAnalysisTree->Branch("TrasverseMomentumEHJ",TransverseMomentumEHJ,"TransverseMomentumEHJ[NumberCaloJet]/F");
  MBUEAnalysisTree->Branch("EtaEHJ",EtaEHJ,"EtaEHJ[NumberCaloJet]/F");
  MBUEAnalysisTree->Branch("PhiEHJ",PhiEHJ,"PhiEHJ[NumberCaloJet]/F");
  
}

  
void MBUEAnalysisRootpleProducerOnlyMC::analyze( const Event& e, const EventSetup& )
{
  
  Handle< HepMCProduct > EvtHandle ;
  
  e.getByLabel( mcEvent.c_str(), EvtHandle ) ;
  
  const HepMC::GenEvent* Evt = EvtHandle->GetEvent() ;
  
  EventKind = Evt->signal_process_id();
  
  Handle< CandidateCollection > CandHandleMC ;
  
  Handle< GenJetCollection > GenJetsHandle ;
  Handle< GenJetCollection > ChgGenJetsHandle ;
  
  e.getByLabel( chgGenPartCollName.c_str(), CandHandleMC );
  
  e.getByLabel(chgJetCollName.c_str(), ChgGenJetsHandle );
  e.getByLabel(genJetCollName.c_str(), GenJetsHandle );
  
  std::vector<math::XYZTLorentzVector> GenPart;
  std::vector<GenJet> ChgGenJetContainer;
  std::vector<GenJet> GenJetContainer;
  
  GenPart.clear();
  ChgGenJetContainer.clear();
  GenJetContainer.clear();
  
  if(ChgGenJetsHandle->size()){
    for(GenJetCollection::const_iterator it=ChgGenJetsHandle->begin();it!=ChgGenJetsHandle->end();it++)
      ChgGenJetContainer.push_back(*it);
    std::stable_sort(ChgGenJetContainer.begin(),ChgGenJetContainer.end(),GenJetSort());
    for(std::vector<GenJet>::const_iterator it = ChgGenJetContainer.begin(); it != ChgGenJetContainer.end(); it++)
      fillChargedJet(it->p(),it->pt(),it->eta(),it->phi());
  }
  
  if(GenJetsHandle->size()){
    for(GenJetCollection::const_iterator it=GenJetsHandle->begin();it!=GenJetsHandle->end();it++)
      GenJetContainer.push_back(*it);
    std::stable_sort(GenJetContainer.begin(),GenJetContainer.end(),GenJetSort());
    for(std::vector<GenJet>::const_iterator it = GenJetContainer.begin(); it != GenJetContainer.end(); it++)
      fillInclusiveJet(it->p(),it->pt(),it->eta(),it->phi());
  }
  
  if(CandHandleMC->size()){
    for(CandidateCollection::const_iterator it = CandHandleMC->begin();it!=CandHandleMC->end();it++){
      GenPart.push_back(it->p4());
    }
    std::stable_sort(GenPart.begin(),GenPart.end(),GreaterPt());
    for(std::vector<math::XYZTLorentzVector>::const_iterator it = GenPart.begin(); it != GenPart.end(); it++)
      fillMCParticles(it->P(),it->Pt(),it->Eta(),it->Phi());
  }
  
  store();
}

void MBUEAnalysisRootpleProducerOnlyMC::endJob(){
  hFile->cd();
  MBUEAnalysisTree->Write();
  hFile->Close();
}

DEFINE_FWK_MODULE(MBUEAnalysisRootpleProducerOnlyMC);
