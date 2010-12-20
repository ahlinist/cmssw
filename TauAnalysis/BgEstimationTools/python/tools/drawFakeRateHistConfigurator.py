import FWCore.ParameterSet.Config as cms
import copy
import os

from TauAnalysis.DQMTools.tools.composeSubDirectoryName import composeSubDirectoryName

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

    def __init__(self, template, dqmDirectories, legendEntries, frTypes):
        self.template = template
        self.dqmDirectories = dqmDirectories
        self.legendEntries = legendEntries
        self.frTypes = frTypes
        self.dqmSubDirectories_process = dict()
        self.plots_afterCut = []
        self.plots_beforeCut = []
        self.plots_configEntry = []
        self.pset = cms.PSet()

    def addProcess(self, process, dqmSubDirectory_process):
        self.dqmSubDirectories_process[process] = dqmSubDirectory_process.value()

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

        self.pset = cms.PSet()

        for processName, dqmSubDirectory_process in self.dqmSubDirectories_process.items():
            for iPlot in range(len(self.plots_configEntry)):

                plot_configEntry = self.plots_configEntry[iPlot]

                drawJobConfig_plots = []

                for frType in self.frTypes:
                    drawJobConfig_plot = cms.PSet()

                    dqmDirectory = self.dqmDirectories[frType]
                    dqmDirectory = dqmDirectory.replace("#PROCESSDIR#", dqmSubDirectory_process)
                    afterCut = self.plots_afterCut[iPlot]
                    beforeCut = self.plots_beforeCut[iPlot]
                    dqmDirectory += '/' + composeSubDirectoryName(afterCut = afterCut, beforeCut = beforeCut)

                    meName_full = os.path.normpath(
                        dqmDirectory + '/' +
                        getattr(plot_configEntry, "meName"))

                    setattr(drawJobConfig_plot, "process", cms.string(frType))
                    setattr(drawJobConfig_plot, "dqmMonitorElements", cms.vstring(meName_full))
                    setattr(drawJobConfig_plot, "drawOptionEntry", cms.string(frType))
                    setattr(drawJobConfig_plot, "legendEntry", cms.string(self.legendEntries[frType]))

                    drawJobConfig_plots.append(drawJobConfig_plot)

                drawJob = copy.deepcopy(self.template)
                setattr(drawJob, "plots", cms.VPSet(drawJobConfig_plots))
                if hasattr(plot_configEntry, "title"):
                    title = getattr(plot_configEntry, "title")
                    setattr(drawJob, "title", cms.string(title))
                if hasattr(plot_configEntry, "PAR"):
                    parameter = getattr(plot_configEntry, "PAR")
                    setattr(drawJob, "parameter", cms.vstring(parameter))

                plot_name = getattr(plot_configEntry, "name")
                drawJobName = plot_name.replace("#PROCESSNAME#", processName)
                setattr(self.pset, drawJobName, drawJob)

        return self.pset

