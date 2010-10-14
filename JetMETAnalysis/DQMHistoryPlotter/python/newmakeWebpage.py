#!/usr/bin/python

## This script writes an index.html page
## displaying all plots contained in the 
## directory.
##
## The file: makeWebpage.html is needed
## to be kept in the same place as the
## script and is used as a template.
## -------------------------------------
## Usage: ./makeWebpage.py DIR
## -------------------------------------
## with DIR = the path to the directoy
## containig the plots.
## DIR will also be the title of the
## webpage.
##
## For the moment it works for .gif's.
## For other formats, change line 42
## accordingly.
##
## After running the script, copy the
## folder containing the plots and the
## index.html to an apropriate place.
## (This can be autmated too, if needed.)


import sys
import os
import re
from optparse import OptionParser


def MainProgram(directory,debug=False):
    # get the plots in the directory
    histlist = os.listdir(directory)
    histlist.sort()
    # open a tmeplate file and the new .html file
    template=open("makeWebpage.html","r")
    page=open(directory+"/index.html","w")

    # write the common part of the index.html
    for line in template:
        if(re.search(r'blablabla',line)!=None):
            page.write(sys.argv[1])
        else:
            page.write(line)

    page.write('<strong>'+sys.argv[1]+' </strong><br>')
    print "Reading through all .png image files"
    count=0
    # add all the plots in the directory
    for i in range(len(histlist)):
        if(re.search(r'png',histlist[i])!=None):
            count=count+1
            if (debug):  print "\t%s"%histlist[i]
            page.write('<a href=" '+histlist[i])
            page.write('" onMouseOver="ThumbnailPlot(')
            page.write("'"+histlist[i]+"'), ClearNotes(),  ThumbnailNote() ")
            page.write('"> <img src="'+histlist[i]+' " style="height:22px; width:25px;"><small>'+histlist[i]+' </small></a> <br> \n')
    print "Found a total of %i .png files"%count
            
    # write the last lines
    page.write('<br>\n')
    page.write('<div id="thumb_gen"><a href="#" id="thumblink_l"><img src="" id="thumb_l" width=0 height=0 border=0></a></div> \n')
    page.write('</body> \n')
    page.write(' </html>\n')

    print "index.html file produced in %s"%directory
    # now copy everything on the wwweth server...
    #os.system("scp -r "+directory+" wwweth.cern.ch:JetMETPromptAnalysis/")
    return




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
        
            

    # index.html is stored in the working directory



    startingdir=os.getcwd()
    workingdir=os.path.abspath(os.path.dirname(sys.argv[0]))


    if (startingdir<>workingdir):
        os.chdir(workingdir)
    MainProgram(options.dir,debug=options.verbose)
    
