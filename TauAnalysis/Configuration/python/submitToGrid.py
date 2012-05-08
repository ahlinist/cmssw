import os
import subprocess

from TauAnalysis.Configuration.prepareConfigFile import prepareConfigFile

#--------------------------------------------------------------------------------
# Create python configuration file for execution of cmsRun and
# crab configuration file for submission of cmsRun job to the Grid,
# then submit the job to the Grid via crab
#
# NOTE: Function needs to be passed the first nine of the following eleven arguments
#       (the tenth argument is an optional parameter)
#
#      (1) configFile
#          name of the original config file (template)
#          to be modified first and then passed to cmsRun
#      (2) channel
#          name of channel to be analyzed
#          (e.g. "ZtoElecMu", "ZtoElecTau", "ZtoMuTau",...)
#      (3) sample
#          name of the signal/background Monte Carlo sample to be analyzed,
#          in "user-friendly format"
#          (e.g. Ztautau)
#      (4) job
#          name of the job submitted to the batch system
#          (allows to distinguish different jobs of same type running simultaneously)
#      (5) dbs_name
#          name of the signal/background Monte Carlo sample to be analyzed,
#          as registered in DBS
#          (e.g. /Ztautau/lusito-CMSSW_2_2_3_SkimMuTauZTT02_GEN_SIM_DIGI_L1_DIGI2RAW_HLT-5c432a197f799755d712fe824b01cf85/USER)
#      (6) dbs_url
#          url of the DBS server in which the sample has been registered
#          (e.g. https://cmsdbsprod.cern.ch:8443/cms_dbs_prod_local_09_writer/servlet/DBSServlet)
#      (7) replFunction
#          (python) function to be called for processing of replacements parameter string,
#          depending of type of cmsRun job
#          (e.g. TauAnalysis/Configuration/python/makeReplacementsAnalysis)
#      (8) replacements
#          list of replace statements to be applied to the original config file,
#          in the format 'paramName1=paramValue1; paramName2=paramValue2;...';
#          in the original config file, each occurence of any paramName
#          will be replaced by the associated paramValue
#      (9) type
#			type of sample; must be "data" or "mc"
#     (10) outputFileNames
#          names of all .root files produced by cmsRun job
#
#          NOTE: .root files missing in this list will not be copied to castor !!
#
#     (11) outputFilePath
#          name of the directory (either on afs area or castor)
#          to which all .root files produced by the cmsRun job will be copied
#          (e.g. "/castor/cern.ch/user/v/veelken/")
#     (12) submit
#          if "yes", jobs will be submitted to the batch system, otherwise config files
#          and scripts are generated but no submission
#
# Author: Christian Veelken, UC Davis
#
#--------------------------------------------------------------------------------

def submitToGrid(configFile = None, channel = None,
                 sample = None, job = None, dbs_name = None, dbs_url = None, 
                 replFunction = None, replacements = "", type = None,
                 outputFileNames = None, outputFilePath = None,
                 submit = "yes"):
    # check that configFile, channel, sample and outputFilePath
    # parameters are defined and non-empty
    if configFile is None:
        raise ValueError("Undefined configFile Parameter !!")
    if channel is None:
        raise ValueError("Undefined channel Parameter !!")
    if sample is None:
        raise ValueError("Undefined sample Parameter !!")
    if job is None:
        raise ValueError("Undefined job Parameter !!")
    if dbs_name is None:
        raise ValueError("Undefined dbs_name Parameter !!")
    if dbs_url is None:
        raise ValueError("Undefined dbs_url Parameter !!")
    if outputFileNames is None:
        raise ValueError("Undefined outputFileNames Parameter !!")
    if outputFilePath is None:
        raise ValueError("Undefined outputFilePath Parameter !!")
    if type is None:
        raise ValueError("Undefined type Parameter !!")

    # in case outputFilePath parameter not terminated by "/",
    # add terminating "/" character to outputFilePath string
    if not outputFilePath.endswith("/"):
        outputFilePath += "/"

    # get name of directory in which config files will be created;
    # add terminating "/" character to submissionDirectory string also,
    # if necessary
    workingDirectory = os.getcwd()
    if not workingDirectory.endswith("/"):
        workingDirectory += "/"
    submissionDirectory = workingDirectory + "crab/"

    # compose name of modified config file including the replacements
    cmsswConfigFile_orig = workingDirectory + configFile
    cmsswConfigFile_mod = submissionDirectory + configFile.replace("_cfg.py", "_" + sample + "@Grid_cfg.py")
    cmsswConfigFile_mod = cmsswConfigFile_mod.replace("%s_%s" % (sample, sample), "%s" % sample)

    if replFunction is not None:
        replacements = replFunction(channel = channel, sample = sample, type = type, replacements = replacements)

    # delete previous version of modified config file if it exists
    if os.path.exists(cmsswConfigFile_mod):
        os.remove(cmsswConfigFile_mod)

    # create new version of modified config file
    prepareConfigFile(configFile_orig = cmsswConfigFile_orig, replacements = replacements, configFile_mod = cmsswConfigFile_mod)

    # if it exists, delete previous version of crab config file
    # for submission of cmsRun job to the Grid
    crabConfigFile_orig = workingDirectory + "crab_template.cfg"
    crabConfigFile_mod = submissionDirectory + "crab" + "_" + configFile.replace("_cfg.py", "_" + sample + "_" + job + ".cfg")
    crabConfigFile_mod = crabConfigFile_mod.replace("%s_%s" % (sample, sample), "%s" % sample)

    # delete previous version of crab config file if it exists
    if os.path.exists(crabConfigFile_mod):
        os.remove(crabConfigFile_mod)

    # create new version of crab config file
    maxEvents = "-1"
    eventsPerJob = "20000"
    replacements = replacements.replace(" ", "")
    replaceStatements = replacements.split(";")
    for replaceStatement in replaceStatements:
        paramNameValuePair = replaceStatement.split("=")
        if len(paramNameValuePair) != 2:
            raise ValueError("Invalid format of replace Statement: " + replaceStatement + " !!")
        if paramNameValuePair[0] == "maxEvents":
            maxEvents = paramNameValuePair[1]
        if paramNameValuePair[0] == "eventsPerJob":
            eventsPerJob = paramNameValuePair[1]
    crabDirectory = submissionDirectory + configFile.replace("_cfg.py", "_" + sample + "_" + job + "/")
    crabDirectory = crabDirectory.replace("%s_%s" % (sample, sample), "%s" % sample)
    crabOutputFiles = ""
    for outputFile in outputFileNames:
        if crabOutputFiles != "":
            crabOutputFiles += ", "
        crabOutputFiles += outputFile.value().replace("_partXX", "")
    replacements += "; datasetpath = " + dbs_name
    replacements += "; dbs_url = " + dbs_url
    replacements += "; pset = " + cmsswConfigFile_mod
    replacements += "; total_number_of_events = " + maxEvents
    replacements += "; events_per_job = " + eventsPerJob
    replacements += "; output_file = " + crabOutputFiles
    replacements += "; ui_working_dir = " + crabDirectory
	#replacements += "; logdir = " + crabDirectory + "res/"
    replacements += "; user_remote_dir = " + outputFilePath
    prepareConfigFile(configFile_orig = crabConfigFile_orig, replacements = replacements, configFile_mod = crabConfigFile_mod)
    
    # finally, submit job to the Grid
    if submit == "yes":
        crabCreateCommand = "crab -create -cfg " + crabConfigFile_mod
        subprocess.call(crabCreateCommand, shell = True)
        crabSubmitCommand = "crab -submit -c "+ crabDirectory
        subprocess.call(crabSubmitCommand, shell = True)
        crabStatusCommand = "crab -status -c "+ crabDirectory
        subprocess.call(crabStatusCommand, shell = True)
