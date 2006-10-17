#include <iostream>
#include <cassert>
#include <TROOT.h>
#include <TSystem.h>
#include <Cintex/Cintex.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TCanvas.h>
#include <TH1.h>
#include <vector>
#include "DataFormats/TrackReco/interface/Track.h"
#include "FWCore/FWLite/src/AutoLibraryLoader.h"
using namespace std;
using namespace reco;

int main() {
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();

  TFile file( "events.root" );
  TTree * events = dynamic_cast<TTree*>( file.Get( "Events" ) );
  assert( events != 0 );
  TBranch * branch = events->GetBranch( "recoTracks_ctfWithMaterialTracks__Test.obj" );
  assert( branch != 0 );

  TrackCollection tracks;
  branch->SetAddress( & tracks );

  TH1F h_pt( "pt", "p_{t}", 50, 0, 20 );
  TH1F h_eta( "eta", "#eta", 50, -3, 3 );
  TH1F h_phi( "phi", "#phi", 50, -M_PI, M_PI );

  int nev = events->GetEntries();
  for( int ev = 0; ev < nev; ++ ev ) {
    branch->GetEntry( ev );
    for( size_t i = 0; i < tracks.size(); ++i ) {
      const Track & track = tracks[ i ];
      h_pt.Fill( track.pt() );
      h_eta.Fill( track.eta() );
      h_phi.Fill( track.phi() );
    }
  }
  file.Close();

  TFile hFile( "histo.root", "RECREATE" );
  h_pt.Write();
  h_eta.Write();
  h_phi.Write();

  gROOT->SetBatch();
  gROOT->SetStyle("Plain");
  TCanvas c;
  h_pt.GetXaxis()->SetTitle( "p_{t} (GeV/c^{2})" );
  h_pt.SetFillColor( kRed );
  h_pt.SetLineWidth( 2 );
  h_pt.Draw();
  c.SaveAs( "pt.jpg" );
  h_eta.GetXaxis()->SetTitle( "#eta" );
  h_eta.SetFillColor( kRed );
  h_eta.SetLineWidth( 2 );
  h_eta.Draw();
  c.SaveAs( "eta.jpg" );
  h_phi.GetXaxis()->SetTitle( "#phi (rad)" );
  h_phi.SetFillColor( kRed );
  h_phi.SetLineWidth( 2 );
  h_phi.Draw();
  c.SaveAs( "phi.jpg" );

  return 0;
}
