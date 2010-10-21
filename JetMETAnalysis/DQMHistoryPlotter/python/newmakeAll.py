#!/usr/bin/python
# usage: python newmakeAll.py <resultdir>
# ex: python newmakeAll.py RunCert_XXXXXX_YYYYYY
# Use "python newmakeAll.py -H to get list of options"

import sys
import os
import re
import string
from optparse import OptionParser

def MakeAll(directory,outputdir=None,debug=False):


    # 'directory' is where the initial .png files are stored
    if not os.path.isdir(directory):
        cont=raw_input( "Directory '%s' does not exist!  Continue to make index file for empty directory (y/n)?  ")
        if string.lower(cont)[0]<>"y":
            return
        os.mkdir(directory)

    if outputdir<>None:
        if os.path.isdir(outputdir):
            cont=raw_input("Directory '%s' already exists!  Overwrite (y/n)?  "%outputdir)
            if string.lower(cont)[0]<>"y":
                return
            os.system("rm -rf %s"%outputdir)
        os.system("cp -r %s %s"%(directory, outputdir))
        directory=outputdir
            
    #os.chdir(directory)
    newmakeWebpage.MainProgram(directory,debug)
    if not os.path.isdir(os.path.join(directory,"Jets")):
        os.mkdir(os.path.join(directory,"Jets"))
    if not os.path.isdir(os.path.join(directory,"MET")):
        os.mkdir(os.path.join(directory,"MET"))
    
    os.system("cp %s %s"%(os.path.join(directory,"*Jet*.png"),
                          (os.path.join(directory,"Jets"))))
    os.system("cp %s %s"%(os.path.join(directory,"*MET*.png"),
                          (os.path.join(directory,"MET"))))
    newmakeWebpage.MainProgram(os.path.join(directory,"Jets"),debug)
    newmakeWebpage.MainProgram(os.path.join(directory,"MET"),debug)


    if directory.startswith("DQMOutput_"):
        newdir=string.split(directory,"DQMOutput_")[1]
        newdir="RunCertification_%s"%newdir
        if os.path.isdir(newdir):
            cont=raw_input("Directory '%s' already exists!  Delete and continue (y/n)?  "%newdir)
            if string.lower(cont)[0]<>"y":
                return
            os.system("rm -rf %s"%newdir)
        os.system("cp -r %s %s"%(directory, newdir))
    else:
        newdir=directory
    os.mkdir(os.path.join(newdir,"HistoryDQM"))
    for suffix in ["*.png","*.html"]:
        os.system("mv %s %s/HistoryDQM"%(os.path.join(newdir,suffix), newdir))
    return

def CopyToBaylor(directory,debug=False):

    if directory.startswith("DQMOutput_"):
        directory=string.split(directory,"DQMOutput_")[1]
        directory="RunCertification_%s"%directory
    if not os.path.isdir(directory):
        print("Error!  Directory '%s' does not exist!"%directory)
        return

    baylor=False
    myfile=os.path.join(os.getenv("HOME"),".ssh","config")
    if not os.path.isfile(myfile):
        print("No file %s found -- skipping attempt to .ssh"%myfile)
        return
    myfile=open(myfile,'r').readlines()
    for i in myfile:
        if i.find("hep06.baylor.edu")>-1:
            baylor=True
            break
    if baylor==False:
        print("Cannot find hep06.baylor.edu listed in %s"%myfile)
        print("Skipping attempt to .ssh")
        return

    cmd="scp -r %s hep06-cmsjmet:/www/htdocs/cmsjmet/JetMETDQM/Commissioning10/%s"%(directory,directory)
    print("Copying directory '%s' to baylor..."%directory)
    print(cmd)
    os.system(cmd)

    return


#######################################################


if __name__=="__main__":

    parser=OptionParser()
    parser.add_option("-d","--dir",
                      dest="dir",
                      default=None,
                      help="Specify directory containing root files")
    parser.add_option("-v","--verbose",
                      dest="verbose",
                      default=False,
                      action="store_true",
                      help="print debug information")
    parser.add_option("-o","--outputdir",
                      dest="outputdir",
                      default=None,
                      help="specify a separate output directory (other than the input dir")
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

    (options,args)=parser.parse_args()
    if options.Help==True:
        parser.print_help()
        sys.exit()

    if options.dir==None:
        for i in args:
            if os.path.isdir(i):
                options.dir=i
                break
            
    if options.dir==None:
        print "Error!  You must specify the directory containing the histogram images to be linked by index.html!\n"
        sys.exit()


    startingdir=os.getcwd()
    workingdir=os.path.abspath(os.path.dirname(sys.argv[0]))


    if (startingdir<>workingdir):
        os.chdir(workingdir)
    import newmakeWebpage
        
    MakeAll(options.dir,
            outputdir=options.outputdir,
            debug=options.verbose)
    if options.copy:
        dir=options.dir
        if options.outputdir<>None:
            dir=options.outputdir
        CopyToBaylor(dir,
                     debug=options.verbose)
