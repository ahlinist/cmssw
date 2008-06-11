#!/usr/bin/env python


###########################################
#
# HcalPromptFromDBSgui.py
#
# v1.0 Beta
#
# by Jeff Temple (jtemple@fnal.gov)
#
# 10 May 2008
#
# GUI is now formed from a specialized class,
# inheriting from DQMfromDBSgui.py in
# DQM/HcalMonitorModule/python/dqmdbsGUI
#
###########################################


import sys
import os # used for sending user commands
import string

base=os.popen2("echo $CMSSW_BASE")
base=base[1].read()
base=string.strip(base)
if len(base)<2:
    print "No $CMSSW_BASE directory can be found."
    print "Are you sure you've set up your CMSSW release area?"
    sys.exit()


try:
    from Tkinter import *
except:
    print "Could not import Tkinter module!"
    print "Cannot display GUI!"
    print "(If you are running from FNAL, this is a known problem --\n"
    print " Tkinter isn't available in the SL4 release of python for some reason."
    print "This is being investigated.)"
    sys.exit()

import tkMessageBox # for displaying warning messages

try:
    sys.path.append(os.path.join(base,"src/DQM/HcalMonitorModule/python/dqmdbsGUI"))
except:
    print "Could not append 'src/DQM/HcalMonitorModule/python/dqmdbsGUI' from CVS"
    print "Have you checked out that package in your %s/src area?"%basedir
    sys.exit()

try:
    os.chdir(os.path.join(base,"src/DQM/HcalMonitorModule/python/dqmdbsGUI"))
    from pyDBSguiBaseClass import dbsBaseGui # get main gui package

    from pyDBSRunClass import DBSRun  # Gets class that holds information as to whether a given run has been processed through DQM
    from pydbsAccessor import dbsAccessor  # sends queries to DBS
    os.chdir(base)
except:
    print "Could not import necessary GUI modules from DQM/HcalMonitorModule/python/dqmdbsGUI"
    print "Are your releases up to date?"
    sys.exit()




class HcalPromGUI(dbsBaseGui):
    '''
    *** HcalPromGUI(inherits from DQMDBSgui) ***
    '''

    def __init__(self,parent=None,debug=False):
        ''' *** HcalPromGUI.__init__ ***
        Call the base class __init__ function, and replaces necessary variables
        (self.basedir, etc.) with HcalPromGUI-specific values.
        '''

        
        dbsBaseGui.__init__(self,parent,debug)

        if (self.debug):
            print self.__doc__
            print self.__init__.__doc__


        # Here we perform all changes to variables that need to be done before the
        # gui is drawn.  (Setting the base directory where files are located,
        # setting the color scheme of the gui, etc.)

        # You *must* set your base directory to the location of your DQM code;
        # otherwise all options will be taken from the default HCAL DQM GUI instance.

        base=os.popen2("echo $CMSSW_BASE")
        base=base[1].read()
        base=string.strip(base)
        if len(base)<2:
            print "No $CMSSW_BASE directory can be found."
            print "Are you sure you've set up your CMSSW release area?"
            sys.exit()
        self.basedir=os.path.join(base,"src/RecoHcal/HcalProm/python/hcalpromGUI")


        # Set new root title
        self.root.title("HcalPrompt DQM from DBS GUI")
        # Set new color scheme
        #self.bg="lightblue"

        return

    def specificSetup(self):
        '''
        *** self.specificSetup() ***
        This performs all setups that need to be done after base class GUI is drawn
        (Such as changing the name on some buttons, etc.)
        '''

        if (self.debug):
            print self.specificSetup.__doc__
            
        self.myDBS.searchStringFile.set("")
        self.myDBS.searchStringDataset.set("*/GlobalCruzet1-A/*-T0ReReco-*/RECO*")
        self.dqmButton.configure(text="Run the Hcal\nPrompt DQM")

        # Set the name of the .cfg file to be run
        self.cfgFileName.set(os.path.join(self.basedir,"hcalprom_gui.cfg"))

        compname=os.uname()[1]
        if compname.startswith("lxplus") and compname.endswith(".cern.ch"):
            if (self.debug):
                print "Working on %s (lxplus)"%compname
        elif compname.startswith("lxb") and compname.endswith(".cern.ch"):
            if (self.debug):
                print "Working on %s (CAF)"%compname
            # We can replace the self.myDBS.dbsInst variable with something more CAF-
            #appropriate here, if we see a significant change in performance.

        self.stripe.configure(bg=self.bg_alt)

        return


    def getcmsRunOutput(self,runnum):
        '''
        ** self.getcmsRunOutput(runnum) **
        This overwrites base class function to look for root files named
        prompt_out.root and move them to prompt_out_<runnum>.root.
        It also looks for a directory with the name HcalPrompt_R0000<runnum>.
        '''

        if (self.debug):
            print self.getcmsRunOutput.__doc__

        success=True
        
        self.cmsRunOutput=[]

        # Directory name now seems to take the form HcalPrompt_R000XXXX_LY_Z
        # Y=lumibegin value
        # Z=lumiend value

        if (runnum<100000):
            outname="HcalPrompt_R0000%i"%runnum
        else:
            outname="HcalPrompt_R000%i"%runnum

        # make fancier success requirement later -- for now, just check that a directory starting with "HcalPrompt_R000.. exists

        for i in os.listdir(self.basedir):
            if i.startswith(outname):
                success=success and True
                if (self.debug):
                    print "<getcmsRunOutput> success=True (directory %s found)!"%i
                outputdir=os.path.join(self.basedir,i)
                self.cmsRunOutput.append(outputdir)



        # now check that prompt_out.root file exists
        success=success and (os.path.exists("%s"%os.path.join(self.basedir,
                                                             "prompt_out.root")))

        # If file exists, we move it to prompt_out_<runnum>.root, and
        #add it to the self.cmsRunOutput list
        if (os.path.exists("%s"%os.path.join(self.basedir,"prompt_out.root"))):
            
            outputroot="prompt_out_%i.root"%runnum
            outputroot=os.path.join(self.basedir,outputroot)
            os.system("mv %s %s"%(os.path.join(self.basedir,
                                               "prompt_out.root"),
                                  outputroot))
            self.cmsRunOutput.append(outputroot)

        if (self.debug):
            print "<getcmsRunOutput>  The following cmsRun outputs were found:"
            for i in self.cmsRunOutput:
                print "\t%s"%i
            print "\nAll files found? %s"%success
            
        return success

    

#####################################

# Run as main GUI:

if __name__=="__main__":

    debug=False

    # Use option parsing to enable/disable debugging from the command line
    try:
        from optparse import OptionParser
        parser=OptionParser()
        # If "-d" option used, set debug variable true
        parser.add_option("-d","--debug",
                          dest="debug",
                          action="store_true",
                          help = "Enable debugging when GUI runs",
                          default=False)

        (options,args)=parser.parse_args(sys.argv)
        debug=options.debug
    except:
        print "Dang, optionParsing didn't work"
        print "Starting GUI anyway"

    mygui=HcalPromGUI(debug=debug)  # set up gui
    mygui.DrawGUI()
    mygui.specificSetup()
    mygui.root.mainloop() # run main loop
