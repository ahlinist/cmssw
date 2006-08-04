// Authors: F. Ambroglini, L. Fano'


#include <iostream>

#include "AnalysisExamples/MinimumBiasUnderlyingEvent/test/MinimumBiasAnalyzer.h"
 
#include "DataFormats/Common/interface/Ref.h" 
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetfwd.h"
#include "DataFormats/JetReco/interface/BasicJet.h"
#include "DataFormats/JetReco/interface/BasicJetfwd.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/HepMCCandidate/interface/HepMCCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
 
#include "TFile.h"
#include "TH1.h"

 
using namespace edm;
using namespace std;
using namespace reco;

class GreaterPt{
public:
  bool operator()( const math::XYZTLorentzVector& a, const math::XYZTLorentzVector& b) {
    return a.Pt() > b.Pt();
  }
};

class GenJetSort{
public:
  bool operator()(const GenJet& a, const GenJet& b) {
    return a.pt() > b.pt();
  }
};

class BasicJetSort{
public:
  bool operator()(const BasicJet& a, const BasicJet& b) {
    return a.pt() > b.pt();
  }
};
 
MinimumBiasAnalyzer::MinimumBiasAnalyzer( const ParameterSet& pset )
   : fOutputFileName( pset.getUntrackedParameter<string>("HistOutFile",std::string("TestHiggsMass.root")) ),
     objectAnalyzed( pset.getUntrackedParameter<string>("UsedCandidate",std::string("MCCandidate")) ),
     fOutputFile(0), fHistPtDist(0), fHistEtaDist(0), fHistPhiDist(0), fHistChgDist(0), 
     fHistChgSimm(0),pdN_vs_dphi(0),pdPt_vs_dphi(0), temp1(0), temp2(0), fHistPtDistJetChg(0),
     fHistEtaDistJetChg(0), fHistPhiDistJetChg(0)
{
}

void MinimumBiasAnalyzer::beginJob( const EventSetup& )
{
 
   fOutputFile   = new TFile( fOutputFileName.c_str(), "RECREATE" ) ;
   fHistPtDist   = new TH1D(  "HistPtDist"  , "Pt Spectra", 100,  0., 4. ) ;
   fHistEtaDist  = new TH1D(  "HistEtaDist" , "#eta Spectra", 100, -5., 5. ) ;
   fHistPhiDist  = new TH1D(  "HistPhiDist" , "#phi Spectra", 100, -4., 4. ) ;    
   fHistChgDist  = new TH1D(  "HistChgDist" , "N chg particles per event",   125, 0., 250. ) ;    
   fHistChgSimm  = new TH1D(  "HistChgSimm" , "Charge simmetry",   11, -5., 5. ) ;    

   pdN_vs_dphi   = new TProfile("dN_vs_dphi","dN vs dphi",100,-180.,180.,0,100);
   pdPt_vs_dphi  = new TProfile("dPt_vs_dphi","dPt vs dphi",100,-180.,180.,0,100);
   pdN_vs_eta   = new TProfile("dN_vs_eta","dN vs eta",20,0.,5.);
   pdN_vs_pt   = new TProfile("dN_vs_pt","dN vs PT",20,0.,10.);

   temp1 = new TH1D("temp1","temp",100,-180.,180.);
   temp2 = new TH1D("temp2","temp",100,-180.,180.);
   temp3 = new TH1D("temp3","temp",20,0.,5.);
   temp4 = new TH1D("temp4","temp",20,0.,10.);

   fHistPtDistJetChg   = new TH1D(  "HistPtDistJetChg", "Pt fisrt charged jet Spectra", 100,  0., 100. ) ;
   fHistEtaDistJetChg  = new TH1D(  "HistEtaDistJetChg", "#eta fisrt charged jet Spectra", 100, -5., 5. ) ;
   fHistPhiDistJetChg  = new TH1D(  "HistPhiDistJetChg", "#phi fisrt charged jet Spectra", 100, -4., 4. ) ;

   return ;
}


void MinimumBiasAnalyzer::analyze( const Event& e, const EventSetup& ){


  Handle< CandidateCollection > CandHandle ;
  objectAnalyzed == "MCCandidate" ? 
    e.getByLabel( "goodParticles", CandHandle ) :
    e.getByLabel( "goodTracks", CandHandle );
  
  Handle< GenJetCollection > GenJetsHandle ;
  Handle< BasicJetCollection > BasicJetsHandle ;

  if(objectAnalyzed == "MCCandidate" ){ 
    e.getByLabel( "iterativeCone5GenJets", GenJetsHandle );
  }else{
    e.getByLabel( "iterativeCone5BasicJets", BasicJetsHandle );
  }

  std::vector<math::XYZTLorentzVector> particles4Jet;
  particles4Jet.clear();

  float_t piG=acos(-1.);


  if(CandHandle->size()){ 
    
    int nchg = 0;
      
    for(CandidateCollection::const_iterator it = CandHandle->begin();it!=CandHandle->end();it++){
      math::XYZTLorentzVector mom = it->p4();
      fHistPtDist->Fill(it->pt());
      fHistEtaDist->Fill(it->eta());
      fHistPhiDist->Fill(it->phi());
      if(it->charge()!=0){
	temp3->Fill(fabs(mom.eta()));
	temp4->Fill(fabs(mom.Pt()));
      
	if(mom.Pt()>=0.5 && fabs(mom.eta())<1) particles4Jet.push_back(mom); // for UE and MB Observables, particles are selected to be central and with PT>500 MeV  
	
	fHistChgSimm->Fill(it->charge());
	nchg++;
      }
    }
    
    fHistChgDist->Fill(nchg);
    
    for(int i=0;i<20;i++)
      {
	pdN_vs_eta->Fill((i*0.25)+0.125,temp3->GetBinContent(i+1)/0.5,1); // 0.5 normalized to 0.25 bin in abs(eta) (2 times) -> 0.5
	pdN_vs_pt->Fill((i*0.5)+0.25,temp4->GetBinContent(i+1)/0.5,1); // 0.5 normalized to 0.5 bin in pt -> 0.5
      }
    
    temp3->Reset();
    temp4->Reset();
  
  
    std::stable_sort(particles4Jet.begin(),particles4Jet.end(),GreaterPt());
  }


  if(objectAnalyzed=="MCCandidate"){
   
    std::vector<GenJet> tmpGenJet;
    tmpGenJet.clear();

    if(GenJetsHandle->size()){ 
      for(GenJetCollection::const_iterator it=GenJetsHandle->begin();it!=GenJetsHandle->end();it++) 
	tmpGenJet.push_back(*it);
      std::stable_sort(tmpGenJet.begin(),tmpGenJet.end(),GenJetSort());
    }
    
    if(tmpGenJet.size()){
      GenJet pJ = tmpGenJet.front();
      
      fHistPtDistJetChg->Fill(pJ.pt());
      fHistEtaDistJetChg->Fill(pJ.eta());
      fHistPhiDistJetChg->Fill(pJ.phi());
      
      if(fabs(pJ.eta())<1&&pJ.pt()>1){
	
	if(particles4Jet.size()){
	  
	  float_t conv = 180/piG;
	  
	  for (std::vector<math::XYZTLorentzVector>::const_iterator par = particles4Jet.begin();
	       par != particles4Jet.end(); par++ ){
	    
	    float_t tmpphitrk_mc=conv*(par->phi());
	    
	    float_t Dphi_mc=pJ.phi()-tmpphitrk_mc;
	    
	    temp1->Fill(Dphi_mc);
	    temp2->Fill(Dphi_mc,par->Pt());
	    
	  }
	  
	  for(int i=0;i<100;i++){
	    float_t bincont1_mc=temp1->GetBinContent(i+1);
	    pdN_vs_dphi->Fill(-180.+i*3.6+1.8,bincont1_mc/(2.*piG/50.),1);
	    float_t bincont2_mc=temp2->GetBinContent(i+1);
	    pdPt_vs_dphi->Fill(-180.+i*3.6+1.8,bincont2_mc/(2.*piG/50.),1);
	  }
	  
	  temp1->Reset();
	  temp2->Reset();
	}
      }
    }
    
  }else{
    std::vector<BasicJet> tmpBasicJet;
    tmpBasicJet.clear();

    if(BasicJetsHandle->size()){ 
      for(BasicJetCollection::const_iterator it=BasicJetsHandle->begin();it!=BasicJetsHandle->end();it++) 
	tmpBasicJet.push_back(*it);
      std::stable_sort(tmpBasicJet.begin(),tmpBasicJet.end(),BasicJetSort());
    }
    
    
    if(tmpBasicJet.size()){
      BasicJet pJ = tmpBasicJet.front();
      
      fHistPtDistJetChg->Fill(pJ.pt());
      fHistEtaDistJetChg->Fill(pJ.eta());
      fHistPhiDistJetChg->Fill(pJ.phi());
      
      if(fabs(pJ.eta())<1&&pJ.pt()>1){
	
	if(particles4Jet.size()){
	  
	  float_t conv = 180/piG;
	  
	  for (std::vector<math::XYZTLorentzVector>::const_iterator par = particles4Jet.begin();
	       par != particles4Jet.end(); par++ ){
	    
	    float_t tmpphitrk_mc=conv*(par->phi());
	    
	    float_t Dphi_mc=pJ.phi()-tmpphitrk_mc;
	    
	    temp1->Fill(Dphi_mc);
	    temp2->Fill(Dphi_mc,par->Pt());
	    
	  }
	  
	  for(int i=0;i<100;i++){
	    float_t bincont1_mc=temp1->GetBinContent(i+1);
	    pdN_vs_dphi->Fill(-180.+i*3.6+1.8,bincont1_mc/(2.*piG/50.),1);
	    float_t bincont2_mc=temp2->GetBinContent(i+1);
	    pdPt_vs_dphi->Fill(-180.+i*3.6+1.8,bincont2_mc/(2.*piG/50.),1);
	  }
	  
	  temp1->Reset();
	  temp2->Reset();
	}
      }
    }
  }

  return ;
}

void MinimumBiasAnalyzer::endJob(){
  
  fOutputFile->Write() ;
  fOutputFile->Close() ;
  
  return ;
  
}

float_t MinimumBiasAnalyzer::delR(const float_t& eta1,const float_t& eta2,const float_t& phi1,const float_t& phi2)
{

  float cono = sqrt((eta1-eta2)*(eta1-eta2)+(phi1-phi2)*(phi1-phi2));

  return cono;

}

 
DEFINE_FWK_MODULE(MinimumBiasAnalyzer)
