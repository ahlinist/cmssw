import FWCore.ParameterSet.VarParsing as VarParsing

# setup 'standard'  options
options = VarParsing.VarParsing()
options.register ('beamEnergy',
                  100, # default value
                  options.multiplicity.singleton, # singleton or list
                  options.varType.float, # string, int, or float
                  "Beam energy to simulate")

options.register ('kevents',
                  0, # default value
                  options.multiplicity.singleton, # singleton or list
                  options.varType.int, # string, int, or float
                  "Number of events in k to simulate")

options.register ('fileSuffix',
                  '', # default value
                  options.multiplicity.singleton, # singleton or list
                  options.varType.string,          # string, int, or float
                  "Label to append to output file names")

options.register ('notracks',
                  '0', # default value
                  options.multiplicity.singleton, # singleton or list
                  options.varType.int, # string, int, or float
                  "Disable tracking?")

options.register('outputCollections',
                 1,
                 options.multiplicity.singleton,
                 options.varType.int,
                 "Write output collections?")

options.register ('minBeamEnergy',
                  -1,
                  options.multiplicity.singleton,
                  options.varType.float,
                  "Minimum beam energy to simulate")

options.register('copyToTmp',
                '0',
                options.multiplicity.singleton,
                options.varType.int,
                "Copy raw files from CASTOR to tmp first?")

options.register('endcapMode',
                 '0',
                 options.multiplicity.singleton,
                 options.varType.int,
                 "Run in testbeam mode?")

# setup any defaults you want
options.minBeamEnergy = -1
options.beamEnergy = 100
options.kevents = 0
options.fileSuffix = ''
options.notracks = 0
options.copyToTmp = 0
options.endcapMode = 0
options.outputCollections = 1

# get and parse the command line arguments
options.parseArguments()

fileLabel = ''


if options.minBeamEnergy > -1.0:
    fileLabel = fileLabel + str(int(options.minBeamEnergy)) + '_'
else:
    options.minBeamEnergy = options.beamEnergy

fileLabel = fileLabel + str(int(options.beamEnergy)) + "GeV"    

if options.kevents <> 0:
    fileLabel = fileLabel + "_" + str(options.kevents) + "k"
    
if options.notracks <> 0:
    fileLabel = fileLabel + "_notracks"

if options.fileSuffix <> '':
    fileLabel = fileLabel + "_" + options.fileSuffix

logLabel = fileLabel + ".txt"
fileLabel = fileLabel + ".root"


print ("Options:")
print "Min beam energy: " + str(options.minBeamEnergy)
print "Beam energy: " + str(options.beamEnergy)
print "kevents: " + str(options.kevents)
print "fileSuffix: " + options.fileSuffix
print "fileLabel: " + fileLabel
print "logLabel: " + logLabel
print "outputCollections?: " + str(options.outputCollections) 


