import FWCore.ParameterSet.Config as cms
import sys
import copy

#--------------------------------------------------------------------------------
# utility function for configuring drawJobs
# displaying template histograms including and excluding background contamination
# compared to distributions expected after all selection criteria
# of the final analysis are applied
#
# Author: Christian Veelken, UC Davis
#
#--------------------------------------------------------------------------------

class drawTemplateHistConfigurator(cms._ParameterTypeBase):

    def __init__(self, template):        
        self.template = template
        
        self.meNames = dict()
        self.title = dict()
        
        self.pset = cms.PSet()

    def add(self, meNames, name, title):
        
        self.meNames[name] = meNames
        self.title[name] = title

    def configure(self):
        for name, meNames in self.meNames.items():
            drawJob = copy.deepcopy(self.template)

            for iMonitorElement in range(len(meNames)):
                drawJob.plots[iMonitorElement].dqmMonitorElements = cms.vstring(meNames[iMonitorElement])
            drawJob.title = self.title[name]

            # add drawJob to configuration parameter set
            setattr(self.pset, name, drawJob)

        return self.pset
