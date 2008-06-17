import FWCore.ParameterSet.Config as cms

import HLTrigger.HLTfilters.hltHighLevel_cfi
SUSYHighPtPhotonHLTPath = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
import HLTrigger.HLTfilters.hltHighLevel_cfi
SUSYControlHighPtPhotonHLTPath = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
SUSYHighPtPhotonHLTPath.HLTPaths = ['HLT_IsoPhoton40_L1R', 'HLT_DoubleIsoPhoton20_L1R', 'HLT_IsoPhoton30_L1I', 'HLT_DoubleIsoPhoton20_L1I']
SUSYControlHighPtPhotonHLTPath.HLTPaths = ['HLT_IsoEle15_L1I']

