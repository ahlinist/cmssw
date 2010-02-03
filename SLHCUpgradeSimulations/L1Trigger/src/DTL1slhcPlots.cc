// -*- C++ -*-
//
// Package:    DTL1slhcPlots
// Class:      DTL1slhcPlots
// 
/**\class DTL1slhcAnalysis

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Ignazio Lazzizzera
//         Created:  Fri Oct 16 15:44:58 CEST 2009
// $Id: DTL1slhcPlots.cc,v 1.1 2010/02/02 18:44:30 arose Exp $
//
//

#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "SLHCUpgradeSimulations/L1Trigger/interface/DTL1slhcPlots.h"


using namespace std;
using namespace edm;
using namespace reco;


//
// constants, enums and typedefs
//

//
// static data member definitions	
//

//
// constructors and destructor
//
DTL1slhcPlots::DTL1slhcPlots(const edm::ParameterSet& pset)
{
  //  DataSource = pset.getParameter<edm::InputTag>( "Data_Source" );
  RootFileNamePlots = pset.getUntrackedParameter<string>("rootFileNamePlots"); 
  GaugeSampleSize = pset.getUntrackedParameter<size_t>("GaugeSampleSize");
  breath = pset.getUntrackedParameter<int>("breath");
  desert = pset.getUntrackedParameter<double>("desert");
  if(GaugeSampleSize > 3000) GaugeSampleSize = 3000;
  cout << GaugeSampleSize << endl;
  cout << breath << endl;
  cout << desert << endl;
}


DTL1slhcPlots::~DTL1slhcPlots()
{
  
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
  
}


//
// member functions
//


// ------------ method called once each job just before starting event loop  ------------
void DTL1slhcPlots::beginJob(const edm::EventSetup&)
{

  EvCounter = 1;

  OutTFilePlots = new TFile(RootFileNamePlots.c_str(), 
			    "RECREATE", 
			    "analysis I. Lazzizzera");
  
  // to look at multiplicities:
  h_DTTracklets_Pt_size = new  TH1F("DTTracklets_Pt_size", 
				    "DTTracklets_Pt_size", 10, 0, 10);

  // to look at resolutions: IMu_3_V
  profile_PtOverGenPt_vs_bendingDT = 
    new TProfile("profile_PtOverGenPt_vs_bending IMu-3-V", 
		 "Profile of PtOverGenPt versus bendingDT IMu-3-V",
		 100, 0., 0.6, 0.5, 5.);
  h_Pt_IMu_3_V = new TH1F("Pt_IMu_3_V", "Pt IMu_3_V", 100, 0., 120.);
  h_invPt_IMu_3_V = new TH1F("invPt_IMu_3_V", "invPt_IMu_3_V", 100, 0., 0.8);

  // to look at resolutions: Mu_3_V
  h_Pt_Mu_3_V = new TH1F("Pt_Mu_3_V", "Pt Mu_3_V", 100, 0., 120.);
  h_invPt_Mu_3_V = new TH1F("invPt_Mu_3_V", "invPt_Mu_3_V", 100, 0., 0.8);

  // to look at resolutions: 5_3_V
  h_Pt_5_3_V = new TH1F("Pt_5_3_V", "Pt 5_3_V", 100, 0., 120.);
  h_invPt_5_3_V = new TH1F("invPt_5_3_V", "invPt_5_3_V", 100, 0., 0.8);

  // to look at resolutions: only_Mu_V
  h_Pt_only_Mu_V = new TH1F("Pt_only_Mu_V", "Pt only_Mu_V", 100, 0., 120.);
  h_invPt_only_Mu_V = new TH1F("invPt_only_Mu_V", "invPt only_Mu_V", 100, 0., 0.8);
}




// ------------ method called to for each event  ------------
void DTL1slhcPlots::analyze(const edm::Event& event, 
			    const edm::EventSetup& eventSetup)
{
  using namespace edm;

  ++EvCounter;

  double MuGenPt = NAN;
  // ***************************************************
  // Assume there is just a single muon in the event!!!!
  // ***************************************************
  Handle<GenParticleCollection> genParticles;
  event.getByLabel("genParticles", genParticles);
  for(size_t i = 0; i < genParticles->size(); ++ i) {
    const GenParticle & p = (*genParticles)[i];
    int id = p.pdgId();
    // int st = p.status();  
    // const Candidate * mom = p.mother();
    if(abs(id) == 13) {
      MuGenPt = p.pt();  //, eta = p.eta(), phi = p.phi(); // mass = p.mass();
      // double px = p.vx(), py = p.vy(), pz = p.vz();
    }
  }

  /**************************************************************
    here things are done easily for a (gunned) single muon!!!
  ***************************************************************/
  Handle<DTSeededTrackletsCollection> trackletsHandle;
  try {
    event.getByLabel("DTL1slhcProd", trackletsHandle);
  }
  catch(...) {
    cout << "\nException from event.getByLabel(\"DTL1slhcProd\", trackletsHandle)"
	 << endl;
    return;
  }
  if(!trackletsHandle.isValid()) return;

  std::vector<DTSeededTracklet*> DTSeededTrackletVector = trackletsHandle->theCollection();
  h_DTTracklets_Pt_size->Fill(DTSeededTrackletVector.size());

  for(size_t j=0; j<DTSeededTrackletVector.size(); j++) 
    {

      float MuPt = 
	(DTSeededTrackletVector[j])->Pt(string("IMu-3-V"));
      float bendingDT =  
	(((DTSeededTrackletVector[j])->theDTTracklet())[0])->bendingDT();
      if( !isnan(MuGenPt) && !isnan(MuPt) && !isnan(bendingDT) ) {
	float r = MuPt/MuGenPt;
	profile_PtOverGenPt_vs_bendingDT->Fill(bendingDT, r);
      }

      float invMuPt = NAN;

      // resolution IMu_3_V
      MuPt = (DTSeededTrackletVector[j])->Pt(string("IMu-3-V"));
      if( !isnan(MuPt) ) {
	h_Pt_IMu_3_V->Fill(MuPt);
	invMuPt = 1/MuPt;
	h_invPt_IMu_3_V->Fill(invMuPt); 
	if(EvCounter <= GaugeSampleSize) {
	  Pt_IMu_3_V[EvCounter-1] = MuPt;
	  invPt_IMu_3_V[EvCounter-1] = invMuPt;
	  if(EvCounter == GaugeSampleSize) {
	    getOptimalRange(h_Pt_IMu_3_V, Pt_IMu_3_V);
	    getOptimalRange(h_invPt_IMu_3_V, invPt_IMu_3_V);
	  }
	}
      }

      // resolution Mu_3_V
      MuPt = (DTSeededTrackletVector[j])->Pt(string("Mu-3-V"));
      if( !isnan(MuPt) ) {
	h_Pt_Mu_3_V->Fill(MuPt); 
	invMuPt = 1/MuPt;
	h_invPt_Mu_3_V->Fill(invMuPt); 
	if(EvCounter <= GaugeSampleSize) {
	  Pt_Mu_3_V[EvCounter-1] = MuPt;
	  invPt_Mu_3_V[EvCounter-1] = invMuPt;
	  if(EvCounter == GaugeSampleSize) {
	    getOptimalRange(h_Pt_Mu_3_V, Pt_Mu_3_V);
	    getOptimalRange(h_invPt_Mu_3_V, invPt_Mu_3_V);
	  }
	}
      }

      // resolution Stubs_5_3_V
      MuPt = (DTSeededTrackletVector[j])->Pt(string("Stubs-5-3-V"));
      if( !isnan(MuPt) ) {
	h_Pt_5_3_V->Fill(MuPt); 
	invMuPt = 1/MuPt;
	h_invPt_5_3_V->Fill(invMuPt); 
	if(EvCounter <= GaugeSampleSize) {
	  Pt_5_3_V[EvCounter-1] = MuPt;
	  invPt_5_3_V[EvCounter-1] = invMuPt;
	  if(EvCounter == GaugeSampleSize) {
	    getOptimalRange(h_Pt_5_3_V, Pt_5_3_V);
	    getOptimalRange(h_invPt_5_3_V, invPt_5_3_V);
	  }
	}
      }
      // resolution only_Mu_V
      MuPt = (DTSeededTrackletVector[j])->Pt(string("only-Mu-V"));
      if( !isnan(MuPt) ) {
	h_Pt_only_Mu_V->Fill(MuPt); 
	invMuPt = 1/MuPt;
	h_invPt_only_Mu_V->Fill(invMuPt); 
	if(EvCounter <= GaugeSampleSize) {
	  Pt_only_Mu_V[EvCounter-1] = MuPt;
	  invPt_only_Mu_V[EvCounter-1] = invMuPt;
	  if(EvCounter == GaugeSampleSize) {
	    getOptimalRange(h_Pt_only_Mu_V, Pt_only_Mu_V);
	    getOptimalRange(h_invPt_only_Mu_V, invPt_only_Mu_V);
	  }
	}
      }

    } // end of loop over DTSeededTrackletVector

  /* 
     #ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
     ESHandle<SetupData> pSetup;
     setup.get<SetupRecord>().get(pSetup);
     #endif
  */
}




// ------------ method called once each job just after ending the event loop  ------------
void DTL1slhcPlots::endJob() {

  OutTFilePlots->cd();
  Int_t w = 600;
  Int_t h = 480;

  // Global style settings
  gStyle->SetFrameFillColor(kWhite);
  gStyle->SetHistLineWidth(4);
  gStyle->SetHistLineColor(kBlue);
  gStyle->SetFuncWidth(3);         // for the fit function 
  gStyle->SetFuncColor(kRed);      // for the fit function 
  gStyle->SetOptFit(111);
  gStyle->SetStatW(0.18);          // set width of stat box
  gStyle->SetStatFontSize(0.04);   // set the font size for stat box


  TCanvas* C0 = new TCanvas("theSizes", "theSizes", w, h);
  C0->cd();
  h_DTTracklets_Pt_size->SetLineWidth(2);
  //  C0->GetFrame()->SetFillColor(21);
  //  C0->GetFrame()->SetBorderSize(6);
  //  C0->GetFrame()->SetBorderMode(-1);
  //h_DTTracklets_Pt_size->SetLineColor(kBlue);
  h_DTTracklets_Pt_size->Draw(); 
  C0->Write();
  delete C0;

  TCanvas* C00 = new TCanvas("profile PtOverGenPt IMu_3_V vs bendingDT", 
			     "profile PtOverGenPt IMu_3_V vs bendingDT", w, h);
  C00->cd();
  profile_PtOverGenPt_vs_bendingDT->SetLineWidth(4);
  profile_PtOverGenPt_vs_bendingDT->SetLineColor(kBlue);
  profile_PtOverGenPt_vs_bendingDT->Draw();
  C00->Write();
  delete C00;

  TCanvas* C1 = new TCanvas("invPt_only_Mu_V", "invPt_only_Mu_V", w, h);
  C1->cd();
  h_invPt_only_Mu_V->SetLineWidth(4);
  h_invPt_only_Mu_V->SetLineColor(kBlue);
  h_invPt_only_Mu_V->Fit("gaus", "Q");
  h_invPt_only_Mu_V->Draw();
  gPad->Update();
  C1->Write();
  delete C1;

  TCanvas* C2 = new TCanvas("Pt_only_Mu_V", "Pt_only_Mu_V", w, h);
  C2->cd();
  h_Pt_only_Mu_V->SetLineWidth(4);
  h_Pt_only_Mu_V->SetLineColor(kBlue);
  h_Pt_only_Mu_V->Fit("gaus", "Q");
  h_Pt_only_Mu_V->Draw();
  gPad->Update();
  C2->Write();
  delete C2;

  TCanvas* C3 = new TCanvas("Pt_IMu_3_V", "Pt_IMu_3_V", w, h);
  C3->cd();
  h_Pt_IMu_3_V->SetLineWidth(4);
  h_Pt_IMu_3_V->SetLineColor(kBlue);
  h_Pt_IMu_3_V->Fit("gaus", "Q");
  h_Pt_IMu_3_V->Draw();
  gPad->Update();
  C3->Write();
  delete C3;

  TCanvas* C4 = new TCanvas("invPt_IMu_3_V", "invPt_IMu_3_V", w, h);
  C4->cd();
  h_invPt_IMu_3_V->SetLineWidth(4);
  h_invPt_IMu_3_V->SetLineColor(kBlue);
  h_invPt_IMu_3_V->Fit("gaus", "Q");
  h_invPt_IMu_3_V->Draw();
  gPad->Update();
  C4->Write();
  delete C4;


  TCanvas* C5 = new TCanvas("Pt_Mu_3_V", "Pt_Mu_3_V", w, h);
  C5->cd();
  h_Pt_Mu_3_V->SetLineWidth(4);
  h_Pt_Mu_3_V->SetLineColor(kBlue);
  h_Pt_Mu_3_V->Fit("gaus", "Q");
  h_Pt_Mu_3_V->Draw();
  gPad->Update();
  C5->Write();
  delete C5;

  TCanvas* C6 = new TCanvas("invPt_Mu_3_V", "invPt_Mu_3_V", w, h);
  C6->cd();
  h_invPt_Mu_3_V->SetLineWidth(4);
  h_invPt_Mu_3_V->SetLineColor(kBlue);
  h_invPt_Mu_3_V->Fit("gaus", "Q");
  h_invPt_Mu_3_V->Draw();
  gPad->Update();
  C6->Write();
  delete C6;

  TCanvas* C7 = new TCanvas("Pt_AllStubs_5_3_V", "Pt_AllStubs_5_3_V", w, h);
  C7->cd();
  h_Pt_5_3_V->SetLineWidth(4);
  h_Pt_5_3_V->SetLineColor(kBlue);
  h_Pt_5_3_V->Fit("gaus", "Q");
  h_Pt_5_3_V->Draw();
  gPad->Update();
  C7->Write();
  delete C7;

  TCanvas* C8 = new TCanvas("invPt_AllStubs_5_3_V", "invPt_AllStubs_5_3_V", w, h);
  C8->cd();
  h_invPt_5_3_V->SetLineWidth(4);
  h_invPt_5_3_V->SetLineColor(kBlue);
  h_invPt_5_3_V->Fit("gaus", "Q");
  h_invPt_5_3_V->Draw();
  gPad->Update();
  C8->Write();
  delete C8;
  

  OutTFilePlots->Close();
  delete OutTFilePlots;

}




//------------------------------------------------------------------------------
void DTL1slhcPlots::getOptimalRange(TH1* h, float entries[])
{

  double Min, Max;
  double rawMin	= h->GetBinLowEdge(1); 
  size_t NBins = static_cast<size_t>(h->GetNbinsX());
  double rawMax = h->GetBinLowEdge(NBins+1);
  Double_t nentries = h->GetEntries();
  double w = (rawMax - rawMin)/NBins; // gives the same as 
  double d = 0.;
  double rd = 0.;
  //  int breath = 5;
  //  double desert = 0.0005;
  int j = 1;
  while(rd < desert)
    {
      d += static_cast<double>(h->GetBinContent(j));
      rd = d/nentries;
      ++j;
    }
  double val = rawMin + (j-breath)*w;
  Min = (val < rawMin)? rawMin: val;
  d = 0.;
  rd = 0.;
  j = 0;
  while(rd < desert)
    {
      d += static_cast<double>(h->GetBinContent(101-j));
      rd = d/nentries;
      ++j;
    }
  val = rawMax + 0.00001 - (j-breath)*w;
  Max = (val > rawMax)? rawMax: val;
  size_t a = breath;
  while(Max <= Min)
    {
      ++a;
      Min = rawMin + (j-a)*w;
      Max = rawMax + 0.00001 - (j-a)*w;
    }
  h->Reset("M");
  h->SetBins(100, Min, Max);
  h->SetEntries(0);
  return;
  for(size_t i=0; i<GaugeSampleSize; i++)
    h->Fill(entries[i]);
}



//define this as a plug-in
//DEFINE_FWK_MODULE(DTL1slhcPlots);

