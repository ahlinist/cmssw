
#--------------------------------------------------------------------------------
# Compose replacements string used to create python configuration files
# for execution of 'cmsRun runZto.._cfg.py' jobs on the CERN batch system
#
# NOTE: The replacements string returned by the makeReplacementsAnalysis function
#       represents a list of replace statements in the format
#         'paramName1=paramValue1, paramName2=paramValue2,...'
#       (the replacements string is parsed by TauAnalysis/Configuration/python/prepareConfigFile.py)
#
#       The paramName strings need to match the "hooks" defined in the original config file (templates)
#       (e.g. "runZtoElecMu_cfg.py", "runZtoElecTau_cfg.py", "runZtoMuTau_cfg.py",...)
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
#       specific to the runZto.._cfg.py config file (templates),
#       depending on the channel and sample parameters.
#
# Author: Christian Veelken, UC Davis
#
#--------------------------------------------------------------------------------

def makeReplacementsAnalysis(channel = None, sample = None, replacements = None):

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

    factorization = None
    
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
        if paramName == "applyFactorization":
            factorization = None
            if paramValue.lower() == "false":
                factorization = ""
            elif paramValue.lower() == "true":
                factorization = "enableFactorization_run" + channel + "(process)"
            else:
                raise ValueError("Invalid factorization option = " + paramValue + " !!")

    # check that factorization option has been defined
    if factorization is None:
        raise ValueError("Undefined factorization option !!")
    replaceStatements_retVal.append("factorization = " + factorization)

    # replace inputFileName parameter by itself
    # (effectively disabling replacement of inputFileName parameter,
    #  as it is already being replaced by crab)
    replaceStatements_retVal.append("inputFileNames = process.source.fileNames")
    
    # replace genPhaseSpaceCut, plotsOutputFileName and patTupleOutputFileName parameters
    # (ommit "_part.." suffix of sample name in case of processes split
    #  into multiple cmsRun job parts, in order to avoid having to specify
    #   genPhaseSpaceCut, plotsOutputFileName and patTupleOutputFileName
    #  again and again for each part)
    genPhaseSpaceCut = "genPhaseSpaceCut" + sample
    plotsOutputFileName = "plotsOutputFileName" + sample
    patTupleOutputFileName = "patTupleOutputFileName" + sample
    if sample.find("_part") != -1:
        genPhaseSpaceCut = genPhaseSpaceCut[:genPhaseSpaceCut.rfind("_part")]
        plotsOutputFileName = "cms.string(" + plotsOutputFileName[:plotsOutputFileName.rfind("_part")]
        plotsOutputFileName += ".value().replace(\'_partXX', '" + sample[sample.rfind("_part"):] + "'))"
        patTupleOutputFileName = "cms.untracked.string(" + patTupleOutputFileName[:patTupleOutputFileName.rfind("_part")]
        patTupleOutputFileName += ".value().replace('_partXX', '" + sample[sample.rfind("_part"):] + "'))"
    replaceStatements_retVal.append("genPhaseSpaceCut = " + genPhaseSpaceCut)
    replaceStatements_retVal.append("plotsOutputFileName = " + plotsOutputFileName)
    replaceStatements_retVal.append("patTupleOutputFileName = " + patTupleOutputFileName)

    replacements_retVal = "; ".join(replaceStatements_retVal)

    return replacements_retVal
