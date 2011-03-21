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
// $Id: DTL1slhcPlots.cc,v 1.1 2010/03/03 13:09:38 arose Exp $
//
//

#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "SLHCUpgradeSimulations/L1DTTrigger/interface/DTL1slhcPlots.h"

#include "TImage.h"
#include "SLHCUpgradeSimulations/L1DTTrigger/interface/RootStyles.h"


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
DTL1slhcPlots::DTL1slhcPlots(const edm::ParameterSet& pset):
  pSet(pset)
{
  //  DataSource = pset.getParameter<edm::InputTag>( "Data_Source" );
  RootFileNamePlots = pset.getUntrackedParameter<string>("rootFileNamePlots"); 
  optimize_plot = pset.getUntrackedParameter<bool>("optimize_plot", false);
  RangeCalibrationSampleSize = 
    pset.getUntrackedParameter<size_t>("RangeCalibrationSampleSize", 1000);
  breath = pset.getUntrackedParameter<int>("breath");
  desert = pset.getUntrackedParameter<double>("desert");
  if(RangeCalibrationSampleSize > 3000) RangeCalibrationSampleSize = 3000;

  EvNo = 0;
  AtLeastOneDTStaubMatchingMuonEvNo = 0;
}


DTL1slhcPlots::~DTL1slhcPlots() {}


//
// member functions
//


// ------------ method called once each job just before starting event loop  ----
void DTL1slhcPlots::beginJob(const edm::EventSetup& eventSetup)
{
  ESHandle<MagneticField> magneticField_handle;
  eventSetup.get<IdealMagneticFieldRecord>().get(magneticField_handle);
  if(magneticField_handle.isValid()) 
    theField = &(*magneticField_handle);

  //Initializer for GUI: needed for interactive interface, in case.
  extern void InitGui(); 
  VoidFuncPtr_t initfuncs[] = {InitGui, 0};
  //Initialize a ROOT instance.
  TROOT root("DTL1slhcROOT","A ROOT session", initfuncs);   
  static TApplication theTAppl("theTAppl", NULL, 0);
  TApplication::NeedGraphicsLibs();
  gApplication->InitializeGraphics();
 
  IStyle = new TStyle("IStyle","Ignazio Style"); 
  setIStyle();

  OutTFilePlots = new TFile(RootFileNamePlots.c_str(), 
			    "RECREATE", 
			    "analysis I. Lazzizzera");

  // to look at multiplicities:
  h_DTTracklets_Pt_size = new  TH1F("DTTracklets_Pt_size", 
				    "DTTracklets_Pt_size", 10, 0, 10);

  // dephi algorithm ------------------------------------------------------------
  Pt_dephiL0L2 = new  TH1F("Pt_dephiL0L2", "Pt_dephiL0L2", 100, 0., 200.);
  dephiL0L2 = new  TH1F("dephiL0L2", "dephiL0L2", 100, 0., 0.05);
  profile_dephiL0L2_vs_invGenPt = new TProfile("profile_dephiL0L2_vs_invGenPt", 
					     "profile_dephiL0L2_vs_invGenPt",
					     100, 0., 0.12, 0., 0.1, "S");
  Pt_dephiL0L3 = new  TH1F("Pt_dephiL0L3", "Pt_dephiL0L3", 100, 0., 200.);
  dephiL0L3 = new  TH1F("dephiL0L3", "dephiL0L3", 100, 0., 0.05);
  profile_dephiL0L3_vs_invGenPt = new TProfile("profile_dephiL0L3_vs_invGenPt", 
					       "profile_dephiL0L3_vs_invGenPt",
					       100, 0., 0.12, 0., 0.1, "S");
  Pt_dephiL1L2 = new  TH1F("Pt_dephiL1L2", "Pt_dephiL1L2", 100, 0., 200.);
  dephiL1L2 = new  TH1F("dephiL1L2", "dephiL1L2", 100, 0., 0.05);
  profile_dephiL1L2_vs_invGenPt = new TProfile("profile_dephiL1L2_vs_invGenPt", 
					     "profile_dephiL1L2_vs_invGenPt",
					     100, 0., 0.12, 0., 0.1, "S");
  Pt_dephiL1L3 = new  TH1F("Pt_dephiL1L3", "Pt_dephiL1L3", 100, 0., 200.);
  dephiL1L3 = new  TH1F("dephiL1L3", "dephiL1L3", 100, 0., 0.05);
  profile_dephiL1L3_vs_invGenPt = new TProfile("profile_dephiL1L3_vs_invGenPt", 
					       "profile_dephiL1L3_vs_invGenPt",
					       100, 0., 0.12, 0., 0.1, "S");
  Pt_dephiL0L8 = new  TH1F("Pt_dephiL0L8", "Pt_dephiL0L8", 100, 0., 200.);
  dephiL0L8 = new  TH1F("dephiL0L8", "dephiL0L8", 100, 0., 0.05);
  profile_dephiL0L8_vs_invGenPt = new TProfile("profile_dephiL0L8_vs_invGenPt", 
					     "profile_dephiL0L8_vs_invGenPt",
					     100, 0., 0.12, 0., 0.1, "S");
  Pt_dephiL0L9 = new  TH1F("Pt_dephiL0L9", "Pt_dephiL0L9", 100, 0., 200.);
  dephiL0L9 = new  TH1F("dephiL0L9", "dephiL0L9", 100, 0., 0.05);
  profile_dephiL0L9_vs_invGenPt = new TProfile("profile_dephiL0L9_vs_invGenPt", 
					       "profile_dephiL0L9_vs_invGenPt",
					       100, 0., 0.12, 0., 0.1, "S");
  Pt_dephiL1L8 = new  TH1F("Pt_dephiL1L8", "Pt_dephiL1L8", 100, 0., 200.);
  dephiL1L8 = new  TH1F("dephiL1L8", "dephiL1L8", 100, 0., 0.05);
  profile_dephiL1L8_vs_invGenPt = new TProfile("profile_dephiL1L8_vs_invGenPt", 
					     "profile_dephiL1L8_vs_invGenPt",
					     100, 0., 0.12, 0., 0.1);
  Pt_dephiL1L9 = new  TH1F("Pt_dephiL1L9", "Pt_dephiL1L9", 100, 0., 200.);
  dephiL1L9 = new  TH1F("dephiL1L9", "dephiL1L9", 100, 0., 0.05);
  profile_dephiL1L9_vs_invGenPt = new TProfile("profile_dephiL1L9_vs_invGenPt", 
					       "profile_dephiL1L9_vs_invGenPt",
					       100, 0., 0.12, 0., 0.1, "S");
  Pt_dephiL2L8 = new  TH1F("Pt_dephiL2L8", "Pt_dephiL2L8", 100, 0., 200.);
  dephiL2L8 = new  TH1F("dephiL2L8", "dephiL2L8", 100, 0., 0.05);
  profile_dephiL2L8_vs_invGenPt = new TProfile("profile_dephiL2L8_vs_invGenPt", 
					     "profile_dephiL2L8_vs_invGenPt",
					     100, 0., 0.12, 0., 0.1, "S");
  Pt_dephiL2L9 = new  TH1F("Pt_dephiL2L9", "Pt_dephiL2L9", 100, 0., 200.);
  dephiL2L9 = new  TH1F("dephiL2L9", "dephiL2L9", 100, 0., 0.05);
  profile_dephiL2L9_vs_invGenPt = new TProfile("profile_dephiL2L9_vs_invGenPt", 
					       "profile_dephiL2L9_vs_invGenPt",
					       100, 0., 0.12, 0., 0.1);
  Pt_dephiL3L8 = new  TH1F("Pt_dephiL3L8", "Pt_dephiL3L8", 100, 0., 200.);
  dephiL3L8 = new  TH1F("dephiL3L8", "dephiL3L8", 100, 0., 0.05);
  profile_dephiL3L8_vs_invGenPt = new TProfile("profile_dephiL3L8_vs_invGenPt", 
					     "profile_dephiL3L8_vs_invGenPt",
					     100, 0., 0.12, 0., 0.1, "S");
  Pt_dephiL3L9 = new  TH1F("Pt_dephiL3L9", "Pt_dephiL3L9", 100, 0., 200.);
  dephiL3L9 = new  TH1F("dephiL3L9", "dephiL3L9", 100, 0., 0.05);
  profile_dephiL3L9_vs_invGenPt = new TProfile("profile_dephiL3L9_vs_invGenPt", 
					       "profile_dephiL3L9_vs_invGenPt",
					       100, 0., 0.12, 0., 0.1, "S");

  // three point algorithm ------------------------------------------------------

  h_Pt_IMu_3_V = new TH1F("Pt_IMu_3_V", "Pt IMu_3_V", 100, 0., 120.);
  h_invPt_IMu_3_V = new TH1F("invPt_IMu_3_V", "invPt_IMu_3_V", 100, 0., 0.25);

  h_Pt_IMu_3_0_St1 = new TH1F("Pt_IMu_3_0_St1", "Pt IMu_3_0_St1", 100, 0., 120.);
  h_Pt_IMu_3_0_St2 = new TH1F("Pt_IMu_3_0_St2", "Pt IMu_3_0_St2", 100, 0., 120.);
  h_Pt_IMu_3_0  = new TH1F("Pt_IMu_3_0", "Pt IMu_3_0", 100, 0., 120.);
  h_invPt_IMu_3_0 = new TH1F("invPt_IMu_3_0", "invPt_IMu_3_0", 100, 0., 0.25);

  h_Pt_Mu_3_0_St1 = new TH1F("Pt_Mu_3_0_St1", "Pt Mu_3_0_St1", 100, 0., 120.);
  h_Pt_Mu_3_0_St2 = new TH1F("Pt_Mu_3_0_St2", "Pt Mu_3_0_St2", 100, 0., 120.);
  h_Pt_Mu_3_0 = new TH1F("Pt_Mu_3_0", "Pt Mu_3_0", 100, 0., 120.);
  h_invPt_Mu_3_0 = new TH1F("invPt_Mu_3_0", "invPt_Mu_3_0", 100, 0., 0.25);

  h_Pt_Stubs_9_3_0 = new TH1F("Pt_Stubs_9_3_0", "Pt Stubs_9_3_0", 100, 0., 120.);
  h_invPt_Stubs_9_3_0 = new TH1F("invPt_Stubs_9_3_0", "invPt_Stubs_9_3_0", 100, 0., 0.25);

  h_Pt_Stubs_9_3_V = new TH1F("Pt_Stubs_9_3_V", "Pt Stubs_9_3_V", 100, 0., 120.);
  h_invPt_Stubs_9_3_V = new TH1F("invPt_Stubs_9_3_V", "invPt_Stubs_9_3_V", 100, 0., 0.25);


  // lin-stubs algorithm --------------------------------------------------------

  h_Pt_LinStubs_9_3_0 = 
    new TH1F("Pt_LinStubs_9_3_0", "Pt LinStubs_9_3_0", 100, 0., 120.);
  h_invPt_LinStubs_9_3_0 = 
    new TH1F("invPt_LinStubs_9_3_0", "invPt_LinStubs_9_3_0", 100, 0., 0.25);
  h_deltaAlpha0_LinStubs_9_3_0 = 
    new  TH1F("deltaAlpha0_LinStubs_9_3_0", 
	      "deltaAlpha0_LinStubs_9_3_0", 100, -0.015, 0.015);
  h_deltad_LinStubs_9_3_0 = 
    new  TH1F("ded_LinStubs_9_3_0", "ded_LinStubs_9_3_0", 100, -0.1, 0.1);

  h_Pt_LinStubs_9_1_0 = 
    new TH1F("Pt_LinStubs_9_1_0", "Pt LinStubs_9_1_0", 100, 0., 120.);
  h_invPt_LinStubs_9_1_0 = 
    new TH1F("invPt_LinStubs_9_1_0", "invPt_LinStubs_9_1_0", 100, 0., 0.25);
  h_deltaAlpha0_LinStubs_9_1_0 = 
    new  TH1F("deltaAlpha0_LinStubs_9_1_0", 
	      "deltaAlpha0_LinStubs_9_1_0", 100, -0.02, 0.02);
  h_deltad_LinStubs_9_1_0 = 
    new  TH1F("ded_LinStubs_9_1_0", "ded_LinStubs_9_1_0", 100, -0.1, 0.1);


  //-----------------------------------------------------------------------------
  profile_RTilde_vs_genPt_st1 = new TProfile("profile_RTilde_vs_genPt_st1", 
					     "profile RTilde vs genPt, st1",
					     100, 0., 180., 200., 400., "S");
  profile_RTilde_vs_genPt_st2 = new TProfile("profile_RTilde_vs_genPt_st2", 
					     "profile RTilde vs genPt, st2",
					     100, 0., 180., 200., 400., "S");

  scatter_RTilde_vs_genPt_st1 = new TH2F("scatter_RTilde_vs_genPt_st1", 
					 "scatter RTilde vs genPt, st1",
					 100, 0., 180., 100, 0., 600.);
  scatter_RTilde_vs_genPt_st2 = new TH2F("scatter_RTilde_vs_genPt_st2", 
					 "scatter RTilde vs genPt, st2",
					 100, 0., 180., 100, 0., 600.);

  profile_RTilde_vs_TkLayer_genPt10_st1 
    = new TProfile("profile_RTilde_vs_TkLayer_genPt10_st1", 
		   "profile RTilde vs TkLayer, genPt10GeV, st1",
		   100, -1, 6, 200., 400., "S");
  profile_RTilde_vs_TkLayer_genPt10_st2 
    = new TProfile("profile_RTilde_vs_TkLayer_genPt10_st2", 
		   "profile RTilde vs TkLayer, genPt10GeV, st2",
		   100, -1, 6, 200., 400., "S");
  profile_RTilde_vs_TkLayer_genPt50_st1 
    = new TProfile("profile_RTilde_vs_TkLayer_genPt50_st1", 
		   "profile RTilde vs TkLayer, genPt50GeV, st1",
		   100, -1, 6, 200., 400., "S");
  profile_RTilde_vs_TkLayer_genPt50_st2 
    = new TProfile("profile_RTilde_vs_TkLayer_genPt50_st2", 
		   "profile RTilde vs TkLayer, genPt50GeV, st2",
		   100, -1, 6, 200., 400., "S");

  //-----------------------------------------------------------------------------
  hIbendingDT_st1 = new  TH1F("IbendingDT_st1", "IBendingDT st1", 
			      100, -0.05, 0.2);
  hIbendingDT_st2 = new  TH1F("IbendingDT_st2", "IBendingDT st2", 
			      100, -0.05, 0.2);
  DeltaAlphaDT_st1 = new  TH1F("DeltaAlphaDT_st1", "DeltaAlphaDT st1", 
			       100, -1.0, 1.0);
  DeltaAlphaDT_st2 = new  TH1F("DeltaAlphaDT_st2", "DeltaAlphaDT st2", 
			       100, -1.0, 1.0);
  DeltaPhiDT_st1 = new  TH1F("DeltaPhiDT_st1", "DeltaPhiDT st1", 
			     100, -0.1, 0.1);
  DeltaPhiDT_st2 = new  TH1F("DeltaPhiDT_st2", "DeltaPhiDT st2", 
			     100, -0.1, 0.1);
  DeltaBendingDT_st1 = new  TH1F("DeltaDeBendingDT_st1", "DeltaBendingDT st1", 
			     100, -0.1, 0.1);
  DeltaBendingDT_st2 = new  TH1F("DeltaDeBendingDT_st2", "DeltaBendingDT st2", 
			     100, -0.1, 0.1);
}



// ------------ method called to for each event  ------------
void DTL1slhcPlots::analyze(const edm::Event& event, 
			    const edm::EventSetup& eventSetup)
{
  using namespace edm;

  ++EvNo;

  Handle<DTStubMatchesCollection> dtStaubMatchesHandle;
  try {
    event.getByLabel("DTL1slhcProd", dtStaubMatchesHandle);
  }
  catch(...) {
    cout << "\nException from event.getByLabel(\"DTL1slhcPlots\", dtStaubMatchesHandle)"
	 << endl;
    return;
  }
  if(!dtStaubMatchesHandle.isValid()) return;
  if(dtStaubMatchesHandle->numDt()) 
    ++AtLeastOneDTStaubMatchingMuonEvNo;
  for(int i=0; i<dtStaubMatchesHandle->numDt(); i++) {
    make_StubStubDephi_plots(dtStaubMatchesHandle, i); 
    make_MuStubStub_plots(dtStaubMatchesHandle, i); 
    make_StubStubStub_plots(dtStaubMatchesHandle, i);
    make_LinStubs_plots(event, dtStaubMatchesHandle, i); 
    make_test_plots(event, dtStaubMatchesHandle, i); 
  }
  /* 
     #ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
     ESHandle<SetupData> pSetup;
     setup.get<SetupRecord>().get(pSetup);
     #endif
  */
}




// ------------ method called once each job just after ending the event loop  ------------
void DTL1slhcPlots::endJob() {
  cout << "AtLeastOneDTStaubMatchingMuonEvNo " << AtLeastOneDTStaubMatchingMuonEvNo << endl;
  cout << "Event total " << EvNo << "\n" << endl;

  OutTFilePlots->cd();
  setIStyle();
  cout << "ehi!" << endl;
  //-- Magnetic filed ----------------------------------------------------------- 
  Int_t w = 800;
  Int_t h = 480;
  TCanvas *cbfield = new TCanvas("cms_magnetic_field", "cms magnetic field", w, h);
  magnetic_field_global_map();
  gPad->RedrawAxis();
  gPad->Update();
  cbfield->Write();
  cbfield->SaveAs("cms_magnetic_field.eps");
  delete cbfield;

  cbfield = new TCanvas("cms_magnetic_field_barrelYaxis", 
			"cms_magnetic_field_barrelYaxis", w, h);
  magnetic_field_barrelYaxis_profile();
  gPad->RedrawAxis();
  gPad->Update();
  cbfield->Write();
  cbfield->SaveAs("cms_magnetic_field_barrelYaxis.eps");
  delete cbfield;

  cout << "ehiehi!" << endl;

  make_StubStubDephi_canvases();
  make_MuStubStub_canvases();
  make_StubStubStub_canvases();
  stack_of_stubstubdephi_invPt_profile_with_linfits();
  make_LinStubs_canvases();
  make_test_canvases();

  OutTFilePlots->Close();
  delete OutTFilePlots;
  return;         

}





//----------------------------------------------------------------------------
void DTL1slhcPlots::update_histo(TH1F *histo, float store[], const double val) 
{
  setIStyle();
  if( isnan(val) ) 
    return;
  histo->Fill(val);
  size_t i = static_cast<size_t>(histo->GetEntries());
  if(i <= RangeCalibrationSampleSize) {
    store[i-1] = val;
    if(i == RangeCalibrationSampleSize)
      getOptimalRange(histo, store);
  }
} 


//------------------------------------------------------------------------------
void DTL1slhcPlots::getOptimalRange(TH1* h, float entries[])
{
  double Min, Max;
  double rawMin	= h->GetBinLowEdge(1); 
  size_t NBins = static_cast<size_t>(h->GetNbinsX());
  double rawMax = h->GetBinLowEdge(NBins+1);
  Double_t nentries = h->GetEntries();
  //  cout << "\n" << h->GetName() << ": " << nentries << "  " <<  h->Integral() << endl;
  double w = (rawMax - rawMin)/NBins;
  double d = 0.;
  double rd = 0.;
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
  //  cout << Min << "\t" << Max << endl;
  h->SetEntries(0);
  //  cout << h->Integral() << " --> " << flush;
  for(size_t i=0; i<RangeCalibrationSampleSize; i++)
    h->Fill(entries[i]);
  //  cout << h->Integral() << endl;
  return;
}


//define this as a plug-in
//DEFINE_FWK_MODULE(DTL1slhcPlots);

