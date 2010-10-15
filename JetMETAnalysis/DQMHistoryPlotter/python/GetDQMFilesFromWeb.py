#!/usr/bin/env python
import os, sys, urllib2, httplib, json
import urllib
from ROOT import *
from array import *
import string

from optparse import OptionParser


'''

GetDQMFilesFromWeb.py

by Jeff Temple (jtemple@fnal.gov)

v1.0
15 October 2010

based on original "dqm_get_json" code from Marco Rovere.

Code reads from a web page in cmsweb.cern.ch, and either saves the output of the page or parses the page for more info.

'''


# I think you need to do a cern setup (voms-proxy-init, etc.) before running this script, or it will ask you for a password for each web page it tries to access.


def ReadHTML_DQMlist(data,run,debug=False):
    thisdata=string.split(data,"</tr>")
    for i in thisdata:
        #print "HEY!", i, " RUN = ",run
        if i.find("%s"%run)>-1 and i.find(".root")>-1:
            thisfile=string.split(i,".root")[0]
            thisfile=string.split(thisfile,"'")[-1]
            if not thisfile.endswith(".root"):
                thisfile="%s.root"%thisfile
            thisfile=os.path.basename(thisfile)
            return thisfile
    return None



def get_x509_params(debug=False):
    '''
    **** get_x509_params ****
    Get the proper userkey, usercert files,
    first by looking for the X509_USER_PROXY or (X509_USER_KEY and X509_USER_CERT) variables,
    and if those are not found, by going to ./globus/user*.pem .
    If the user*.pem files are used, though, the user will need to enter their password
    before each web access attempt, so it is definitely better to do
    "voms-proxy-init" prior to trying to run this.
    '''

    key_file = cert_file = None

    x509_path = os.getenv("X509_USER_PROXY", None)
    if x509_path and os.path.exists(x509_path):
        key_file = cert_file = x509_path

    if not key_file:
        x509_path = os.getenv("X509_USER_KEY", None)

        if x509_path and os.path.exists(x509_path):
            key_file = x509_path

    if not cert_file:
        x509_path = os.getenv("X509_USER_CERT", None)
        if x509_path and os.path.exists(x509_path):
            cert_file = x509_path

    if not key_file:
        x509_path = os.getenv("HOME") + "/.globus/userkey.pem"
        if os.path.exists(x509_path):
            key_file = x509_path

    if not cert_file:
        x509_path = os.getenv("HOME") + "/.globus/usercert.pem"
        if os.path.exists(x509_path):
            cert_file = x509_path

    if not key_file or not os.path.exists(key_file):
        sys.stderr.write("ERROR:  no certificate private key file found\n\n")
        sys.exit(1)

    if not cert_file or not os.path.exists(cert_file):
        sys.stderr.write( "ERROR:  no certificate public key file found\n\n")
        sys.exit(1)

    if debug:
        print "Using SSL private key:", key_file
        print "Using SSL public key:", cert_file
    return key_file, cert_file


def get_web_file(dir="/dqm/offline/data/browse/ROOT/OfflineData/Run2010/StreamExpress/",
                 subdir=None,
                 runfile=None,
                 host="cmsweb.cern.ch",
                 save=True,
                 debug=False
                 ):
    ssl_key_file, ssl_cert_file = get_x509_params(debug)
    fileToGet=dir

    #runfile="DQM_V0007_R000147451__StreamExpress__Run2010B-Express-v2__DQM.root"
    
    if subdir<>None:
        fileToGet=os.path.join(fileToGet,subdir)
        if runfile<>None:
            fileToGet=os.path.join(fileToGet,runfile)
    # Apparently, the ending / is important!
    if not fileToGet.endswith("/"):
        fileToGet="%s/"%fileToGet

    thisFile="%s%s"%(host,fileToGet)
    while thisFile.startswith("/"):
        thisFile=thisFile[1:]
    if debug:
        print "Trying to get file from https://%s"%thisFile

    conn = httplib.HTTPSConnection(
        host=host,
        key_file=ssl_key_file,
        cert_file=ssl_cert_file
        )

    # Read the list of files in the subdirectory 0001474xx
    conn.request("GET",
                 fileToGet)

    try:
        r1=conn.getresponse()
    except:
        print "Could not get a response from the site!  Have you set up your environment with 'voms-proxy-init' yet?"
        return
    print "Checking that connection is OK:"
    print "\t%s\t%s"%(r1.status, r1.reason)
    if r1.reason<>"OK":
        sys.stderr.write("ERROR:  connection response is '%s'\n\n"%r1.reason)
        sys.exit()

    if runfile==None:
        dirname=None
        outdir=string.split(dir,"/")
        for i in range(len(outdir)-1,-1,-1):
            if outdir[i]<>"":
                dirname=outdir[i]
                break
        if dirname.endswith(".root"):
            outputname=dirname
        else:
            if dirname==None:
                dirname="UnknownStream"
            if subdir.endswith(".root"):
                outputname=subdir
            elif subdir==None:
                subdir=""
                outputname="%s_Run%s.html"%(string.strip(dirname,"/"),string.strip(subdir,"/"))
    else:
        outputname=runfile
    if (save):
        print "Writing to file '%s'"%outputname
        data=r1.read()
        myout=file(outputname,'w')
        myout.write(data)
        myout.close()
    else:
        data=r1.read()
        
    conn.close()

    if runfile==None:
        return data  # if what was asked for was not a file, we'll probably want to parse the output
    else:
        return None



if __name__=="__main__":
    parser=OptionParser()
    parser.add_option("-r","--run",
                      action="append",
                      default=[],
                      type="int",
                      dest="run",
                      help="Enter run(s) to be evaluated")
    parser.add_option("-R","--runlistfile",
                      default=None,
                      dest="runlistfile",
                      help="use a text file of line-separated run numbers for the run list")
    parser.add_option("-m","--runRangeMode",
                      default=False,
                      dest="runRangeMode",
                      action="store_true",
                      help="If set true, all runs in the range [min(run), max(run)] will be evaluated.")
    
    parser.add_option("-s","--stream",
                      default="StreamExpress",
                      dest="stream",
                      help="Specify DQM stream you wish to access")
    
    parser.add_option("-v","--verbose",
                      default=False,
                      action="store_true",
                      dest="debug",
                      help="Turn on debug info")
    
    parser.add_option("-H","--Help",
                      dest="Help",
                      action="store_true",
                      default=False,
                      help="alternate help action")




    (options,args)=parser.parse_args()
    if options.Help==True:
        parser.print_help()
        sys.exit()


    print (" Looking for runs...")

    # Get list of runs to use from command line
    runs=options.run
    runs.sort()
    
    # Get runs from txt file, if provided
    if options.runlistfile<>None:
        if not os.path.isfile(options.runlistfile):
            print ("Sorry, run file '%s' does not exist!"%options.runlistfile)
        else:
            temp=open(options.runlistfile,'r')
            for i in temp:
                try:
                    thisrun=string.strip(i)
                    thisrun=string.atoi(thisrun)
                    if thisrun>0:
                        runs.append(thisrun)
                    else:
                        runs.append(abs(thisrun))  # reference runs have "-" signs in front
                except:
                    print("Error:  Cannot parse line '%s' in file %s"%(i,options.runlist))

    runs.sort()

    # If runRangeMode==True, make list of all runs in a given range
    if options.runRangeMode==True:
        if len(runs)<2:  # need at least 2 runs to make a range
            print ("You must specify at least two runs to make a valid run range!")
            sys.exit()
        newruns=[]  # new list that includes all runs in range
        for i in range(runs[0],runs[-1]+1):  # runs have already been sorted
            newruns.append(i)
        runs=newruns  # replace original runs with inclusive list

    runs.sort()  

    # Specify default host and directory
    dir="/dqm/offline/data/browse/ROOT/OfflineData/Run2010/StreamExpress/"
    host="cmsweb.cern.ch"

    # If no runs specified, assume that the tool is being used just to get web files directly
    if len(runs)==0:
        for arg in args:
            print "Grabbing web page '%s'"%arg
            splitarg=string.split(arg,"/")
            host=splitarg[2]
            dir=string.split(arg,host)[1]
            name=string.split(dir,"/")[-1]
            if name.find(".")==-1 and not dir.endswith("/"):
                dir="%s/"%dir
            print "\t HOST = %s  Directory = %s"%(host,dir)
            get_web_file(host=host,
                         dir=dir,
                         debug=options.debug)

    # Otherwise, we have a list of runs (root files) to grab
    else:
        # Can only support one 'base' https directory right now
        if len(args)>1:
            print "APOLOGY::  Sorry, the code is only set up to grab runs from one directory at a time at this point!"

        # If an argument is put on the command line, that will replace the default host/dir
        if len(args)>0:
            print "Will use web page '%s'"%arg[0]
            splitarg=string.split(arg[0],"/")
            host=splitarg[2]
            dir="/%s"%string.split(arg[0],host)[1]
            name=string.split(dir,"/")[-1]
            if name.find(".")==-1 and not dir.endswith("/"):
                dir="%s/"%dir
            print "\t HOST = %s  Directory = %s"%(host,dir) 

        # Now make a list of files for each run
        rundict={}  # each key is a filename of the htmldirectory ("0001474xx/", etc.), and each value is a list of runs in that directory
        for r in runs:
            # Assume that structure will always be 7 digis + "xx/"
            htmlfile="%ixx/"%(r/100)  
            while len(htmlfile)<10:
                htmlfile="0%s"%htmlfile
            if htmlfile not in rundict.keys():
                rundict[htmlfile]=[r]
            else:
                rundict[htmlfile].append(r)

        runkeys=rundict.keys()
        runkeys.sort()

        for r in runkeys:  # Sort over run subdirectories
            print "DIR = ",dir
            print "HOST = ",host

            # Get the web page listing all runs in the 000....xx/ subdirectory
            data=get_web_file(dir=dir,
                              subdir=r,
                              runfile=None,
                              host=host,
                              save=False, # will read the data directly; don't need to save file
                              debug=options.debug
                              )
            if data==None:
                print "Unable to get data for %s%s%s"%(host,dir,r)
                continue
            # Check over all runs that should be present in this subdirectory, read their locations from the 'data' web page output
            for run in rundict[r]:
                newfile=ReadHTML_DQMlist(data,run)
                if newfile<>None:
                    # Get all root files
                    print "Getting file '%s' from https::%s%s%s"%(newfile,host,dir,r)
                    get_web_file(dir=dir,
                                 subdir=r,
                                 runfile=newfile,
                                 host=host,
                                 debug=options.debug)

