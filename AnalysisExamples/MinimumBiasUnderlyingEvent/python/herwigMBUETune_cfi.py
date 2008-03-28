import FWCore.ParameterSet.Config as cms

# Herwig DataCard for MBUETune 
HerwigParameters = cms.PSet(
    parameterSets = cms.vstring('herwigMBUETune'),
    herwigMBUETune = cms.vstring('IPROC      = 1500       ! QCD 2->2 processes', 'JMUEO      = 1          ! multiparton interaction model', 'PTMIN      = 3.0      ! minimum pt in hadronic jet production', 'PTJIM      = 4.872      ! 2.8x(sqrt(S)/1.8Tev)^0.27', 'JMRAD(73)  = 1.8        ! inverse proton radius squared', 'PRSOF      = 0.0        ! probability of a soft underlying event', 'MODPDF(1)  = 10041      ! PDF set according to LHAGLUE', 'MODPDF(2)  = 10041      ! CTEQ6L ')
)

