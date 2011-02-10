import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# utility function to get/set attributes recursively,
# e.g.:
#
#   recursiveSetAttr("selectedPatMuonsPFRelIsoCumulative", "chargedHadronIso.ptMin", 1.0)
#
# Author: Christian Veelken, UC Davis
#
#--------------------------------------------------------------------------------

def recursiveGetAttr(obj, attrName):
    if isinstance(attrName, cms._ParameterTypeBase):
        attrName = attrName.value()
    if attrName.find(".") != -1:
        objAttrName = attrName[:attrName.find(".")]
        attrName_new = attrName[attrName.find(".") + 1:]
        if hasattr(obj, objAttrName):
            obj_new = getattr(obj, objAttrName)
        else:
            raise ValueError("Attribute = " + objAttrName + " does not exist !!")
        return recursiveGetAttr(obj_new, attrName_new)
    else:
        return getattr(obj, attrName)

def recursiveSetAttr(obj, attrName, attrValue):
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
        recursiveSetAttr(obj_new, attrName_new, attrValue)
    else:
        setattr(obj, attrName, attrValue)
