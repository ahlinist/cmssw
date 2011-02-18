import FWCore.ParameterSet.Config as cms

def getPSetAttributes(object):

    attributes = []

    for attribute in dir(object):

        # check that "attribute" is not an internal attribute or method of cms.PSet
        isInternalAttribute = False

        for classAttribute in dir(cms.PSet):
            if attribute == classAttribute:
                isInternalAttribute = True
        if attribute.startswith("_"):
            isInternalAttribute = True

        if not isInternalAttribute:
            attributes.append(attribute)

    return attributes
