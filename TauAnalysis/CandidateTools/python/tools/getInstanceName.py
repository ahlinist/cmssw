import FWCore.ParameterSet.Config as cms
import sys

#--------------------------------------------------------------------------------
# utility function to access name (label) of module,
# regardless whether it is attached or not yet attached to the process object
#
# Author: Christian Veelken, UC Davis
#
#--------------------------------------------------------------------------------

def getInstanceName(obj, pyNameSpace = None, process = None):
    if process is not None:
        return obj.label()
    else:
        if pyNameSpace is not None:
            for name, ref in pyNameSpace.items():
                if ref is obj : return name
        else:
            for pyModule in sys.modules.values():
                for name, ref in pyModule.__dict__.items():
                    if ref is obj : return name
        return None            
