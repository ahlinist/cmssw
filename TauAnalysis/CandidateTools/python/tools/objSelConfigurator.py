import FWCore.ParameterSet.Config as cms
import sys

from TauAnalysis.CandidateTools.tools.getInstanceName import getInstanceName
from TauAnalysis.CandidateTools.tools.composeModuleName import composeModuleName

#--------------------------------------------------------------------------------
# utility function for generation of sequences
# producing collections of selected pat::Electrons,
# pat::Muons, pat::Taus (and other objects)
#
# Author: Christian Veelken, UC Davis
#
#--------------------------------------------------------------------------------

class objSelConfigurator(cms._ParameterTypeBase):

    def __init__(self, objSelList, src = None, srcAttr = "src", systematics = None,
                 pyModuleName = None, doSelCumulative = True, doSelIndividual = False):
        self.objSelList = objSelList
        self.src = src
        self.srcAttr = srcAttr
        self.systematics = systematics
        self.pyModuleName = pyModuleName,
        self.doSelCumulative = doSelCumulative
        self.doSelIndividual = doSelIndividual

    class _getterCumulative:
        # auxiliary class for composing name of module selecting "cumulative" collection
        @staticmethod
        def get_src(src, lastModuleName):
            if lastModuleName is None:
                return src
            else:
                return cms.InputTag(lastModuleName)
        @staticmethod
        def get_moduleName(name, sysName = None):
            moduleName_base = name
            if sysName is not None:
                moduleName_base = composeModuleName([ name, sysName ])
            return composeModuleName([ moduleName_base, "Cumulative" ])

    class _getterIndividual:
        # auxiliary class for composing name of module selecting "individual" collection
        @staticmethod
        def get_src(src, lastModuleName):
            return src
        @staticmethod
        def get_moduleName(name, sysName = None):
            moduleName_base = name
            if sysName is not None:
                moduleName_base = composeModuleName([ name, sysName ])
            return composeModuleName([ moduleName_base, "Individual" ])

    def _addModule(self, objSelItem, getter, sysName = None, sysInputTag = None, pyNameSpace = None, process = None):
        # create module
        moduleType = objSelItem.type_()
        module = cms.EDFilter(moduleType)

        # set module attributes
        for objSelAttrName in dir(objSelItem):
            objSelAttr = getattr(objSelItem, objSelAttrName)
            if isinstance(objSelAttr, cms._ParameterTypeBase) and not objSelAttrName in ["pluginName", "pluginType"]:
                setattr(module, objSelAttrName, objSelAttr)

        moduleName = getter.get_moduleName(getInstanceName(objSelItem, pyNameSpace, process), sysName)
        module.setLabel(moduleName)        

        src = None
        if sysName is None:
            src = getter.get_src(self.src, self.lastModuleName)
        else:
            src = getter.get_src(sysInputTag, self.lastModuleName)
        if not isinstance(src, cms.InputTag):
            src = cms.InputTag(src)
        setattr(module, self.srcAttr, src)

        # if process object exists, attach module to process object;
        # else register module in global python name-space
        if process is not None:
            setattr(process, moduleName, module)
        else:
            pyModule = sys.modules[self.pyModuleName[0]]
            if pyModule is None:
                raise ValueError("'pyModuleName' Parameter invalid !!")
            setattr(pyModule, moduleName, module)

        self.lastModuleName = moduleName

        # add module to sequence
        if self.sequence is None:
            self.sequence = module
        else:
            self.sequence *= module

    def configure(self, pyNameSpace = None, process = None):
        # configure modules for "cumulative" and "individual" collections
        # of objects passing selection

        if self.src is None:
            raise ValueError("'src' Parameter must not be empty !!")

        self.sequence = None

        if self.doSelCumulative:
            getter = objSelConfigurator._getterCumulative()
            self.lastModuleName = None
            for objSelItem in self.objSelList:
                self._addModule(objSelItem, getter, pyNameSpace = pyNameSpace, process = process)
            if self.systematics is not None:
                for sysName, sysInputTag in self.systematics.items():
                    self.lastModuleName = None
                    for objSelItem in self.objSelList:                        
                        self._addModule(objSelItem, getter, sysName = sysName, sysInputTag = sysInputTag,
                                        pyNameSpace = pyNameSpace, process = process)

        if self.doSelIndividual:
            getter = objSelConfigurator._getterIndividual()
            for objSelItem in self.objSelList:
                self._addModule(objSelItem, getter, pyNameSpace = pyNameSpace, process = process)

        return cms.Sequence(self.sequence)
