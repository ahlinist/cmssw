#!/usr/bin/env python

"""
Takes a path to a castor directory and gets the total number of
events, the total size and the size per event of all root files 
in there. Assumes that all the files are EDM (reads the
TTree "Events") and PyROOT is installed.
Can by used both as a command line tool and python module.
Command line usage: ./sampleSize.py <castor_dir>
Python usage:
  from sampleSize import sampleSize
  events, size, eventSize = sampleSize(castorDir)
""" 

import os, sys, ROOT
from pathutils import formatbytes

def sampleSize(path):
  """
  Takes a path to a castor directory and gets the total number of
  events, the total size and the size per event of all root files 
  in there. Assumes that all the files are EDM (reads the
  TTree "Events") and PyROOT is installed.
  """
  print "Getting sample size for %s." % (path, )

  ROOT.gSystem.Load("libFWCoreFWLite")
  ROOT.AutoLibraryLoader.enable()
  chain = ROOT.TChain("Events")

  size = 0

  files = os.popen("nsls %s | grep root" % path).read().split()
  print "Number of files:", len(files)
  sys.stdout.write("Reading files ... ")
  sys.stdout.flush()

  for f in files:
    chain.Add("rfio:" + path + "/" + f)
    size += int(os.popen( "nsls -l %s/%s" % (path, f) ).read().split()[4])

  events = chain.GetEntries()
  sys.stdout.write("done.\n")
  sys.stdout.flush()

  if events == 0:
    print "ERROR: No events found!"
    eventSize = -1
  else: 
    eventSize = float(size) / events

  print "Total size: %d B (%s)" % ( size, formatbytes(size, nospace=False) )
  print "Total events: ", events
  print "Size per event: ", formatbytes(eventSize, nospace=False)
  sys.stdout.flush()

  return events, size, eventSize

# sampleSize(path) ------------------------------------------------------------------


if __name__ == "__main__":

  if len(sys.argv) < 2:
    sys.exit("Usage: %s <castor_dir_with_root_files>" % sys.argv[0])
  path = sys.argv[1]
  sampleSize(path)

# if __name__ == "__main__" --------------------------------------------------------

