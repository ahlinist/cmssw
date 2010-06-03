# ===============
# includes
# ===============
import ROOT, time, os, math
execfile("helpers.py")
execfile("Main.py")

# ===============
# list of nTuples
# ===============

filesToRun=[]
filesToRun.append("Zee_ptCut_1El_ePho_jetHad.root")
filesToRun.append("Zgamma.root")

for fileToRun in filesToRun:
    Main(fileToRun)






# IGNORE
# TAB COMPLETION
try:
    import readline
except ImportError:
    print "Module readline not available."
else:
    import rlcompleter
    readline.parse_and_bind("tab: complete")
    
