#!/usr/bin/env python

# Dictionary and lists of good pion runs from ECAL/HCAL test beam by energy
# Jamie Ballin, Imperial College London
# November 2008

# Use this by issuing the incantation,
#     from RecoParticleFlow.PFAnalyses.RunDict import *

gev2 = [30139, 30140, 30141, 30146, 29739, 29740, 29950, 29951, 29953, 
        29954, 29955, 29956, 29957, 29958, 29959 ]
gev3 = [29689, 29690, 29691, 29694, 29695, 29703, 29704, 29705, 29706, 
        29707, 30127, 30128, 30129, 30131, 30133]
gev4 = [29976, 29977, 29983, 29984, 29986, 29988, 29989, 29990, 29991]
gev5 = [29713, 29728, 29729, 29730, 29731, 29754, 29755, 30114, 30115, 
        30116, 30117, 30120 ]
gev6 = [29721, 29722, 29725, 29726, 29757, 29758, 29759, 30111, 30112, 
        30113 ]
gev7 = [29732, 29733, 29734, 29760, 29761, 29762, 30110 ]
gev8 = [29735, 29736, 29763, 29764, 29767 ]
gev9 = [29650, 29737, 29738, 30109 ]
gev20 = [30379 ]
gev30 = [30553 ]
gev50 = [30384 ]
gev100 = [30386 ]
gev150 = [30387 ]
gev200 = [30388 ]
gev300 = [30389, 30390 ]

# All runs > 10 GeV
gev1000 = [30379, 30553, 30384, 30386, 30387, 30388, 30389, 30390, 16031]
gev0 = gev2 + gev3 + gev4 + gev5 + gev6 + gev7 + gev8 + gev9 + gev1000

energies = {0:gev0, 2:gev2, 3:gev3, 4:gev4, 5:gev5, 6:gev6, 7:gev7, 8:gev8, 
            9:gev9, 20:gev20, 30:gev30, 50:gev50, 100:gev100, 150:gev150, 
            200:gev200, 300:gev300, 1000:gev1000}


endcap9 = [15507]
endcap50 = [16031]
endcap = {9:endcap9, 50:endcap50}

testbeam_cuts = "/afs/cern.ch/user/b/ballin/scratch0/cmssw/src/RecoParticleFlow/PFAnalyses/macros/testbeam_cuts_310.root"

