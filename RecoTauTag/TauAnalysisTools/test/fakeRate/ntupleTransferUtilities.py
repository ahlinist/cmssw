#!/usr/bin/env python

'''

Scripts for transferring Tau Fake Rate ntuples between CERN/FNAL

Author: Evan Friis

'''

from optparse import OptionParser
import tarfile
import os
import glob
import shutil
import subprocess

DATA_SAMPLES = ['Dijet120_170', 'Dijet20_30', 'Dijet30_50', 'Dijet50_80',
                'Dijet80_120', 'Ztautau', 'WJets', 'InclusiveMu15']

def makeArchives(location='/tmp/fakeRates.tar.bz2', samples=tuple(DATA_SAMPLES)):
    " Archive all Ntuple result files into a tar file"
    tar = tarfile.open(location, 'w:bz2')
    for sample in samples:
        for to_add in glob.glob(os.path.join(sample, "*")):
            print "Adding:", to_add
            tar.add(to_add)
    tar.close()

def transferArchives(local_copies='/tmp/fakeRates.tar.bz2', host='lxplus301.cern.ch'):
    " Transfer a file via scp "
    proc = subprocess.Popen(['scp', local_copies, '%s:%s' % (host, '/tmp/fakeRates.tar')])
    status = os.waitpid(proc.pid, 0)
    print "Completed with status: %s" % status

if __name__=="__main__":
    #makeArchives()
    parser = OptionParser()
    parser.add_option('-a', '--action')
    (options, args) = parser.parse_args()
    if options.action == 'make':
        makeArchives()
    if options.action == 'transfer':
        transferArchives()




    



