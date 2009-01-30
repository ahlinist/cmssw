#!/usr/bin/env python

import sys, re

def main(argv) :
    """
    Very simple script to get skimming efficiency
    from Trigger Report outputs
    
    Usage:
    getSkimEfficiencyFromLogs <FILELIST>
    e.g.: ./getSkimEfficiencyFromLogs.py LOGDIR/*.log'
    
    """

    # Get file list from argument vector
    alen = len(argv)
    if alen < 1:
        sys.exit()
    FLIST   = argv
                
    ### Define list of filter modules here!!! ###
    Filters =  ['selectedTaus','selectedMuons','muTauPairs','selectedMuTauPairs']

    ### Init counters ###
    NEvents = 0
    NPassed = [0] * len(Filters)

    # Reg. Expr. patterns to find right lines
    fullEntryPattern = re.compile('TrigReport ---------- Modules in Path: muTauSkimPath ----[\t\n\r\f\v\W\w]+TrigReport ------ Modules in End-Path:')
    ppat = 'TrigReport [\W\w]+ '
    p = []
    for i in range(0,len(Filters)):
        p.append(re.compile(ppat+Filters[i]))

    # Loop over all files
    for filen in FLIST:
        input = file(filen)
        fullTxt = input.read() # read full txt
        m0 = fullEntryPattern.search(fullTxt) # extract region
        if (m0):
            lines = re.split('\n',m0.group())
            for line in lines:
                for i in range(0,len(p)):
                    m = p[i].search(line)
                    if (m):
                        tabs = re.split('[\s]+',m.group())
                        if (i==0):
                            NEvents = NEvents + int(tabs[3])
                        NPassed[i] = NPassed[i] + int(tabs[4])            
        input.close()

    print '\n******************************'
    print '*** skimMuTau Efficiencies ***'
    print '******************************'
    print 'Events processed:',NEvents
    print 'Filter                      Passed   Efficiency   Cumul. Efficiency'
    print '---------------------------------------------------------------'
    eff = -1.
    # Treat first entry
    if (float(NEvents!=0)):
        eff = float(NPassed[0])/float(NEvents)
    print '%24s: %8d     %5.3f       %5.3f'%(Filters[0],NPassed[0],float(NPassed[0])/float(NEvents),float(NPassed[0])/float(NEvents))
    for i in range(1,len(Filters)):
        if (float(NPassed[i-1]==0)):
            eff = -1.
        else:
            eff = float(NPassed[i])/float(NPassed[i-1])                
        print '%24s: %8d     %5.3f       %5.3f'%(Filters[i],NPassed[i],eff,float(NPassed[i])/float(NEvents))
    print '---------------------------------------------------------------'
    last = len(Filters)-1
    print '%24s: %8d                 %5.3f'%('Total',NPassed[last],float(NPassed[last])/float(NEvents))
    
    
if __name__ == '__main__' :
    main(sys.argv[1:])

