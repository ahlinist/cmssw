import FWCore.ParameterSet.Config as cms

import HLTrigger.HLTfilters.hltHighLevel_cfi
SUSYHighPtPhotonHLTPath = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
import HLTrigger.HLTfilters.hltHighLevel_cfi
SUSYControlHighPtPhotonHLTPath = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
SUSYHighPtPhotonHLTPath.HLTPaths = ['HLT1PhotonRelaxed', 'HLT2PhotonRelaxed', 'HLT1Photon', 'HLT2Photon']
SUSYControlHighPtPhotonHLTPath.HLTPaths = ['HLT1Electron']

