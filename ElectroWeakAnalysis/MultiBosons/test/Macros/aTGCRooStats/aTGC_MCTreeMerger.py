import sys
import os
from optparse import OptionParser

import ROOT

def main(options,args):

    out = ROOT.TFile.Open(options.output,'RECREATE')

    ROOT.gROOT.ProcessLine(
        'struct TreeContents {\
        Float_t '+options.obsVar+';\
        Float_t '+options.par1Name+'_grid;\
        Float_t '+options.par2Name+'_grid;\
        Double_t weight;\
        }')

    treecontents = ROOT.TreeContents()

    inTreeContents = ROOT.TreeContents()
        
    outTree = ROOT.TTree(options.treeName,'The Merged Tree')
    outTree.Branch(options.obsVar,
                   ROOT.AddressOf(treecontents,options.obsVar),
                   options.obsVar+'/F')
    outTree.Branch(options.par1Name+'_grid',
                   ROOT.AddressOf(treecontents,options.par1Name+'_grid'),
                   options.par1Name+'_grid/F')
    outTree.Branch(options.par2Name+'_grid',
                   ROOT.AddressOf(treecontents,options.par2Name+'_grid'),
                   options.par2Name+'_grid/F')
    outTree.Branch('weight',ROOT.AddressOf(treecontents,'weight'),'weight/D')

    for f in args:
        coup1= f.split(".root")[0].split(options.par1Name+'_')[-1].split('_')[0]
        coup2= f.split(".root")[0].split(options.par2Name+'_')[-1].split('_')[0]
        print f,'\t',coup1,'\t',coup2
        currentFile = ROOT.TFile.Open(f)
        currentTree = currentFile.Get(options.treeName)
        currentTree.SetBranchAddress(options.obsVar,
                                     ROOT.AddressOf(inTreeContents,options.obsVar))
        setattr(treecontents,options.par1Name+'_grid',float(coup1))
        setattr(treecontents,options.par2Name+'_grid',float(coup2))
        treecontents.weight = currentTree.GetWeight()*float(options.intLumi)/15.0

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
    parser.add_option("--treeName",dest="treeName",help="The name of input TTrees.")
    parser.add_option("--obsVar",dest="obsVar",help="Name of the observable in the TTree.")
    parser.add_option("--intLumi",dest="intLumi",help="Integrated luminosity to scale to.")
    parser.add_option("--par1Name",dest="par1Name",help="Name of aTGC parameter 1")
    parser.add_option("--par2Name",dest="par2Name",help="Name of aTGC parameter 2")
    
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
    if options.intLumi is None:
        print 'Need to specify --intLumi'
        miss_options=True
    if options.par1Name is None:
        print 'Need to specify --par1Name'
        miss_options=True
    if options.par2Name is None:
        print 'Need to specify --par2Name'
        miss_options=True
    if len(args) == 0:
        print 'You need to pass at least one root file!'
        miss_options=True

    if miss_options:
        exit(1)

    main(options,args)
