#include <iostream>
#include <cassert>
#include <TROOT.h>
#include <Cintex/Cintex.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TCanvas.h>
#include <TH1.h>
#include <vector>
#include "DataFormats/TrackReco/interface/TrackExtra.h"
using namespace std;
using namespace reco;
int main() {
  seal::cintex::Cintex::enable();

  TFile file("reco.root");
  TTree * events = dynamic_cast<TTree*>( file.Get( "Events" ) );
  assert( events != 0 );
  TBranch * branch = events->GetBranch( "TrackExtra_AllTrackExtras__ORCA.obj" );
  assert( branch != 0 );

  TrackExtraCollection trkExtra;
  branch->SetAddress( & trkExtra );

  TH1F histo( "nofhits", "Number of hits", 21, -0.5, 20.5 );

  int nev = events->GetEntries();
  for( int ev = 0; ev < nev; ++ ev ) {
    branch->GetEntry( ev );
    for( size_t i = 0; i < trkExtra.size(); ++i ) {
      histo.Fill( trkExtra[ i ].recHitsSize() );
    }
  }
  file.Close();

  TFile hFile( "histo.root", "RECREATE" );
  histo.Write();

  gROOT->SetBatch();
  gROOT->SetStyle("Plain");
  TCanvas c;
  histo.Draw();
  c.SaveAs( "hits.gif" );

  return 0;
}
