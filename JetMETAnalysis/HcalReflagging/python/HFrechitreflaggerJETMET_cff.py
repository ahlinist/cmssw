import FWCore.ParameterSet.Config as cms

from JetMETAnalysis.HcalReflagging.hfrechitreflaggerJETMET_cfi import *
from copy import deepcopy
from math import log

#######################################################################
# v1 = "Loose conditions" -- short cut set to 0.98
HFrechitreflaggerJETMETv1 = deepcopy(hfrechitreflaggerJETMET)
HFrechitreflaggerJETMETv1.PETstat.short_R=[0.98]
HFrechitreflaggerJETMETv1.PETstat.short_R_29=[0.98]

########################################################################
# v2 = "Medium conditions"  -- short R cut set to 0.8, rather than 0.98
HFrechitreflaggerJETMETv2 = deepcopy(hfrechitreflaggerJETMET)
HFrechitreflaggerJETMETv2.PETstat.short_R=[0.8]
HFrechitreflaggerJETMETv2.PETstat.short_R_29=[0.8]
#)

########################################################################
# v3 = "Tight conditions"  -- short R cut set to 0.8
# long S9S1 cut set to:  slope = 0.09, int = -0.09*log(55)
HFrechitreflaggerJETMETv3 = deepcopy(hfrechitreflaggerJETMET)
HFrechitreflaggerJETMETv3.PETstat.short_R=[0.8]
HFrechitreflaggerJETMETv3.PETstat.short_R_29=[0.8]
HFrechitreflaggerJETMETv3.PETstat.long_R_29=[0.8]
HFrechitreflaggerJETMETv3.S9S1stat.long_optimumSlope=[]
for i in range(29,42):
    HFrechitreflaggerJETMETv3.S9S1stat.long_optimumSlope.append(0.09)
HFrechitreflaggerJETMETv3.S9S1stat.longEnergyParams=[]
for i in range(29,42):
    HFrechitreflaggerJETMETv3.S9S1stat.longEnergyParams.append(eval('-0.09*log(55)'))


##########################################################################
# v4 = "Medium + time conditions"  -- short R cut set to 0.8, rather than 0.98
#       Also require that reconstructed time occurs within specified (default) window

HFrechitreflaggerJETMETv4 = deepcopy(hfrechitreflaggerJETMET)
HFrechitreflaggerJETMETv4.PETstat.short_R=[0.8]
HFrechitreflaggerJETMETv4.PETstat.short_R_29=[0.8]
HFrechitreflaggerJETMETv4.FlagsToSet=(4,3,0,2)

#############################################################################
#  v5 = "v3 + time conditions"
#

HFrechitreflaggerJETMETv5 = deepcopy(hfrechitreflaggerJETMET)
HFrechitreflaggerJETMETv5.PETstat.short_R=[0.8]
HFrechitreflaggerJETMETv5.PETstat.short_R_29=[0.8]
HFrechitreflaggerJETMETv5.PETstat.long_R_29=[0.8]
HFrechitreflaggerJETMETv5.S9S1stat.long_optimumSlope=[]
for i in range(29,42):
    HFrechitreflaggerJETMETv3.S9S1stat.long_optimumSlope.append(0.09)
HFrechitreflaggerJETMETv5.S9S1stat.longEnergyParams=[]
for i in range(29,42):
    HFrechitreflaggerJETMETv3.S9S1stat.longEnergyParams.append(eval('-0.09*log(55)'))
HFrechitreflaggerJETMETv5.FlagsToSet=(4,3,0,2)

##############################################################################
#   v10 = default conditions as of 7 July 10
#
HFrechitreflaggerJETMETv10 = deepcopy(hfrechitreflaggerJETMET)
