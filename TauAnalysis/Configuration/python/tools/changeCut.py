import FWCore.ParameterSet.Config as cms

from TauAnalysis.RecoTools.objSelConfigurator import *

def changeCut(process, name, value, attribute = "cut"):

    cutNames = []
    cutNames.append(objSelConfigurator._composeModuleName(name, "Cumulative"))
    cutNames.append(objSelConfigurator._composeModuleName(name, "Individual"))

    for cutName in cutNames:
        if not hasattr(process, cutName):
            raise ValueError("No cut = " + cutName + " attached to process object !!")

        cut = getattr(process, cutName)

        if not hasattr(cut, attribute):
            raise ValueError("Attribute = " + attribute + " not defined for cut = " + cutName + " !!")

        setattr(cut, attribute, value)

