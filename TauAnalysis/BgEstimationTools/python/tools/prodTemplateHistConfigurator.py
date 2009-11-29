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
        self.numBins = dict()
        self.binEdges = dict()
        self.sequence = None

    def addProcess(self, processName, fileNames):
        self.fileSets[processName] = fileNames

    def addSelection(self, processName, treeSelection, kineEventReweight = None):
        self.selections[processName] = treeSelection
        self.kineEventReweights[processName] = kineEventReweight

    @staticmethod
    def _getBinEdges(numBins, min, max):

        binEdges = []

        binWidth = (max - min)/numBins
        
        for iBin in range(0,numBins + 1):
            binEdge = min + (iBin)*binWidth
            binEdges.append(binEdge)

        return binEdges

    def addTemplate(self, meName,
                    branchName, numBins, min = None, max = None, binEdges = None):
        # check that either min and max or binEdges parameters (but not all) are specified
        if (min is None or max is None) and binEdges is None:
            raise ValueError("Definition of either 'min' and 'max' Parameters or 'binEdges' Parameter missing !!")
        if (min is not None or max is not None) and binEdges is not None:
            raise ValueError("Duplicate definition of 'min', 'max' and 'binEdges' Parameters !!")
        
        self.branchNames[meName] = branchName
        self.numBins[meName] = [ numBins, ]
        if min is not None and max is not None:
            self.binEdges[meName] = [ getBinEdges(numBins, min, max), ]
        if binEdges is not None:
            self.binEdges[meName] = [ binEdges, ]

    def addTemplate(self, meName,
                    branchNameX, numBinsX, xMin = None, xMax = None, binEdgesX = None,
                    branchNameY, numBinsY, yMin = None, yMax = None, binEdgesY = None):
        # check that either xMin and xMax or binEdgesX parameters (but not all) are specified
        if (xMin is None or xMax is None) and binEdgesX is None:
            raise ValueError("Definition of either 'xMin' and 'xMax' Parameters or 'binEdgesX' Parameter missing !!")
        if (xMin is not None or xMax is not None) and binEdgesX is not None:
            raise ValueError("Duplicate definition of 'xMin', 'xMax' and 'binEdgesX' Parameters !!")
        if (yMin is None or yMax is None) and binEdgesY is None:
            raise ValueError("Definition of either 'yMin' and 'yMax' Parameters or 'binEdgesY' Parameter missing !!")
        if (yMin is not None or yMax is not None) and binEdgesY is not None:
            raise ValueError("Duplicate definition of 'yMin', 'yMax' and 'binEdgesY' Parameters !!")
        
        self.branchNames[meName] = [ branchNameX, branchNameY ]  
        self.numBins[meName] = [ numBinsX, numBinsY ]
        if xMin is not None and xMax is not None:
            self.binEdges[meName] = [ getBinEdges(numBinsX, xMin, xMax), ]
        if binEdges is not None:
            self.binEdges[meName] = [ binEdgesX, ]
        if yMin is not None and yMax is not None:
            self.binEdges[meName].append(getBinEdges(numBinsY, yMin, yMax))
        if binEdges is not None:
            self.binEdges[meName].append(binEdgesY)

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
                
                for meName in self.branchNames.keys():

                    configEntry = cms.PSet()

                    setattr(configEntry, "meName", cms.string(dqmDirectory_process + "/" + meName))

                    numVariables = len(self.branchNames[meName])
                    if numVariables > 2:
                        ValueError("Maximum supported dimensionality is two right now !!")

                    for iVariable in range(len(self.branchNames[meName])):

                        varEntry = cms.PSet()
                        
                        setattr(varEntry, "branchName", cms.string(self.branchNames[meName][iVariable]))
                        setattr(varEntry, "numBins", cms.uint32(self.numBins[meName][iVariable]))
                        setattr(varEntry, "binEdges", cms.vdouble(self.binEdges[meName][iVariable]))

                        varEntryName = None
                        if iVariable == 1:
                            varEntryName = "x"
                        elif iVariable == 2:
                            varEntryName = "y"
                        elif iVariable == 3:
                            varEntryName = "z"    
                        setattr(configEntry, "varEntryName", varEntry)

                    config.append(configEntry)

                setattr(module, "config", config)

                # add module to sequence
                if self.sequence == None:
                    self.sequence = module
                else:
                    self.sequence *= module

        return cms.Sequence(self.sequence)                 

def makeTemplateHistProdSequence(process, moduleTemplate, dataSets, bgEstEventSelections, kineEventReweights,
                                 dqmDirectory, meName,
                                 branchNames, numBins, min = None, max = None, binEdges = None):
        
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
        configurator.addTemplate(meName,
                                 branchName, numBins, min = min, max = max, binEdges = binEdges)

        module = configurator.configure(process)
        setattr(process, moduleName, module)

        if isFirstModule:
            sequence = cms.Sequence( module )
            isFirstModule = False
        else:    
            sequence._seq = sequence._seq * module

    return sequence

def makeTemplateHistProdSequence(process, moduleTemplate, dataSets, bgEstEventSelections, kineEventReweights,
                                 dqmDirectory, meName,
                                 branchNamesX, numBinsX, xMin = None, xMax = None, binEdgesX = None,
                                 branchNamesY, numBinsY, yMin = None, yMax = None, binEdgesY = None):

    sequence = None
    isFirstModule = True

    if len(branchNamesX) != len(branchNamesY):
        raise ValueError("Mismatch in number of entries in 'branchNamesX' and 'branchNamesY' Parameters !!")

    for processName in branchNamesX.keys():
 
        moduleName = "prodTemplateHistBgEst" + processName + "Enriched" + "_" + meName

        configurator = prodTemplateHistConfigurator(
            moduleName, moduleTemplate, dqmDirectory
        )

        for dataSetName, fileNames in dataSets.items():
            configurator.addProcess(dataSetName, fileNames)

        bgEstEventSelection = bgEstEventSelections[processName]
        kineEventReweight = kineEventReweights[processName]
        configurator.addSelection(processName, bgEstEventSelection, kineEventReweight)

        branchNameX = branchNamesX[processName]
        branchNameY = branchNamesY[processName]
        configurator.addTemplate(meName,
                                 branchNameX, numBinsX, xMin = xMin, xMax = xMax, binEdgesX = binEdgesX,
                                 branchNameY, numBinsY, yMin = yMin, yMax = yMax, binEdgesY = binEdgesY)

        module = configurator.configure(process)
        setattr(process, moduleName, module)

        if isFirstModule:
            sequence = cms.Sequence( module )
            isFirstModule = False
        else:    
            sequence._seq = sequence._seq * module

    return sequence
