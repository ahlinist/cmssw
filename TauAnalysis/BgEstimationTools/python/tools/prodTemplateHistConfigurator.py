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
        self.branchNames = dict()
        self.numBinsX = dict()
        self.xMin = dict()
        self.xMax = dict()
        self.sequence = None

    def addProcess(self, processName, fileNames):
        self.fileSets[processName] = fileNames

    def addSelection(self, processName, treeSelection):
        self.selections[processName] = treeSelection

    def addTemplate(self, meName, branchName, numBinsX, xMin, xMax):
        self.branchNames[meName] = branchName
        self.numBinsX[meName] = numBinsX
        self.xMin[meName] = xMin
        self.xMax[meName] = xMax

    def configure(self, process):
        for processName_sel, treeSelection in self.selections.items():
            for processName_sample, fileNames in self.fileSets.items():
                module = copy.deepcopy(self.template)
                    
                setattr(module, "fileNames", fileNames)
                setattr(module, "treeSelection", treeSelection)

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
                    setattr(configEntry, "xMin", cms.double(self.xMin[meName]))
                    setattr(configEntry, "xMax", cms.double(self.xMax[meName]))

                    config.append(configEntry)

                setattr(module, "config", config)

                # add module to sequence
                if self.sequence == None:
                    self.sequence = module
                else:
                    self.sequence *= module

        return cms.Sequence(self.sequence)                 
