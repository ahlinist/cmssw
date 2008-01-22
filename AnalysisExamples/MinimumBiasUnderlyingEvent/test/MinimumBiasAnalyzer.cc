// Authors: F. Ambroglini, L. Fano'


#include <iostream>

#include "AnalysisExamples/MinimumBiasUnderlyingEvent/test/MinimumBiasAnalyzer.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/BasicJet.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
 
#include "TH1.h"
#include "TVector3.h"
 
using namespace edm;
using namespace std;
using namespace reco;

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
     objectAnalyzed( pset.getUntrackedParameter<string>("UsedCandidate",std::string("MCCandidate")) )
     /*     fOutputFile(0), fHistPtDist(0), fHistEtaDist(0), fHistPhiDist(0), fHistChgDist(0), 
     fHistChgSimm(0),pdN_vs_dphi(0),pdPt_vs_dphi(0), temp1(0), temp2(0), fHistPtDistJetChg(0),
     fHistEtaDistJetChg(0), fHistPhiDistJetChg(0)*/
{
}

void MinimumBiasAnalyzer::beginJob( const EventSetup& )
{
 
  hFile = new TFile ( "trackcluster.root", "RECREATE" );
  nt = new TTree("nt","a Tree");

  nt->Branch("Ev_kind",&Ev_kind,"Ev_kind/I");

  nt->Branch("Np",&Np,"Np/I");
  nt->Branch("p_",p_,"p_[Np]/F");
  nt->Branch("pt_",pt_,"pt_[Np]/F");
  nt->Branch("eta_",eta_,"eta_[Np]/F");
  nt->Branch("phi_",phi_,"phi_[Np]/F");
  nt->Branch("pid_",phi_,"pid_[Np]/I");
  nt->Branch("charge_",charge_,"charge_[Np]/I");

  nt->Branch("Ntk",&Ntk,"Ntk/I");
  nt->Branch("p_tk",p_tk,"p_tk[Ntk]/F");
  nt->Branch("pt_tk",pt_tk,"pt_tk[Ntk]/F");
  nt->Branch("eta_tk",eta_tk,"eta_tk[Ntk]/F");
  nt->Branch("phi_tk",phi_tk,"phi_tk[Ntk]/F");
  nt->Branch("nhits_tk",nhits_tk,"nhits_tk[Ntk]/I");
  

  // general purpose

  dN_vs_eta = new TProfile("dN_vs_eta","dN vs eta",100,0.,5.);
  dN_vs_pt = new TProfile("dN_vs_pt","dN vs PT",100,0.,4.5);

  dN_vs_etaMC = new TProfile("dN_vs_etaMC","dN vs eta",100,0.,5.);
  dN_vs_ptMC = new TProfile("dN_vs_ptMC","dN vs PT",100,0.,4.5);

  dN_vs_dphiMC = new TProfile("dN_vs_dphiMC","#delta N vs #delta #phi",100,-180.,180.,0,100);
  dPT_vs_dphiMC = new TProfile("dPT_vs_dphiMC","#delta PTsum vs #delta #phi",100,-180.,180.,0,100);

  dN_vs_dphi = new TProfile("dN_vs_dphi","#delta N vs #delta #phi",100,-180.,180.,0,100);
  dPT_vs_dphi = new TProfile("dPT_vs_dphi","#delta PTsum vs #delta #phi",100,-180.,180.,0,100);



  temp1 = new TH1D("temp1","temp",100,0.,5.);
  temp2 = new TH1D("temp2","temp",100,0.,4.5);

  temp1MC = new TH1D("temp1MC","temp",100,0.,5.);
  temp2MC = new TH1D("temp2MC","temp",100,0.,4.5);

  temp_dnMC = new TH1D("temp_dnMC","temp",100,-180.,180.);
  temp_dptMC = new TH1D("temp_dptMC","temp",100,-180.,180.);

  temp_dn = new TH1D("temp_dn","temp",100,-180.,180.);
  temp_dpt = new TH1D("temp_dpt","temp",100,-180.,180.);

}


void MinimumBiasAnalyzer::analyze( const Event& e, const EventSetup& )
{

  float piG = acos(-1.);


  Handle<HepMCProduct> MCEvt;
  e.getByType(MCEvt);
  HepMC::GenEvent * generated_event = new HepMC::GenEvent(*(MCEvt->GetEvent()));
  HepMC::GenEvent::particle_iterator p;

  edm::Handle<reco::TrackCollection> trackHandle;
  e.getByLabel("selectTracks", trackHandle);
  reco::TrackCollection theTracks = *(trackHandle.product());

  Ev_kind =generated_event->signal_process_id();

  Np=0;
  float tmp_=0;
  int idx_tmp_=-99;

  for (p = generated_event->particles_begin(); p != generated_event->particles_end(); p++)
    { 

      if((*p)->status()==1)
	{

	  charge_[Np]=charge((*p)->pdg_id());
	  pid_[Np]=(*p)->pdg_id();
	  p_[Np]=sqrt((*p)->momentum().px()*(*p)->momentum().px()+(*p)->momentum().py()*(*p)->momentum().py()+(*p)->momentum().pz()*(*p)->momentum().pz());
	  pt_[Np]= (*p)->momentum().perp();
	  eta_[Np]= (*p)->momentum().eta();
	  phi_[Np]= (*p)->momentum().phi();
	  if(abs(eta_[Np])<2. && pt_[Np]>0.9 && charge_[Np]!=0)
	    {
	      temp1MC->Fill(abs(eta_[Np]));
	      temp2MC->Fill(pt_[Np]);
	      
	      // find the leading particle
	      if(pt_[Np]>tmp_)
		{
		  tmp_=pt_[Np];
		  idx_tmp_=Np;
		}
	    }
	  Np++;
	}  
    }

  lead_tk=idx_tmp_;


  Ntk=0;
  tmp_=0;
  idx_tmp_=-99;  

  for (std::vector<reco::Track>::const_iterator itTk = theTracks.begin(); itTk != theTracks.end(); itTk++) 
    {
      p_tk[Ntk]=itTk->outerP();
      pt_tk[Ntk]=itTk->outerPt();
      eta_tk[Ntk]=itTk->outerEta();
      phi_tk[Ntk]=itTk->outerPhi();
      nhits_tk[Ntk]=itTk->found();

      if(nhits_tk[Ntk] < 8)
	std::cout<<"numero hit = "<<nhits_tk[Ntk]<<std::endl;

      if(abs(eta_tk[Ntk])<2. && pt_tk[Ntk]>0.9 && nhits_tk[Ntk]>7)
	{
	  temp1->Fill(abs(eta_tk[Ntk]));
	  temp2->Fill(pt_tk[Ntk]);
	  if(pt_tk[Ntk]>tmp_)
	    {
	      tmp_=pt_tk[Ntk];
	      idx_tmp_=Ntk;
	    }
	}

      Ntk++;
    }

  lead_MC=idx_tmp_;



  // attivita' UE usando la leading track come scala di energia dell'evento
  //  faccio separatamente MC e REC (secondo me piu' giusto che richiedere
  // i 2 indici, contemporaneamente, diversi da -99
  if(lead_MC!=-99)
    {
      for(int i=0;i<Np;i++)
	{
	  if(i!=lead_MC && abs(eta_[i])<2. && pt_[i]>0.9 && charge_[i]!=0)
	    {
    // 3-vector  
	      TVector3 * leadvector = new TVector3;
	      leadvector->SetPtEtaPhi(pt_[lead_MC], eta_[lead_MC], phi_[lead_MC]);

	      TVector3 * particlevector = new TVector3;
	      particlevector->SetPtEtaPhi(pt_[i], eta_[i], phi_[i]);

	      float_t d_phi = (180./piG) * leadvector->DeltaPhi(*particlevector);

	      temp_dnMC->Fill(d_phi);
	      temp_dptMC->Fill(d_phi,pt_[i]);
	    }
	}
    }

  if(lead_tk!=-99)
    {
      for(int i=0;i<Ntk;i++)
	{
	  if(i!=lead_tk && abs(eta_tk[i])<2. && pt_tk[i]>0.9  && nhits_tk[i]>7)
	    {
	      // 3-vector  
	      TVector3 * leadtrack = new TVector3;
	      leadtrack->SetPtEtaPhi(pt_tk[lead_tk], eta_tk[lead_tk], phi_tk[lead_tk]);

	      TVector3 * trackvector = new TVector3;
	      trackvector->SetPtEtaPhi(pt_tk[i], eta_tk[i], phi_tk[i]);

	      float_t d_phi = (180./(piG*1.)) * leadtrack->DeltaPhi(*trackvector);


	      temp_dn->Fill(d_phi);
	      temp_dpt->Fill(d_phi,pt_tk[i]);



	    }
	}
    }
  








  // grandezze differenziali base

  for(int i=0;i<100;i++)
    {
      dN_vs_eta->Fill((i*0.05)+0.025,temp1->GetBinContent(i+1)/0.1,1);
      dN_vs_pt->Fill((i*0.045)+0.0225,temp2->GetBinContent(i+1)/0.045,1);
      dN_vs_etaMC->Fill((i*0.05)+0.025,temp1MC->GetBinContent(i+1)/0.1,1);
      dN_vs_ptMC->Fill((i*0.045)+0.0225,temp2MC->GetBinContent(i+1)/0.045,1);

      dN_vs_dphiMC->Fill((-180+i*3.6+1.8), temp_dnMC->GetBinContent(i+1)/(3.6*2*(piG/180.)));
      dPT_vs_dphiMC->Fill((-180+i*3.6+1.8), temp_dptMC->GetBinContent(i+1)/(3.6*2*(piG/180.)));

      dN_vs_dphi->Fill((-180+i*3.6+1.8), temp_dn->GetBinContent(i+1)/(3.6*2*(piG/180.)));
      dPT_vs_dphi->Fill((-180+i*3.6+1.8), temp_dpt->GetBinContent(i+1)/(3.6*2*(piG/180.)));
    }

  temp1->Reset();
  temp2->Reset(); 
  temp1MC->Reset();
  temp2MC->Reset();
  temp_dnMC->Reset();
  temp_dptMC->Reset();
  temp_dn->Reset();
  temp_dpt->Reset();

  nt->Fill();
}

void MinimumBiasAnalyzer::endJob(){
  

    nt->Write();
    hFile->Write();
    hFile->Close();

  
}

float_t MinimumBiasAnalyzer::delR(const float_t& eta1,const float_t& eta2,const float_t& phi1,const float_t& phi2)
{

  float cono = sqrt((eta1-eta2)*(eta1-eta2)+(phi1-phi2)*(phi1-phi2));

  return cono;

}

int MinimumBiasAnalyzer::charge(const int& Id){
  
  //...Purpose: to give three times the charge for a particle/parton.

  //      ID     = particle ID
  //      hepchg = particle charge times 3

  int kqa,kq1,kq2,kq3,kqj,irt,kqx,kqn;
  int hepchg;


  int ichg[109]={-1,2,-1,2,-1,2,-1,2,0,0,-3,0,-3,0,-3,0,
		 -3,0,0,0,0,0,0,3,0,0,0,0,0,0,3,0,3,6,0,0,3,6,0,0,-1,2,-1,2,-1,2,0,0,0,0,
		 -3,0,-3,0,-3,0,0,0,0,0,-1,2,-1,2,-1,2,0,0,0,0,
		 -3,0,-3,0,-3,0,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


  //...Initial values. Simple case of direct readout.
  hepchg=0;
  kqa=abs(Id);
  kqn=kqa/1000000000%10;
  kqx=kqa/1000000%10;
  kq3=kqa/1000%10;
  kq2=kqa/100%10;
  kq1=kqa/10%10;
  kqj=kqa%10;
  irt=kqa%10000;

  //...illegal or ion
  //...set ion charge to zero - not enough information
  if(kqa==0 || kqa >= 10000000) {

    if(kqn==1) {hepchg=0;}
  }
  //... direct translation
  else if(kqa<=100) {hepchg = ichg[kqa-1];}
  //... KS and KL (and undefined)
  else if(kqj == 0) {hepchg = 0;}
  //C... direct translation
  else if(kqx>0 && irt<100)
    {
      hepchg = ichg[irt-1];
      if(kqa==1000017 || kqa==1000018) {hepchg = 0;}
      if(kqa==1000034 || kqa==1000052) {hepchg = 0;}
      if(kqa==1000053 || kqa==1000054) {hepchg = 0;}
      if(kqa==5100061 || kqa==5100062) {hepchg = 6;}
    }
  //...Construction from quark content for heavy meson, diquark, baryon.
  //...Mesons.
  else if(kq3==0)
    {
      hepchg = ichg[kq2-1]-ichg[kq1-1];
      //...Strange or beauty mesons.
      if((kq2==3) || (kq2==5)) {hepchg = ichg[kq1-1]-ichg[kq2-1];}
    }
  else if(kq1 == 0) {
    //...Diquarks.
    hepchg = ichg[kq3-1] + ichg[kq2-1];
  }

  else{
    //...Baryons
    hepchg = ichg[kq3-1]+ichg[kq2-1]+ichg[kq1-1];
  }

  //... fix sign of charge
  if(Id<0 && hepchg!=0) {hepchg = -1*hepchg;}

  // cout << hepchg<< endl;
  return hepchg;


}

 
DEFINE_FWK_MODULE(MinimumBiasAnalyzer);
