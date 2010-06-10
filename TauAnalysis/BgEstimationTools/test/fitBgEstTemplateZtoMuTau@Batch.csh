#!/bin/csh
limit vmem unlim
cd /afs/cern.ch/user/v/veelken/scratch0/CMSSW_3_6_1/src/TauAnalysis/BgEstimationTools/test/
eval `scramv1 runtime -csh`
cd -
cmsRun /afs/cern.ch/user/v/veelken/scratch0/CMSSW_3_6_1/src/TauAnalysis/BgEstimationTools/test/fitBgEstTemplateZtoMuTau_cfg.py
set epsFiles=(`/bin/ls *.eps`)
foreach epsFile (${epsFiles})
    echo "copying ${epsFile} to /afs/cern.ch/user/v/veelken/scratch0/CMSSW_3_6_1/src/TauAnalysis/BgEstimationTools/test/plots/lxbatch"
    cp ${epsFile} /afs/cern.ch/user/v/veelken/scratch0/CMSSW_3_6_1/src/TauAnalysis/BgEstimationTools/test/plots/lxbatch
end
set rootFiles=(`/bin/ls *.root`)
foreach rootFile (${rootFiles})
    echo "copying ${rootFile} to /afs/cern.ch/user/v/veelken/scratch0/CMSSW_3_6_1/src/TauAnalysis/BgEstimationTools/test/plots/lxbatch"
    cp ${rootFile} /afs/cern.ch/user/v/veelken/scratch0/CMSSW_3_6_1/src/TauAnalysis/BgEstimationTools/test/plots/lxbatch
end
