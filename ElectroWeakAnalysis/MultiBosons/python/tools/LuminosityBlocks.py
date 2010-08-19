import ROOT
import inspect
import sys
import optparse
from FWCore.ParameterSet.VarParsing import VarParsing


ROOT.gSystem.Load("libFWCoreFWLite.so")
ROOT.AutoLibraryLoader.enable()

# Whether warn() should print anythingg
quietWarn = False

def setQuietWarn (quiet = True):
    global quietWarn
    quietWarn = quiet

def warn (*args, **kwargs):
    """print out warning with line number and rest of arguments"""
    if quietWarn: return
    frame = inspect.stack()[1]
    filename = frame[1]
    lineNum  = frame[2]
    #print "after '%s'" % filename
    blankLines = kwargs.get('blankLines', 0)
    if blankLines:
        print '\n' * blankLines
    spaces = kwargs.get('spaces', 0)
    if spaces:
        print ' ' * spaces,
    if len (args):
        print "%s (%s): " % (filename, lineNum),
        for arg in args:
            print arg,
        print
    else:
        print "%s (%s):" % (filename, lineNum)


class LuminosityBlocks:
    """Python interface to FWLite LuminosityBlock class"""

    def __init__(self, inputFiles, **kwargs):
        """inputFiles    => Either a single filename or a list of filenames
        Optional arguments:
        forceEvent  => Use fwlite::Event IF there is only one file
        maxEvents   => Maximum number of events to process
        """        
        self._veryFirstTime      = True
        self._luminosityblock    = 0
        self._lumiCounts         = 0
        self._maxLumis           = 0
        self._forceLumi          = False
        self._mode               = None
        self._secondaryFilenames = None
        if isinstance (inputFiles, list):
            # it's a list
            self._filenames = inputFiles[:]
        elif isinstance(inputFiles, ROOT.TFile):
            self._filenames = inputFiles
        elif isinstance (inputFiles, VarParsing):
            # it's a VarParsing object
            options = inputFiles
            self._maxEvents           = options.maxEvents
            self._filenames           = options.inputFiles
            self._secondaryFilenames  = options.secondaryInputFiles
        else:
            # it's probably a single string
            self._filenames = [inputFiles]
        ##############################
        ## Parse optional arguments ##
        ##############################
        if kwargs.has_key ('maxLumis'):
            self._maxLumis = kwargs['maxLumis']
            del kwargs['maxLumis']
        if kwargs.has_key ('forceLumi'):
            self._forceEvent = kwargs['forceLumi']
            del kwargs['forceLumi']
        if kwargs.has_key ('options'):
            options = kwargs ['options']
            self._maxLumis            = options.maxLumis
            self._filenames           = options.inputFiles
            self._secondaryFilenames  = options.secondaryInputFiles
            del kwargs['options']
        # Since we deleted the options as we used them, that means
        # that kwargs should be empty.  If it's not, that means that
        # somebody passed in an argument that we're not using and we
        # should complain.
        if len (kwargs):
            raise RuntimeError, "Unknown arguments %s" % kwargs
        if not self._filenames:
            raise RuntimeError, "No input files given"


    def to (self, entryIndex):
        """Jumps to lumi in entryIndex"""
        if self._veryFirstTime:
            self._createFWLiteLuminosityBlock()
        self._luminosityblock.to ( long(entryIndex) )

        
    def toBegin (self):
        """Called to reset event loop to first event."""
        self._toBegin = True


    def size (self):
        """Returns number of events"""
        if self._veryFirstTime:
            self._createFWLiteLuminosityBlock()
        return self._luminosityblock.size()


    def luminosityBlockAuxiliary (self):
        """Returns luminosityBlockAuxiliary object"""
        if self._veryFirstTime:
            raise RuntimeError, "luminosityBlockAuxiliary() called before "\
                  "toBegin() or to()"
        return self._luminosityblock.luminosityBlockAuxiliary()


    def object (self):
        """Returns event object"""
        return self._luminosityblock


    def getByLabel (self, *args):
        """Calls FWLite's getByLabel.  Called:
        getByLabel (moduleLabel, handle)
        getByLabel (moduleLabel, productInstanceLabel, handle),
        getByLabel (moduleLabel, productInstanceLabel, processLabel, handle),
        or
        getByLabel ( (mL, pIL,pL), handle)
        """
        if self._veryFirstTime:
            self._createFWLiteLuminosityBlock()        
        length = len (args)
        if length < 2 or length > 4:
            # not called correctly
            raise RuntimeError, "Incorrect number of arguments"
        # handle is always the last argument
        argsList = list (args)
        handle = argsList.pop()
        if len(argsList)==1 and \
               ( isinstance (argsList[0], tuple) or
                 isinstance (argsList[0], list) ) :
            if len (argsList) > 3:
                raise RuntimeError, "getByLabel Error: label tuple has too " \
                      "many arguments '%s'" % argsList[0]
            argsList = list(argsList[0])
        while len(argsList) < 3:
            argsList.append ('')
        (moduleLabel, productInstanceLabel, processLabel) = argsList
        labelString = "'" + "', '".join(argsList) + "'"
        handle._setStatus ( self._luminosityblock.getByLabel( handle._typeInfoGetter(),
                                                              moduleLabel,
                                                              productInstanceLabel,
                                                              processLabel,
                                                              handle._addressOf() ),
                            labelString )
        return handle.isValid()

                    
    def __iter__ (self):
        return self._next()


    def fileIndex (self):
        if self._luminosityblock:
            return self._luminosityblock.fileIndex()
        else:
            # default non-existant value is -1.  Return something else
            return -2


    def secondaryFileIndex (self):
        if self._luminosityblock:
            return self._luminosityblock.secondaryFileIndex()
        else:
            # default non-existant value is -1.  Return something else
            return -2


    def fileIndicies (self):
        return (self.fileIndex(), self.secondaryFileIndex())


    ## Private Member Functions ##


    def _parseOptions (self, options):
        """(Internal) Parse options"""


    def _toBeginCode (self):
        """(Internal) Does actual work of toBegin() call"""
        self._toBegin = False
        self._luminosityblock.toBegin()
        self._lumiCounts = 0


    def __del__ (self):
        """(Internal) Destructor"""
        # print "Goodbye cruel world, I'm leaving you today."
        del self._luminosityblock
        # print "Goodbye, goodbye, goodbye."


    def _createFWLiteLuminosityBlock (self):
        """(Internal) Creates an FWLite LuminosityBlock"""
        self._veryFirstTime = False
        self._toBegin = True
        if isinstance (self._filenames, ROOT.TFile):
            self._luminosityblock = ROOT.fwlite.LuminosityBlock (self._filenames)
            self._mode = 'single'
            return self._mode
        if len (self._filenames) == 1 and self._forceEvent:
            self._tfile = ROOT.TFile.Open (self._filenames[0])
            self._luminosityblock = ROOT.fwlite.LuminosityBlock (self._tfile)
            self._mode = 'single'
            return self._mode
        #        filenamesSVec = ROOT.vector("string") ()
        #        for name in self._filenames:
        #            filenamesSVec.push_back (name)
        #        if self._secondaryFilenames:
        #            secondarySVec =  ROOT.vector("string") ()
        #            for name in self._secondaryFilenames:
        #                secondarySVec.push_back (name)
        #            self._luminosityblock = ROOT.fwlite.MultiChainEvent (filenamesSVec,
        #                                                       secondarySVec)
        #            self._mode = 'multi'
        #        else:
        #            self._luminosityblock = ROOT.fwlite.ChainEvent (filenamesSVec)
        #            self._mode = 'chain'
        return self._mode


    def _next (self):
        """(Internal) Iterator internals"""
        if self._veryFirstTime:
            self._createFWLiteLuminosityBlock()
        if self._toBegin:
            self._toBeginCode()
        while not self._luminosityblock.atEnd() :
            yield self
            self._lumiCounts += 1
            if self._maxLumis > 0 and self._lumiCounts >= self._maxLumis:
                break
            # Have we been asked to go to the first event?
            if self._toBegin:
                self._toBeginCode()
            else:
                # if not, lets go to the next event
                self._luminosityblock.__preinc__()

if __name__ == "__main__":
    pass
