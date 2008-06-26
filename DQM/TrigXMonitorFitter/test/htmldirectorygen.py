#!/usr/bin/python

#Authorship:
#Richard Ruiz
#2008 June 26th
#Based on CDF's XTMon Fitter

#Imports
import sys
import os

#Static Variables
directory = sys.argv[0]
source_base = "./"
target_base = "./HTML/"
dirlist = "L1","L2","L3"

#Dynamic Variables
args = sys.argv[0:]

# print "This is my magical lightning machine"
# print "Yes, I created the Algorithim =D"


#Folder generating function
def generator():

    if len(args) > 1:
        ifiles = args[1:]

        for htmlfile in ifiles:
#            print htmlfile
            array = list(htmlfile)
#            print array
            level = "".join(array[2:4])
#            print level
            basename = "".join(array[5:-5])
#            print basename


            print "\n"

            commandI   = "mkdir ./HTML/"+level+"/"+basename
            #print commandI
            os.system(commandI)

            commandII  = "mv "+htmlfile+" ./HTML/"+level+"/"+basename+"/"
            #print commandII
            os.system(commandII)
               
            commandIII = "cp ./"+level+"/"+basename+"*.eps ./HTML/"+level+"/"+basename+"/"
            #print commandIII
            os.system(commandIII)

            commandIV  = "cp ./"+level+"/"+basename+"*.gif ./HTML/"+level+"/"+basename+"/"
            #print commandIV
            os.system(commandIV)
        
    else:
        sys.exit("How about using files next time!")

    print "\n"




#Program

print args

generator()
