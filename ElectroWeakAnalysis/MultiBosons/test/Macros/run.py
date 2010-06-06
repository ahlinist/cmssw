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
filesToRun.append("rfio:/castor/cern.ch//user/p/poter/VGamma_Ntuples/CMSSW357_Jun04_Skim/35x_Wgamma_Pythia/35x_Wgamma_MC.root")
filesToRun.append("Zgamma.root")
filesToRun.append("Zee.root")
filesToRun.append("Wenu.root")

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
    
