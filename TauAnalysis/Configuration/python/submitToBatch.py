import os
import subprocess

from TauAnalysis.Configuration.prepareConfigFile import prepareConfigFile

#--------------------------------------------------------------------------------
# Create python configuration file for execution of cmsRun and
# shell script for submission of cmsRun job to the CERN batch system,
# then submit the job
#
# NOTE: Function needs to be passed the first eight of the following ten arguments
#       (arguments nine and ten are optional parameters)
#
#      (1) configFile
#          name of the original config file (template)
#          to be modified first and then passed to cmsRun
#      (2) channel
#          name of channel to be analyzed
#          (e.g. "ZtoElecMu", "ZtoElecTau", "ZtoMuTau",...)
#      (3) sample
#          name of the signal/background Monte Carlo sample to be analyzed;
#
#          NOTE: sample needs to match one of the names defined in
#                TauAnalysis/Configuration/python/recoSampleDefinitionsZtoElecMu_cfi.py
#                TauAnalysis/Configuration/python/recoSampleDefinitionsZtoElecTau_cfi.py
#                TauAnalysis/Configuration/python/recoSampleDefinitionsZtoMuTau_cfi.py
#                ...
#          (e.g. ZtautauPlusJets_part1)
#      (4) replFunction
#          (python) function to be called for processing of replacements parameter string,
#          depending of type of cmsRun job
#          (e.g. TauAnalysis/Configuration/python/makeReplacementsAnalysis)
#      (5) replacements
#          list of replace statements to be applied to the original config file,
#          in the format 'paramName1=paramValue1; paramName2=paramValue2;...';
#          in the original config file, each occurence of any paramName
#          will be replaced by the associated paramValue
#      (6) job
#          name of the job submitted to the batch system
#          (allows to distinguish in the 'bjobs -w' output
#           different types of jobs running simultaneously)
#      (7) queue
#          name of the batch queue to which is cmsRun job is to be submitted
#          (e.g. '1nh' (1 hour), '1nd' (24 hours), '1nw' (1 week execution time limit),...)
#      (8) outputFilePath
#          name of the directory (either on afs area or castor)
#          to which all .root files produced by the cmsRun job will be copied
#          (e.g. "/castor/cern.ch/user/v/veelken/")
#      (9) resourceRequest
#          resource request to be passed to the -R option of 'busb', 
#          as defined by the CERN LSF software
#     (10) submit
#          if "yes", jobs will be submitted to the batch system, otherwise config files
#          and scripts are generated but no submission
#
# Author: Christian Veelken, UC Davis
#
#--------------------------------------------------------------------------------

def submitToBatch(configFile = None, channel = None, sample = None,
                  replFunction = None, replacements = "",
                  job = "job", queue = "1nd", outputFilePath = None,
                  resourceRequest = None, submit = "yes"):
    # check that configFile, channel, sample and outputFilePath
    # parameters are defined and non-empty
    if configFile is None:
        raise ValueError("Undefined configFile Parameter !!")
    if channel is None:
        raise ValueError("Undefined channel Parameter !!")
    if sample is None:
        raise ValueError("Undefined sample Parameter !!")
    if outputFilePath is None:
        raise ValueError("Undefined outputFilePath Parameter !!")

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
    submissionDirectory = workingDirectory + "lxbatch/"

    # compose name of modified config file including the replacements
    configFile_orig = configFile
    configFile_base = None
    if configFile.find("/") != -1:
        configFile_base = submissionDirectory + configFile[configFile.rfind("/") + 1::]
    else:    
        configFile_base = submissionDirectory + configFile
    configFile_mod = configFile_base.replace("_cfg.py", "_" + sample + "@Batch_cfg.py")

    if replFunction is not None:
        replacements = replFunction(channel = channel, sample = sample, replacements = replacements)

    # delete previous version of modified config file if it exists
    if os.path.exists(configFile_mod):
        os.remove(configFile_mod)

    # create new version of modified config file
    prepareConfigFile(configFile_orig = configFile_orig, replacements = replacements, configFile_mod = configFile_mod)

    # if it exists, delete previous version of shell script
    # for submission of cmsRun job to the CERN batch system 
    scriptFile = configFile_base.replace("_cfg.py", "_" + sample + "@Batch.csh")
    if os.path.exists(scriptFile):
        os.remove(scriptFile)

    # create shell script for submission of cmsRun job to the CERN batch system
    # (copy all .root files produced by the cmsRun job to directory specified
    #  by outputFilePath parameter given as function argument;
    #  use 'rfcp' for copying to castor and 'scp' for copying to afs area)
    cp = None
    if outputFilePath.find("/castor") != -1:
        cp = 'rfcp'
    else:
        cp = 'scp'
        
    script = """#!/bin/csh
limit vmem unlim
cd %(subDir)s
eval `scramv1 runtime -csh`
cd -
cmsRun %(config)s
set rootFiles=(`/bin/ls *.root`)
foreach rootFile (${rootFiles})
    echo "copying ${rootFile} to %(outDir)s"
    %(cp)s ${rootFile} %(outDir)s
end
""" % {'subDir': submissionDirectory, 'config': configFile_mod, 'outDir':outputFilePath, 'cp':cp}

    scf = open(scriptFile,"w")
    scf.write(script)
    scf.close()    

    # make shell script executable
    os.chmod(scriptFile,0744)
    
    # finally, submit job to the CERN batch system
    if submit == "yes":
        logFile = configFile_base.replace("_cfg.py", "_" + sample + "@Batch.out")
        jobName = job + channel + "_" + sample
        bsubCommand = 'bsub -q ' + queue + ' -J ' + jobName + ' -L /bin/csh -eo ' + logFile + ' -oo ' + logFile
        if resourceRequest != None:
            bsubCommand += ' -R \"' + resourceRequest + '\" '
        bsubCommand += ' < ' + scriptFile
        subprocess.call(bsubCommand, shell = True)
