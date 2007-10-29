10.10.2007	CMSSW_1_6_5	jet calibration data needed:
				cvs co -r jet_corrections_16X JetMETCorrections/MCJet/data

29.10.2007      CMSSW_1_6_7	Old SimpleTree version tagged with name "SimpleTree". 
				Contains the analysis, event filter, and storing 
				data using a simple root tree.
29.10.2007	CMSSW_1_6_7	Analysis changed to pTDR ORCA style using MyEvent 
				class for storing information in the root files. 
				In cvs head.

	How to compile:
	- First make event dictionaries for the MyEvent class
		cd HiggsAnalysis/HeavyChHiggsToTauNu/src
		chmod +x dictionary.csh
		dictionary.csh
	-compile
		cd HiggsAnalysis/HeavyChHiggsToTauNu
		scramv1 b

	-run
	Take jet calibration data for 1_5_X samples:
	cvs co -r jet_corrections_16X JetMETCorrections/MCJet/data

	Example for reading and analyzing the produced root files can be found in:
	/afs/cern.ch/user/s/slehti/public/CMSSW/analysis_src

