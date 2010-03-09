import FWCore.ParameterSet.Config as cms

def replaceStrings(origStringList, origSubString, modSubString):

    modStringList = []

    for origString in origStringList:
        modString = origString.replace(origSubString, modSubString)
        
        modStringList.append(modString)

    return cms.vstring(modStringList)
