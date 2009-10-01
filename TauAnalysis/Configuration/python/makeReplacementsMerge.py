
#--------------------------------------------------------------------------------
# Compose replacements string used to create python configuration files
# for execution of 'cmsRun mergeZto.._cfg.py' jobs on the CERN batch system
#
# NOTE: The replacements string returned by the makeReplacementsMerge function
#       represents a list of replace statements in the format
#         'paramName1=paramValue1, paramName2=paramValue2,...'
#       (the replacements string is parsed by TauAnalysis/Configuration/python/prepareConfigFile.py)
#
#       The paramName strings need to match the "hooks" defined in the original config file (templates)
#       (e.g. "mergeZtoElecTau_cfg.py")
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
#       specific to the mergeZto.._cfg.py config file (templates),
#       depending on the channel and sample parameters.
#
# Authors: Christian Veelken, UC Davis
#			Jeff Kolb, Notre Dame
#
#--------------------------------------------------------------------------------

def makeReplacementsMerge(channel = None, sample = None, replacements = None):

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
	minFileNum = -1
	maxFileNum = -1
	part = -1
	inputFileNameBase = None

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

		if paramName == "inputFileNameBase":
			inputFileNameBase = paramValue
		if paramName == "minFileNum":
			minFileNum = int(paramValue)
		if paramName == "maxFileNum":
			maxFileNum = int(paramValue)
		if paramName == "part":
			part = int(paramValue)

# replace inputFileNames parameter
	inputFileNames = "cms.untracked.vstring(\n\t'"
	max = maxFileNum + 1
	for i in range(minFileNum,max) :
		inputFileNames += "rfio:" + inputFileNameBase + str(i) + ".root'"
		if i < maxFileNum :
			inputFileNames += ",\n\t'"
		else :
			inputFileNames += "\n"
	inputFileNames += ")"

	replaceStatements_retVal.append("inputFileNames = " + inputFileNames)

# replace outputFileName parameter
	outputFileName = "cms.untracked.string('skimElecTau_" + sample + ".root')"
	replaceStatements_retVal.append("outputFileName = " + outputFileName)


	replacements_retVal = "; ".join(replaceStatements_retVal)

	return replacements_retVal
