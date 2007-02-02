/* \class TraclPlotsExample
 *
 * \author Luca Lista, INFN
 *
 * \version $Id: TrackPlotsExample.h,v 1.4 2006/10/30 13:33:04 llista Exp $
 *
 */
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

class TH1F; 
class TFile;

class TrackPlotsExample : public edm::EDAnalyzer {
public:
  TrackPlotsExample( const edm::ParameterSet & );
  ~TrackPlotsExample();
private:
  void beginJob( const edm::EventSetup & );
  void analyze( const edm::Event& , const edm::EventSetup& );
  void endJob();
  edm::InputTag src_;
  TFile * outputFile_;
  TH1F * h_pt, * h_eta, * h_phi;
};

#include "DataFormats/TrackReco/interface/Track.h"
#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <TROOT.h>
#include <TH1.h>
#include <TSystem.h>
#include <TFile.h>
#include <TCanvas.h>
#include <cmath>
#include <iostream>
using namespace edm;
using namespace reco;
using namespace std;

TrackPlotsExample::TrackPlotsExample( const ParameterSet & cfg ) :
  src_( cfg.getParameter<edm::InputTag>( "src" ) ) {
}

TrackPlotsExample::~TrackPlotsExample() {
  delete outputFile_;
}

void TrackPlotsExample::beginJob( const EventSetup & ) {
  outputFile_ = new TFile( "histo.root", "RECREATE" );
  h_pt  = new TH1F( "pt", "p_{t}", 50, 0, 20 );
  h_eta = new TH1F( "eta", "#eta", 50, -3, 3 );
  h_phi = new TH1F( "phi", "#phi", 50, -M_PI, M_PI );
}

void TrackPlotsExample::analyze( const Event& evt, const EventSetup& es ) {
  Handle<TrackCollection> tracks;
  evt.getByLabel( src_, tracks );

  for( TrackCollection::const_iterator track = tracks->begin(); track != tracks->end(); ++ track ) {
    h_pt->Fill( track->pt() );
    h_eta->Fill( track->eta() );
    h_phi->Fill( track->phi() );
  }
}

void TrackPlotsExample::endJob() {
  cout << ">> plotting histograms" << endl;
  gROOT->SetBatch();
  gROOT->SetStyle( "Plain" );
  TCanvas c;
  h_pt->GetXaxis()->SetTitle( "p_{t} (GeV/c^{2})" );
  h_pt->SetFillColor( kRed );
  h_pt->SetLineWidth( 2 );
  h_pt->Draw();
  c.SaveAs( "pt.eps" );
  h_eta->GetXaxis()->SetTitle( "#eta" );
  h_eta->SetFillColor( kRed );
  h_eta->SetLineWidth( 2 );
  h_eta->Draw();
  c.SaveAs( "eta.eps" );
  h_phi->GetXaxis()->SetTitle( "#phi (rad)" );
  h_phi->SetFillColor( kRed );
  h_phi->SetLineWidth( 2 );
  h_phi->Draw();
  c.SaveAs( "phi.eps" );
  cout << ">> saving histograms" << endl;
  outputFile_->Write() ;
  outputFile_->Close() ;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE( TrackPlotsExample );
