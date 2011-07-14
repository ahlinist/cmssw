'''

TauVarParsing

Extend the default options provided by the VarParsing class to include
some common tau related use cases.

Author: Evan K. Friis, UW Madison

See: https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideAboutPythonConfigFile#Passing_Command_Line_Arguments_T

'''
import FWCore.ParameterSet.VarParsing as VarParsing
import Configuration.PyReleaseValidation.autoCond as autoCond

class TauVarParsing(VarParsing.VarParsing):
    '''
    TauVarParsing
    Create an object that will parse the CLI arguments.
    You can additionally specify additional options in the constructor.
    The type of the option will be inferred.

    Example:
    >>> # Create a parser with some extra options and defaults
    >>> parse = TauVarParsing(myIndex=2, myLabel="pippo", myFlag=False)
    >>> parse.myIndex
    2
    >>> parse.myLabel
    'pippo'
    >>> parse.myFlag
    False
    >>> # The default global tag is automatically configured
    >>> 'MC_' in parse.globalTag
    True
    '''
    type_map = {
        str : VarParsing.VarParsing.varType.string,
        bool : VarParsing.VarParsing.varType.int,
        int : VarParsing.VarParsing.varType.int,
        float : VarParsing.VarParsing.varType.float
    }
    def __init__(self, **kwargs):
        # Call the base constructor with the 'analysis' defaults
        super(TauVarParsing, self).__init__('analysis')
        # Now register some extra common cases
        self.register('globalTag',
                      autoCond.autoCond['mc'], # Default value
                      self.multiplicity.singleton,
                      self.varType.string,
                      "Global Tag Conditions")
        self.register('hltProcess',
                      'HLT',
                      self.multiplicity.singleton,
                      self.varType.string,
                      "HLT Process name")
        self.register('processName',
                      'TauAnalysis',
                      self.multiplicity.singleton,
                      self.varType.string,
                      "cms.Process name")
        self.register('saveFinalEvents',
                      'TauAnalysis',
                      self.multiplicity.singleton,
                      self.varType.int,
                      "Save the final events")
        self.register('triggerBits',
                      '',
                      self.multiplicity.list,
                      self.varType.string,
                      "List of trigger bit specifications [RunRange:]Path")

        # Get extra options w/ their defaults
        for key, value in kwargs.iteritems():
            self.register(key,
                          value,
                          self.multiplicity.singleton,
                          self.type_map[type(value)],
                          "Custom arg: %s" % key)

if __name__ == "__main__":
    import doctest
    doctest.testmod()

