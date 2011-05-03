import sys
import time
import optparse
import commands

from ROOT import TFile, TH1F, TH2F, TProfile
from DataFormats.FWLite import Events, Handle

##############################################
# Get options
##############################################

parser = optparse.OptionParser("usage: %prog [options] HBM14 HBM10\n")

parser.add_option ('--run', dest='runnum', type='int',
                   default = '-1',
                   help="Run number")

parser.add_option ('--skip_to_ls', dest='skip_to_ls', type='int', nargs = 2,
                   default = '-1',
                   help="LS range")

options, args = parser.parse_args()

runnum = options.runnum
skip_to_ls = options.skip_to_ls
if runnum < 0: print "Please enter run number with run option '--run 163337'.  Exiting."

rbx_list = []
if len(args) == 0:
    print "Searching for RBX data loss in ANY RBX."
    print "This is not well tested.  You'll have better luck inputing the RBX names (e.g. HBM10) you want."
    print "Good luck ..."
    for irbx in range(1,19):
        for idet in ["HB","HE"]:
            for iside in ["M","P"]:
                rbx_list.append(idet+iside+str(irbx))
else:
    for arg in args:
        rbx_list.append(arg)

############################################################
# Get Files
############################################################

#dataset = "/Jet/Run2011A-PromptReco-v2/RECO"
dataset = "/MuEG/Run2011A-PromptReco-v2/RECO"

command_string = 'dbs --search --query="find file where dataset='+dataset+' and run='+str(runnum)+'"'

print "Searching for RBX data loss"
print "  in RBXes =",
for rbx in rbx_list: print rbx,
print ""
print "  in run =", runnum
print "  in dataset =", dataset

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

if len(tempfiles) == 0: print "No files found.  Exiting."; sys.exit()


#tempfiles2 = ['/store/data/Run2011A/Jet/RECO/PromptReco-v2/000/163/337/F86615A2-2370-E011-B5F3-0019DB2F3F9A.root',
#'/store/data/Run2011A/Jet/RECO/PromptReco-v2/000/163/337/B4994D64-2670-E011-AB58-003048F024F6.root',
#'/store/data/Run2011A/Jet/RECO/PromptReco-v2/000/163/337/A28C8D20-1270-E011-B98D-003048D2BE12.root',
#'/store/data/Run2011A/Jet/RECO/PromptReco-v2/000/163/337/5A5F9CD5-2270-E011-92D1-001617E30CD4.root',
#'/store/data/Run2011A/Jet/RECO/PromptReco-v2/000/163/337/0C09D722-1270-E011-B532-003048F118C2.root'
#]

#print tempfiles2
#print tempfilesnTotal = 0


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
    idet  = -99
    iside = -99
    irbx  = -99

    if ieta < 0:
        iside = "M"
        ieta = -1*ieta
    else:
        iside = "P"


    if ieta <= 14 and idep == 1  : idet = "HB"
    elif ieta == 15 and idep <= 2: idet = "HB"
    elif ieta == 16 and idep <= 2: idet = "HB"
    elif ieta == 16 and idep == 3: idet = "HE"
    elif ieta == 17 and idep == 1: idet = "HE"
    elif ieta <= 26 and idep <= 2: idet = "HE"
    elif ieta <= 28 and idep <= 3: idet = "HE"
    else: idet = False

    
    irbx = getRBXfromPHI(iphi)


    if not idet:
        return False
    else:
        return idet+iside+str(irbx)
    

# For speed make dictionary that maps eta, phi, dep to RBX name
# (e.g. HBM10)

rechit_to_rbx_dict = {}

for ieta in range(-28,29):
    if ieta == 0: continue
    for iphi in range(1,73):
        for idep in range(1,4):
            triplet = getHBHERBX(ieta, iphi, idep)
            if triplet[0] == -99 or triplet[1] == -99 or triplet[2] == -99:
                print "Broke"
                sys.exit()
            if triplet:
                rechit_to_rbx_dict[ ieta , iphi , idep ] = triplet



############################################################
# Get data from files
############################################################

print "Reading files."
events = Events (files)
print "Done reading files."
 
hbherhHandle = Handle("edm::SortedCollection<HBHERecHit,edm::StrictWeakOrdering<HBHERecHit>>")
hbherhLabel = ("hbhereco")


############################################################
# Event loop
############################################################

nEvent = 0
missing_dictionary = False


print "Starting event loop.  Processing %i events." % events.size()

for event in events:
    nEvent += 1
    #    if nEvent < 3400: continue
    # print stuff
    if nEvent%1000==1:
        print "record:",nEvent,"Run:",event.object().id().run(),\
              "event:",event.object().id().event()


    ls = int(event.object().luminosityBlock())
    if skip_to_ls > 0:
        if ls < skip_to_ls[0] or ls > skip_to_ls[1] : continue

    # Copy rbx_list once for each event
    tmp_rbx_list = []
    for rbx in rbx_list: tmp_rbx_list.append(rbx)
    
        
    # Get rechits
    event.getByLabel(hbherhLabel,hbherhHandle)    
    hbherhs = hbherhHandle.product()
    
    # loop over rechits
    for rh in  hbherhs:
        # skip ieta=29 for now
        if abs(rh.id().ieta()) == 29: continue
        # Get RBX for rechit
        rbx = rechit_to_rbx_dict[rh.id().ieta(), rh.id().iphi(), rh.id().depth()]
        if ls == 692 and rbx == "HBM18":
            print "692, HBM18 =", rh.id().ieta(), rh.id().iphi(), rh.id().depth()
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

        # loop over rbxes w/ no entries:
        for rbx in tmp_rbx_list:
            unix_time = int(event.object().time().unixTime())
            if rbx not in missing_dictionary:
                # if the rbx has no dictionary entry, create it.
                missing_dictionary[rbx] = set([ls])
                missing_sub_dictionary[(rbx,ls)] = set([unix_time])
            else:
                missing_dictionary[rbx].add(ls)
                if (rbx, ls) not in missing_sub_dictionary:
                    missing_sub_dictionary[(rbx,ls)] = set([unix_time])
                else:
                    missing_sub_dictionary[(rbx,ls)].add(unix_time)
                    
    #if nEvent > 5000: break
                

print "Done with event loop.  Processed %i events." % nEvent
print "\n"

# Print out results
if missing_dictionary :
    print "Instances of RBX data loss:"
    print "\n"
    print "RBX     #LS     LS range        Start time (GMT)"  
    print "---     ---     --------        ----------------"
    for rbx, ls_set in missing_dictionary.iteritems():

        # Make LS set into list and sort.
        ls_list   = []
        for ls   in ls_set  : ls_list  .append(ls  )
        ls_list.sort()
        
        # Make "rdl instances" out of contiguous groups of LS's :
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

        # Loop over individual instances of data loss and print info
        for j_inst, inst_ls_list in rdl_instances.iteritems():

            # Get starting time:
            time_list = []
            time_set = missing_sub_dictionary[(rbx,inst_ls_list[0])]
            for utime in time_set: time_list.append(utime)
            time_list.sort()
            t0 = time.gmtime(time_list[0])
            if t0.tm_min < 10: str_min = "0"+str(t0.tm_min)
            else: str_min = str(t0.tm_min)
            if t0.tm_sec < 10: str_sec = "0"+str(t0.tm_sec)
            else: str_sec = str(t0.tm_sec)
            str_t0 = str(t0.tm_mon)+"/"+str(t0.tm_mday)+" "+str(t0.tm_hour)+":"+str_min+":"+str_sec
            # print info
            print "%s   %i       [%i,%i]         %s" % (rbx, len(inst_ls_list), inst_ls_list[0], inst_ls_list[len(inst_ls_list)-1], str_t0)
                   
else:
    print "No problem RBXes found."


  
        



