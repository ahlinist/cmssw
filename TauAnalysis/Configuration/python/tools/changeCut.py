import FWCore.ParameterSet.Config as cms

from TauAnalysis.CandidateTools.tools.composeModuleName import *

def changeCut(process, name, value, attribute = "cut"):

    cutNames = []
    cutNames.append(composeModuleName(name, "Cumulative"))
    cutNames.append(composeModuleName(name, "Individual"))

    oldValue = 0.
    oldValue_initialized = False

    for cutName in cutNames:
        if not hasattr(process, cutName):
            raise ValueError("No cut = " + cutName + " attached to process object !!")

        cut = getattr(process, cutName)

        if not hasattr(cut, attribute):
            raise ValueError("Attribute = " + attribute + " not defined for cut = " + cutName + " !!")

        # Note: "cumulative" and "individual" cuts both point to the same cut value object;
        #       as a consequence, the cut value actually changes for both
        #       the "cumulative" and "individual" cuts upon the first setattr call
        #      --> need to initialize oldValue variable before first setattr call
        if not oldValue_initialized:
            oldValue = getattr(cut, attribute).value()
        oldValue_initialized = True
            
        setattr(cut, attribute, value)
        
        print("Replaced " + cutName + "." + attribute + ": '" + str(oldValue) + "' => '" + str(value) + "'")
