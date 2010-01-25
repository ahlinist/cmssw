import FWCore.ParameterSet.Config as cms

def getCutName(cut):
    if ( isinstance(cut, cms.PSet) ):
        return getattr(cut, "pluginName").value()
    elif ( isinstance(cut, cms.string) ):
        return cut.value()
    elif ( isinstance(cut, str) ):            
        return cut
    else:
        raise ValueError("Invalid type for 'cut' Parameter !!")

def composeSubDirectoryName(afterCut = None, beforeCut = None):
    # auxiliary function to compose name of dqmSubDirectory
    # in which histograms filled after applying afterCut,
    # but before applying beforeCut, are stored

    dqmSubDirectory = ""
    if afterCut is not None:
        afterCut_name = getCutName(afterCut)
        dqmSubDirectory += "after" + afterCut_name[0:1].capitalize() + afterCut_name[1:]
    if beforeCut is not None:
        beforeCut_name = getCutName(beforeCut)
        if dqmSubDirectory != "":
            dqmSubDirectory += "_"
        dqmSubDirectory += "before" + beforeCut_name[0:1].capitalize() + beforeCut_name[1:]

    return dqmSubDirectory;
