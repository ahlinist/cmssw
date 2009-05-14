import FWCore.ParameterSet.Config as cms

def addAnalyzer(process,refAnalyzer,filter="",**pars):
    analyzer = refAnalyzer.label()
    seq = "analysis"

    if filter:
        if filter[0] == "!":
            analyzer += "_Not" + filter[1:]
            seq += "_Not" + filter[1:]
        else:
            analyzer += "_" + filter
            seq += "_" + filter
    for key in pars:
        analyzer += "_" + pars[key]
        seq += "_" + pars[key]

    setattr(process,analyzer,refAnalyzer.clone(**pars))

    if filter:
        if filter[0] == "!":
            setattr(process,seq,cms.Sequence(~getattr(process,filter[1:])*getattr(process,analyzer)))
        else:
            setattr(process,seq,cms.Sequence(getattr(process,filter)*getattr(process,analyzer)))
    else:
        setattr(process,seq,cms.Sequence(getattr(process,analyzer)))

    return getattr(process,seq)

def addPath(process,sequence):
    path = sequence.label() + "_step"
    setattr(process,path,cms.Path(sequence))

def makeAnalysis(process,refAnalyzer='edmDumpAnalysis',attributes={},filters=[]):
    analyzer = getattr(process,refAnalyzer)
    process.analysis = cms.Sequence(analyzer)
    addPath(process,process.analysis)

    for attr in attributes:
        for item in attributes[attr]:
            replace = {attr:item}
            addPath(process,addAnalyzer(process,analyzer,**replace))

    for item in filters:
        addPath(process,addAnalyzer(process,analyzer,item))
