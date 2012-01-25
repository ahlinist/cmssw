/*
Simple standalone program to run on the ntuple, reproducing the
default cuts in CosmicSplittingResolutionHistos. To compile+run, do:

g++ -Wall `root-config --cflags --libs --glibs` -I${CMSSW_BASE}/src ../../plugins/CosmicSplittingResolutionNtuple.cc example.cc -o exe.example && ./exe.example filename.root
*/

#include "TCanvas.h"
#include "TH1F.h"
#include "TFile.h"
#include "TStyle.h"
#include "MuonAnalysis/Cosmics/plugins/CosmicSplittingResolutionNtuple.h"
#include "example_selection.h"

int main(int argc, char** argv) {
  if (argc < 2) {
    fprintf(stderr, "need a filename.root\n");
    return 1;
  }
    
  const TString filename(argv[1]);
  const TString directory("UTpickedTracks");
  const TString tree_name("t");

  TFile* file = new TFile(filename);
  TTree* tree = 0;

  if (file != 0 && file->IsOpen()) {
    TDirectory* dir = (TDirectory*)file->Get(directory);
    if (dir != 0)
      tree = (TTree*)dir->Get(tree_name);
  }

  if (tree == 0) {
    fprintf(stderr, "could not get tree from file!\n");
    return 1;
  }

  CosmicSplittingResolutionNtuple* nt = new CosmicSplittingResolutionNtuple;
  read_from_tree(tree, nt);


  TH1F* h_ref_pt = new TH1F("h_ref_pt", "", 200, 0, 2000);
  TH1F* h_tkonly_relres = new TH1F("h_tkonly_relres", "", 100, -0.75, 0.75);
  
  printf("\nEvent loop:\n");
  long jentry;
  for (jentry = 0; jentry < tree->GetEntriesFast(); ++jentry) {
    if (jentry % 50000 == 0) {
      printf("\r%li events done", jentry);
      fflush(stdout);
    }

    if (tree->LoadTree(jentry) < 0) break;
    if (tree->GetEntry(jentry) <= 0) continue;

    if (cut(nt))
      continue;

    h_ref_pt->Fill(nt->ref_pt);
    if (nt->ref_pt > 350 && nt->ref_pt < 2000)
      h_tkonly_relres->Fill((nt->unprop_charge[tk_tkonly][lower]*nt->unprop_charge[tk_tkonly][upper]*nt->unprop_pt[tk_tkonly][lower]/nt->unprop_pt[tk_tkonly][upper] - 1)/1.41421356);
  }

  printf("\ndone with %li events!\n", jentry);
  gStyle->SetOptStat("ourme");
  TCanvas* c = new TCanvas("c", "", 1000, 800);

  h_ref_pt->Draw();
  c->SaveAs("example_refpt.root");
  c->SaveAs("example_refpt.png");
  h_tkonly_relres->Draw();
  c->SaveAs("example_tkonly_relres.root");
  c->SaveAs("example_tkonly_relres.png");
}
