import FWCore.ParameterSet.Config as cms

from JetMETAnalysis.HcalReflagging.hcalrechitreflaggerJETMET_cfi import *
from copy import deepcopy
from math import log

# v1 = "Loose conditions" -- same as default
HFrechitreflaggerJETMET_v1 = deepcopy(hcalrechitReflaggerJETMET)


# v2 = "Medium conditions"  -- short R cut set to 0.8, rather than 0.98
HFrechitreflaggerJETMET_v2 = deepcopy(hcalrechitReflaggerJETMET)
HFrechitreflaggerJETMET_v2.hf_Short_Rcut=cms.PSet(
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

# v3 = "Tight conditions"  -- short R cut set to 0.8
# long S9S1 cut set to:  slope = 0.9, int = -0.09*log(55)
HFrechitreflaggerJETMET_v3 = deepcopy(hcalrechitReflaggerJETMET)
HFrechitreflaggerJETMET_v3.hf_Short_Rcut=cms.PSet(
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
HFrechitreflaggerJETMET_v3.hf_Long_Rcut.hf_Long_Rcut_values29=cms.vdouble([0.8])
# Slope is 0.09, intercept = -0.09*log(55) for all ieta
HFrechitreflaggerJETMET_v3.hf_Long_S9S1_Slope_Intercept = cms.PSet(hf_Long_S9S1_SlopeIntercept29=cms.vdouble([0.09,eval('-0.09*log(55)')]),
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
