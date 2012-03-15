/*
Copy the ntuple and apply a preselection, perhaps reproducing the
default cuts in CosmicSplittingResolutionHistos, or just filtering out bad runs. To compile+run, do:

g++ -Wall `root-config --cflags --libs --glibs` -I${CMSSW_BASE}/src ../../plugins/CosmicSplittingResolutionNtuple.cc preselect.cc -o exe.preselect && ./exe.preselect filename.root newfilename.root
*/

#include "TFile.h"
#include "MuonAnalysis/Cosmics/plugins/CosmicSplittingResolutionNtuple.h"
#include "example_selection.h"

int main(int argc, char** argv) {
  if (argc < 3) {
    fprintf(stderr, "usage: preselect input_file.root output_file.root");
    return 1;
  }
    
  const TString input_filename(argv[1]);
  const TString output_filename(argv[2]);
  const TString input_directory("UTpickedTracks");
  const TString input_tree_name("t");
  const TString output_tree_name("t");

  TFile* input_file = new TFile(input_filename);
  TTree* input_tree = 0;

  if (input_file != 0 && input_file->IsOpen()) {
    TDirectory* input_dir = (TDirectory*)input_file->Get(input_directory);
    if (input_dir != 0)
      input_tree = (TTree*)input_dir->Get(input_tree_name);
  }

  if (input_tree == 0) {
    fprintf(stderr, "could not get tree from input file %s\n", input_filename.Data());
    return 1;
  }

  CosmicSplittingResolutionNtuple* nt = new CosmicSplittingResolutionNtuple;
  read_from_tree(input_tree, nt);

  TFile* output_file = new TFile(output_filename, "create");
  TTree* output_tree = 0;

  if (output_file != 0 && output_file->IsOpen())
    output_tree = new TTree(output_tree_name, "");

  if (output_tree == 0) {
    fprintf(stderr, "could not write tree to output file %s\n", output_filename.Data());
    return 1;
  }

  write_to_tree(output_tree, nt);

  selection sel(nt);
  sel.min_pixel_layers = 0;
  sel.min_strip_layers = 0;
  sel.no_csc_allowed = false;
  sel.no_shared_hits = false;
  sel.only_2011 = true;

  printf("\nEvent loop:\n");
  long jentry;
  for (jentry = 0; jentry < input_tree->GetEntriesFast(); ++jentry) {
    if (jentry % 50000 == 0) {
      printf("\r%li events done", jentry);
      fflush(stdout);
    }

    if (input_tree->LoadTree(jentry) < 0) break;
    if (input_tree->GetEntry(jentry) <= 0) continue;

    if (!sel.cut())
      output_tree->Fill();
  }

  printf("\ndone with %li events!\n", jentry);

  output_tree->Write();
  output_file->Close();
  input_file->Close();

  delete output_file;
  delete input_file;
  delete nt;
}
