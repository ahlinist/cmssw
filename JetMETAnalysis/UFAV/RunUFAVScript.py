#!/usr/bin/env python

import sys
import os
import re
import string
from optparse import OptionParser










#######################################################


if __name__=="__main__":

        parser=OptionParser()
        parser.add_option("-r","--run",
                          dest="run",
                          default=[],
                          action="append",
                          type="int",
                          help="Specify run to be processed")
        parser.add_option("-f","--ref",
                          dest="ref",
                          default=None,
                          type=int,
                          help="Specify reference run")
        parser.add_option("-R","--runlistfile",
                          dest="runlistfile",
                          default=None,
                          help="Specify list of runs to be processed")
	parser.add_option("-m","--runRangeMode",
			  default=False,
			  dest="runRangeMode",
			  action="store_true",
			  help="If set true, all runs in the range [min(run), max(run)] will be evaluated.")
        parser.add_option("-v","--verbose",
                          dest="verbose",
                          default=False,
                          action="store_true",
                          help="print debug information")
        parser.add_option("-c","--copy",
                          dest="copy",
                          action="store_true",
                          default=False,
                          help="If active, will scp output directory to Baylor at hep06-cmsjmet:/www/htdocs/cmsjmet/JetMETDQM/Commissioning10")
        parser.add_option("-H","--Help",
                          dest="Help",
                          action="store_true",
                          default=False,
                          help="alternate help action")

	parser.add_option("-b","--basedir",
			  dest=basedir,
			  default="/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/data/OfflineData/Run2010/StreamExpress",
			  help="Set base directory where files can be found.  Default is /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/data/OfflineData/Run2010/StreamExpress")
        
        (options,args)=parser.parse_args()
        if options.Help==True:
		parser.print_help()
		sys.exit()

	ref=options.ref
	runs=options.run
	if options.runlistfile<>None:
		if not os.path.isfile(options.runlistfile):
			print "ERROR!  Run list file '%s' does not exist!"%options.runlistfile
		else:
			
