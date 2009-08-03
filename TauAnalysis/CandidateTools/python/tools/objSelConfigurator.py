import FWCore.ParameterSet.Config as cms
import sys

#--------------------------------------------------------------------------------
# utility function for generation of sequences
# producing collections of selected pat::Electrons,
# pat::Muons, pat::Taus (and other objects)
#
# Author: Christian Veelken, UC Davis
#
#--------------------------------------------------------------------------------

class objSelConfigurator(cms._ParameterTypeBase):

    def __init__(self, objSelList, src = None, srcAttr = "src", pyModuleName = None, doSelCumulative = True, doSelIndividual = False):
        self.objSelList = objSelList
        self.src = src
        self.srcAttr = srcAttr
        self.pyModuleName = pyModuleName,
        self.doSelCumulative = doSelCumulative
        self.doSelIndividual = doSelIndividual

    @staticmethod
    def _composeModuleName(part_1, part_2):
        # auxiliary function for concatenating two strings;
        # if the last character of part_1 is lower-case (upper-case),
        # capitalize (lowercase) the first character of part_2
        if part_1[-1].islower() or part_1[-1].isdigit():
            return part_1 + part_2.capitalize()
        else:
            return part_1 + part_2[0].lower() + part_2[1:]

    @staticmethod    
    def _getInstanceName(obj, namespace):
        if namespace is not None:
            for name, ref in namespace.items():
                if ref is obj : return name
        else:
            for pyModule in sys.modules.values():
                for name, ref in pyModule.__dict__.items():
                    if ref is obj : return name
        return None            

    class _getterCumulative:
        # auxiliary class for composing name of module selecting "cumulative" collection
        @staticmethod
        def get_src(src, lastModuleName):
            if lastModuleName == None:
                return src
            else:
                return lastModuleName
        @staticmethod
        def get_moduleName(name):
            return objSelConfigurator._composeModuleName(name, "Cumulative")

    class _getterIndividual:
        # auxiliary class for composing name of module selecting "individual" collection
        @staticmethod
        def get_src(src, lastModuleName):
            return src
        @staticmethod
        def get_moduleName(name):
            return objSelConfigurator._composeModuleName(name, "Individual")

    def _addModule(self, objSelItem, namespace, getter):
        # create module
        moduleType = objSelItem.type_()
        module = cms.EDFilter(moduleType)

        # set module attributes
        for objSelAttrName in dir(objSelItem):
            objSelAttr = getattr(objSelItem, objSelAttrName)
            if isinstance(objSelAttr, cms._ParameterTypeBase) and not objSelAttrName in ["pluginName", "pluginType"]:
                setattr(module, objSelAttrName, objSelAttr)
        src = getter.get_src(self.src, self.lastModuleName)
        setattr(module, self.srcAttr, cms.InputTag(src))
        
        moduleName = getter.get_moduleName(self._getInstanceName(objSelItem, namespace))
        module.setLabel(moduleName)
               
        # register module in global python name-space
        pyModule = sys.modules[self.pyModuleName[0]]
        if pyModule is None:
            raise ValueError("'pyModuleName' Parameter invalid !!")
        setattr(pyModule, moduleName, module)
        
        self.lastModuleName = moduleName

        # add module to sequence
        if self.sequence == None:
            self.sequence = module
        else:
            self.sequence *= module

    def configure(self, namespace = None):
        # configure modules for "cumulative" and "individual" collections
        # of objects passing selection

        if self.src is None:
            raise ValueError("'src' Parameter must not be empty !!")

        self.sequence = None
        self.lastModuleName = None

        if self.doSelCumulative:
            getter = objSelConfigurator._getterCumulative()
            for objSelItem in self.objSelList:
                self._addModule(objSelItem, namespace, getter)

        if self.doSelIndividual:
            getter = objSelConfigurator._getterIndividual()
            for objSelItem in self.objSelList:
                self._addModule(objSelItem, namespace, getter)

        return cms.Sequence(self.sequence)
