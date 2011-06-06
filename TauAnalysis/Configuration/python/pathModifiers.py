import FWCore.ParameterSet.Config as cms

class Scanner(object):
    """Class to scan a sequence and give a list of analyzer used and a list of their names"""
    def __init__(self):
        self._analyzerRef = []
    def enter(self,visitee):
        self._analyzerRef.append(visitee)
    def modules(self):
        return self._analyzerRef
    def leave(self, visitee):
        pass

def strMatch(test,searchList):
    for search in searchList:
        if test.lower().find(search.lower()) == -1:
            return False
    return True

def PathRemover(path,serchLabel,verbose = False):
    "PathRemover(path,serchLabel,verbose = False) --> None \nPerforms a case insensitive search on the modules contained in the path whose label match with the list serchLabel (the label must contain each list entry). Modules that match are removed.\nThe procedure is case-insensitive"
    scanner = Scanner()
    path.visit(scanner)
    removed = 0
    notRem = 0
    for module in scanner.modules():
        #        if (type(module) is cms.EDAnalyzer) or (type(module) is cms.EDFilter) or (type(module) is cms.EDProducer):
        if hasattr(module,'_Labelable__label'):
            if strMatch(getattr(module,'_Labelable__label'),serchLabel):
                if path.remove(module):
                    removed += 1
                else:
                    notRem += 1
    if verbose:
        scannerEnd = Scanner()
        path.visit(scannerEnd)
        print "###############################################"
        print "PathRemover Summary:"
        if hasattr(path,'_Labelable__label'):
            print "    path label                  :   ",
            print getattr(path,'_Labelable__label')
        print "    labels searched             :   ",
        print serchLabel
        print "    path lenght at beginning    : %i" % len(scanner.modules())
        print "    modules deleted             : %i" % removed
        print "    modules impossible to delete: %i" % notRem
        print "    path lenght at end          : %i" % len(scannerEnd.modules())
        print "###############################################"
    
def ExtractorAddColumn(pset,pluginType, columnName,columnAttr,verbose = False):
    '''ExtractorAddColumn(pset,pluginType, columnName,columnAttr,verbose = False) --> None
    This macro runs over a certain type of extractor and adds a column if it is not there yet'''
    modified = 0
    alreadyMod = 0
    unable = 0
    for psetName in pset.parameterNames_():
        module = getattr(pset,psetName)
        if hasattr(module,'pluginType'):
            if getattr(module,'pluginType').value() == pluginType:
                if hasattr(module,'columns'):
                    if not hasattr(module.columns,columnName):
                        setattr(module.columns,columnName,columnAttr)
                        if hasattr(module.columns,columnName):
                            modified += 1
                        else:
                            unable += 1
                    else:
                        alreadyMod += 1
    if verbose:
        print "###############################################"
        print "ExtractorAddColumn Summary:"
        if hasattr(pset,'_Labelable__label'):
            print "    path label                    :   ",
            print getattr(pset,'_Labelable__label')
        print "    Plugin Searched               : "+pluginType
        print "    column added                  : " + columnName
        print "    modules modified              : %i" % modified
        print "    modules unable to modify      : %i" % unable
        print "    modules w/ already the column : %i" % alreadyMod
        print "###############################################"
    
def PathModifier(path,serchAttr, value, verbose = False):
    " PathModifier(path,serchAttr, value, verbose = False) --> None \nSearches in the modules inserted in a path for a an attribute name and sets it to value"
    scanner = Scanner()
    path.visit(scanner)
    modified = 0
    for module in scanner.modules():
        #        if (type(module) is cms.EDAnalyzer) or (type(module) is cms.EDFilter) or (type(module) is cms.EDProducer):
        if hasattr(module,serchAttr):
            setattr(module,serchAttr,value)
            modified += 1
    if verbose:
        print "###############################################"
        print "PathModifier Summary:"
        if hasattr(path,'_Labelable__label'):
            print "    path label                  :   ",
            print getattr(path,'_Labelable__label')
        print "    labels searched             :   ",
        print serchAttr
        print "    path lenght at beginning    : %i" % len(scanner.modules())
        print "    modules deleted             : %i" % modified
        print "###############################################"
