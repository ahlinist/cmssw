import FWCore.ParameterSet.Config as cms
import sys

from TauAnalysis.CandidateTools.tools.getInstanceName import getInstanceName
from TauAnalysis.CandidateTools.tools.composeModuleName import composeModuleName

#--------------------------------------------------------------------------------
# utility function for generation of a set of modules
# producing combinations of leptonic and hadronic decay products
# (pairs of pat::Electrons, pat::Muons and pat::Taus)
# for estimation of systematic uncertainties
#
# Author: Christian Veelken, UC Davis
#
#--------------------------------------------------------------------------------

class objProdConfigurator(cms._ParameterTypeBase):

    def __init__(self, objProd, systematics = None, pyModuleName = None):
        self.objProd = objProd
        self.systematics = systematics
        self.pyModuleName = pyModuleName,

    @staticmethod   
    def _recursiveSetAttr(obj, attrName, attrValue):
        if isinstance(attrName, cms._ParameterTypeBase):
            attrName = attrName.value()
        if attrName.find(".") != -1:
            objAttrName = attrName[:attrName.find(".")]
            attrName_new = attrName[attrName.find(".") + 1:]
            if hasattr(obj, objAttrName):
                obj_new = getattr(obj, objAttrName)
            else:
                obj_new = cms.PSet()
                setattr(obj, objAttrName, obj_new)
            objProdConfigurator._recursiveSetAttr(obj_new, attrName_new, attrValue)
        else:
            setattr(obj, attrName, attrValue)

    def _addModule(self, objProdItem, sysName, sysAttributes, pyNameSpace = None, process = None):
        # create module
        moduleType = objProdItem.type_()
        module = cms.EDProducer(moduleType)

        # set module attributes
        # to default values
        for objProdAttrName in dir(objProdItem):
            objProdAttr = getattr(objProdItem, objProdAttrName)
            if isinstance(objProdAttr, cms._ParameterTypeBase) and not objProdAttrName in [ "pluginName", "pluginType" ]:
                if isinstance(objProdAttr, cms.PSet):
                    # CV: need to clone configuration parameters of type cms.PSet,...
                    #     in order to avoid that recursiveSetAttr function
                    #     overwrites objProd "template" object passed to objProdConfigurator constructor !!
                    setattr(module, objProdAttrName, objProdAttr.clone())
                else:
                    setattr(module, objProdAttrName, objProdAttr)

        # set names of source collections
        # to objects shifted in energy/transverse momentum, theta, phi...
        for sysAttrName, sysAttrValue in sysAttributes.items():
            self._recursiveSetAttr(module, sysAttrName, sysAttrValue)
                
        moduleName = composeModuleName([ getInstanceName(objProdItem, pyNameSpace, process), sysName ])
        module.setLabel(moduleName)

        # if process object exists, attach module to process object;
        # else register module in global python name-space
        if process is not None:
            setattr(process, moduleName, module)
        else:
            pyModule = sys.modules[self.pyModuleName[0]]
            if pyModule is None:
                raise ValueError("'pyModuleName' Parameter invalid !!")
            setattr(pyModule, moduleName, module)

        # add module to sequence
        if self.sequence == None:
            self.sequence = module
        else:
            self.sequence *= module

    def configure(self, pyNameSpace = None, process = None):
        # configure set of modules
        # producing different combinations of leptonic and hadronic decay products
        # for estimation of systematic uncertainties

        # add original module (for production of central value) to sequence
        self.sequence = self.objProd

        if self.systematics is not None:
            for sysName, sysAttributes in self.systematics.items():
                self._addModule(self.objProd, sysName = sysName, sysAttributes = sysAttributes,
                                pyNameSpace = pyNameSpace, process = process)

        return cms.Sequence(self.sequence)
