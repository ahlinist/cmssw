#!/usr/bin/python


#Authorship:
#Richard Ruiz
#2008 June 26th
#Based on CDF's XTMon Fitter
#This script can take more than one input file. : )
#We are assuming that each input file is a text file

#Imports
import sys
import os


#Static Variables
target = "./HTML/root/"
source = "./txmon_new", "./txmon_old"
prefix = "TXMon_txsec"

#Program Arguments
args   = sys.argv[0:]
prog   = args[0]

#Helpdesk
help = """================================================================================
Usage: """+prog+""" [-flag] filename

"""+prog+""" needs at least 1 flag in the form of '-f'.
Flags:
-f filename       => use the Run Numbers in file entitled 'filename'
-h                => this Help screen
================================================================================
"""

#Begin

if len(args) == 1:
    sys.exit(prog+" requires flags and an input. Type: "+prog+" -h for help")

else:    
    for flag in args[1:]:
        if flag == '-h':
            print flag
            sys.exit(help)

                        
    for ifile in args[1:]:            
        #This is where we extract run numbers from the text file
        #Believe it or not, this part was fun to write
        #Three days later I now have a different opinion
        
        ifile = open(ifile, 'r')
        Numbers = ifile.read()
        print Numbers
        NumArray = list(Numbers)
        
        nlist = "1234567890"
        nlist = list(nlist)
        #print nlist
        
        
        while NumArray[0] not in nlist:
            NumArray.pop(0)
        while NumArray[-1] not in nlist:
            NumArray.pop(-1)
        NumArray.append('\n')
        #print NumArray

        ##The Algorithm
        runlist = ['' for x in range(1)]

        #print type(NumArray)
        #print type(runlist)

        while len(NumArray) > 1:

            if NumArray[0] not in nlist:
                #print NumArray[0]
                NumArray.pop(0)

                if NumArray[0] in nlist:
                    #print NumArray[0]
                    runlist.append(NumArray[0])
                    NumArray.pop(0)
                    
                    if NumArray[0] not in nlist:
                        #print NumArray[0]
                        runlist.append(NumArray[0])
                        NumArray.pop(0)

            else:
                #print NumArray[0]
                runlist.append(NumArray[0])
                NumArray.pop(0)
                                
                if NumArray[0] not in nlist:
                    #print NumArray[0]
                    runlist.append(NumArray[0])

                    NumArray.pop(0) 
            ##The End of the Algorithm

        while runlist[0] not in nlist:
            runlist.pop(0)
        #print runlist
        #print len(runlist)
            
        i = 0
        while i < len(runlist):
            #print runlist[i]
            if runlist[i] not in nlist:                
                #print runlist
                runlist[i] = ' '
            #I originally had the next line indented. : )
            i = i+1                

        runs = ['' for x in range(len(runlist))]
        runs = list(runs)
        #print runs
        
        j = 0
        while len(runlist) > 1:
            index = runlist.index(' ')            
            runs[j] = ''.join(runlist[0:index])
            #print runs[j]
            #print runlist
            j = j + 1
            k = 0
            while k < index + 1:
                runlist.pop(0)
                k = k + 1
                #print runlist
                                
        while runs[0] is '':
            runs.pop(0)
        while runs[-1] is '':
            runs.pop(-1)

        print "Runs: "
        print runs

        #Setting paths
        rootfiles = ['' for x in range(1)]
        #print rootfiles

        for RunNumber in runs:
            for source_dir in source:
                paths = source_dir+"/"+prefix+RunNumber+"*.root"
                #print paths
                rootfiles.append(paths)


        rootfiles.pop(0)
        #print rootfiles

        for path in rootfiles:
            command = "cp "+path+" "+target
            print command
            os.system(command)

            


# ./copyRoot.py ./runnumbers.txt 
