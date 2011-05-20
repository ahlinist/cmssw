#!/usr/bin/env python
import os
import re
import sys
from optparse import OptionParser

_basePath = "/mnt/tier2/store/user/veverka/"

## Main workhorse
def main(option, args):
    ## check if path to dataset was supplied
    if len(args) == 0:
        ## offer a list of datasets
        myPath = _basePath

        print "Available datasets in %s:" % myPath
        for d in os.listdir(myPath):
            if os.path.isdir(os.path.join(myPath, d)):
                print " ", d
        print "Choose a directory: "
        myDir = sys.stdin.readline().strip()
        myPath = os.path.join(myPath, myDir)
        if not os.path.isdir(myPath):
            raise RunTimeError, "Illegal dir `%s'!" % myPath

        print "Available datasets in %s:" % myPath
        myDirs = []
        for d in os.listdir(myPath):
            if os.path.isdir(os.path.join(myPath, d)):
                myDirs.append(d)
        if len(myDirs) == 0:
            raise RunTimeError, "Didn't find any directories in `%s'!" % myPath
        elif len(myDirs) == 1:
            myDir = myDirs[0]
        else:
            for d in myDirs:
                print " ", d
            print "Choose a sub-directory: "
            myDir = sys.stdin.readline().strip()
        myPath = os.path.join(myPath, myDir)
        if not os.path.isdir(myPath):
            raise RunTimeError, "Illegal dir `%s'!" % myPath

        print "Available datasets in %s:" % myPath
        myDirs = []
        for d in os.listdir(myPath):
            if os.path.isdir(os.path.join(myPath, d)):
                myDirs.append(d)
        if len(myDirs) == 0:
            raise RunTimeError, "Didn't find any directories in `%s'!" % myPath
        elif len(myDirs) == 1:
            myDir = myDirs[0]
        else:
            for d in myDirs:
                print " ", d
            print "Choose a sub-sub-directory: "
            myDir = sys.stdin.readline().strip()
        myPath = os.path.join(myPath, myDir)
        args.append(myPath)

    rootRE = re.compile(".+\.root")
    fileNames = []

    for myPath in args:
        if not os.path.isdir(myPath):
            raise RunTimeError, "Illegal dir `%s'!" % myPath
        for root, dirs, files in os.walk(myPath):
            for f in files:
                if rootRE.match(f):
                    fileNames.append(os.path.join(root, f))
            if fileNames:
                print "  Added %d files from %s ..." % (len(fileNames), root)
            ## sort by job number
            fileNames.sort(key = lambda f: int(f.split("_")[-3]) )

    print "Enter an output file: "
    myFileName = sys.stdin.readline().strip()

    if len(myFileName) != 0:
        myFile = open(myFileName, 'w')

        for f in fileNames:
            myFile.write( "file:%s\n" % f )

        myFile.close()

if __name__ == "__main__":
    import user
    parser = OptionParser(description="%prog : Make a list of root files found at a given destination.",
                          usage="listFiles.py [path]")
    (options,args) = parser.parse_args()

    main(options, args)
