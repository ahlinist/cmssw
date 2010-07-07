import FWCore.ParameterSet.Config as cms
import string

# Get basic parameters for reflagging from the standard reco file
# (We keep a copy of it here so that one doesn't need to include all the
#  RecHit dependencies by checking out RecoLocalCalo/HcalRecProducers)
import JetMETAnalysis.HcalReflagging.HcalHitReconstructor_hf_copy as hf_reco

# This is the HF PMT window reflagger
hfrechitreflaggerJETMET = cms.EDProducer('HFRecHitReflaggerJETMET',
                                         debug          = cms.untracked.int32(0),
                                         hfInputLabel   = cms.untracked.InputTag("hfreco"),

                                         FlagsToSet     = cms.vint32(4,3,0),
                                         # Define first PSet to store topological cut parameters

                                         # The following parameters all come directly from the HF reconstruction cfg file
                                         
                                         HFInWindowStat=hf_reco.hfreco.HFInWindowStat,
                                         PETstat=hf_reco.hfreco.PETstat,
                                         S9S1stat=hf_reco.hfreco.S9S1stat,
                                         S8S1stat=hf_reco.hfreco.S8S1stat,
                                         
)
# Add a (useless) flagsToSkip value to the HFInWindowStat pset
hfrechitreflaggerJETMET.HFInWindowStat.flagsToSkip=cms.int32(0)

#hfrechitreflaggerJETMET.HFInWindowStat.hflongMaxWindowTime=[50]  # This would change the max time
#hfrechitreflaggerJETMET.FlagsToSet.append(2)  # Add this to set HFInTimeWindow flag

# These settings should be used in data:
#hfrechitreflaggerJETMET.PETstat.flagsToSkip =string.atoi('10',2)
#hfrechitreflaggerJETMET.S8S1stat.flagsToSkip=string.atoi('10010',2)
#hfrechitreflaggerJETMET.S9S1stat.flagsToSkip=string.atoi('11010',2)

