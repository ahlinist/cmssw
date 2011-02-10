import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# utility function to compose module name by concatenating two strings;
# if the last character of part_1 is lower-case (upper-case),
# capitalize (lowercase) the first character of part_2
#
# Author: Christian Veelken, UC Davis
#
#--------------------------------------------------------------------------------

def composeModuleName(parts):
    if not len(parts) > 0:
        raise ValueError("Empty list !!")
        
    moduleName = ""

    lastPart = None
    for part in parts:

        if part is None or part == "":
            continue
        
        if lastPart is None:
            moduleName += part
            lastPart = part
        else:
            if lastPart[-1].islower() or lastPart[-1].isdigit():
                moduleName += part[0].capitalize() + part[1:]
            else:
                moduleName += part[0].lower() + part[1:]
            lastPart = part    

    return moduleName
    
