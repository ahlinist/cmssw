#!/usr/bin/env python

import sys
import os
import re
import string
from optparse import OptionParser


def SearchDir(dir=None,dict={},subdircount=0,maxrecursion=2,debug=False):
    ''' This will search a directory and all its subdirectories (up to a maximum recursion level of "maxrecursion") for files of the type "*_R*.root". Any time in which multiple files are found for one run, an error message will be thrown, and the program will exit.'''

    # 'dict' stores the list of files for the runs, with each run used as a key
    subdircount=subdircount+1
    if (subdircount>maxrecursion):  # set maximum recursion level
        return dict
    if not os.path.isdir(dir):
        print("Sorry, directory '%s' does not exist -- skipping!"%dir)
        return dict
    for i in os.listdir(dir):  # Search all files in directory
        if os.path.isfile(os.path.join(dir,i)):
            # not sure about the re.search value here -- why not use string .endswith?
            if(i.find("_R")==-1 or not re.search(".root$",i)):  # look only at root files
                continue

            # Get the run number from the root file name
            try:
                run=string.atoi(i[(i.find("_R")+2):(i.find("_R")+11)])
            except:
                print("Unable to parse run number from file '%s'"%os.path.join(dir,i))
                continue
            # Add run to dictionary
            if run in dict.keys():
                if (dict[run]==None):
                    dict[run]=os.path.join(dir,i)
                else:
                    print ("ERROR!  Run #%i has multiple files!"%run)
                    print ("File #1:  ",dict[run])
                    print ("File #2:  ",os.path.join(dir,i))


        # If subdirectory found, process files in that subdirectory
        elif os.path.isdir(os.path.join(dir,i)):
            dict=SearchDir(dir=os.path.join(dir,i),
                           dict=dict,
                           subdircount=subdircount,
                           maxrecursion=maxrecursion)
    return dict



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
    parser.add_options("-t","--tar",
                       dest="tar",
                       action="store_true",
                       default=False,
                       help="created tarball from output")
    
    parser.add_option("-b","--basedir",
		      dest="basedir",
		      default="/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/data/OfflineData/Run2010/StreamExpress",
		      help="Set base directory where files can be found.  Default is /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/data/OfflineData/Run2010/StreamExpress")
    
    (options,args)=parser.parse_args()
    if options.Help==True:
        parser.print_help()
        sys.exit()

    refdict={}
    if options.ref<>None:
        refdict[options.ref]=None
    runs=options.run
    rundict={}
    for r in runs:
        if r not in rundict.keys():
            rundict[r]=None
        else:
            print "WARNING!  Run %i has already been included!"%r
            
    if options.runlistfile<>None:
        if not os.path.isfile(options.runlistfile):
            print "ERROR!  Run list file '%s' does not exist!"%options.runlistfile
            
        else:
            myfile=open(options.runlistfile)
            for i in myfile.readlines:
                try:
                    run=string.strip(i)
                    run=string.atoi(i)
                    if (run>0):
                        if run not in rundict.keys():
                            rundict[run]=None
                        else:
                            print "Warning!  Run %i has already been included!"%run
                    elif run<0:
                        if len(refdict.keys())>0:
                            print "ERROR!  Trying to specify reference run '%i' when another reference  '%s' has already been chosen!"%(run,refdict)
                            print "Exiting..."
                            sys.exit()
                        else:
                            refdict[abs(run)]=None
                except:
                    print "Could not parse line '%s'"%i
                
    
                        
    #Get directory names
    rundict=SearchDir(dir=options.basedir,
                      dict=rundict,
                      debug=options.verbose)
    refdict=SearchDir(dir=options.basedir,
                      dict=refdict,
                      debug=options.verbose)
    print "The following runs have been found:"
    for i in rundict.keys():
        print i, rundict[i]
    print "The following reference will be used:"
    for r in refdict.keys():
        print r, refdict[r]

    refrun=refdict.keys()[0]

    runs=rundict.keys()
    runs.sort()

    created_dirs=[]
    for r in runs:
        if rundict[r]==None:
            print "ERROR!  No file found for run #%i!"%r
            continue
        stream=string.split(rundict[r],"/")
        if len(stream)>2:
            stream=stream[-3]
        else:
            stream="UnknownStream"
        cmd = "./bin/ExecuteDQM %s %s %i %i Run%i_vs_Run%i_%s"%(rundict[r],
                                                                refdict[refrun],
                                                                r,
                                                                refrun,
                                                                r,
                                                                refrun,
                                                                stream)
        print cmd
        os.system(cmd)
        if os.path.isdir("Run%i_vs_Run%i_%s"%(r,refrun,stream)):
            created_dirs.append("Run%i_vs_Run%i_%s"%(r,refrun,stream))

    if options.tar:
        cmd="tar cvzf  RunOutput.tar.gz"
        for i in created_dirs:
            cmd=cmd+" %s"%i
        os.system(cmd)

        # Next, copy tarball?
