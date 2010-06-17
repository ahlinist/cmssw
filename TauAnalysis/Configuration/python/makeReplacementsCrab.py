
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

def makeReplacementsCrab(channel = None, sample = None, type = None, replacements = None):

	# check that channel, sample and replacements parameters are defined and non-empty
	if channel is None:
		raise ValueError("Undefined channel Parameter !!")
	if sample is None:
		raise ValueError("Undefined sample Parameter !!")
	if (type != "mc" and type != "data") :
		raise ValueError("Undefined type Parameter !!")
	if replacements is None:
		raise ValueError("Undefined replacements Parameter !!")

	# remove all white-space characters from replacements parameter string
	replacements = replacements.replace(" ", "")

	# split replacements string into list of individual replace statements
	# (separated by ";" character)
	replaceStatements = replacements.split(";")

	replaceStatements_retVal = []

	factorization = None
	systematics = None
	disableEventDump = None
	inputFileType = None

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
		if paramName == "globalTag":
			replaceStatements_retVal.append(replaceStatement)
		if paramName == "eventsPerJob":
			replaceStatements_retVal.append(replaceStatement)
		if paramName == "inputFileType":
			inputFileType = paramValue								
		if paramName == "applyFactorization":
			factorization = None
			if paramValue.lower() == "false":
				factorization = ""
			elif paramValue.lower() == "true":
				factorization = "enableFactorization_run" + channel + "(process)"
			else:
				raise ValueError("Invalid factorization option = " + paramValue + " !!")
		if paramName == "estimateSysUncertainties":
	 		if paramValue.lower() == "false":
		 		systematics = "disableSysUncertainties_run" + channel + "(process)"
	 		elif paramValue.lower() == "true":
		 		systematics = "enableSysUncertainties_run" + channel + "(process)"
	 		else:
		 		raise ValueError("Invalid systematics option = " + paramValue + " !!")
	 	if paramName == "disableEventDump":
		 	disableEventDump = paramValue

	# check that factorization option has been defined
	if factorization is None:
		raise ValueError("Undefined factorization option !!")
	replaceStatements_retVal.append("factorization = " + factorization)

	# check that systematics option has been defined
	if systematics is None:
		raise ValueError("Undefined systematics option !!")
	replaceStatements_retVal.append("systematics = " + systematics)

	# check if event-dump output is to be disabled
	# (keep event-dump output enabled per default,
	#  in case disableEventDump option has not been explicitely specified)
	if disableEventDump is not None and disableEventDump.lower() == "true":
		replaceStatements_retVal.append("disableEventDump = setattr(process, 'disableEventDump', cms.PSet())")

	# replace inputFileName parameter by itself
	# (effectively disabling replacement of inputFileName parameter,
	#  as it is already being replaced by crab)
	replaceStatements_retVal.append("inputFileNames = process.source.fileNames")

	# when running over RECO samples, produce PAT-tuple as well
	if inputFileType == "RECO/AOD":
		patTupleProduction = "process.p.replace(process.producePatTuple" + channel + "Specific, process.producePatTupleAll)"
		replaceStatements_retVal.append("patTupleProduction = " + patTupleProduction)

	# replace genPhaseSpaceCut, patTupleOutputFileName and plotsOutputFileName parameters
	# (ommit "_part.." suffix of sample name in case of processes split
	#  into multiple cmsRun job parts, in order to avoid having to specify
	#   genPhaseSpaceCut, plotsOutputFileName and patTupleOutputFileName
	#  again and again for each part)
	genPhaseSpaceCut = "genPhaseSpaceCut" + channel + "_" + sample
	patTupleOutputFileName = "patTupleOutputFileName" + channel + "_" + sample
	plotsOutputFileName = "plotsOutputFileName" + channel + "_" + sample
	if sample.find("_part") != -1:
		genPhaseSpaceCut = genPhaseSpaceCut[:genPhaseSpaceCut.rfind("_part")]
		patTupleOutputFileName = "cms.untracked.string(" + patTupleOutputFileName[:patTupleOutputFileName.rfind("_part")]
		patTupleOutputFileName += ".value().replace('_partXX', '')"
		plotsOutputFileName = "cms.string(" + plotsOutputFileName[:plotsOutputFileName.rfind("_part")]
		plotsOutputFileName += ".value().replace(\'_partXX', '')"
	else:
		plotsOutputFileName += ".value().replace(\'_partXX', '')"
		patTupleOutputFileName += ".value().replace('_partXX', '')"
	replaceStatements_retVal.append("genPhaseSpaceCut = " + genPhaseSpaceCut)
	replaceStatements_retVal.append("patTupleOutputFileName = " + patTupleOutputFileName)
	replaceStatements_retVal.append("plotsOutputFileName = " + plotsOutputFileName)
    
	# disable automatic systematics calculation and PAT-tuple production
	replaceStatements_retVal.append("isBatchMode = setattr(process, 'isBatchMode', cms.PSet())")

	# if running data, run function to remove MC-dependent modules
	if type is "data":
		replaceStatements_retVal.append("#switchToData(process) = switchToData(process)")
	replacements_retVal = "; ".join(replaceStatements_retVal)

	return replacements_retVal
