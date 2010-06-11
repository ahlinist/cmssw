#!/bin/csh

limit vmem unlim

cd /afs/cern.ch/user/v/veelken/scratch0/CMSSW_3_6_1/src/TauAnalysis/BgEstimationTools/test/
eval `scramv1 runtime -csh`
cd -

mkdir plots

cmsRun /afs/cern.ch/user/v/veelken/scratch0/CMSSW_3_6_1/src/TauAnalysis/BgEstimationTools/test/fitBgEstTemplateZtoMuTau_cfg.py

set rootFiles=(`/bin/ls *.root`)
foreach rootFile (${rootFiles})
    echo "copying ${rootFile} to /afs/cern.ch/user/v/veelken/scratch0/CMSSW_3_6_1/src/TauAnalysis/BgEstimationTools/test/plots/RooFit"
    cp ${rootFile} /afs/cern.ch/user/v/veelken/scratch0/CMSSW_3_6_1/src/TauAnalysis/BgEstimationTools/test/plots/RooFit
end

cd plots

set epsFiles=(`/bin/ls *.eps`)
foreach epsFile (${epsFiles})
    echo "copying ${epsFile} to /afs/cern.ch/user/v/veelken/scratch0/CMSSW_3_6_1/src/TauAnalysis/BgEstimationTools/test/plots/RooFit"
    cp ${epsFile} /afs/cern.ch/user/v/veelken/scratch0/CMSSW_3_6_1/src/TauAnalysis/BgEstimationTools/test/plots/RooFit
end
set pngFiles=(`/bin/ls *.png`)
foreach pngFile (${pngFiles})
    echo "copying ${pngFile} to /afs/cern.ch/user/v/veelken/scratch0/CMSSW_3_6_1/src/TauAnalysis/BgEstimationTools/test/plots/RooFit"
    cp ${pngFile} /afs/cern.ch/user/v/veelken/scratch0/CMSSW_3_6_1/src/TauAnalysis/BgEstimationTools/test/plots/RooFit
end
cd -
