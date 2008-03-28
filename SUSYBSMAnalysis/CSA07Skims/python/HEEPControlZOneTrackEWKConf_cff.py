import FWCore.ParameterSet.Config as cms

import copy
from ElectroWeakAnalysis.ZReco.zToEEOneTrack_cfi import *
#
# Reuse EWK modules, but reconfigure for higher mass cut
#
# This is the only real change
HEEPControlZOneTrack = copy.deepcopy(zToEEOneTrack)
import copy
from ElectroWeakAnalysis.ZReco.zToEEOneTrackGenParticlesMatch_cfi import *
# change the input tags to the changed module
HEEPControlZOneTrackGenParticlesMatch = copy.deepcopy(zToEEOneTrackGenParticlesMatch)
import copy
from ElectroWeakAnalysis.ZReco.zToEEOneTrackFilter_cfi import *
HEEPControlZOneTrackFilter = copy.deepcopy(zToEEOneTrackFilter)
HEEPControlZOneTrack.massMin = 60.
HEEPControlZOneTrackGenParticlesMatch.src = 'HEEPControlZOneTrack'
HEEPControlZOneTrackFilter.src = 'HEEPControlZOneTrack'

