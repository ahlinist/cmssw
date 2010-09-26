# Starting with a skeleton process which gets imported with the following line
from PhysicsTools.PatAlgos.patTemplate_cfg import *

# let it run
process.p = cms.Path(
    process.patDefaultSequence
    )

process.out.outputCommands+= [
    "keep *_globalMuons_*_*",
    "keep *_generalTracks_*_*",
    "keep *_standAloneMuons_*_*"
    ]
