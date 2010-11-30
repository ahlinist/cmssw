import sys
import os
from optparse import OptionParser

import ROOT

def main(options,args):

    out = ROOT.TFile.Open(options.output,'RECREATE')

    if options.inputTreeName is None:
        options.inputTreeName = options.treeName

    if options.inputVar is None:
        options.inputVar = options.obsVar


    ROOT.gROOT.ProcessLine('struct InputContents { Double_t '+options.inputVar+';}')

    ROOT.gROOT.ProcessLine('struct TreeContents { Double_t '+options.obsVar+';}')

    treecontents = ROOT.TreeContents()

    inTreeContents = ROOT.InputContents()
        
    outTree = ROOT.TTree(options.treeName,'The Background')
    outTree.Branch(options.obsVar,
                   ROOT.AddressOf(treecontents,options.obsVar),
                   options.obsVar+'/D')

    for f in args:        
        print f
        currentFile = ROOT.TFile.Open(f)
        currentTree = currentFile.Get(options.inputTreeName)
        currentTree.SetBranchAddress(options.inputVar,ROOT.AddressOf(inTreeContents,options.inputVar))
        
        for i in range(currentTree.GetEntries()):
            currentTree.GetEntry(i)
            setattr(treecontents,options.obsVar,getattr(inTreeContents,options.inputVar))
            outTree.Fill()

        currentFile.Close()

    out.cd()
    outTree.Write()    
    out.Close()
    

if __name__ == "__main__":
    parser = OptionParser(description="%prog : A RooStats Implementation of Anomalous Triple Gauge Coupling Analysis.",
                          usage="%prog file1.root file2.root ... --output=<file> --treeName=<name>")
    parser.add_option("--output",dest="output",help="The name of your output file.")
    parser.add_option("--obsVar",dest="obsVar",help="Name of the observable in the TTree.")
    parser.add_option("--inputVar",dest="inputVar",help="Name of the variable in the input tree.")
    parser.add_option("--inputTreeName",dest="inputTreeName",help="Name of input TTree.h")
    parser.add_option("--treeName",dest="treeName",help="The name of input TTrees.")    

    (options,args) = parser.parse_args()

    miss_options = False

    if options.output is None:
        print 'Need to specify --output'
        miss_options=True
    if options.obsVar is None:
        print 'Need to specify --obsVar'
        miss_options=True
    if options.treeName is None:
        print 'Need to specify --treeName'
        miss_options=True    
    if len(args) == 0:
        print 'You need to pass at least one root file!'
        miss_options=True

    if miss_options:
        exit(1)

    main(options,args)
