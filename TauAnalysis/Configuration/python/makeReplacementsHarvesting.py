
#--------------------------------------------------------------------------------
# Compose replacements string used to create python configuration files
# for execution of 'cmsRun harvestZto.._cfg.py' jobs on the CERN batch system
#
# NOTE: The replacements string returned by the makeReplacementsHarvesting function
#       represents a list of replace statements in the format
#         'paramName1=paramValue1, paramName2=paramValue2,...'
#       (the replacements string is parsed by TauAnalysis/Configuration/python/prepareConfigFile.py)
#
#       The paramName strings need to match the "hooks" defined in the original config file (templates)
#       (e.g. "harvestZtoElecMu_cfg.py", "harvestZtoElecTau_cfg.py", "harvestZtoMuTau_cfg.py",...)
#
#       The function needs to be passed all of the following three arguments
#      (1) channel
#          name of channel to be analyzed
#          (e.g. "ZtoElecMu", "ZtoElecTau", "ZtoMuTau",...)
#      (2) sample
#          name of the signal/background Monte Carlo sample to be analyzed;
#
#          NOTE: sample needs to match one of the names defined in
#                TauAnalysis/Configuration/python/plotZtoElecMu_processes_cfi.py
#                TauAnalysis/Configuration/python/plotZtoElecTau_processes_cfi.py
#                TauAnalysis/Configuration/python/plotZtoMuTau_processes_cfi.py
#                ...
#          (e.g. ZtautauPlusJets)
#      (3) replacements
#          list of replace statements
#
#       The replacements string given as function argument will be extended by statements
#       specific to the harvestZto.._cfg.py config file (templates),
#       depending on the channel and sample parameters.
#
# Author: Christian Veelken, UC Davis
#
#--------------------------------------------------------------------------------

def makeReplacementsHarvesting(channel = None, sample = None, replacements = None):

    # check that channel, sample and replacements parameters are defined and non-empty
    if channel is None:
        raise ValueError("Undefined channel Parameter !!")
    if sample is None:
        raise ValueError("Undefined sample Parameter !!")

    replaceStatements_retVal = []

    # replace process parameter
    process = sample
    replaceStatements_retVal.append("process = " + process)

    # replace inputFileNames parameter
    inputFileNames = "process" + channel + "_" + sample + ".config_dqmFileLoader"
    replaceStatements_retVal.append("inputFileNames = " + inputFileNames)

    # replace outputFileName parameter
    outputFileName = "'plots" + channel + "_" + sample + "Sum.root'"
    replaceStatements_retVal.append("outputFileName = " + outputFileName)

    replacements_retVal = "; ".join(replaceStatements_retVal)

    return replacements_retVal
