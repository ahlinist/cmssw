''' 
ntauples_example.py

Author: Evan K. Friis, UC Davis
'''
# Load ntauple tools
from RecoTauTag.TauAnalysisTools.tools.ntauples import *

# Load FWLite libraries (prevents warnings)
from PhysicsTools.PythonAnalysis import *
from ROOT import *
gSystem.Load("libFWCoreFWLite.so")
AutoLibraryLoader.enable()
gROOT.SetBatch(True)

# Single file example
#my_file = TFile("ntuple.root")
#events = my_file.Get("Events")

#Multiple files
events = TChain("Events")
events.AddFile("/afs/cern.ch/user/f/friis/public/ntauple_example/ntuple.root")
#events.AddFile("ntuple_*.root")  etc...

# Get all the ntuples available
ntuple_manager = TauNtupleManager(events)

# Print a list of available ntuples:
ntuple_manager.printNtuples()
''' Example output:

Available ntuples:
        fixedConePFTau
        shrinkingConePFTau

'''

# Select an ntuple to use
shrinking_ntuple = ntuple_manager.shrinkingConePFTau

# Print information about this ntuple and it's available variables
print shrinking_ntuple
''' Example output:

 * Ntuple: shrinkingConePFTau
 * Collection "ref" 
 * ------ pt 
 * Collection "matched" 
 * ------ AgainstElectron 
 * ------ AgainstMuon 
 * ------ ByEcalIsolation 
 * ------ ByIsolation 
 * ------ ByLeadPionPt 
 * ------ ByLeadTrack 
 * ------ ByLeadTrackPt 
 * ------ ByTaNCRaw 
 * ------ ByTaNCfrHalf 
 * ------ ByTaNCfrOne 
 * ------ ByTaNCfrQuarter 
 * ------ ByTaNCfrTenth 
 * ------ ByTrackIsolation 
 * ------ DecayMode 
 * ------ charge 
 * ------ eta 
 * ------ jetEta 
 * ------ jetPt 
 * ------ jetWidth 
 * ------ mass 
 * ------ nTrks 
 * ------ phi 
 * ------ pt 
 * ------ matchedIndex
'''

# Note the special "ref" collection.  This is the collection that was matching source
# and is avaialbe when the colleciton is "matched" by prepending "$ref_" to the variable name.

# Now we know what our ntuples are, the available collections, are and what
# variables we have.  Let's make some plots!

# Set which collection to use.  Our only options right now are "ref" or
# "matched".  Let's used 'matched'.  In this case, these are reco taus that
# were matched to gen-level hadronic taus.
shrinking_ntuple.set_collection("matched")

# Let's draw the pt resolution.
pt_res = draw(
    events,                 # the events branch
    shrinking_ntuple,       # which ntuple to use
    expr="$pt - $ref_pt",   # pt - true pt
    selection="abs($eta) < 2.1", # only look at the eta < 2.1
    output_hist="pt_res",   # If we want to save this histo in the root file.
    binning=(20, 0, 10),    # Same format as THXF
)

c1 = TCanvas()
pt_res.Draw()
pt_res.SetTitle("Pt resolution")
c1.SaveAs("pt_res.png")

# Now let's draw the TaNC efficiency vs. pt, for taus with pt > 15.  The
# efficiency function returns a tuple containing both the efficiency
# TGraphAsymmErrors and a convienent blank histogram to serves as the
# background for the TGraph

bkg, my_eff = efficiency(
                        events, 
                        shrinking_ntuple,
                        expr="$pt",
                        numerator="$ByTaNCfrOne && $pt > 15",
                        denominator="$pt > 15",
                        output_eff="iso_eff",
                        binning=(20, 0, 60))

bkg.Draw()
my_eff.Draw()
c1.SaveAs("pt_eff_TaNC.png")


