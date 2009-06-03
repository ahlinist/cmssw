import FWCore.ParameterSet.Config as cms

def addAnalyzer(process,refAnalyzer,filter="",**pars):
    analyzer = refAnalyzer.label()
    if filter:
        if filter[0] == "!":
            analyzer += "_Not" + filter[1:]
        else:
            analyzer += "_" + filter
    for key in pars:
        analyzer += "_" + key + "_" + str(pars[key])

    setattr(process,analyzer,refAnalyzer.clone(**pars))

    seq = "analysis_" + analyzer
    if filter:
        if filter[0] == "!":
            setattr(process,seq,cms.Sequence(~getattr(process,filter[1:])*getattr(process,analyzer)))
        else:
            setattr(process,seq,cms.Sequence(getattr(process,filter)*getattr(process,analyzer)))
    else:
        setattr(process,seq,cms.Sequence(getattr(process,analyzer)))

    return getattr(process,seq)

def addPath(process,sequence,preSeq=None):
    path = None
    if preSeq:
        path = sequence.label() + "_" + preSeq.label() + "_step"
        setattr(process,path,cms.Path(preSeq*sequence))
    else:
        path = sequence.label() + "_step"
        setattr(process,path,cms.Path(sequence))

def makeAnalysis(process,refAnalyzer="edmDumpAnalysis",attributes=[],filters=[],prepend=''):
    analyzer = getattr(process,refAnalyzer)
    seq = "analysis_" + analyzer.label()
    setattr(process,seq,cms.Sequence(analyzer))

    pre_seq = None
    if prepend:
        pre_seq = getattr(process,prepend)
    
    addPath(process,getattr(process,seq),pre_seq)
    for replace in attributes:
        addPath(process,addAnalyzer(process,analyzer,**replace),pre_seq)

    for item in filters:
        addPath(process,addAnalyzer(process,analyzer,item),pre_seq)
