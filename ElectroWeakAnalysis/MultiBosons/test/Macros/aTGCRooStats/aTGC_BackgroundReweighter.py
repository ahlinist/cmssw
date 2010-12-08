import sys
import os
from optparse import OptionParser

import ROOT

def main(options,args):

    out = ROOT.TFile.Open(options.output,'RECREATE')

    ROOT.gROOT.ProcessLine('struct TreeContents { Float_t '+options.obsVar+'; Double_t weight; }')

    treecontents = ROOT.TreeContents()

    
        
    outTree = ROOT.TTree(options.treeName,'The Background')
    outTree.Branch(options.obsVar,
                   ROOT.AddressOf(treecontents,options.obsVar),
                   options.obsVar+'/F')
    outTree.Branch('weight',ROOT.AddressOf(treecontents,'weight'),'weight/D')

    for f in args:        
        print f
        currentFile = ROOT.TFile.Open(f)
        currentTree = currentFile.Get(options.treeName)

        ROOT.gROOT.ProcessLine('struct InTreeContents { '+
                               currentTree.GetLeaf(options.obsVar).GetTypeName()+
                               ' '+options.obsVar+'; '+
                               currentTree.GetLeaf('weight').GetTypeName()+' weight; }')
        inTreeContents = ROOT.InTreeContents()
        
        currentTree.SetBranchAddress(options.obsVar,ROOT.AddressOf(inTreeContents,options.obsVar))        
        treecontents.weight = currentTree.GetWeight()*float(options.intLumi)/float(options.inputLumi)

        for i in range(currentTree.GetEntries()):
            currentTree.GetEntry(i)
            setattr(treecontents,options.obsVar,getattr(inTreeContents,options.obsVar))
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
    parser.add_option("--treeName",dest="treeName",help="The name of input TTrees.")
    parser.add_option("--intLumi",dest="intLumi",help="Integrated luminosity to scale to.")
    parser.add_option("--inputLumi",dest="inputLumi",help="The equivalent luminosity of each of the input samples in inverse picobarns, must be same for all.")

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
    if options.inputLumi is None:
        print 'Need to specify --inputLumi'
        miss_options=True
    if options.intLumi is None:
        print 'Need to specify --intLumi'
        miss_options=True
    if len(args) == 0:
        print 'You need to pass at least one root file!'
        miss_options=True

    if miss_options:
        exit(1)

    main(options,args)
