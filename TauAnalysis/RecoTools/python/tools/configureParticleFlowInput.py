import FWCore.ParameterSet.Config as cms

def _setInputTag(process, moduleName, srcAttrName, input):
    if hasattr(process, moduleName):
        module = getattr(process, moduleName)
        setattr(module, srcAttrName, input)
    else:    
        print("Error in <setInputTag>: No module of name = " + moduleName + " defined for process !!")

def setParticleFlowTrackInput(process, input, postfix = ""):
    _setInputTag(process, "trackerDrivenElectronSeeds" + postfix, "TkColList", cms.VInputTag(cms.InputTag(input)))
    _setInputTag(process, "trackerOnlyConversions" + postfix, "src", cms.VInputTag(cms.InputTag(input)))
    _setInputTag(process, "generalV0Candidates" + postfix, "trackRecoAlgorithm", cms.InputTag(input))
    _setInputTag(process, "firstnuclearSeed" + postfix, "producer", cms.InputTag(input))
    _setInputTag(process, "firstnuclearInteractionMaker" + postfix, "primaryProducer", cms.InputTag(input))
    _setInputTag(process, "particleFlowDisplacedVertexCandidate" + postfix, "trackCollection", cms.InputTag(input))
    _setInputTag(process, "pfDisplacedTrackerVertex" + postfix, "trackColl", cms.InputTag(input))
    _setInputTag(process, "preIdAnalyzer" + postfix, "TrackCollection", cms.InputTag(input))
