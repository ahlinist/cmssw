#!/usr/bin/env python

#---Import
import sys
import string
from optparse import OptionParser
import os.path
from ROOT import *
import re


#---Read .dat table for current dataset
data={}
column=[]
lineCounter = int(0)

for j,line in enumerate( open( "73699.txt" ) ):
    
    if( re.search("#", line) or re.search("Add", line) ):
        continue

    line = string.strip(line,"\n")
    
    if lineCounter == 0:
        for i,piece in enumerate(line.split()):
            column.append(piece)
    else:
        for i,piece in enumerate(line.split()):
            if i == 0:
                data[lineCounter] = {}
                row = lineCounter
            else:
                data[row][ column[i] ] = piece
                #print data[row][ column[i] ] 
                
    lineCounter = lineCounter+1


#print data

#for j,line in enumerate( data ):

#    print data[line]['FED'] + " " + data[line]["SPIGOT"] + " " + data[line]["HTR_EMPTY_EVENT_COUNT"] + "\n"
    

#Plot the array into a histogram
outputTFile = TFile("EmptyEvents.root","RECREATE")
histo = TH2F("EE_73699","EE in FED and SPIGOT",33,699,732,15,0,15,)
histo.GetXaxis().SetTitle("FED")
histo.GetYaxis().SetTitle("SPIGOT")

for j,line in enumerate( data ):
    print "FED = ",data[line]["FED"]," SPIG = ",data[line]["SPIGOT"]
    x=string.atoi(data[line]["FED"])
    y=string.atoi(data[line]["SPIGOT"])
    histo.Fill(x,y)
    

for i in range (0, 33):
    for j in range (0, 15):
        if( histo.GetBinContent(i,j) > 1.):
            histo.SetBinContent(i,j,1.)

histo.Draw('box')
while(1):
    pass


