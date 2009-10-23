import FWCore.ParameterSet.Config as cms
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

    def __init__(self, template, dqmDirectory_prefix, dqmDirectory_suffix):
        self.template = template
        self.dqmDirectory_prefix = dqmDirectory_prefix.value()
        if not self.dqmDirectory_prefix.endswith("/"):
            self.dqmDirectory_prefix += "/"
        self.dqmDirectory_suffix = dqmDirectory_suffix.value()
        if not self.dqmDirectory_suffix.endswith("/"):
            self.dqmDirectory_suffix += "/"
        self.dqmSubDirectories_process = dict()
        self.plots_afterCut = []
        self.plots_beforeCut = []
        self.plots_configEntry = []
        self.pset = cms.PSet()

    def addProcess(self, process, dqmSubDirectory):
        self.dqmSubDirectories_process[process] = dqmSubDirectory.value()

        if not self.dqmSubDirectories_process[process].endswith("/"):
            self.dqmSubDirectories_process[process] += "/"

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

        for processName, dqmSubDirectory_process in self.dqmSubDirectories_process.items():

            drawJobConfigurator_process = drawJobConfigurator(
                template = self.template,
                dqmDirectory = self.dqmDirectory_prefix + dqmSubDirectory_process + self.dqmDirectory_suffix
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
    
