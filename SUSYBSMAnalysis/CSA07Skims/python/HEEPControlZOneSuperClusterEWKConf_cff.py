import FWCore.ParameterSet.Config as cms

import copy
from ElectroWeakAnalysis.ZReco.zToEEOneSuperCluster_cfi import *
#
# Reuse EWK modules, but reconfigure for higher mass cut
#
# This is the only real change
HEEPControlZOneSuperCluster = copy.deepcopy(zToEEOneSuperCluster)
import copy
from ElectroWeakAnalysis.ZReco.zToEEOneSuperClusterGenParticlesMatch_cfi import *
# change the input tags to the changed module
HEEPControlZOneSuperClusterGenParticlesMatch = copy.deepcopy(zToEEOneSuperClusterGenParticlesMatch)
import copy
from ElectroWeakAnalysis.ZReco.zToEEOneSuperClusterFilter_cfi import *
HEEPControlZOneSuperClusterFilter = copy.deepcopy(zToEEOneSuperClusterFilter)
HEEPControlZOneSuperCluster.massMin = 60.
HEEPControlZOneSuperClusterGenParticlesMatch.src = 'HEEPControlZOneSuperCluster'
HEEPControlZOneSuperClusterFilter.src = 'HEEPControlZOneSuperCluster'

