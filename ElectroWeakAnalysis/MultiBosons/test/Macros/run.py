# ===============
# includes
# ===============

import ROOT, time, os, math, getopt, sys
execfile("helpers.py")
execfile("Main.py")
#execfile("plotHistos.py")

os.system("source clean.sh")
# ===============
# list of nTuples
# ===============
filesToRun=[]
filesToRun.append("rfio:/castor/cern.ch/user/p/poter/VGamma_Ntuples/CMSSW357_Jun04_Skim/35x_Wgamma_Pythia.root")
filesToRun.append("rfio:/castor/cern.ch/user/p/poter/VGamma_Ntuples/CMSSW357_Jun04_Skim/35x_Zgamma_Pythia.root")
#filesToRun.append("rfio:/castor/cern.ch/user/p/poter/VGamma_Ntuples/CMSSW357_Jun04_Skim/35x_WW_Pythia.root")
#filesToRun.append("rfio:/castor/cern.ch/user/p/poter/VGamma_Ntuples/CMSSW357_Jun04_Skim/35x_WZ_Pythia.root")
#filesToRun.append("rfio:/castor/cern.ch/user/p/poter/VGamma_Ntuples/CMSSW357_Jun04_Skim/35x_ZZ_Pythia.root")
#filesToRun.append("rfio:/castor/cern.ch/user/p/poter/VGamma_Ntuples/CMSSW357_Jun04_Skim/35x_Wenu_Pythia.root")
#filesToRun.append("rfio:/castor/cern.ch/user/p/poter/VGamma_Ntuples/CMSSW357_Jun04_Skim/35x_Zee_Pythia.root")
#filesToRun.append("rfio:/castor/cern.ch/user/p/poter/VGamma_Ntuples/CMSSW357_Jun04_Skim/35x_TTbar_Pythia.root")

for fileToRun in filesToRun:
    Main(fileToRun)

#plotHistos()




# IGNORE
# TAB COMPLETION
try:
    import readline
except ImportError:
    print "Module readline not available."
else:
    import rlcompleter
    readline.parse_and_bind("tab: complete")
    
