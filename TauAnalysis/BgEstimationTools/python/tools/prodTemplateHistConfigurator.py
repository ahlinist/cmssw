import FWCore.ParameterSet.Config as cms
import sys
import copy

#--------------------------------------------------------------------------------
# utility function for generation of sequences
# producing template histograms from Ntuple input
#
# Author: Christian Veelken, UC Davis
#
#--------------------------------------------------------------------------------

class prodTemplateHistConfigurator(cms._ParameterTypeBase):

    def __init__(self, moduleName, template, dqmDirectory = None):
        self.moduleName = moduleName
        self.template = template
        self.dqmDirectory = dqmDirectory
        self.fileSets = dict()
        self.selections = dict()
        self.kineEventReweights = dict()
        self.branchNames = dict()
        self.numBinsX = dict()
        self.xMin = dict()
        self.xMax = dict()
        self.xBins = dict()
        self.sequence = None

    def addProcess(self, processName, fileNames):
        self.fileSets[processName] = fileNames

    def addSelection(self, processName, treeSelection, kineEventReweight = None):
        self.selections[processName] = treeSelection
        self.kineEventReweights[processName] = kineEventReweight

    def addTemplate(self, meName, branchName, numBinsX, xMin = None, xMax = None, xBins = None):
        # check that either xMin and xMax or xBins parameters (but not all) are specified
        if (xMin is None or xMax is None) and xBins is None:
            raise ValueError("Definition of either 'xMin' and 'xMax' Parameters or 'xBins' Parameter missing !!")
        if (xMin is not None or xMax is not None) and xBins is not None:
            raise ValueError("Duplicate definition of 'xMin', 'xMax' and 'xBins' Parameters !!")
        
        self.branchNames[meName] = branchName
        self.numBinsX[meName] = numBinsX       
        if xMin is not None and xMax is not None:
            self.xMin[meName] = xMin
            self.xMax[meName] = xMax
        if xBins is not None:
            self.xBins[meName] = xBins

    def configure(self, process):
        for processName_sel, treeSelection in self.selections.items():
            for processName_sample, fileNames in self.fileSets.items():
                module = copy.deepcopy(self.template)
                    
                setattr(module, "fileNames", fileNames)
                setattr(module, "treeSelection", treeSelection)

                if self.kineEventReweights[processName_sel] is not None:
                    branchNamesEventWeight = getattr(module, "branchNamesEventWeight")
                    branchNamesEventWeight.append(self.kineEventReweights[processName_sel])

                moduleName = self.moduleName + "_" + processName_sel + "_"
                dqmDirectory_process = ""
                if self.dqmDirectory is not None:
                    dqmDirectory_process += self.dqmDirectory + "/"
                dqmDirectory_process += processName_sel + "/"
                if processName_sel == processName_sample:
                    moduleName += "pure"
                    dqmDirectory_process += "pure" 
                else:
                    moduleName += processName_sample
                    dqmDirectory_process += processName_sample
                    
                setattr(process, moduleName, module)

                config = cms.VPSet()
                
                for meName, branchName in self.branchNames.items():
                    configEntry = cms.PSet()

                    setattr(configEntry, "meName", cms.string(dqmDirectory_process + "/" + meName))
                    setattr(configEntry, "branchName", cms.string(branchName))
                    setattr(configEntry, "numBinsX", cms.uint32(self.numBinsX[meName]))
                    if self.xMin.get(meName) is not None and self.xMax.get(meName) is not None:
                        setattr(configEntry, "xMin", cms.double(self.xMin[meName]))
                        setattr(configEntry, "xMax", cms.double(self.xMax[meName]))
                    if self.xBins.get(meName) is not None:
                        setattr(configEntry, "xBins", cms.vdouble(self.xBins[meName]))

                    config.append(configEntry)

                setattr(module, "config", config)

                # add module to sequence
                if self.sequence == None:
                    self.sequence = module
                else:
                    self.sequence *= module

        return cms.Sequence(self.sequence)                 

def makeTemplateHistProdSequence(process, moduleTemplate, dataSets, bgEstEventSelections, branchNames, kineEventReweights,
                                 dqmDirectory, meName, numBinsX, xMin = None, xMax = None, xBins = None):
    
    sequence = None
    isFirstModule = True

    for processName, branchName in branchNames.items():
 
        moduleName = "prodTemplateHistBgEst" + processName + "Enriched" + "_" + meName

        configurator = prodTemplateHistConfigurator(
            moduleName, moduleTemplate, dqmDirectory
        )

        for dataSetName, fileNames in dataSets.items():
            configurator.addProcess(dataSetName, fileNames)

        bgEstEventSelection = bgEstEventSelections[processName]
        kineEventReweight = kineEventReweights[processName]
        configurator.addSelection(processName, bgEstEventSelection, kineEventReweight)

        branchName = branchNames[processName]
        configurator.addTemplate(meName, branchName, numBinsX, xMin = xMin, xMax = xMax, xBins = xBins)

        module = configurator.configure(process)
        setattr(process, moduleName, module)

        if isFirstModule:
            sequence = cms.Sequence( module )
            isFirstModule = False
        else:    
            sequence._seq = sequence._seq * module

    return sequence
