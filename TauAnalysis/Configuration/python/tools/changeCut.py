import FWCore.ParameterSet.Config as cms

def changeCut(process, name, value, attribute = "cut"):

    cutNames = []
    cutNames.append(name + "Cumulative")
    cutNames.append(name + "Individual")

    for cutName in cutNames:
        if not hasattr(process, cutName):
            raise ValueError("No cut = " + cutName + " attached to process object !!")

        cut = getattr(process, cutName)

        if not hasattr(cut, attribute):
            raise ValueError("Attribute = " + attribute + " not defined for cut = " + cutName + " !!")

        setattr(cut, attribute, value)

