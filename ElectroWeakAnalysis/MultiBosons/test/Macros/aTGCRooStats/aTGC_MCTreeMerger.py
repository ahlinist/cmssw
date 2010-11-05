import sys
import os
from optparse import OptionParser

import ROOT

def main(options,args):

    out = ROOT.TFile.Open(options.output,'RECREATE')

    ROOT.gROOT.ProcessLine(
        "struct TreeContents {\
             float photonEt;\
             float h3_3x3;\
             float h4_3x3;\
        }")

    treecontents = ROOT.TreeContents()

    inTreeContents = ROOT.TreeContents()
        
    outTree = ROOT.TTree(options.treeName,'The Merged Tree')
    outTree.Branch('photonEt',ROOT.AddressOf(treecontents,'photonEt'),'photonEt/F')
    outTree.Branch('h3_3x3',ROOT.AddressOf(treecontents,'h3_3x3'),'h3_3x3/F')
    outTree.Branch('h4_3x3',ROOT.AddressOf(treecontents,'h4_3x3'),'h4_3x3/F')

    for f in args:
        coup1= f.split(".root")[0].split("h3_")[-1].split("_")[0]
        coup2= f.split(".root")[0].split("h4_")[-1].split("_")[0]
        print f,'\t',coup1,'\t',coup2
        currentFile = ROOT.TFile.Open(f)
        currentTree = currentFile.Get(options.treeName)
        currentTree.SetBranchAddress('photonEt',ROOT.AddressOf(inTreeContents,'photonEt'))
        treecontents.h3_3x3 = float(coup1)
        treecontents.h4_3x3 = float(coup2)

        for i in range(currentTree.GetEntries()):
            currentTree.GetEntry(i)
            treecontents.photonEt = inTreeContents.photonEt
            outTree.Fill()

        currentFile.Close()

    out.cd()

    outTree.Write()    
    out.Close()
    

if __name__ == "__main__":
    parser = OptionParser(description="%prog : A RooStats Implementation of Anomalous Triple Gauge Coupling Analysis.",
                          usage="%prog file1.root file2.root ... --output=<file> --treeName=<name>")
    parser.add_option("--output",dest="output",help="The name of your output file.")
    parser.add_option("--treeName",dest="treeName",help="The name of input TTrees.")

    (options,args) = parser.parse_args()

    miss_options = False

    if options.output is None:
        print 'Need to specify --output'
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
