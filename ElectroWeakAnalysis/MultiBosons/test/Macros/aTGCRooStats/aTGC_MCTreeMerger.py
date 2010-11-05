import sys
import os
from optparse import OptionParser

import ROOT

def main(options,args):

    out = ROOT.TFile.Open(options.output,'RECREATE')

    photon_et=0.0
    h3=0.0
    h4=0.0
    
    outTree = ROOT.TTree(options.treeName,'The Merged Tree')
    outTree.Branch('photonEt',photon_et,'photonEt/F')
    outTree.Branch('h3_3x3',h3,'h3_3x3/F')
    outTree.Branch('h4_3x3',h4,'h4_3x3/F')

    for f in args:
        coup1= f.split(".root")[0].split("h3_")[-1].split("_")[0]
        coup2= f.split(".root")[0].split("h4_")[-1].split("_")[0]
        print f,' ',coup1,' ',coup2


    outTree.Write()
    outFile.Write()
    outFile.Close()
    

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
