from RecoTauTag.TauAnalysisTools.fakeRate.makeHistograms import *

import sys
import os

files = sys.argv[1:]

#Filter non-existent files
files[:] = [file for file in files if os.path.exists(file)]

# Constant weights
weights = [1 for file in files]

files_and_weights = zip(files, weights)

print files_and_weights

gROOT.SetBatch(True)

make_plots( files_and_weights,
      output_file="ppMuX_histograms.root",
      ntuple_name="shrinking",
      ntuple_setup = lambda ntuple: ntuple.set_collection("unmatched"),
      #x_bins = [0, 10, 20, 50, 80, 120, 200],
      x_bins = [0, 10, 20, 50, 80, 120],
      y_bins = [0, 0.5, 1.0, 1.2, 1.5, 2.0, 2.5],
      z_bins = [0, 0.01, 0.02, 0.04, 0.08, 0.1, 0.15])

