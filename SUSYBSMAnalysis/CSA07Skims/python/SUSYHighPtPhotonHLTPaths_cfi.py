import FWCore.ParameterSet.Config as cms

import copy
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
SUSYHighPtPhotonHLTPath = copy.deepcopy(hltHighLevel)
import copy
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
SUSYControlHighPtPhotonHLTPath = copy.deepcopy(hltHighLevel)
SUSYHighPtPhotonHLTPath.HLTPaths = ['HLT1PhotonRelaxed', 'HLT2PhotonRelaxed', 'HLT1Photon', 'HLT2Photon']
SUSYControlHighPtPhotonHLTPath.HLTPaths = ['HLT1Electron']

