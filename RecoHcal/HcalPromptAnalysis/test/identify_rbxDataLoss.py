##################################################
# FWLite script for identifying LS
# associated with and start time of RBX data loss
#
# Author : Jim Hirschauer (jhirsch@fnal.gov)
#
# Execute:
# python identify_rbxDataLoss.py --run 163337 HBM14 HBM10
# python identify_rbxDataLoss.py --run 163270 HBM18 --skip_to_ls 690 694
#
# More details:
# https://twiki.cern.ch/twiki/bin/view/CMS/HcalPFGRecipes#Find_associated_LS_and_starting
# https://twiki.cern.ch/twiki/bin/view/CMS/HcalRBXDataLossHistory
###################################################

import sys
import time
import optparse
import commands

from DataFormats.FWLite import Events, Handle

##############################################
# Get options
##############################################

parser = optparse.OptionParser("usage: python %prog [options] [RBXes]\n  e.g. python identify_rbxDataLoss.py --run 163270 --skip_to_ls 690 694 --ds /Jet/Run2011A-PromptReco-v2/RECO HBM18 HEP01 ")

parser.add_option ('--run', dest='runnum', type='int', metavar="",
                   default = '-1',
                   help="run number")

parser.add_option ('--ds', dest='dataset', type='string',
                   default = '/Jet/Run2011A-PromptReco-v2/RECO',
                   help="dataset to process\n (%default)")

parser.add_option ('--skip_to_ls', dest='skip_to_ls', type='int', nargs = 2,
                   default = '-1',
                   help="LS range to process (All)")

parser.add_option ('--noStartTime', dest='doStartTime', action="store_false",
                   default = 'True',
                   help="do not try to get start time w/ 1 sec precision. faster.")

options, args = parser.parse_args()

doStartTime = options.doStartTime
dataset = options.dataset
runnum = options.runnum
skip_to_ls = options.skip_to_ls
if runnum < 0: print "Please enter run number with run option '--run 163337'.  Exiting."

tmp_rbx_list = []
if len(args) == 0:
    print "Searching for RBX data loss in ANY RBX."
    print "This is not well tested.  You'll have better luck inputing the RBX names (e.g. HBM10) you want."
    print "Good luck ..."
    for irbx in range(1,19):
        for idet in ["HB","HE"]:
            for iside in ["M","P"]:
                tmp_rbx_list.append(idet+iside+str(irbx))
else:
    for arg in args:
        tmp_rbx_list.append(arg)

# Make sure that RBXes are labeled as HBM09 instead of HBM9

rbx_list = []
for rbx in tmp_rbx_list:
    det = 0; side = 0; num = 0
    if len(rbx.split("M")) > 1:
        det = rbx.split("M")[0]
        side = "M"
        num = rbx.split("M")[1]        
    elif len(rbx.split("P")) > 1:
        det = rbx.split("P")[0]
        side = "P"
        num = rbx.split("P")[1]
    else:
        print "Unexpected RBX name. Exiting."
        sys.exit()

    # this has the same effect for num = "09" and "9":

    if int(num) < 10: rbx = det+side+"0"+str(int(num))
    rbx_list.append(rbx)

############################################################
# Get Files
############################################################

def findFiles(runnum, dataset):
    command_string = 'dbs --search --query="find file where dataset='+dataset+' and run='+str(runnum)+'"'
    print ""
    print "Finding files via DBS: ", command_string
    print ""
    dbs_out = commands.getoutput(command_string).split("\n")

    # skip first 4 lines of dbs output
    tempfiles = []
    print "Files are:"
    for iline in range(4,len(dbs_out)):
        tempfiles.append(str(dbs_out[iline]))
        print str(dbs_out[iline])
    print ""
    return tempfiles



tempfiles = findFiles(runnum, dataset)

if len(tempfiles) == 0 and dataset == "/Jet/Run2011A-PromptReco-v2/RECO":
    dataset
    print "No files found in /Jet/Run2011A-PromptReco-v2/RECO.  Trying v1."

    dataset = "/Jet/Run2011A-PromptReco-v1/RECO"
    tempfiles = findFiles(runnum, dataset)

    if len(tempfiles) == 0:
        print "No files found in /Jet/Run2011A-PromptReco-v2/RECO or -v1."
        print "Please specify dataset with --ds option.  Exiting."
        sys.exit()

print "Searching for RBX data loss"
print "  in RBXes =",
for rbx in rbx_list: print rbx,
print ""
print "  in run =", runnum
print "  in dataset =", dataset

#tempfiles = [
#'/store/data/Run2011A/Jet/RECO/PromptReco-v2/000/163/337/F86615A2-2370-E011-B5F3-0019DB2F3F9A.root',
#'/store/data/Run2011A/Jet/RECO/PromptReco-v2/000/163/337/B4994D64-2670-E011-AB58-003048F024F6.root',
#'/store/data/Run2011A/Jet/RECO/PromptReco-v2/000/163/337/A28C8D20-1270-E011-B98D-003048D2BE12.root',
#'/store/data/Run2011A/Jet/RECO/PromptReco-v2/000/163/337/5A5F9CD5-2270-E011-92D1-001617E30CD4.root',
#'/store/data/Run2011A/Jet/RECO/PromptReco-v2/000/163/337/0C09D722-1270-E011-B532-003048F118C2.root'
#]

cernRecoString = 'rfio:/castor/cern.ch/cms/'
#cernExpString = 'rfio://castorcms/?svcClass=t0express&path=/castor/cern.ch/cms'
#fnalString = 'dcap://cmsdca.fnal.gov:24136/pnfs/fnal.gov/usr/cms/WAX/11'
prefix = cernRecoString
files = []
for tempfile in tempfiles:
    files.append(prefix+tempfile)

############################################################
# Make map from (ieta, iphi, depth) to (detector, side, irbx)
############################################################

# First define phi ranges
phi_to_rbx = {}
phi_to_rbx[1,2] = 1
phi_to_rbx[71,72] = 1

istart = 3
for irbx in range(2,19):
    phi_to_rbx[istart+4*(irbx-2),istart+4*(irbx-2)+3] = irbx

# Helper function that gets RBX number from iphi
def getRBXfromPHI(iphi):
    for phi_range, irbx in phi_to_rbx.iteritems():
        if iphi>= phi_range[0] and iphi <= phi_range[1]: return irbx

    print "Invalid iphi.  Exiting."
    sys.exit()

# Main function that gets RBX name (e.g. HBM10)
# from eta, phi, dep
def getHBHERBX(ieta, iphi, idep):
    idet  = -99; iside = -99; irbx  = -99

    if ieta < 0: iside = "M"
    else: iside = "P"

    ieta = abs(ieta)

    if ieta <= 14 and idep == 1  : idet = "HB"
    elif ieta == 15 and idep <= 2: idet = "HB"
    elif ieta == 16 and idep <= 2: idet = "HB"
    elif ieta == 16 and idep == 3: idet = "HE"
    elif ieta == 17 and idep == 1: idet = "HE"
    elif ieta <= 26 and idep <= 2: idet = "HE"
    elif ieta <= 28 and idep <= 3: idet = "HE"
    else: idet = False

    irbx = getRBXfromPHI(iphi)
    # Add "0" to numbers < 10"
    if irbx >= 10: irbx_str = str(irbx)
    else : irbx_str = "0"+str(irbx)
    if not idet: return False
    else: return idet+iside+irbx_str

# For speed make dictionary that maps eta, phi, dep to RBX name
# (e.g. HBM10)

rechit_to_rbx_dict = {}

for ieta in range(-28,29):
    if ieta == 0: continue
    for iphi in range(1,73):
        for idep in range(1,4):
            triplet = getHBHERBX(ieta, iphi, idep)
            if triplet[0] == -99 or triplet[1] == -99 or triplet[2] == -99:
                print "Unexpected problem making map.  Exiting."
                sys.exit()
            if triplet:
                rechit_to_rbx_dict[ ieta , iphi , idep ] = triplet

############################################################
# Get data from files
############################################################


print ""
print "Reading files.  This may take awhile."
events = Events (files)

hbherhHandle = Handle("edm::SortedCollection<HBHERecHit,edm::StrictWeakOrdering<HBHERecHit>>")
hbherhLabel = ("hbhereco")

############################################################
# Event loop
############################################################

nEvent = 0
missing_dictionary = False

print "Starting event loop.  Processing %i events." % events.size()


ls_analyzed = set([])
ls_found = {}

for event in events:
    nEvent += 1
    #    if nEvent < 3400: continue
    # print stuff
    ls = int(event.object().luminosityBlock())
    if nEvent%1000==1:
        print "record:",nEvent,"Run:",event.object().id().run(),\
              "event:",event.object().id().event(),\
              "ls:", ls


    if skip_to_ls > 0:
        if ls < skip_to_ls[0] or ls > skip_to_ls[1] : continue

    ls_analyzed.add(ls)

    # Check whether LS can be skipped:
    if not doStartTime:
        if missing_dictionary:
            #            print "missing_dictionary exists."
            skip = True
            for rbx in rbx_list:
                if rbx not in missing_dictionary:
                    #       print rbx, "is not in missing_dictionary. do not skip"
                    skip = False
                elif ls not in missing_dictionary[rbx]:
                    #                    print "ls is not in missing_dictionary[rbx]. do not skip"
                    skip = False
                    #                else:
                    #                    print "ls is already in missing_dictionary[rbx].  skip"
            if skip : continue
            #        else:
            #            print "missing_dictionary does NOT exist. do not skip"

#    print "Not skipping."
        
                    

    tmp_rbx_list = []
    for rbx in rbx_list: tmp_rbx_list.append(rbx)
    
    # Get rechits
    event.getByLabel(hbherhLabel,hbherhHandle)    
    hbherhs = hbherhHandle.product()
    
    # loop over rechits
    for rh in hbherhs:
        # skip ieta=29 for now
        if abs(rh.id().ieta()) == 29: continue
        # Get RBX for rechit
        rbx = rechit_to_rbx_dict[rh.id().ieta(), rh.id().iphi(), rh.id().depth()]
        # Remove RBX from list of RBXes if it's there.
        if tmp_rbx_list.count(rbx) > 0: tmp_rbx_list.remove(rbx)
        # Once RBX list is empty, we have found data from each RBX in an event
        # and we can stop checking.
        if len(tmp_rbx_list) == 0: break

    # I've looked through all rechits, check if there are RBXes with no data in the event:
    if len(tmp_rbx_list) != 0:
        # if so, record their info in dictionary
        
        # if the dictionary of info doesn't exist yet create it.
        # Structure of dictionary will be (see below)
        # rbx (idet, iside, num) -> (Set of unique LS, Set of unique LS, unix time seconds)
        if not missing_dictionary:
            missing_dictionary = {}
            missing_sub_dictionary = {}

        # loop over rbxes that reported no rechits in the event:
        for rbx in tmp_rbx_list:
            # get the time
            unix_time = int(event.object().time().unixTime())
            if rbx not in missing_dictionary:
                # if the rbx has no dictionary entries, create them:
                missing_dictionary[rbx] = set([ls])
                missing_sub_dictionary[(rbx,ls)] = set([unix_time])
            else:
                # if it has a main dictionary entry add the LS
                missing_dictionary[rbx].add(ls)
                # now check to see if the LS is entered in the sub-dictionary of times:
                if (rbx, ls) not in missing_sub_dictionary:
                    # if not create it
                    missing_sub_dictionary[(rbx,ls)] = set([unix_time])
                else:
                    # if so, add it
                    missing_sub_dictionary[(rbx,ls)].add(unix_time)

        #    if nEvent > 10: break

print "Done with event loop.  Processed %i events." % nEvent
print "\n"

# Print out results
if missing_dictionary :
    print "Summary of RBX data loss instances in run %i:" % runnum
    print "   RBX     #LS     LS range        Start time (GMT)"  
    print "   ---     ---     --------        ----------------"
    for rbx, ls_set in missing_dictionary.iteritems():

        # Make LS set into list so that you can sort it:
        ls_list   = []
        for ls in ls_set : ls_list.append( ls )
        ls_list.sort()
        
        # Separate contiguous groups of LS's into "rdl instances",
        # Put in a dictionary:
        rdl_instances = {}
        i_inst = 0
        iLS = 0
        for ls in ls_list:
            if i_inst not in rdl_instances: rdl_instances[i_inst] = []
            rdl_instances[i_inst].append(ls)
            # if LS is NOT 1 away from next in list, increment instance number:
            if iLS+1 < len(ls_list)-1:
                if ls != ls_list[iLS+1]-1:
                    i_inst += 1
            iLS += 1

        # Loop over individual instances of data loss in the new dictionary and print info
        for j_inst, inst_ls_list in rdl_instances.iteritems():

            # Get starting unix time from sub dictionary:
            time_list = []
            time_set = missing_sub_dictionary[(rbx,inst_ls_list[0])]
            for utime in time_set: time_list.append(utime)
            time_list.sort()
            # Put time in human readable form:
            t0 = time.gmtime(time_list[0])
            if t0.tm_min < 10: str_min = "0"+str(t0.tm_min)
            else: str_min = str(t0.tm_min)
            if t0.tm_sec < 10: str_sec = "0"+str(t0.tm_sec)
            else: str_sec = str(t0.tm_sec)
            str_t0 = str(t0.tm_mon)+"/"+str(t0.tm_mday)+" "+str(t0.tm_hour)+":"+str_min+":"+str_sec
            # print info
            print "   %s   %i       [%i,%i]         %s" % (rbx, len(inst_ls_list), inst_ls_list[0], inst_ls_list[len(inst_ls_list)-1], str_t0)
                   
else:
    print "   No problem RBXes found."

print ""
print "Summary of LS analyzed:"
# Print LS's analyzed:
ls_analyzed_list   = []
for ls in ls_analyzed : ls_analyzed_list.append( ls )
ls_analyzed_list.sort()

if skip_to_ls > 0:
    print "   User specified to only analyze LS range [%i,%i]." % (skip_to_ls[0], skip_to_ls[1])
    
print "   Analyzed %i LS in range [%i,%i]."  % (len(ls_analyzed_list),ls_analyzed_list[0],ls_analyzed_list[len(ls_analyzed_list)-1])



# Find skipped LS:

nLS = len(ls_analyzed_list)

# Make list of expected LS based on range of analyzed LS:
skipped_ls = []
for iLS in range(ls_analyzed_list[0],ls_analyzed_list[nLS-1]+1):
    skipped_ls.append(iLS)

# Remove analyzed LS from list of expected.  Remainder were missed:
for ls in ls_analyzed_list:
    skipped_ls.remove(ls)

print "   LS in this range with no events were: ",  skipped_ls
print "   Please take care if data loss occurred at beginning or end of run;"
print "   this script would completely miss LS before %i and after %i for " % (ls_analyzed_list[0],ls_analyzed_list[nLS-1])
print "   which there were no events in the %s dataset" % dataset





  
        



