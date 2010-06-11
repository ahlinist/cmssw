#!/bin/csh

rm /afs/cern.ch/user/v/veelken/scratch0/CMSSW_3_6_1/src/TauAnalysis/BgEstimationTools/test/plots/RooFit/fitBgEstTemplateZtoMuTau.out
bsub -q 1nw -R "type==SLC5_64" -J fitBgEstTemplateZtoMuTau -L /bin/csh -eo /afs/cern.ch/user/v/veelken/scratch0/CMSSW_3_6_1/src/TauAnalysis/BgEstimationTools/test/plots/RooFit/fitBgEstTemplateZtoMuTau.out -oo /afs/cern.ch/user/v/veelken/scratch0/CMSSW_3_6_1/src/TauAnalysis/BgEstimationTools/test/plots/RooFit/fitBgEstTemplateZtoMuTau.out < fitBgEstTemplateZtoMuTau@Batch.csh
