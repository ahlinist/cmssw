
#--------------------------------------------------------------------------------
# Compose replacements string used to create python configuration files
# for execution of 'cmsRun skimZto.._cfg.py' jobs on the CERN batch system
#
# NOTE: The replacements string returned by the makeReplacementsAnalysis function
#       represents a list of replace statements in the format
#         'paramName1=paramValue1, paramName2=paramValue2,...'
#       (the replacements string is parsed by TauAnalysis/Configuration/python/prepareConfigFile.py)
#
#       The paramName strings need to match the "hooks" defined in the original config file (templates)
#       (e.g. "skimZtoElecMu_cfg.py", "skimZtoElecTau_cfg.py", "skimZtoMuTau_cfg.py",...)
#
#       The function needs to be passed all of the following three arguments
#      (1) channel
#          name of channel to be analyzed
#          (e.g. "ZtoElecMu", "ZtoElecTau", "ZtoMuTau",...)
#      (2) sample
#          name of the signal/background Monte Carlo sample to be analyzed;
#
#          NOTE: sample needs to match one of the names defined in
#                TauAnalysis/Configuration/python/recoSampleDefinitionsZtoElecMu_cfi.py
#                TauAnalysis/Configuration/python/recoSampleDefinitionsZtoElecTau_cfi.py
#                TauAnalysis/Configuration/python/recoSampleDefinitionsZtoMuTau_cfi.py
#                ...
#          (e.g. ZtautauPlusJets_part01)
#      (3) replacements
#          list of replace statements
#
#       The replacements string given as function argument will be extended by statements
#       specific to the skimZto.._cfg.py config file (templates),
#       depending on the channel and sample parameters.
#
# Author: Christian Veelken, UC Davis
#
#--------------------------------------------------------------------------------

def makeReplacementsBgEstSkim(channel = None, sample = None, replacements = None):

    # check that channel, sample and replacements parameters are defined and non-empty
    if channel is None:
        raise ValueError("Undefined channel Parameter !!")
    if sample is None:
        raise ValueError("Undefined sample Parameter !!")
    if replacements is None:
        raise ValueError("Undefined replacements Parameter !!")

    # remove all white-space characters from replacements parameter string
    replacements = replacements.replace(" ", "")

    # split replacements string into list of individual replace statements
    # (separated by ";" character)
    replaceStatements = replacements.split(";")

    replaceStatements_retVal = []
    
    for replaceStatement in replaceStatements:

        # split replacement string into name, value pairs
        paramNameValuePair = replaceStatement.split("=")

        # check that replacement string matches 'paramName=paramValue' format
        if len(paramNameValuePair) != 2:
            raise ValueError("Invalid format of replace Statement: " + replaceStatement + " !!")

        # extract name and value to be used for replacement
        paramName = paramNameValuePair[0]
        paramValue = paramNameValuePair[1]

        if paramName == "maxEvents":
            replaceStatements_retVal.append(replaceStatement)

    # replace inputFileName parameter
    inputFileNames = "fileNames" + channel +"_" + sample
    replaceStatements_retVal.append("inputFileNames = " + inputFileNames)
    
    # replace genPhaseSpaceCut and bgEstSampleOutputFileName parameters
    # (ommit "_part.." suffix of sample name in case of processes split
    #  into multiple cmsRun job parts, in order to avoid having to specify
    #   genPhaseSpaceCut and bgEstSampleOutputFileName
    #  again and again for each part)
    genPhaseSpaceCut = "genPhaseSpaceCut" + channel +"_" + sample
    bgEstSampleOutputFileName = "bgEstSampleOutputFileName" + channel +"_" + sample
    if sample.find("_part") != -1:
        genPhaseSpaceCut = genPhaseSpaceCut[:genPhaseSpaceCut.rfind("_part")]
        bgEstSampleOutputFileName = "cms.untracked.string(" + bgEstSampleOutputFileName[:bgEstSampleOutputFileName.rfind("_part")]
        bgEstSampleOutputFileName += ".value().replace(\'_partXX', '" + sample[sample.rfind("_part"):] + "'))"
    replaceStatements_retVal.append("genPhaseSpaceCut = " + genPhaseSpaceCut)
    replaceStatements_retVal.append("bgEstSampleOutputFileName = " + bgEstSampleOutputFileName)

    replacements_retVal = "; ".join(replaceStatements_retVal)

    return replacements_retVal
