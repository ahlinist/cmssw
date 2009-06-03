import FWCore.ParameterSet.Config as cms

def addAnalyzer(process,refAnalyzer,refSequence=None,filter="",**pars):
    analyzer = refAnalyzer.label()
    if filter:
        if filter[0] == "!":
            analyzer += "_Not" + filter[1:]
        else:
            analyzer += "_" + filter
    if refSequence:
        analyzer += "_" + refSequence.label()
    for key in pars:
        analyzer += "_" + key + "_" + str(pars[key])

    if not hasattr(process,analyzer):
        setattr(process,analyzer,refAnalyzer.clone(**pars))

    seq = "analysis_" + analyzer
    sequence = cms.Sequence(getattr(process,analyzer))
    filter_seq = None
    if filter:
        if filter[0] == "!":
            filter_seq = cms.Sequence(~getattr(process,filter[1:]))
        else:
            filter_seq = cms.Sequence(getattr(process,filter))
  
    if refSequence: sequence = cms.Sequence(refSequence*sequence)
    if filter_seq: sequence = cms.Sequence(filter_seq*sequence) 

    setattr(process,seq,sequence)

    return getattr(process,seq)

def addPath(process,sequence):
    path = sequence.label() + "_step"
    setattr(process,path,cms.Path(sequence))

def makeAnalysis(process,refAnalyzer='edmDumpAnalysis',refSequence=None,attributes=[],filters=[]):
    analyzer = getattr(process,refAnalyzer)
    addPath(process,addAnalyzer(process,analyzer,refSequence))

    for replace in attributes:
        addPath(process,addAnalyzer(process,analyzer,refSequence,**replace))

    for item in filters:
        addPath(process,addAnalyzer(process,analyzer,refSequence,item))
