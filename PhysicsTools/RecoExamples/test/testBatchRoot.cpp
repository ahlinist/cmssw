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
#include "PhysicsTools/FWLite/src/AutoLibraryLoader.h"
using namespace std;
using namespace reco;

int main() {
  gSystem->Load( "libPhysicsToolsFWLite" );
  AutoLibraryLoader::enable();

  TFile file( "reco.root" );
  TTree * events = dynamic_cast<TTree*>( file.Get( "Events" ) );
  assert( events != 0 );
  TBranch * branch = events->GetBranch( "recoTracks_trackp__RoadSearch.obj" );
  assert( branch != 0 );

  TrackCollection tracks;
  branch->SetAddress( & tracks );

  TH1F histo( "pt", "Transverse momentum", 100, 0, 20 );

  int nev = events->GetEntries();
  for( int ev = 0; ev < nev; ++ ev ) {
    branch->GetEntry( ev );
    for( size_t i = 0; i < tracks.size(); ++i ) {
      histo.Fill( tracks[ i ].pt() );
    }
  }
  file.Close();

  TFile hFile( "histo.root", "RECREATE" );
  histo.Write();

  gROOT->SetBatch();
  gROOT->SetStyle("Plain");
  TCanvas c;
  histo.Draw();
  c.SaveAs( "pt.jpg" );

  return 0;
}
