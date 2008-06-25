#!/usr/bin/python


#Authorship:
#Richard Ruiz
#2008 June 26th
#Based on CDF's XTMon Fitter


#Imports
import sys
import os

#Static Variables
gmake       = "gmake"
gmake_clean = "gmake clean"
AImgs       = "./removeAImgfiles.tcsh"
SImgs       = "./removeSImgfiles.tcsh"
HTMLs       = "./removeHTMLfiles.tcsh"

#Program Arguments
args = sys.argv[0:]
prog = args[0]

#Helpdesk
help = """================================================================================
Usage: """+prog+""" [-flag1] [-flag2] ...

"""+prog+""" needs at least 1 flag in the form of '-f'.
Flags:
-a                => executes RemoveAImgfiles
-s                => executes RemoveSImgfiles
-i                => executes RemoveHtmlfiles
-b                => executes both RemoveImgfiles and RemoveHtmlfiles
-g                => executes gmake
-c                => executes gmake clean
-u                => executes RemoveAImgfiles, RemoveHtmlfiles and gmake
-v                => executes RemoveSImgfiles and gmake
-n                => executes all Removefiles, gmake clean, and gmake
-w                => executes all Removefiles and gmake clean
-h                => this Help screen
================================================================================"""

#Begin
flagsize = len(args)
if flagsize == 1:
    print "\nHave a nice day!\n"
    sys.exit

flag = sys.argv[1:flagsize]

for option_help in flag:
    cut = list(option_help)
    option = cut[-1].lower()    
    if option == 'h':
        print help
        sys.exit()    

for option in flag:
    cut = list(option)
    option = cut[-1].lower()
    
    if option == 'h':
        print help
        sys.exit()
    elif option == 'a':
        os.system(AImgs)
        #print AImgs
    elif option == 's':
        os.system(SImgs)
        #print SImgs
    elif option == 'i':
        os.system(HTMLs)
        #print HTMLs
    elif option == 'b':
        os.system(AImgs)
        os.system(SImgs)
        os.system(HTMLs)
        #print 'b'
    elif option == 'g':
        os.system(gmake)
        #print gmake
    elif option == 'c':
        os.system(gmake_clean)
        #print gmake_clean
    elif option == 'u':
        os.system(AImgs)
        os.system(HTMLs)
        os.system(gmake)
        #print 'u'
    elif option == 'v':
        os.system(SImgs)
        os.system(HTMLs)
        os.system(gmake)
        #'v'
    elif option == 'w':
        os.system(AImgs)
        os.system(SImgs)
        os.system(HTMLs)
        os.system(gmake_clean)
        #print 'w'
    elif option == 'n':
        os.system(AImgs)
        os.system(SImgs)
        os.system(HTMLs)
        os.system(gmake_clean)
        os.system(gmake)
        #print 'n'
    else:
        print "Why do you choose options that are not available to you in life?"
        print help
        sys.exit()


#Goodbye
print prog+", wishes that you have a nice day!"
print "Goodbye!"

#End!
