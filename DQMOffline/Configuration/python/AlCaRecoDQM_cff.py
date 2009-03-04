import FWCore.ParameterSet.Config as cms

#DQM services
#be sure to include the standard sequences
from DQMServices.Components.MEtoEDMConverter_cfi import *
#Subsystem Includes 
# Tracker Alignment
from DQMOffline.Alignment.ALCARECOTkAlDQM_cff import *
# Muon  Alignment
from DQMOffline.Alignment.ALCARECOMuAlDQM_cff import *
# Ecal Calibration
from DQMOffline.Configuration.ALCARECOEcalCalDQM_cff import *
# unfortunally the ALCARECOTkAl-Producers can not go here because they are filters.
pathALCARECODQM = cms.Path(MEtoEDMConverter)

