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

endcap3 = []
endcap4 = []
endcap5 = []
endcap6 = []
endcap7 = []
endcap8 = []
endcap9 = []
endcap20 = []
endcap30 = []
endcap50 = []
endcap100 = []
endcap150 = []
endcap225 = []
endcap300 = []

endcap3.append('15498.0001')
endcap3.append('15498.0002')
endcap3.append('15498.0003')
endcap3.append('15498.0004')
endcap3.append('15498.0005')
endcap3.append('15498.0006')
endcap3.append('15498.0007')
endcap3.append('15558.0001')
endcap3.append('15558.0002')
endcap3.append('15558.0003')
endcap3.append('15558.0004')
endcap3.append('15558.0005')
endcap3.append('15558.0006')
endcap3.append('15558.0007')
endcap3.append('15558.0008')
endcap3.append('15558.0009')
endcap3.append('15558.0010')
endcap3.append('15558.0011')
endcap3.append('15558.0012')
endcap3.append('15558.0013')
endcap3.append('15558.0014')
endcap3.append('15558.0015')
endcap3.append('15558.0016')
endcap3.append('15558.0017')
endcap3.append('15558.0018')
endcap3.append('15558.0019')
endcap3.append('15558.0020')
endcap4.append('15494.0001')
endcap4.append('15494.0002')
endcap4.append('15494.0003')
endcap4.append('15494.0004')
endcap4.append('15494.0005')
endcap4.append('15494.0006')
endcap4.append('15554.0001')
endcap4.append('15554.0010')
endcap4.append('15554.0011')
endcap4.append('15554.0012')
endcap4.append('15554.0013')
endcap4.append('15554.0014')
endcap4.append('15554.0015')
endcap4.append('15554.0016')
endcap4.append('15554.0017')
endcap4.append('15554.0018')
endcap4.append('15554.0019')
endcap4.append('15554.0020')
endcap4.append('15554.0021')
endcap4.append('15554.0022')
endcap4.append('15554.0023')
endcap4.append('15554.0024')
endcap4.append('15554.0025')
endcap4.append('15554.0026')
endcap4.append('15554.0027')
endcap4.append('15554.0028')
endcap4.append('15554.0029')
endcap5.append('15535.0020')
endcap5.append('15535.0021')
endcap5.append('15535.0022')
endcap5.append('15535.0023')
endcap5.append('15535.0024')
endcap5.append('15535.0025')
endcap5.append('15535.0026')
endcap5.append('15535.0027')
endcap5.append('15535.0028')
endcap5.append('15535.0029')
endcap5.append('15535.0030')
endcap5.append('15535.0031')
endcap5.append('15535.0032')
endcap5.append('15535.0033')
endcap5.append('15535.0034')
endcap5.append('15535.0035')
endcap5.append('15535.0036')
endcap5.append('15535.0037')
endcap5.append('15535.0038')
endcap5.append('15535.0039')
endcap6.append('15524.0001')
endcap6.append('15524.0002')
endcap6.append('15524.0003')
endcap6.append('15524.0004')
endcap6.append('15524.0005')
endcap6.append('15524.0006')
endcap6.append('15524.0007')
endcap6.append('15524.0008')
endcap6.append('15524.0009')
endcap6.append('15524.0010')
endcap6.append('15524.0011')
endcap6.append('15524.0012')
endcap6.append('15524.0013')
endcap6.append('15524.0014')
endcap6.append('15524.0015')
endcap6.append('15524.0016')
endcap6.append('15524.0017')
endcap6.append('15524.0018')
endcap6.append('15524.0019')
endcap7.append('15518.0001')
endcap7.append('15520.0001')
endcap7.append('15520.0002')
endcap7.append('15520.0003')
endcap7.append('15520.0004')
endcap7.append('15520.0005')
#endcap7.append('15520.0006')
endcap7.append('15520.0007')
endcap7.append('15520.0008')
endcap7.append('15520.0009')
endcap7.append('15520.0010')
endcap7.append('15520.0011')
endcap8.append('15514.0001')
endcap8.append('15514.0002')
endcap8.append('15514.0003')
endcap8.append('15514.0004')
endcap8.append('15514.0005')
endcap9.append('15507.0011')
endcap9.append('15507.0012')
endcap9.append('15507.0013')
#endcap9.append('15507.0014')
#endcap9.append('15508.0015')
#endcap9.append('15508.0016')
#endcap9.append('15508.0017')
#endcap9.append('15508.0018')
endcap20.append('16024.0000')
endcap20.append('16024.0001')
endcap20.append('16025.0000')
endcap20.append('16025.0001')
endcap20.append('16025.0002')
endcap20.append('16025.0003')
endcap30.append('16027.0000')
endcap30.append('16027.0001')
endcap30.append('16028.0000')
endcap30.append('16028.0001')
endcap50.append('16031.0000')
endcap50.append('16031.0001')
endcap50.append('16031.0002')
endcap50.append('16031.0003')
endcap100.append('16037.0000')
#endcap100.append('16037.0001')
endcap100.append('16037.0002')
endcap150.append('16038.0000')
endcap150.append('16038.0001')
endcap150.append('16038.0002')
endcap225.append('16044.0000')
endcap225.append('16044.0001')
endcap225.append('16044.0002')
endcap225.append('16044.0003')
endcap300.append('16045.0000')
endcap300.append('16045.0001')
endcap300.append('16045.0002')

endcap0 = endcap3 + endcap4 + endcap5 + endcap6 + endcap7 + endcap8 + endcap9 + endcap20 + endcap30 + endcap50 + endcap100 + endcap150 + endcap225 + endcap300

endcap = {0:endcap0, 3:endcap3, 4:endcap4, 5:endcap5, 6:endcap6, 7:endcap7, 8:endcap8, 9:endcap9, 20:endcap20, 30:endcap30, 50:endcap50, 100:endcap100, 150:endcap150, 225:endcap225, 300:endcap300}

#testbeam_cuts = "/afs/cern.ch/user/b/ballin/scratch0/cmssw/src/RecoParticleFlow/PFAnalyses/macros/testbeam_cuts_310_endcap.root"
testbeam_cuts = "/afs/cern.ch/user/b/ballin/scratch0/cmssw/src/RecoParticleFlow/PFAnalyses/macros/testbeam_cuts_test.root"
