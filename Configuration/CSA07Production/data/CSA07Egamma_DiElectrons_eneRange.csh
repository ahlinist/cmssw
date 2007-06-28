cat > CSA07Egamma_DiElectrons_Ene$1to$2_GEN_SIM.cfg <<EOF
process Sim  = {

   untracked PSet maxEvents = {untracked int32 input = 500000}

   untracked PSet configurationMetadata = {
           untracked string version = "$Revision: 1.1 $"
           untracked string name = "$Source: /local/projects/CMSSW/rep/CMSSW/Configuration/CSA07Production/data/CSA07Example1_GEN_SIM.cfg,v $"
           untracked string annotation = "back-to-back e+e-; flat energy spectrum between $1 and $2 GeV"
   }

# testing other additions for digi
  include "Configuration/ReleaseValidation/data/Services.cff"
  include "Configuration/StandardSequences/data/FakeConditions.cff"

   include "FWCore/MessageService/data/MessageLogger.cfi"
#   replace MessageLogger.cout.threshold = "ERROR"
#   replace MessageLogger.cerr.default.limit = 10

  include "SimGeneral/HepPDTESSource/data/pythiapdt.cfi"

  source = FlatRandomEGunSource 
  { 	
    untracked PSet PGunParameters =
    {
      untracked vint32 PartID = { 11 }
      untracked double MinEta = -2.7
      untracked double MaxEta =  2.7
      untracked double MinPhi = -3.14159265358979323846 # in radians
      untracked double MaxPhi =  3.14159265358979323846
      untracked double MinE  = $1.
      untracked double MaxE  = $2.
    }
    untracked bool AddAntiParticle = true # back-to-back particles
    untracked int32 Verbosity = 0 # set to 1 (or greater)  for printouts
  }
   
  include "Configuration/StandardSequences/data/Simulation.cff"
  include "Configuration/StandardSequences/data/VtxSmearedGauss.cff"
  include "Configuration/StandardSequences/data/MixingNoPileUp.cff"

  path p1 = {psim} 

  include "Configuration/EventContent/data/EventContent.cff"

   module GEN-SIM = PoolOutputModule
   {
        using FEVTSIMEventContent
        untracked string fileName = "file:DiElectrons_Ene$1to$2.root"
	untracked PSet dataset = {
	    untracked string dataTier = "GEN-SIM"
      	}
   }
   endpath outpath = {GEN-SIM}
   schedule = {p1,outpath}
}
EOF
