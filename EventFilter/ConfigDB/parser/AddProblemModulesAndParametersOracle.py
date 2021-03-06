#!/usr/bin/env python

# AddProblemModulesAndParameters.py
# Patch for ConfDB problems (Oracle version)
# Jonathan Hollar LLNL Nov. 1, 2007

import os, string, sys, posix, tokenize, array, getopt

#sys.path.append(os.environ.get("CMS_PATH") + "/sw/slc4_ia32_gcc345/external/py2-cx-oracle/4.2/lib/python2.4/site-packages/")

sys.path.append("/afs/cern.ch/user/j/jjhollar/private/")

import cx_Oracle

def main(argv):

    input_cmsswrel = os.environ.get("CMSSW_VERSION")
    input_dbname = "hltdb1"
    input_dbuser = "CMS_HLT_TEST"
    input_dbpwd = "hltdevtest1"
    input_host = "CMS_ORCOFF_INT2R"
    input_dbtype = "Oracle"
    input_problemfile = "myproblems.txt"
    input_verbose = 0;
    
    # Parse command line options
    opts, args = getopt.getopt(sys.argv[1:], "r:p:b:w:c:v:d:u:s:t:o:l:e:a:z:nh", ["release=","sourcepath=","blacklist=","whitelist=","releasename=","verbose=","dbname=","user=","password=","dbtype=","hostname=","configfile=","parsetestdir=","addtorelease=","problemfile=","noload=","help="])
    for o, a in opts:
	if o in ("-r","release="):
	    input_cmsswrel = str(a)
	    print "Fixing release " + input_cmsswrel
	if o in ("-v","verbose="):
	    input_verbose = int(a)
	    print "Verbosity = " + str(input_verbose)
	if o in ("-d","dbname="):
	    input_dbname = str(a)
	    print "Using DB named " + input_dbname
	if o in ("-u","user="):
	    input_dbuser = str(a)
	    print "Connecting as user " + input_dbuser
	if o in ("-s","password="):
	    input_dbpwd = str(a)
	    print "Use DB password " + input_dbpwd
	if o in ("-o","hostname="):
	    input_host = str(a)
	    print "Use hostname " + input_host
        if o in ("-z","problemfile="):
            input_problemfile = str(a)
            print "Using problem file " + input_problemfile

    myFixer = AddProblemModulesAndParametersOracle(input_verbose,1)

    # Connect to DB, get module types
    connection = cx_Oracle.connect(input_dbuser+"/"+input_dbpwd+"@"+input_host)
    
    cursor = connection.cursor() 
    myFixer.ConfdbOracleConnect(cursor,input_cmsswrel)

    problemfile = open(input_problemfile)
    problemlines = problemfile.readlines()
    currentcomponent = ''
    componenttable = ''
    componentsuperid = 0
    baseclass = ''
    varname = ''
    vartype = ''
    varval = ''
    vecvarvals = []	
    modcvstag = 'V00-00-00'
    thenewparams = []
    thenewvecparams = []
    thenewpsets = []
    thenewvpsets = []

    for problemline in problemlines:
	if(len(problemline.split()) == 0):
	    continue
	if(problemline.split()[0] == "Module"):
	    currentcomponent = problemline.split()[2]
	    baseclass = problemline.split()[1]
	    componenttable = "ModuleTemplates"
	    print "Baseclass is " + baseclass

	    packageid = myFixer.ConfdbInsertPackageSubsystem(cursor,problemline.split()[3].lstrip().rstrip(),problemline.split()[4].lstrip().rstrip())

	    componentsuperid = myFixer.ConfdbCheckModuleExistence(cursor,baseclass,currentcomponent,"")
	    if(componentsuperid == 0):
		print "Add module"
		componentsuperid = myFixer.ConfdbLoadNewModuleTemplate(cursor,currentcomponent,baseclass,modcvstag,packageid)
	    else:
		print "Module exists"
	elif(problemline.split()[0] == "Service"):
	    currentcomponent = problemline.split()[1]
	    componenttable = "ServiceTemplates"

	    packageid = myFixer.ConfdbInsertPackageSubsystem(cursor,problemline.split()[2].lstrip().rstrip(),problemline.split()[3].lstrip().rstrip())

	    componentsuperid = myFixer.ConfdbCheckServiceExistence(cursor,currentcomponent,"")
	    if(componentsuperid == 0):
		print "Add Service"
		componentsuperid = myFixer.ConfdbLoadNewServiceTemplate(cursor,currentcomponent,modcvstag,packageid)
	    else:
		print "Service exists"
	elif(problemline.split()[0] == "ESSource"):
	    currentcomponent = problemline.split()[1]
	    componenttable = "ESSourceTemplates"

	    packageid = myFixer.ConfdbInsertPackageSubsystem(cursor,problemline.split()[2].lstrip().rstrip(),problemline.split()[3].lstrip().rstrip())

	    componentsuperid = myFixer.ConfdbCheckESSourceExistence(cursor,currentcomponent,"")
	    if(componentsuperid == 0):
		print "Add ESSource"
		componentsuperid = myFixer.ConfdbLoadNewESSourceTemplate(cursor,currentcomponent,modcvstag,packageid)
	    else:
		print "ESSource exists"
	elif(problemline.split()[0] == "EDSource"):
	    currentcomponent = problemline.split()[1]
	    componenttable = "EDSourceTemplates"

	    packageid = myFixer.ConfdbInsertPackageSubsystem(cursor,problemline.split()[2].lstrip().rstrip(),problemline.split()[3].lstrip().rstrip())

	    componentsuperid = myFixer.ConfdbCheckEDSourceExistence(cursor,currentcomponent,"")
	    if(componentsuperid == 0):
		print "Add EDSource"
		componentsuperid = myFixer.ConfdbLoadNewEDSourceTemplate(cursor,currentcomponent,modcvstag,packageid)
	    else:
		print "EDSource exists"
	elif(problemline.split()[0] == "ESModule"):
	    currentcomponent = problemline.split()[1]
	    componenttable = "ESModuleTemplates"

	    packageid = myFixer.ConfdbInsertPackageSubsystem(cursor,problemline.split()[2].lstrip().rstrip(),problemline.split()[3].lstrip().rstrip())

	    componentsuperid = myFixer.ConfdbCheckESModuleExistence(cursor,currentcomponent,"")
	    if(componentsuperid == 0):
		print "Add ESModule"
		componentsuperid = myFixer.ConfdbLoadNewESModuleTemplate(cursor,currentcomponent,modcvstag,packageid)
	    else:
		print "ESModule exists"
	else:
	    # This is a parameter of the component
	    varline = problemline.split()[0].split('.')
	    varname = problemline.split()[1].lstrip().rstrip()
	    vartype = problemline.split()[2].lstrip().rstrip()

            if(problemline.find('tracked') == -1 and problemline.find('untracked') == -1):
                print 'Parameter ' + varname + ' was not defined as tracked or untracked. Will not be loaded'
                continue

	    if(vartype == "vstring" or vartype == "vint32" or vartype == "vdouble" or vartype == "vuint32" or vartype == "VInputTag"):
		if(problemline.find('{') != -1 and problemline.find('}') != -1):
                    #Vector
                    if(problemline.find(',') != -1):
                        vecvarvals = problemline.split('{')[1].split('}')[0].split(',')
                    else:
                        vecvarvals = []
                        singleval = problemline.split('{')[1].split('}')[0].lstrip().rstrip()
                        vecvarvals.append(singleval)
		    vartracked = problemline.split('}')[1].lstrip().rstrip()
	    elif(len(problemline.split()) == 5):
		# Regular parameter with a default value
		varval = problemline.split()[3].lstrip().rstrip()
		vartracked = problemline.split()[4].lstrip().rstrip()
            elif(problemline.find('"') != -1):
                varval = '"' + problemline.split('"')[1] + '"'
                vartracked = problemline.split('"')[2].lstrip().rstrip()
	    else:
		# Regular parameter with no default
		varval = 'None'
		vartracked = problemline.split()[3].lstrip().rstrip()

	    paramexists = myFixer.CheckParameterExistence(cursor,currentcomponent,componentsuperid,varname,vartype)
	    if(paramexists != 0):
		continue

	    thenewseq = myFixer.GetNextSequenceNb(cursor,currentcomponent,componenttable)

	    if(len(varline) == 2):		
		print "\tComponent is " + varline[0] + ", parameter is " + vartype + " " + varname + " = " + str(varval) + " (" + vartracked +")" + "sequence nb is " + str(thenewseq)

	    if(len(varline) == 3):
		print "\tComponent is " + varline[0] + ", parameter is " + " (in  (V)Pset " + varline[1] + ") " + vartype + " " + varname + " = " + str(varval) + " (" + vartracked +")" + "sequence nb is " + str(thenewseq)

	    if(vartype == "PSet"):
		thenewpsets.append((varname,'','','',vartracked,0,'',thenewseq))
		myFixer.ConfdbAttachParameterSets(cursor,componentsuperid,thenewpsets,thenewvpsets)
	    elif(vartype == "VPSet"):
		thenewvpsets.append((varname,'','','',vartracked,0,0,thenewseq))
		myFixer.ConfdbAttachParameterSets(cursor,componentsuperid,thenewpsets,thenewvpsets)
	    elif (vartype == "vstring" or vartype == "vint32" or vartype == "vdouble" or vartype == "vuint32" or vartype == "VInputTag"):
		for vecval in vecvarvals:
		    print '\tVector entry ' + vecval
                if(vartracked == 'untracked'):
                    vecvarvals = []
		thenewvecparams.append((vartype, varname, vecvarvals, vartracked, thenewseq))
		myFixer.ConfdbAttachParameters(cursor,componentsuperid,thenewparams,thenewvecparams)
	    else:
                if(vartracked == 'untracked'):
                    varval = None
		thenewparams.append((vartype,varname,varval,vartracked,thenewseq))
		myFixer.ConfdbAttachParameters(cursor,componentsuperid,thenewparams,thenewvecparams)

	    thenewparams = []
	    thenewvecparams = []
	    thenewpsets = []
	    thenewvpsets = []
	    vecvarvals = []

    myFixer.CheckForDuplicates(cursor,input_cmsswrel)
    connection.commit()
    connection.close()

class AddProblemModulesAndParametersOracle:

    def __init__(self, verbosity, addtorelease):
	self.data = []
	self.changes = []
        self.allmodules = []
        self.paramtypedict = {}
        self.modtypedict = {}
	self.releasekey = -1
	self.verbose = int(verbosity)
	self.addtorel = int(addtorelease)
	self.connection = None
	self.fwknew = 0
	self.fwkunchanged = 0
	self.fwkchanged = 0

    # Connect to the Confdb db
    def ConfdbOracleConnect(self,thecursor,therelease):
        # Do some one-time operations - get dictionaries of parameter, module,
        # and service type mappings so we don't have to do this every time
        thecursor.execute("SELECT ParameterTypes.paramType, ParameterTypes.paramTypeId FROM ParameterTypes")
        temptuple = thecursor.fetchall()
	for temptype, tempname in temptuple:
	    self.paramtypedict[temptype] = tempname
            
        thecursor.execute("SELECT ModuleTypes.type, ModuleTypes.typeId FROM ModuleTypes")
        temptuple = thecursor.fetchall()
	for temptype, tempname in temptuple:
	    self.modtypedict[temptype] = tempname
            
        thecursor.execute("SELECT SoftwareReleases.releaseId FROM SoftwareReleases WHERE (releaseTag = '" + therelease + "')")
        therelnum =  thecursor.fetchone()

	self.releasekey = therelnum[0]
        

    # Given a tag of a module, check if its template exists in the DB
    def ConfdbCheckModuleExistence(self,thecursor,modtype,modname,modtag):
	thecursor.execute("SELECT * FROM SuperIds")

        # Get the module type (base class) ID
	modtypestr = str(self.modtypedict[modtype])

        # See if a module of this type, name, and CVS tag already exists
	if(self.verbose > 2):
	    print "SELECT ModuleTemplates.superId FROM ModuleTemplates JOIN SuperIdReleaseAssoc ON (SuperIdReleaseAssoc.superId = ModuleTemplates.superId) WHERE (SuperIdReleaseAssoc.releaseId = " + str(self.releasekey) + ") AND (ModuleTemplates.name = '" + modname + "') AND (ModuleTemplates.typeId = '" + modtypestr + "')"
	thecursor.execute("SELECT ModuleTemplates.superId FROM ModuleTemplates JOIN SuperIdReleaseAssoc ON (SuperIdReleaseAssoc.superId = ModuleTemplates.superId) WHERE (SuperIdReleaseAssoc.releaseId = " + str(self.releasekey) + ") AND (ModuleTemplates.name = '" + modname + "') AND (ModuleTemplates.typeId = '" + modtypestr + "')")

	modsuperid = thecursor.fetchone()

        if(modsuperid):
            return modsuperid[0]
        else:
            return 0
        
        return modsuperid

    # Given a tag of a service, check if its template exists in the DB
    def ConfdbCheckServiceExistence(self,thecursor,servname,servtag):
	thecursor.execute("SELECT * FROM SuperIds")

        # See if a service of this name and CVS tag already exists
	if(self.verbose > 2):
	    print "SELECT ServiceTemplates.superId FROM ServiceTemplates JOIN SuperIdReleaseAssoc ON (SuperIdReleaseAssoc.superId = ServiceTemplates.superId) WHERE (SuperIdReleaseAssoc.releaseId = " + str(self.releasekey) + ") AND (ServiceTemplates.name = '" + servname + "')"
	thecursor.execute("SELECT ServiceTemplates.superId FROM ServiceTemplates JOIN SuperIdReleaseAssoc ON (SuperIdReleaseAssoc.superId = ServiceTemplates.superId) WHERE (SuperIdReleaseAssoc.releaseId = " + str(self.releasekey) + ") AND (ServiceTemplates.name = '" + servname + "')")

	servsuperid = thecursor.fetchone()

        if(servsuperid):
            return servsuperid[0]
        else:
            return 0
        
        return servsuperid

    # Given a tag of an es_source, check if its template exists in the DB
    def ConfdbCheckESSourceExistence(self,thecursor,srcname,srctag):
	thecursor.execute("SELECT * FROM SuperIds")

        # See if a service of this name and CVS tag already exists
	if(self.verbose > 2):
	    print "SELECT ESSourceTemplates.superId FROM ESSourceTemplates JOIN SuperIdReleaseAssoc ON (SuperIdReleaseAssoc.superId = ESSourceTemplates.superId) WHERE (SuperIdReleaseAssoc.releaseId = " + str(self.releasekey) + ") AND (ESSourceTemplates.name = '" + srcname + "')"
	thecursor.execute("SELECT ESSourceTemplates.superId FROM ESSourceTemplates JOIN SuperIdReleaseAssoc ON (SuperIdReleaseAssoc.superId = ESSourceTemplates.superId) WHERE (SuperIdReleaseAssoc.releaseId = " + str(self.releasekey) + ") AND (ESSourceTemplates.name = '" + srcname + "')")

	srcsuperid = thecursor.fetchone()

        if(srcsuperid):
            return srcsuperid[0]
        else:
            return 0
        
        return srcsuperid

    # Given a tag of an ed_source, check if its template exists in the DB
    def ConfdbCheckEDSourceExistence(self,thecursor,srcname,srctag):
	thecursor.execute("SELECT * FROM SuperIds")

        # See if a service of this name and CVS tag already exists
	if(self.verbose > 2):
	    print "SELECT EDSourceTemplates.superId FROM EDSourceTemplates JOIN SuperIdReleaseAssoc ON (SuperIdReleaseAssoc.superId = EDSourceTemplates.superId) WHERE (SuperIdReleaseAssoc.releaseId = " + str(self.releasekey) + ") AND (EDSourceTemplates.name = '" + srcname + "')"
	thecursor.execute("SELECT EDSourceTemplates.superId FROM EDSourceTemplates JOIN SuperIdReleaseAssoc ON (SuperIdReleaseAssoc.superId = EDSourceTemplates.superId) WHERE (SuperIdReleaseAssoc.releaseId = " + str(self.releasekey) + ") AND (EDSourceTemplates.name = '" + srcname + "')")

	srcsuperid = thecursor.fetchone()

        if(srcsuperid):
            return srcsuperid[0]
        else:
            return 0
        
        return srcsuperid

    # Given a tag of an es_module, check if its template exists in the DB
    def ConfdbCheckESModuleExistence(self,thecursor,srcname,srctag):
	thecursor.execute("SELECT * FROM SuperIds")

        # See if a service of this name and CVS tag already exists
	if(self.verbose > 2):
	    print "SELECT ESModuleTemplates.superId FROM ESModuleTemplates JOIN SuperIdReleaseAssoc ON (SuperIdReleaseAssoc.superId = ESModuleTemplates.superId) WHERE (SuperIdReleaseAssoc.releaseId = " + str(self.releasekey) + ") AND (ESModuleTemplates.name = '" + srcname + "')"
	thecursor.execute("SELECT ESModuleTemplates.superId FROM ESModuleTemplates JOIN SuperIdReleaseAssoc ON (SuperIdReleaseAssoc.superId = ESModuleTemplates.superId) WHERE (SuperIdReleaseAssoc.releaseId = " + str(self.releasekey) + ") AND (ESModuleTemplates.name = '" + srcname + "')")

	esmodsuperid = thecursor.fetchone()

        if(esmodsuperid):
            return esmodsuperid[0]
        else:
            return 0
        
        return esmodsuperid
               
    # Create a new module template in the DB
    #def ConfdbLoadNewModuleTemplate(self,thecursor,modclassname,modbaseclass,modcvstag,parameters,vecparameters,paramsets,vecparamsets):
    def ConfdbLoadNewModuleTemplate(self,thecursor,modclassname,modbaseclass,modcvstag,softpackageid):
	
	self.fwknew = self.fwknew + 1

	# Allocate a new SuperId
	newsuperid = -1
	thecursor.execute("INSERT INTO SuperIds VALUES('')")

#	thecursor.execute("SELECT LAST_INSERT_ID()")

#	thecursor.execute("SELECT superId FROM SuperIds ORDER BY superId DESC");
        thecursor.execute("SELECT SuperId_Sequence.currval from dual")  
	newsuperid = (thecursor.fetchall()[0])[0]

	# Attach this template to the currect release
	thecursor.execute("INSERT INTO SuperIdReleaseAssoc (superId, releaseId) VALUES (" + str(newsuperid) + ", " + str(self.releasekey) + ")")

	# Get the module type (base class)
	modbaseclassid = self.modtypedict[modbaseclass]

	# Now create a new module
	if(self.verbose > 2):
	    print "INSERT INTO ModuleTemplates (superId, typeId, name, cvstag, packageId) VALUES (" + str(newsuperid) + ", " + str(modbaseclassid) + ", '" + modclassname + "', '" + modcvstag +  "', '" + str(softpackageid) + "')"
            
        #JH - checking for duplicates
        print "SELECT * FROM ModuleTemplates JOIN SuperIdReleaseAssoc ON (SuperIdReleaseAssoc.superId = ModuleTemplates.superId) JOIN SoftwareReleases ON (SuperIdReleaseAssoc.releaseId = " + str(self.releasekey) + ") WHERE ModuleTemplates.name = '" + str(modclassname) + "'"
        thecursor.execute("SELECT * FROM ModuleTemplates JOIN SuperIdReleaseAssoc ON (SuperIdReleaseAssoc.superId = ModuleTemplates.superId) JOIN SoftwareReleases ON (SuperIdReleaseAssoc.releaseId = " + str(self.releasekey) + ") WHERE ModuleTemplates.name = '" + str(modclassname) + "'")
        thematch = thecursor.fetchall()
        if(thematch):
            print 'DISASTER! - inserting a duplicate ModuleTemplate!'
            print thematch
        
	thecursor.execute("INSERT INTO ModuleTemplates (superId, typeId, name, cvstag, packageId) VALUES (" + str(newsuperid) + ", " + str(modbaseclassid) + ", '" + modclassname + "', '" + modcvstag +  "', '" + str(softpackageid) + "')")

	
	# Now deal with parameters
#	self.ConfdbAttachParameters(thecursor,newsuperid,parameters,vecparameters)
#	self.ConfdbAttachParameterSets(thecursor,newsuperid,paramsets,vecparamsets)

	return newsuperid 

    # End ConfdbLoadNewModuleTemplate
	
    # Create a new service template in the DB
    #def ConfdbLoadNewServiceTemplate(self,thecursor,servclassname,servcvstag,parameters,vecparameters,paramsets,vecparamsets):
    def ConfdbLoadNewServiceTemplate(self,thecursor,servclassname,servcvstag,softpackageid):

	self.fwknew = self.fwknew + 1

	# Allocate a new SuperId
	newsuperid = -1
	thecursor.execute("INSERT INTO SuperIds VALUES('')")

#	thecursor.execute("SELECT LAST_INSERT_ID()")

#	thecursor.execute("SELECT superId FROM SuperIds ORDER BY superId DESC")
        thecursor.execute("SELECT SuperId_Sequence.currval from dual")  
	newsuperid = (thecursor.fetchall()[0])[0]

	# Attach this template to the currect release
	thecursor.execute("INSERT INTO SuperIdReleaseAssoc (superId, releaseId) VALUES (" + str(newsuperid) + ", " + str(self.releasekey) + ")")

	# Now create a new service
	thecursor.execute("INSERT INTO ServiceTemplates (superId, name, cvstag, packageId) VALUES (" + str(newsuperid) + ", '" + servclassname + "', '" + servcvstag +  "', '" + str(softpackageid) + "')")
	if(self.verbose > 2):
	    print "INSERT INTO ServiceTemplates (superId, name, cvstag, packageId) VALUES (" + str(newsuperid) + ", '" + servclassname + "', '" + servcvstag +  "', '" + str(softpackageid) + "')"
	
	# Now deal with parameters
#	self.ConfdbAttachParameters(thecursor,newsuperid,parameters,vecparameters)
#	self.ConfdbAttachParameterSets(thecursor,newsuperid,paramsets,vecparamsets)

	return newsuperid 

    # End ConfdbLoadNewServiceTemplate

    # Create a new es_source template in the DB
    #def ConfdbLoadNewESSourceTemplate(self,thecursor,srcclassname,srccvstag,parameters,vecparameters,paramsets,vecparamsets):
    def ConfdbLoadNewESSourceTemplate(self,thecursor,srcclassname,srccvstag,softpackageid):
	
	self.fwknew = self.fwknew + 1

	# Allocate a new SuperId
	newsuperid = -1
	thecursor.execute("INSERT INTO SuperIds VALUES('')")

#	thecursor.execute("SELECT LAST_INSERT_ID()")

#	thecursor.execute("SELECT superId FROM SuperIds ORDER BY superId DESC")
        thecursor.execute("SELECT SuperId_Sequence.currval from dual")  
	newsuperid = (thecursor.fetchall()[0])[0]

        print "newsuperid = " + str(newsuperid)
        print "releasekey = " + str(self.releasekey)

	# Attach this template to the currect release
	thecursor.execute("INSERT INTO SuperIdReleaseAssoc (superId, releaseId) VALUES (" + str(newsuperid) + ", " + str(self.releasekey) + ")")

	# Now create a new es_source
	thecursor.execute("INSERT INTO ESSourceTemplates (superId, name, cvstag, packageId) VALUES (" + str(newsuperid) + ", '" + srcclassname + "', '" + srccvstag +  "', '" + str(softpackageid) + "')")
	if(self.verbose > 2):
	    print "INSERT INTO ESSourceTemplates (superId, name, cvstag, packageId) VALUES (" + str(newsuperid) + ", '" + srcclassname + "', '" + srccvstag +  "', '" + str(softpackageid) + "')"
	
	# Now deal with parameters
#	self.ConfdbAttachParameters(thecursor,newsuperid,parameters,vecparameters)
#	self.ConfdbAttachParameterSets(thecursor,newsuperid,paramsets,vecparamsets)

	return newsuperid 

    # End ConfdbLoadNewESSourceTemplate

    # Create a new ed_source template in the DB
    #def ConfdbLoadNewEDSourceTemplate(self,thecursor,srcclassname,srccvstag,parameters,vecparameters,paramsets,vecparamsets):
    def ConfdbLoadNewEDSourceTemplate(self,thecursor,srcclassname,srccvstag,softpackageid):
	
	self.fwknew = self.fwknew + 1

	# Allocate a new SuperId
	newsuperid = -1
	thecursor.execute("INSERT INTO SuperIds VALUES('')")

#	thecursor.execute("SELECT LAST_INSERT_ID()")

#	thecursor.execute("SELECT superId FROM SuperIds ORDER BY superId DESC")
        thecursor.execute("SELECT SuperId_Sequence.currval from dual")  
	newsuperid = (thecursor.fetchall()[0])[0]
        
	# Attach this template to the currect release
	thecursor.execute("INSERT INTO SuperIdReleaseAssoc (superId, releaseId) VALUES (" + str(newsuperid) + ", " + str(self.releasekey) + ")")

	# Now create a new es_source
	thecursor.execute("INSERT INTO EDSourceTemplates (superId, name, cvstag, packageId) VALUES (" + str(newsuperid) + ", '" + srcclassname + "', '" + srccvstag +  "', '" + str(softpackageid) + "')")
	if(self.verbose > 2):
	    print "INSERT INTO EDSourceTemplates (superId, name, cvstag, packageId) VALUES (" + str(newsuperid) + ", '" + srcclassname + "', '" + srccvstag +  "', '" + str(softpackageid) + "')"
	
	# Now deal with parameters
#	self.ConfdbAttachParameters(thecursor,newsuperid,parameters,vecparameters)
#	self.ConfdbAttachParameterSets(thecursor,newsuperid,paramsets,vecparamsets)

	return newsuperid 

    # End ConfdbLoadNewEDSourceTemplate

    # Create a new es_module template in the DB
    #def ConfdbLoadNewESModuleTemplate(self,thecursor,modclassname,modcvstag,parameters,vecparameters,paramsets,vecparamsets):
    def ConfdbLoadNewESModuleTemplate(self,thecursor,modclassname,modcvstag,softpackageid):
	
	self.fwknew = self.fwknew + 1

	# Allocate a new SuperId
	newsuperid = -1
	thecursor.execute("INSERT INTO SuperIds VALUES('')")

#	thecursor.execute("SELECT LAST_INSERT_ID()")

#	thecursor.execute("SELECT superId FROM SuperIds ORDER BY superId DESC");
        thecursor.execute("SELECT SuperId_Sequence.currval from dual")  
	newsuperid = (thecursor.fetchall()[0])[0]

	# Attach this template to the currect release
	thecursor.execute("INSERT INTO SuperIdReleaseAssoc (superId, releaseId) VALUES (" + str(newsuperid) + ", " + str(self.releasekey) + ")")

	# Now create a new module
	thecursor.execute("INSERT INTO ESModuleTemplates (superId, name, cvstag, packageId) VALUES (" + str(newsuperid) + ", '" + modclassname + "', '" + modcvstag +  "', '" + str(softpackageid) + "')")
	if(self.verbose > 2):
	    print "INSERT INTO ESModuleTemplates (superId, name, cvstag, packageId) VALUES (" + str(newsuperid) + ", " + modclassname + "', '" + modcvstag +  "', '" + str(softpackageid) + "')"
	
	# Now deal with parameters
#	self.ConfdbAttachParameters(thecursor,newsuperid,parameters,vecparameters)
#	self.ConfdbAttachParameterSets(thecursor,newsuperid,paramsets,vecparamsets)

	return newsuperid 

    # End ConfdbLoadNewESModuleTemplate

    # Associate a list of parameters with a component template (via superId)
    def ConfdbAttachParameters(self,thecursor,newsuperid,parameters,vecparameters):

	# First the non-vectors
	for paramtype, paramname, paramval, paramistracked, paramseq in parameters:

	    # int32
	    if(paramtype == "int32" or paramtype == "int" or paramtype == "int32_t"):
		type = self.paramtypedict['int32']

		# Fill Parameters table
		newparamid = self.AddNewParam(thecursor,newsuperid,paramname,type,paramistracked,paramseq)

		if(paramval):
		    if(paramval.find('.') != -1):
			paramval = str(int(float(paramval)))
		    elif(not paramval.isdigit()):
			paramval = None

		# Fill ParameterValues table
		if(paramval == None):
		    if(self.verbose > 2):
			print "No default parameter value found"
		else:
		    thecursor.execute("INSERT INTO Int32ParamValues (paramId, value) VALUES (" + str(newparamid) + ", " + paramval + ")")

	    # uint32
	    elif(paramtype == "uint32" or paramtype == "unsigned int" or paramtype == "uint32_t" or paramtype == "unsigned" or paramtype == "uint"):
		type = self.paramtypedict['uint32']

		if(paramval):
		    if(str(paramval).endswith("U")):
			paramval = (str(paramval).rstrip("U"))

		    if(paramval.find('.') != -1):
			paramval = str(int(float(paramval)))
		    elif(not paramval.isdigit()):
			paramval = None

		# Fill Parameters table
		newparamid = self.AddNewParam(thecursor,newsuperid,paramname,type,paramistracked,paramseq)    

		# Fill ParameterValues table
		if(paramval == None):
		    if(self.verbose > 2):
			print "No default parameter value found"
		else:
		    thecursor.execute("INSERT INTO UInt32ParamValues (paramId, value) VALUES (" + str(newparamid) + ", " + paramval + ")")

            # uint64
            elif(paramtype == "uint64" or paramtype == "uint64_t" or paramtype == "unsigned long long"):
                type = self.paramtypedict['uint64']
                
                if(paramval):
                    if(str(paramval).endswith("U")):
                        paramval = (str(paramval).rstrip("U"))
                        
                    if(paramval.find('.') != -1):
                        paramval = str(int(float(paramval)))
                    elif(not paramval.isdigit()):
                        paramval = None
                                
                # Fill Parameters table
                newparamid = self.AddNewParam(thecursor,newsuperid,paramname,type,paramistracked,paramseq)
                
                # Fill ParameterValues table
                if(paramval == None):
                    if(self.verbose > 2):
                        print "No default parameter value found"
                else:
                    thecursor.execute("INSERT INTO UInt64ParamValues (paramId, value) VALUES (" + str(newparamid) + ", " + paramval + ")")
                    
	    # bool
	    elif(paramtype == "bool"):
		type = self.paramtypedict['bool']

		# Fill Parameters table
		newparamid = self.AddNewParam(thecursor,newsuperid,paramname,type,paramistracked,paramseq)   

                boolval = str(paramval).strip('"').strip()
                if(boolval == "true"):
                    paramval = str(1)
                if(boolval == "false"):
                    paramval = str(0)
                
		# Fill ParameterValues table
		if(paramval == None):
		    if(self.verbose > 2):
			print "No default parameter value found"
		else:
                    if(self.verbose > 2):
                        print "INSERT INTO BoolParamValues (paramId, value) VALUES (" + str(newparamid) + ", " + paramval + ")"
		    thecursor.execute("INSERT INTO BoolParamValues (paramId, value) VALUES (" + str(newparamid) + ", " + paramval + ")")


	    # double
	    elif(paramtype == "double"):
		type = self.paramtypedict['double']

		# Fill Parameters table
		newparamid = self.AddNewParam(thecursor,newsuperid,paramname,type,paramistracked,paramseq)

		if(paramval):
		    if(paramval.find('.') == -1 and (not paramval.isdigit())):
			paramval = None

		# Fill ParameterValues table
		if(paramval == None):
		    if(self.verbose > 2):
			print "No default parameter value found"
		else:
		    thecursor.execute("INSERT INTO DoubleParamValues (paramId, value) VALUES (" + str(newparamid) + ", " + paramval + ")")

	    #string
	    elif(paramtype == "string"):
		type = self.paramtypedict['string']

		# Fill Parameters table
		newparamid = self.AddNewParam(thecursor,newsuperid,paramname,type,paramistracked,paramseq)

		if(paramval == None):
		    if(self.verbose > 2):
			print "No default parameter value found"
		else:
		    # Stupid special case for string variables defined in 
		    # single quotes in .cf* files
		    if(paramval.find("'") != -1):
			# Fill ParameterValues table
			thecursor.execute("INSERT INTO StringParamValues (paramId, value) VALUES (" + str(newparamid) + ", " + paramval + ")")
		    elif(paramval.find('"') != -1):
			thecursor.execute("INSERT INTO StringParamValues (paramId, value) VALUES (" + str(newparamid) + ", '" + paramval + "')")
		    else:
			print "\tWarning: Attempted to load a non-string value to string table:"
			print "\t\tstring " + str(paramname) + " = " + str(paramval)
			print "\t\tLoading parameter with no default value"

	    #FileInPath
	    elif(paramtype == "FileInPath"):
		type = self.paramtypedict['FileInPath']

		# Fill Parameters table
		newparamid = self.AddNewParam(thecursor,newsuperid,paramname,type,paramistracked,paramseq)

		if(paramval == None):
		    if(self.verbose > 2):
			print "No default parameter value found"
		else:
		    # Stupid special case for string variables defined in 
		    # single quotes in .cf* files
		    if(paramval.find("'") != -1):
			# Fill ParameterValues table
			thecursor.execute("INSERT INTO FileInPathParamValues (paramId, value) VALUES (" + str(newparamid) + ", " + paramval + ")")
		    elif(paramval.find('"') != -1):
			thecursor.execute("INSERT INTO FileInPathParamValues (paramId, value) VALUES (" + str(newparamid) + ", '" + paramval + "')")
		    else:
			print "\tWarning: Attempted to load a non-string value to FileInPath table:"
			print "\t\tFileInPath " + str(paramname) + " = " + str(paramval)
			print "\t\tLoading parameter with no default value"


	    # InputTag
	    elif(paramtype == "InputTag"):
		type = self.paramtypedict['InputTag']

		# Fill Parameters table
		newparamid = self.AddNewParam(thecursor,newsuperid,paramname,type,paramistracked,paramseq)

		# Fill ParameterValues table
		if(paramval == None or paramval == ''):
		    if(self.verbose > 2):
			print "No default parameter value found"
		else:
		    if(paramval.find("'") != -1):
                        print "INSERT INTO InputTagParamValues (paramId, value) VALUES (" + str(newparamid) + ", " + paramval + ")"
			thecursor.execute("INSERT INTO InputTagParamValues (paramId, value) VALUES (" + str(newparamid) + ", " + paramval + ")")
		    else:
                        print "INSERT INTO InputTagParamValues (paramId, value) VALUES (" + str(newparamid) + ", '" + paramval + "')"
			thecursor.execute("INSERT INTO InputTagParamValues (paramId, value) VALUES (" + str(newparamid) + ", '" + paramval + "')")

	    else:
		print '\tError: Unknown param type ' + paramtype + ' ' + paramname + ' - do nothing'
	    
	# Now deal with any vectors
	for vecptype, vecpname, vecpvals, vecpistracked, vecpseq in vecparameters:

	    # vector<int32>
	    if(vecptype == "vint32" or vecptype == "int32" or vecptype == "int" or vecptype == "int32_t"):
		type = self.paramtypedict['vint32']

		# Fill Parameters table
		newparamid = self.AddNewParam(thecursor,newsuperid,vecpname,type,vecpistracked,vecpseq)

		sequencer = 0

		for vecpval in vecpvals:
		    if(vecpval):
			# Fill ParameterValues table
			if(self.verbose > 2):
			    print "INSERT INTO VInt32ParamValues (paramId, sequenceNb, value) VALUES (" + str(newparamid) + ", " + str(sequencer) + ", " + vecpval + ")"
			thecursor.execute("INSERT INTO VInt32ParamValues (paramId, sequenceNb, value) VALUES (" + str(newparamid) + ", " + str(sequencer) + ", " + vecpval + ")")   
			sequencer = sequencer + 1

	    # vector<uint32>
	    elif(vecptype == "vunsigned" or vecptype == "uint32" or vecptype == "unsigned int" or vecptype == "uint32_t" or vecptype == "unsigned" or vecptype == "uint32" or vecptype == "uint" or vecptype == "vuint32"):
		type = self.paramtypedict['vuint32']

		# Fill Parameters table
		newparamid = self.AddNewParam(thecursor,newsuperid,vecpname,type,vecpistracked,vecpseq)

		sequencer = 0

		for vecpval in vecpvals:
		    if(vecpval):
			# Fill ParameterValues table
			if(self.verbose > 2):
			    print "INSERT INTO VUInt32ParamValues (paramId, sequenceNb, value) VALUES (" + str(newparamid) + ", " + str(sequencer) + ", " + vecpval + ")"
			thecursor.execute("INSERT INTO VUInt32ParamValues (paramId, sequenceNb, value) VALUES (" + str(newparamid) + ", " + str(sequencer) + ", " + vecpval + ")")   
			sequencer = sequencer + 1

	    #vector<double>
	    elif(vecptype == "vdouble" or vecptype == "double"):
		type = self.paramtypedict['vdouble']

		# Fill Parameters table
		newparamid = self.AddNewParam(thecursor,newsuperid,vecpname,type,vecpistracked,vecpseq)

		sequencer = 0

		for vecpval in vecpvals:
		    if(vecpval):
			# Fill ParameterValues table
			if(self.verbose > 2):
			    print "INSERT INTO VDoubleParamValues (paramId, sequenceNb, value) VALUES (" + str(newparamid) + ", " + str(sequencer) + ", " + vecpval + ")"
			thecursor.execute("INSERT INTO VDoubleParamValues (paramId, sequenceNb, value) VALUES (" + str(newparamid) + ", " + str(sequencer) + ", " + vecpval + ")")   
			sequencer = sequencer + 1

	    # vector<InputTag>
	    elif(vecptype == "VInputTag" or vecptype == "InputTag"):
		type = self.paramtypedict['VInputTag']

		# Fill Parameters table
		newparamid = self.AddNewParam(thecursor,newsuperid,vecpname,type,vecpistracked,vecpseq)

		sequencer = 0

		for vecpval in vecpvals:
		    if(vecpval):
			# Fill ParameterValues table
			if(self.verbose > 2):
			    print "INSERT INTO VInputTagParamValues (paramId, sequenceNb, value) VALUES (" + str(newparamid) + ", " + str(sequencer) + ", '" + vecpval + "')"
			thecursor.execute("INSERT INTO VInputTagParamValues (paramId, sequenceNb, value) VALUES (" + str(newparamid) + ", " + str(sequencer) + ", '" + vecpval + "')")   
			sequencer = sequencer + 1

	    # vector<string>
	    elif(vecptype == "vstring" or vecptype == "vString" or vecptype == "string"):
		type = self.paramtypedict['vstring']

		# Fill Parameters table
		newparamid = self.AddNewParam(thecursor,newsuperid,vecpname,type,vecpistracked,vecpseq)

		sequencer = 0

		for vecpval in vecpvals:
		    if(vecpval):
			# Handle signle quoted strings
			if(vecpval.find("'") != -1):
			    # Fill ParameterValues table
			    if(self.verbose > 2):
				print "INSERT INTO VStringParamValues (paramId, sequenceNb, value) VALUES (" + str(newparamid) + ", " + str(sequencer) +", " + vecpval + ")"
			    thecursor.execute("INSERT INTO VStringParamValues (paramId, sequenceNb, value) VALUES (" + str(newparamid) + ", " + str(sequencer) + ", " + vecpval + ")")   
			else:
			    # Fill ParameterValues table
			    if(self.verbose > 2):
				print "INSERT INTO VStringParamValues (paramId, sequenceNb, value) VALUES (" + str(newparamid) + ", " + str(sequencer) + ", '" + vecpval + "')"
			    thecursor.execute("INSERT INTO VStringParamValues (paramId, sequenceNb, value) VALUES (" + str(newparamid) + ", " + str(sequencer) + ", '" + vecpval + "')")   

			sequencer = sequencer + 1

	    else:
		if(self.verbose > 0):
		    print 'Unknown vector param type ' + vecptype + ' ' + vecpname + ' - do nothing'

    # End ConfdbAttachParameters


    # Associate a ParameterSet/VParameterSet with a component template 
    def ConfdbAttachParameterSets(self,thecursor,newsuperid,paramsets,vecparamsets):

	lastpsetname = ''
	psetcache = []
	lastpsetseqdict = {}
	localseqcount = 0
        psetsuperiddict = {}

	for pset, psettype, psetname, psetval, psettracked, psetseq, psetnesting, psetpsetseq in paramsets:
	    # If this is the first entry in this PSet for this component, add it to the ParameterSets table
	    if(not pset in psetcache):
		psetcache.append(pset)

		localseqcount = 0

                if(psettracked == 'tracked'):
                    psettracked = str(1)
                elif(psettracked == 'untracked'):
                    psettracked = str(0)

		thecursor.execute("INSERT INTO SuperIds VALUES('')")
#		thecursor.execute("SELECT LAST_INSERT_ID()")

#		thecursor.execute("SELECT superId FROM SuperIds ORDER BY superId DESC")
                thecursor.execute("SELECT SuperId_Sequence.currval from dual")  
		newparamsetid = thecursor.fetchone()[0]	

		# Add a new PSet
		if(self.verbose > 2):
		    print "INSERT INTO ParameterSets (superId, name, tracked) VALUES (" + str(newparamsetid) + ", '" + pset + "', " + psettracked + ")"
		thecursor.execute("INSERT INTO ParameterSets (superId, name, tracked) VALUES (" + str(newparamsetid) + ", '" + pset + "', " + psettracked + ")")

                psetsuperiddict[pset+str(newsuperid)] = newparamsetid
                
		# Each new top level PSet points to the framework component
		if(psetnesting == 'None' or psetnesting == ''):
		    # Attach the PSet to a Fwk component via their superIds
		    if(self.verbose > 2):
			print "INSERT INTO SuperIdParamSetAssoc (superId, psetId, sequenceNb) VALUES (" + str(newsuperid) + ", " + str(newparamsetid) + ", " + str(psetpsetseq) + ")"
		    thecursor.execute("INSERT INTO SuperIdParamSetAssoc (superId, psetId, sequenceNb) VALUES (" + str(newsuperid) + ", " + str(newparamsetid) + ", " + str(psetpsetseq) + ")")

		# Nested PSets point to the relevant top level PSet 
		else:
                    # Attach the PSet to another PSet component via their superIds
                    # We can't rely on Oracle to keep the ordering on 2-node machines, so get the parent association
                    # from an internal dictionary instead...
                    #		    if(self.verbose > 2):
                    #			print "SELECT ParameterSets.superId FROM ParameterSets WHERE (name = '" + psetnesting + "') ORDER BY ParameterSets.superId DESC"
                    #		    thecursor.execute("SELECT ParameterSets.superId FROM ParameterSets WHERE (name = '" + psetnesting + "') ORDER BY ParameterSets.superId DESC")
                    #
                    #		    toplevelid = thecursor.fetchone()[0]

                    toplevelid = psetsuperiddict[psetnesting + str(newsuperid)]
                    
                    if(psetnesting in lastpsetseqdict):
                        psetpsetseq = lastpsetseqdict[psetnesting]
                    else:
                        psetpsetseq = 0

		    lastpsetseqdict[psetnesting] = psetpsetseq + 1
		    lastpsetseqdict[pset] = localseqcount

		    if(self.verbose > 2):
			print "INSERT INTO SuperIdParamSetAssoc (superId, psetId, sequenceNb) VALUES (" + str(toplevelid) + ", " + str(newparamsetid) + ", " + str(psetpsetseq) + ")"
		    thecursor.execute("INSERT INTO SuperIdParamSetAssoc (superId, psetId, sequenceNb) VALUES (" + str(toplevelid) + ", " + str(newparamsetid) + ", " + str(psetpsetseq) + ")")   
	    else:
		if(pset in lastpsetseqdict):
		    localseqcount = lastpsetseqdict[pset]
		else:
		    localseqcount = 0		    
		    lastpsetseqdict[pset] = localseqcount

	    # Now make new entries for each parameter in this PSet if they exist
	    if(psettype == '' or psetname == ''):
		continue

	    if(psettype == "int" or psettype == "int32_t"):
		psettype = "int32"
	    if(psettype == "uint32_t" or psettype == "unsigned int" or psettype == "uint"):
		psettype = "uint32"
#	    if(psettype == "FileInPath"):
#		psettype = "string"
	    if(psettype == "vunsigned"):
		psettype = "vuint32"

	    if(not (psettype in self.paramtypedict)):
		continue

	    type = self.paramtypedict[psettype]

	    psetseq = localseqcount
	    localseqcount = localseqcount + 1
	    lastpsetseqdict[pset] = localseqcount

	    # Fill Parameters table
	    newparammemberid = self.AddNewParam(thecursor,newparamsetid,psetname,type,psettracked,psetseq)	    
	    if(psetval == ''):
		continue

	    if(psettype == "int32" or psettype == "int" or psettype == "int32_t"):
		# Protect against loading non-integer values. Also deal with implicit fp->int conversions and hex.
		if(psetval):
		    if(psetval.find('.') != -1):
			psetval = str(int(float(psetval)))
		    elif(not psetval.isdigit()):
			psetval = None

		if(psetval == None):
		    if(self.verbose > 2):
			print "No default parameter value found"
		else:
		    thecursor.execute("INSERT INTO Int32ParamValues (paramId, value) VALUES (" + str(newparammemberid) + ", " + psetval + ")")
	    elif(psettype == "uint32" or psettype == "unsigned int" or psettype == "uint32_t" or psettype == "uint"):
		if(str(psetval).endswith("U")):
		    psetval = (str(psetval).rstrip("U"))

		# Protect against loading non-integer values. Also deal with implicit fp->int conversions and hex.
		if(psetval):
		    if(psetval.find('.') != -1):
			psetval = str(int(float(psetval)))
		    elif(not psetval.isdigit()):
			psetval = None

		if(psetval == None):
		    if(self.verbose > 2):
			print "No default parameter value found"
		else:
		    thecursor.execute("INSERT INTO UInt32ParamValues (paramId, value) VALUES (" + str(newparammemberid) + ", " + psetval + ")")

	    elif(psettype == "bool"):
                print "psetval = " + str(psetval)
                boolval = str(psetval).strip('"').strip()
                if(boolval == "true"):
                    psetval = str(1)
                if(boolval == "false"):
                    psetval = str(0)

                print "INSERT INTO BoolParamValues (paramId, value) VALUES (" + str(newparammemberid) + ", " + psetval + ")"

		thecursor.execute("INSERT INTO BoolParamValues (paramId, value) VALUES (" + str(newparammemberid) + ", " + psetval + ")")
	    elif(psettype == "double"):
		if(psetval):
		    if(psetval.find('.') == -1 and (not psetval.isdigit())):
			psetval = None

		if(psetval == None):
		    if(self.verbose > 2):
			print "No default parameter value found"
		else:
		    thecursor.execute("INSERT INTO DoubleParamValues (paramId, value) VALUES (" + str(newparammemberid) + ", " + psetval + ")")
	    elif(psettype == "string"):
		if(psetval.find("'") != -1):
		    thecursor.execute("INSERT INTO StringParamValues (paramId, value) VALUES (" + str(newparammemberid) + ", " + psetval + ")")
		elif(psetval.find('"') != -1):
		    thecursor.execute("INSERT INTO StringParamValues (paramId, value) VALUES (" + str(newparammemberid) + ", '" + psetval + "')")
		else:
		    print "\tWarning: Attempted to load a non-string value to string table:"
		    print "\t\tstring " + str(psetname) + " = " + str(psetval)
		    print "\t\tLoading parameter with no default value"
	    elif(psettype == "FileInPath"):
		if(psetval.find("'") != -1):
		    thecursor.execute("INSERT INTO FileInPathParamValues (paramId, value) VALUES (" + str(newparammemberid) + ", " + psetval + ")")
		elif(psetval.find('"') != -1):
		    thecursor.execute("INSERT INTO FileInPathParamValues (paramId, value) VALUES (" + str(newparammemberid) + ", '" + psetval + "')")
		else:
		    print "\tWarning: Attempted to load a non-string value to FileInPath table:"
		    print "\t\tFileInPath " + str(psetname) + " = " + str(psetval)
		    print "\t\tLoading parameter with no default value"
	    elif(psettype == "InputTag"):
		if(psetval.find("'") != -1):
		    thecursor.execute("INSERT INTO InputTagParamValues (paramId, value) VALUES (" + str(newparammemberid) + ", " + psetval + ")")
		else:
		    thecursor.execute("INSERT INTO InputTagParamValues (paramId, value) VALUES (" + str(newparammemberid) + ", '" + psetval + "')")
	    elif(psettype == "vint32"):
		sequencer = 0
		entries = psetval.lstrip().rstrip().lstrip('{').rstrip('}').split(',')
		for entry in entries:
		    thecursor.execute("INSERT INTO VInt32ParamValues (paramId, sequenceNb, value) VALUES (" + str(newparammemberid) + ", " + str(sequencer) + ", " + entry.lstrip().rstrip() + ")")   
		    sequencer = sequencer + 1	
	    elif(psettype == "vunsigned" or psettype == "vuint32"):
		sequencer = 0
		entries = psetval.lstrip().rstrip().lstrip('{').rstrip('}').split(',')
		for entry in entries:
		    thecursor.execute("INSERT INTO VUInt32ParamValues (paramId, sequenceNb, value) VALUES (" + str(newparammemberid) + ", " + str(sequencer) + ", " + entry.lstrip().rstrip() + ")")   
		    sequencer = sequencer + 1	
	    elif(psettype == "vdouble"):
		sequencer = 0
		entries = psetval.lstrip().rstrip().lstrip('{').rstrip('}').split(',')
		for entry in entries:
		    thecursor.execute("INSERT INTO VDoubleParamValues (paramId, sequenceNb, value) VALUES (" + str(newparammemberid) + ", " + str(sequencer) + ", " + entry.lstrip().rstrip() + ")")   
		    sequencer = sequencer + 1
	    elif(psettype == "vstring" or psettype == "vString"):
		sequencer = 0
		entries = psetval.lstrip().rstrip().lstrip('{').rstrip('}').split(',')
		for entry in entries:
		    thecursor.execute("INSERT INTO VStringParamValues (paramId, sequenceNb, value) VALUES (" + str(newparammemberid) + ", " + str(sequencer) + ", '" + entry.lstrip().rstrip() + "')")   
		    sequencer = sequencer + 1		
	    elif(psettype == "VInputTag"):
		sequencer = 0
		entries = psetval.lstrip().rstrip().lstrip('{').rstrip('}').split(',')
		for entry in entries:
		    thecursor.execute("INSERT INTO VInputTagParamValues (paramId, sequenceNb, value) VALUES (" + str(newparammemberid) + ", " + str(sequencer) + ", " + entry.lstrip().rstrip() + ")")   
		    sequencer = sequencer + 1	


	# Now VPSets
	vpsetcache = []

	for vpset, vpsettype, vpsetname, vpsetval, vpsettracked, vpsetindex, vpsetseq, vpsetpsetseq in vecparamsets:
	    # If this is the first entry in this VPSet for this component, add it to the ParameterSets table
	    if(not vpset in vpsetcache):
		vpsetcache.append(vpset)

		localseqcount = 0
                if(vpsettracked == 'tracked'):
                    vpsettracked = str(1)
                elif(vpsettracked == 'untracked'):
                    vpsettracked = str(0)
                
		# Each new VPSet gets a new SuperId
		thecursor.execute("INSERT INTO SuperIds VALUES('')")
#		thecursor.execute("SELECT LAST_INSERT_ID()")

#		thecursor.execute("SELECT superId FROM SuperIds ORDER BY superId DESC")
                thecursor.execute("SELECT SuperId_Sequence.currval from dual")  
		newvparamsetid = thecursor.fetchone()[0]	

		# Add a new VPSet
		if(self.verbose > 2):
		    print "INSERT INTO VecParameterSets (superId, name, tracked) VALUES (" + str(newvparamsetid) + ", '" + vpset + "', " + vpsettracked + ")"
		thecursor.execute("INSERT INTO VecParameterSets (superId, name, tracked) VALUES (" + str(newvparamsetid) + ", '" + vpset + "', " + vpsettracked + ")")

		# Attach the PSet to a Fwk component via their superIds
		if(self.verbose > 2):
		    print "INSERT INTO SuperIdVecParamSetAssoc (superId, vpsetId, sequenceNb) VALUES (" + str(newsuperid) + ", " + str(newvparamsetid) + ", " + str(vpsetpsetseq) + ")"
		thecursor.execute("INSERT INTO SuperIdVecParamSetAssoc (superId, vpsetId, sequenceNb) VALUES (" + str(newsuperid) + ", " + str(newvparamsetid) + ", " + str(vpsetpsetseq) + ")")

	    lastvpsetname = vpset

	    # Now make new entries for each parameter in this VPSet if they exist
	    if(vpsettype == '' or vpsetname == ''):
		continue

	    if(vpsettype == "int" or vpsettype == "int32_t"):
		vpsettype = "int32"
	    if(vpsettype == "uint32_t" or vpsettype == "unsigned int" or vpsettype == "uint"):
		vpsettype = "uint32"
#	    if(vpsettype == "FileInPath"):
#		vpsettype = "string"
	    if(vpsettype == "vunsigned"):
		vpsettype = "vuint32"

	    type = self.paramtypedict[vpsettype]

	    vpsetseq = localseqcount
	    localseqcount = localseqcount + 1
	    lastpsetseqdict[vpset] = localseqcount

	    # Fill Parameters table
	    newvparammemberid = self.AddNewParam(thecursor,newvparamsetid,vpsetname,type,vpsettracked,vpsetseq)	    

	    if(vpsettype == "int32" or vpsettype == "int" or vpsettype == "int32_t"):
		# Protect against loading non-integer values. Also deal with implicit fp->int conversions and hex.
		if(vpsetval):
		    if(vpsetval.find('.') != -1):
			vpsetval = str(int(float(vpsetval)))
		    elif(not vpsetval.isdigit()):
			vpsetval = None

		if(vpsetval == None):
		    if(self.verbose > 2):
			print "No default parameter value found"
		else:
		    thecursor.execute("INSERT INTO Int32ParamValues (paramId, value) VALUES (" + str(newvparammemberid) + ", " + vpsetval + ")")
	    elif(vpsettype == "uint32" or vpsettype == "unsigned int" or vpsettype == "uint32_t" or vpsettype == "uint"):
		if(vpsetval):
		    if(str(vpsetval).endswith("U")):
			vpsetval = (str(vpsetval).rstrip("U"))

		    # Protect against loading non-integer values. Also deal with implicit fp->int conversions and hex.		
		    if(vpsetval.find('.') != -1):
			vpsetval = str(int(float(vpsetval)))
		    elif(not vpsetval.isdigit()):
			vpsetval = None

		if(vpsetval == None):
		    if(self.verbose > 2):
			print "No default parameter value found"
		else:
		    thecursor.execute("INSERT INTO UInt32ParamValues (paramId, value) VALUES (" + str(newvparammemberid) + ", " + vpsetval + ")")
	    elif(vpsettype == "bool"):
                boolval = str(vpsetval).strip('"').strip()
                if(boolval == "true"):
                    vpsetval = str(1)
                if(boolval == "false"):
                    vpsetval = str(0)
 
		thecursor.execute("INSERT INTO BoolParamValues (paramId, value) VALUES (" + str(newvparammemberid) + ", " + vpsetval + ")")
	    elif(vpsettype == "double"):
		if(vpsetval):
		    if(vpsetval.find('.') == -1 and (not paramval.isdigit())):
			vpsetval = None

		if(vpsetval == None):
		    if(self.verbose > 2):
			print "No default parameter value found"
		else:
		    thecursor.execute("INSERT INTO DoubleParamValues (paramId, value) VALUES (" + str(newvparammemberid) + ", " + vpsetval + ")")
	    elif(vpsettype == "string"):
		if(vpsetval.find("'") != -1):
		    thecursor.execute("INSERT INTO StringParamValues (paramId, value) VALUES (" + str(newvparammemberid) + ", " + vpsetval + ")")
		elif(vpsetval.find('"') != -1):
		    thecursor.execute("INSERT INTO StringParamValues (paramId, value) VALUES (" + str(newvparammemberid) + ", '" + vpsetval + "')")
		else:
		    print "\tWarning: Attempted to load a non-string value to string table:"
		    print "\t\tstring " + str(vpsetname) + " = " + str(vpsetval)
		    print "\t\tLoading parameter with no default value" 
	    elif(vpsettype == "FileInPath"):
		if(vpsetval.find("'") != -1):
		    thecursor.execute("INSERT INTO FileInPathParamValues (paramId, value) VALUES (" + str(newvparammemberid) + ", " + vpsetval + ")")
		elif(vpsetval.find('"') != -1):
		    thecursor.execute("INSERT INTO FileInPathParamValues (paramId, value) VALUES (" + str(newvparammemberid) + ", '" + vpsetval + "')")
		else:
		    print "\tWarning: Attempted to load a non-string value to FileInPath table:"
		    print "\t\tFileInPath " + str(vpsetname) + " = " + str(vpsetval)
		    print "\t\tLoading parameter with no default value" 
	    elif(vpsettype == "InputTag"):
		if(vpsetval.find("'") != -1):
		    thecursor.execute("INSERT INTO InputTagParamValues (paramId, value) VALUES (" + str(newvparammemberid) + ", " + vpsetval + ")")
		else:
		    thecursor.execute("INSERT INTO InputTagParamValues (paramId, value) VALUES (" + str(newvparammemberid) + ", '" + vpsetval + "')")

    # End ConfdbAttachParameterSets

    # Add package and subsystem information
    def ConfdbInsertPackageSubsystem(self,thecursor,thesubsystem,thepackage):
        if(self.verbose > 2):
            print "SELECT SoftwareSubsystems.subsysId FROM SoftwareSubsystems WHERE (SoftwareSubsystems.name = '" + thesubsystem + "')"

        # Insert the subsystem if it doesn't yet exist
        thecursor.execute("SELECT SoftwareSubsystems.subsysId FROM SoftwareSubsystems WHERE (SoftwareSubsystems.name = '" + thesubsystem + "')")
        subsys = thecursor.fetchone()
        if(subsys):
            subsys = subsys[0]

        if(subsys == None):
            if(self.verbose > 2):
                print "INSERT INTO SoftwareSubsystems (name) VALUES ('" + str(thesubsystem) + "')"

            thecursor.execute("INSERT INTO SoftwareSubsystems (name) VALUES ('" + str(thesubsystem) + "')")
#            thecursor.execute("SELECT LAST_INSERT_ID()")
#	    thecursor.execute("SELECT subsysId FROM SoftwareSubsystems ORDER BY subsysId DESC")
            thecursor.execute("SELECT SubsysId_Sequence.currval from dual")
            
            subsys = thecursor.fetchone()
            if(subsys):
                subsys = subsys[0]

        if(self.verbose > 2):
            print "SELECT SoftwarePackages.packageId FROM SoftwarePackages JOIN SoftwareSubsystems ON (SoftwarePackages.subsysId = " + str(subsys) + ") WHERE (SoftwarePackages.name = '" + str(thepackage) + "')"
        thecursor.execute("SELECT SoftwarePackages.packageId FROM SoftwarePackages JOIN SoftwareSubsystems ON (SoftwarePackages.subsysId = " + str(subsys) + ") WHERE (SoftwarePackages.name = '" + str(thepackage) + "')")

        pack = thecursor.fetchone()

        if(pack):
            pack = pack[0]

        if(pack == None):
            if(self.verbose > 2):
                print "INSERT INTO SoftwarePackages (name,subsysId) VALUES ('" + str(thepackage) + "', " + str(subsys) + ")"

            thecursor.execute("INSERT INTO SoftwarePackages (name,subsysId) VALUES ('" + str(thepackage) + "', " + str(subsys) + ")")
#	    thecursor.execute("SELECT packageId FROM SoftwarePackages ORDER BY packageId DESC")
            thecursor.execute("SELECT PackageId_Sequence.currval from dual")
            
	    pack = thecursor.fetchone()
	    if(pack):
	        pack = pack[0]

	return pack

    # Utility function for adding a new parameter 
    def AddNewParam(self,thecursor,sid,pname,ptype,ptracked,pseq):
        if(ptracked == 'tracked'):
            ptracked = str(1)
        elif(ptracked == 'untracked'):
            ptracked = str(0)

	if(self.verbose > 2):
	    print "INSERT INTO Parameters (paramTypeId, name, tracked) VALUES (" + str(ptype) + ", '" + pname + "', " + ptracked + ")"

	thecursor.execute("INSERT INTO Parameters (paramTypeId, name, tracked) VALUES (" + str(ptype) + ", '" + pname + "', " + ptracked + ")")
	
#	thecursor.execute("SELECT LAST_INSERT_ID()")

#	thecursor.execute("SELECT paramId FROM Parameters ORDER BY paramId DESC")
        thecursor.execute("SELECT ParamId_Sequence.currval from dual")
	newparamid = thecursor.fetchone()[0]

	# Fill Parameter <-> Super ID table
	if(self.verbose > 2):
	    print "INSERT INTO SuperIdParameterAssoc (superId, paramId, sequenceNb) VALUES (" + str(sid) + ", " + str(newparamid) + ", " + str(pseq) + ")"
	thecursor.execute("INSERT INTO SuperIdParameterAssoc (superId, paramId, sequenceNb) VALUES (" + str(sid) + ", " + str(newparamid) + ", " + str(pseq) + ")")

	return newparamid

    # Utility function for returning the paramId of a parameter
    def RetrieveParamId(self,thecursor,pname,sid):
	thecursor.execute("SELECT SuperIdParameterAssoc.paramId FROM SuperIdParameterAssoc JOIN Parameters ON (Parameters.name = '" + pname + "') WHERE (SuperIdParameterAssoc.superId = " + str(sid) + ") AND (SuperIdParameterAssoc.paramId = Parameters.paramId)")
	
	oldparamid = thecursor.fetchone()

	if(self.verbose > 2):
	    print "SELECT SuperIdParameterAssoc.paramId FROM SuperIdParameterAssoc JOIN Parameters ON (Parameters.name = '" + pname + "') WHERE (SuperIdParameterAssoc.superId = " + str(sid) + ") AND (SuperIdParameterAssoc.paramId = Parameters.paramId)"
	    if(oldparamid):	    
		print "Old param id was " + str(oldparamid[0])

	if(oldparamid):
	    return oldparamid[0]
	else:
	    return oldparamid

    # Set the verbosity
    def SetVerbosity(self, verbosity):
	self.verbose = verbosity

    def GetNextSequenceNb(self,thecursor,componentname,componenttablename):

	seqtuple = []

        if(self.verbose > 2):
            print "SELECT " + componenttablename + ".superId FROM " + componenttablename + " JOIN SuperIdReleaseAssoc ON (SuperIdReleaseAssoc.releaseId = " + str(self.releasekey) + " AND SuperIdReleaseAssoc.superId = " + componenttablename + ".superId) WHERE " + componenttablename + ".name = '" + componentname + "'";
        thecursor.execute("SELECT " + componenttablename + ".superId FROM " + componenttablename + " JOIN SuperIdReleaseAssoc ON (SuperIdReleaseAssoc.releaseId = " + str(self.releasekey) + " AND SuperIdReleaseAssoc.superId = " + componenttablename + ".superId) WHERE " + componenttablename + ".name = '" + componentname + "'")
	thesuperid = (thecursor.fetchall()[0])[0]

        if(self.verbose > 2):
            print "SELECT sequenceNb FROM SuperIdParameterAssoc WHERE SuperIdParameterAssoc.superId = " + str(thesuperid)
	thecursor.execute("SELECT sequenceNb FROM SuperIdParameterAssoc WHERE SuperIdParameterAssoc.superId = " + str(thesuperid))
	seqs = thecursor.fetchall()
	for seq in seqs:
	    seqtuple.append(seq[0])

        if(self.verbose > 2):
            print "SELECT sequenceNb FROM SuperIdParamSetAssoc WHERE SuperIdParamSetAssoc.superId = " + str(thesuperid)
	thecursor.execute("SELECT sequenceNb FROM SuperIdParamSetAssoc WHERE SuperIdParamSetAssoc.superId = " + str(thesuperid))
	seqs = thecursor.fetchall()
	for seq in seqs:
	    seqtuple.append(seq[0])

        if(self.verbose):
            print "SELECT sequenceNb FROM SuperIdVecParamSetAssoc WHERE SuperIdVecParamSetAssoc.superId = " + str(thesuperid)
	thecursor.execute("SELECT sequenceNb FROM SuperIdVecParamSetAssoc WHERE SuperIdVecParamSetAssoc.superId = " + str(thesuperid))
	seqs = thecursor.fetchall()
	for seq in seqs:
	    seqtuple.append(seq[0])

	seqtuple.sort()

	outoforder = False
	orderedseq = 0
	prevseq = 0
	thenextseq = 0
	for seq in seqtuple:
	    if seq != orderedseq:
		print "Out of order after seqnueceNb = " + str(prevseq)
		thenextseq = orderedseq
		outoforder = True
	    orderedseq = orderedseq + 1
	    prevseq = seq

	if(outoforder == True):
	    return thenextseq

        return (orderedseq)

	
    # Does this parameter exist (i.e. has it already been fixed)?
    def CheckParameterExistence(self,thecursor,thecurrentcomponent,thecomponentsuperid,thevarname,strvartype):
	
	if(strvartype == "PSet"):
#	    print "SELECT SuperIdParamSetAssoc.psetId FROM SuperIdParamSetAssoc JOIN ParameterSets ON (ParameterSets.name = '" + thevarname + "') WHERE (SuperIdParamSetAssoc.superId = " + str(thecomponentsuperid) + ") AND (SuperIdParamSetAssoc.psetId = ParameterSets.superId)"
	    thecursor.execute("SELECT SuperIdParamSetAssoc.psetId FROM SuperIdParamSetAssoc JOIN ParameterSets ON (ParameterSets.name = '" + thevarname + "') WHERE (SuperIdParamSetAssoc.superId = " + str(thecomponentsuperid) + ") AND (SuperIdParamSetAssoc.psetId = ParameterSets.superId)")
	    matchparams = thecursor.fetchall()
	    if(len(matchparams) > 0):
		print "The parameter " + thevarname + " already exists in the component " + thecurrentcomponent
		return 1
	    else:
		return 0
	    
	elif(strvartype == "VPSet"):
#	    print "SELECT SuperIdVecParamSetAssoc.vpsetId FROM SuperIdVecParamSetAssoc JOIN VecParameterSets ON (VecParameterSets.name = '" + thevarname + "') WHERE (SuperIdVecParamSetAssoc.superId = " + str(thecomponentsuperid) + ") AND (SuperIdVecParamSetAssoc.vpsetId = VecParameterSets.superId)"
	    thecursor.execute("SELECT SuperIdVecParamSetAssoc.vpsetId FROM SuperIdVecParamSetAssoc JOIN VecParameterSets ON (VecParameterSets.name = '" + thevarname + "') WHERE (SuperIdVecParamSetAssoc.superId = " + str(thecomponentsuperid) + ") AND (SuperIdVecParamSetAssoc.vpsetId = VecParameterSets.superId)")
	    matchparams = thecursor.fetchall()
	    if(len(matchparams) > 0):
		print "The parameter " + thevarname + " already exists in the component " + thecurrentcomponent
		return 1
	    else:
		return 0

	else:
	    thevartype = self.paramtypedict[strvartype]

#	    print "SELECT SuperIdParameterAssoc.paramId FROM SuperIdParameterAssoc JOIN Parameters ON (Parameters.name = '" + thevarname + "') WHERE (SuperIdParameterAssoc.superId = " + str(thecomponentsuperid) + ") AND (SuperIdParameterAssoc.paramId = Parameters.paramId) AND (Parameters.paramTypeId = " + str(thevartype) +")"
	    thecursor.execute("SELECT SuperIdParameterAssoc.paramId FROM SuperIdParameterAssoc JOIN Parameters ON (Parameters.name = '" + thevarname + "') WHERE (SuperIdParameterAssoc.superId = " + str(thecomponentsuperid) + ") AND (SuperIdParameterAssoc.paramId = Parameters.paramId) AND (Parameters.paramTypeId = " + str(thevartype) + ")")
	    matchparams = thecursor.fetchall()
	    if(len(matchparams) > 0):
 		print "The parameter " + thevarname + " already exists in the component " + thecurrentcomponent
		return 1
	    else:
		return 0

    # Connect to the Confdb db
    def CheckForDuplicates(self,thecursor,therelease):

        print "SELECT ModuleTemplates.name, ModuleTemplates.superId, ModuleTemplates.typeId, ModuleTemplates.cvstag, SoftwarePackages.name, SoftwareSubsystems.name FROM ModuleTemplates JOIN SuperIdReleaseAssoc ON (ModuleTemplates.superId = SuperIdReleaseAssoc.superId) JOIN SoftwareReleases ON (SuperIdReleaseAssoc.releaseId = SoftwareReleases.releaseId) JOIN SoftwarePackages ON (SoftwarePackages.packageId = ModuleTemplates.packageId) JOIN SoftwareSubsystems ON (SoftwarePackages.subsysId = SoftwareSubsystems.subsysId) WHERE SoftwareReleases.releaseTag = '" + str(therelease) + "'"

        thecursor.execute("SELECT ModuleTemplates.name, ModuleTemplates.superId, ModuleTemplates.typeId, ModuleTemplates.cvstag, SoftwarePackages.name, SoftwareSubsystems.name FROM ModuleTemplates JOIN SuperIdReleaseAssoc ON (ModuleTemplates.superId = SuperIdReleaseAssoc.superId) JOIN SoftwareReleases ON (SuperIdReleaseAssoc.releaseId = SoftwareReleases.releaseId) JOIN SoftwarePackages ON (SoftwarePackages.packageId = ModuleTemplates.packageId) JOIN SoftwareSubsystems ON (SoftwarePackages.subsysId = SoftwareSubsystems.subsysId) WHERE SoftwareReleases.releaseTag = '" + str(therelease) + "'")

        themodules = thecursor.fetchall()
        for themodule in themodules:
            print "\t" + str(themodule[0]) + " " + str(themodule[4]) + " " + str(themodule[5]) + " " + str(themodule[1]) + " " + str(themodule[2]) + " " + str(themodule[3])
            if(str(themodule[0]) in self.allmodules):
                print '****FAIL!!! two modules named ' + str(themodule[0]) + '****'
            self.allmodules.append(str(themodule[0]))

if __name__ == "__main__":
    main(sys.argv[1:])
