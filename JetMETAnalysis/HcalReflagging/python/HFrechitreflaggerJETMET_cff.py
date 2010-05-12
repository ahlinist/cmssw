import FWCore.ParameterSet.Config as cms

from JetMETAnalysis.HcalReflagging.hfrechitreflaggerJETMET_cfi import *
from copy import deepcopy
from math import log

#######################################################################
# v1 = "Loose conditions" -- same as default
HFrechitreflaggerJETMETv1 = deepcopy(hfrechitreflaggerJETMET)


########################################################################
# v2 = "Medium conditions"  -- short R cut set to 0.8, rather than 0.98
HFrechitreflaggerJETMETv2 = deepcopy(hfrechitreflaggerJETMET)
HFrechitreflaggerJETMETv2.TopoFlag.hf_Short_Rcut=cms.PSet(
    hf_Short_Rcut_parameterizeET=cms.untracked.bool(False),
    hf_Short_Rcut_values29=cms.vdouble([0.8]),
    hf_Short_Rcut_values30=cms.vdouble([0.8]),
    hf_Short_Rcut_values31=cms.vdouble([0.8]),
    hf_Short_Rcut_values32=cms.vdouble([0.8]),
    hf_Short_Rcut_values33=cms.vdouble([0.8]),
    hf_Short_Rcut_values34=cms.vdouble([0.8]),
    hf_Short_Rcut_values35=cms.vdouble([0.8]),
    hf_Short_Rcut_values36=cms.vdouble([0.8]),
    hf_Short_Rcut_values37=cms.vdouble([0.8]),
    hf_Short_Rcut_values38=cms.vdouble([0.8]),
    hf_Short_Rcut_values39=cms.vdouble([0.8]),
    hf_Short_Rcut_values40=cms.vdouble([0.8]),
    hf_Short_Rcut_values41=cms.vdouble([0.8]),
)

########################################################################
# v3 = "Tight conditions"  -- short R cut set to 0.8
# long S9S1 cut set to:  slope = 0.09, int = -0.09*log(55)
HFrechitreflaggerJETMETv3 = deepcopy(hfrechitreflaggerJETMET)
HFrechitreflaggerJETMETv3.TopoFlag.hf_Short_Rcut=cms.PSet(
    hf_Short_Rcut_parameterizeET=cms.untracked.bool(False),
    hf_Short_Rcut_values29=cms.vdouble([0.8]),
    hf_Short_Rcut_values30=cms.vdouble([0.8]),
    hf_Short_Rcut_values31=cms.vdouble([0.8]),
    hf_Short_Rcut_values32=cms.vdouble([0.8]),
    hf_Short_Rcut_values33=cms.vdouble([0.8]),
    hf_Short_Rcut_values34=cms.vdouble([0.8]),
    hf_Short_Rcut_values35=cms.vdouble([0.8]),
    hf_Short_Rcut_values36=cms.vdouble([0.8]),
    hf_Short_Rcut_values37=cms.vdouble([0.8]),
    hf_Short_Rcut_values38=cms.vdouble([0.8]),
    hf_Short_Rcut_values39=cms.vdouble([0.8]),
    hf_Short_Rcut_values40=cms.vdouble([0.8]),
    hf_Short_Rcut_values41=cms.vdouble([0.8]),
)
HFrechitreflaggerJETMETv3.TopoFlag.hf_Long_Rcut.hf_Long_Rcut_values29=cms.vdouble([0.8])
# Slope is 0.09, intercept = -0.09*log(55) for all ieta
HFrechitreflaggerJETMETv3.TopoFlag.hf_Long_S9S1_Slope_Intercept = cms.PSet(
    hf_Long_S9S1_SlopeIntercept29=cms.vdouble([0.09,eval('-0.09*log(55)')]),
    hf_Long_S9S1_SlopeIntercept30=cms.vdouble([0.09,eval('-0.09*log(55)')]),
    hf_Long_S9S1_SlopeIntercept31=cms.vdouble([0.09,eval('-0.09*log(55)')]),
    hf_Long_S9S1_SlopeIntercept32=cms.vdouble([0.09,eval('-0.09*log(55)')]),
    hf_Long_S9S1_SlopeIntercept33=cms.vdouble([0.09,eval('-0.09*log(55)')]),
    hf_Long_S9S1_SlopeIntercept34=cms.vdouble([0.09,eval('-0.09*log(55)')]),
    hf_Long_S9S1_SlopeIntercept35=cms.vdouble([0.09,eval('-0.09*log(55)')]),
    hf_Long_S9S1_SlopeIntercept36=cms.vdouble([0.09,eval('-0.09*log(55)')]),
    hf_Long_S9S1_SlopeIntercept37=cms.vdouble([0.09,eval('-0.09*log(55)')]),
    hf_Long_S9S1_SlopeIntercept38=cms.vdouble([0.09,eval('-0.09*log(55)')]),
    hf_Long_S9S1_SlopeIntercept39=cms.vdouble([0.09,eval('-0.09*log(55)')]),
    hf_Long_S9S1_SlopeIntercept40=cms.vdouble([0.09,eval('-0.09*log(55)')]),
    hf_Long_S9S1_SlopeIntercept41=cms.vdouble([0.09,eval('-0.09*log(55)')]),
)

##########################################################################
# v4 = "Medium + time conditions"  -- short R cut set to 0.8, rather than 0.98
#       Also require that reconstructed time occurs within specified (default) window

HFrechitreflaggerJETMETv4 = deepcopy(hfrechitreflaggerJETMET)
HFrechitreflaggerJETMETv4.TopoFlag.hf_Short_Rcut=cms.PSet(
    hf_Short_Rcut_parameterizeET=cms.untracked.bool(False),
    hf_Short_Rcut_values29=cms.vdouble([0.8]),
    hf_Short_Rcut_values30=cms.vdouble([0.8]),
    hf_Short_Rcut_values31=cms.vdouble([0.8]),
    hf_Short_Rcut_values32=cms.vdouble([0.8]),
    hf_Short_Rcut_values33=cms.vdouble([0.8]),
    hf_Short_Rcut_values34=cms.vdouble([0.8]),
    hf_Short_Rcut_values35=cms.vdouble([0.8]),
    hf_Short_Rcut_values36=cms.vdouble([0.8]),
    hf_Short_Rcut_values37=cms.vdouble([0.8]),
    hf_Short_Rcut_values38=cms.vdouble([0.8]),
    hf_Short_Rcut_values39=cms.vdouble([0.8]),
    hf_Short_Rcut_values40=cms.vdouble([0.8]),
    hf_Short_Rcut_values41=cms.vdouble([0.8]),
)
HFrechitreflaggerJETMETv4.TimeFlag.useFlag=True

#############################################################################
#  v5 = "v3 + time conditions"
#

HFrechitreflaggerJETMETv5 = deepcopy(hfrechitreflaggerJETMET)
HFrechitreflaggerJETMETv5.TopoFlag.hf_Short_Rcut=cms.PSet(
    hf_Short_Rcut_parameterizeET=cms.untracked.bool(False),
    hf_Short_Rcut_values29=cms.vdouble([0.8]),
    hf_Short_Rcut_values30=cms.vdouble([0.8]),
    hf_Short_Rcut_values31=cms.vdouble([0.8]),
    hf_Short_Rcut_values32=cms.vdouble([0.8]),
    hf_Short_Rcut_values33=cms.vdouble([0.8]),
    hf_Short_Rcut_values34=cms.vdouble([0.8]),
    hf_Short_Rcut_values35=cms.vdouble([0.8]),
    hf_Short_Rcut_values36=cms.vdouble([0.8]),
    hf_Short_Rcut_values37=cms.vdouble([0.8]),
    hf_Short_Rcut_values38=cms.vdouble([0.8]),
    hf_Short_Rcut_values39=cms.vdouble([0.8]),
    hf_Short_Rcut_values40=cms.vdouble([0.8]),
    hf_Short_Rcut_values41=cms.vdouble([0.8]),
)
HFrechitreflaggerJETMETv5.TopoFlag.hf_Long_Rcut.hf_Long_Rcut_values29=cms.vdouble([0.8])
# Slope is 0.09, intercept = -0.09*log(55) for all ieta
HFrechitreflaggerJETMETv5.TopoFlag.hf_Long_S9S1_Slope_Intercept = cms.PSet(
    hf_Long_S9S1_SlopeIntercept29=cms.vdouble([0.09,eval('-0.09*log(55)')]),
    hf_Long_S9S1_SlopeIntercept30=cms.vdouble([0.09,eval('-0.09*log(55)')]),
    hf_Long_S9S1_SlopeIntercept31=cms.vdouble([0.09,eval('-0.09*log(55)')]),
    hf_Long_S9S1_SlopeIntercept32=cms.vdouble([0.09,eval('-0.09*log(55)')]),
    hf_Long_S9S1_SlopeIntercept33=cms.vdouble([0.09,eval('-0.09*log(55)')]),
    hf_Long_S9S1_SlopeIntercept34=cms.vdouble([0.09,eval('-0.09*log(55)')]),
    hf_Long_S9S1_SlopeIntercept35=cms.vdouble([0.09,eval('-0.09*log(55)')]),
    hf_Long_S9S1_SlopeIntercept36=cms.vdouble([0.09,eval('-0.09*log(55)')]),
    hf_Long_S9S1_SlopeIntercept37=cms.vdouble([0.09,eval('-0.09*log(55)')]),
    hf_Long_S9S1_SlopeIntercept38=cms.vdouble([0.09,eval('-0.09*log(55)')]),
    hf_Long_S9S1_SlopeIntercept39=cms.vdouble([0.09,eval('-0.09*log(55)')]),
    hf_Long_S9S1_SlopeIntercept40=cms.vdouble([0.09,eval('-0.09*log(55)')]),
    hf_Long_S9S1_SlopeIntercept41=cms.vdouble([0.09,eval('-0.09*log(55)')]),
)

HFrechitreflaggerJETMETv5.TimeFlag.useFlag=True
