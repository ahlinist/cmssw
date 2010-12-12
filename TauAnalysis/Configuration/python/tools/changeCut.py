import FWCore.ParameterSet.Config as cms

from TauAnalysis.CandidateTools.tools.composeModuleName import *

def changeCut(process, name, value, attribute = "cut"):

    cutNames = []
    cutNames.append(name)
    cutNames.append(composeModuleName([ name, "Cumulative" ]))
    cutNames.append(composeModuleName([ name, "Individual" ]))

    oldValue = None
    oldValue_initialized = False

    numCutsChanged = 0

    for cutName in cutNames:
        if hasattr(process, cutName):
             cut = getattr(process, cutName)

             # Note: "cumulative" and "individual" cuts both point to the same cut value object;
             #       as a consequence, the cut value actually changes for both
             #       the "cumulative" and "individual" cuts upon the first setattr call
             #      --> need to initialize oldValue variable before first setattr call
             if not oldValue_initialized:
                 oldValue = getattr(cut, attribute).value()
             oldValue_initialized = True
            
             setattr(cut, attribute, value)

             numCutsChanged += 1

    if numCutsChanged > 0:
        print("Replaced " + cutName + "." + attribute + ": '" + str(oldValue) + "' => '" + str(value) + "'")
    else:
        raise ValueError("No cut = " + cutName + " attached to process object !!")
