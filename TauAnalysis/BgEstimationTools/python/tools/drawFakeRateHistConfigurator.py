import FWCore.ParameterSet.Config as cms
import sys
import copy

from TauAnalysis.DQMTools.drawJobConfigurator import *

#--------------------------------------------------------------------------------
# utility function for configuring drawJobs
# displaying histograms obtained by replacing cuts on tau id. disriminators
# by event weights based on fake-rates
# compared to distributions expected after all selection criteria
# of the final analysis (including cuts on tau id. discriminators) are applied
#
# Author: Christian Veelken, UC Davis
#
#--------------------------------------------------------------------------------

class drawFakeRateHistConfigurator(cms._ParameterTypeBase):

    def __init__(self, template, dqmDirectory):
        self.template = template
        self.dqmDirectory = dqmDirectory        
        self.dqmDirectories_type = dict()
        self.dqmDirectories_process = dict()
        self.plots_afterCut = []
        self.plots_beforeCut = []
        self.plots_configEntry = []
        self.pset = cms.PSet()

    def addProcess(self, process, dqmDirectory):
        self.dqmDirectories_process[process] = dqmDirectory.value()

        if not self.dqmDirectories_process[process].endswith("/"):
            self.dqmDirectories_process[process] += "/"

    def addPlots(self, afterCut = None, beforeCut = None, plot = None, plots = None):
        
        # check validity of parameters passed as function arguments
        if self.template is None:
            raise ValueError("Invalid 'template' Parameter !!")
        if plot is None and plots is None:
            raise ValueError("Invalid 'plot' and 'plots' Parameters !!")
        if afterCut is None and beforeCut is None:
            raise ValueError("Invalid 'afterCuts' and 'beforeCuts' Parameters !!")

        # check if need to call recursively
        # in case of multiple plots
        if plots is not None:
            for plot in plots:
                self.addPlots(afterCut = afterCut, beforeCut = beforeCut, plot = plot)
            return

        self.plots_afterCut.append(afterCut)
        self.plots_beforeCut.append(beforeCut)
        self.plots_configEntry.append(plot)

    def configure(self):

        dqmSubDirectoryStructure = self.dqmDirectory.split("/", 1)
        dqmDirectory_part01 = dqmSubDirectoryStructure[0]
        if not dqmDirectory_part01.endswith("/"):
            dqmDirectory_part01 += "/"
        dqmDirectory_part02 = dqmSubDirectoryStructure[1]
        if not dqmDirectory_part02.endswith("/"):
            dqmDirectory_part02 += "/"
        
        for processName, dqmDirectory_process in self.dqmDirectories_process.items():

            drawJobConfigurator_process = drawJobConfigurator(
                template = self.template,
                dqmDirectory = dqmDirectory_part01 + dqmDirectory_process + dqmDirectory_part02
            )
            
            for iPlot in range(len(self.plots_configEntry)):
                drawJobConfigEntry_plot = copy.deepcopy(self.plots_configEntry[iPlot])

                name_orig = getattr(self.plots_configEntry[iPlot], "name")
                name_mod = name_orig.replace("#PROCESSNAME#", processName)
                setattr(drawJobConfigEntry_plot, "name", name_mod)
                
                drawJobConfigurator_process.add(
                    afterCut = self.plots_afterCut[iPlot],
                    beforeCut = self.plots_beforeCut[iPlot],
                    plot = drawJobConfigEntry_plot
                )

            drawJobConfig_process = drawJobConfigurator_process.configure()

            for drawJobConfigEntryName_process in dir(drawJobConfig_process):            
                drawJobConfigEntry_process = getattr(drawJobConfig_process, drawJobConfigEntryName_process)

                if isinstance(drawJobConfigEntry_process, cms.PSet):
                    setattr(self.pset, drawJobConfigEntryName_process, drawJobConfigEntry_process)

        return self.pset
    
