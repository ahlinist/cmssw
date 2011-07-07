import FWCore.ParameterSet.Config as cms

from TauAnalysis.Configuration.recoSampleDefinitionsAHtoMuTau_7TeV_grid_cfi import *

#--------------------------------------------------------------------------------
# utility function for configuring DQMExportAnalysisResults module
#
# Author: Christian Veelken, UC Davis
#
#--------------------------------------------------------------------------------

def configureDQMExportAnalysisResults(channels, channelsToExport, processes, processesToExport, higgsMassPoints, outputFilePath):
    psetsChannels = []
    for channelName in channels.keys():
        if channelName in channelsToExport:
            pset = cms.PSet(
                name = cms.string(channelName),
                shortName = cms.string(channels[channelName]['shortName']),
                binning = cms.string(
                    (channels[channelName]['dqmDirectoryTemplate']
                   + channels[channelName]['meNameTemplate']) % (channels[channelName]['processes']['Ztautau'])
                ),
                dataIntLumi = cms.double(channels[channelName]['dataIntLumi'])
            )
            psetsChannels.append(pset)

    psetProcesses = cms.PSet()
    for processName in processesToExport:
        psetProcess = cms.PSet(
            distributions = cms.PSet(),
            outputFilePath = cms.string(""),
            outputFileName = cms.string(processes[processName]['outputFileName'])
        )
        if processes[processName].get('xSection') is not None:
            setattr(psetProcess, "xSection", cms.double(processes[processName]['xSection']))
        for channelName in channels.keys():
            if channelName in channelsToExport:
                if channels[channelName]['processes'].get(processName) is not None:
                    psetChannel = cms.PSet(
	                normalization = cms.PSet(
	                    numEventsProcessed = cms.string(
                                (channels[channelName]['dqmDirectoryFilterStat'] 
                               + channels[channelName]['meNameNumEventsProcessed']) % (channels[channelName]['processes'][processName])
                            )
                        )
                    )
    	            if channels[channelName]['doExportSystematics'] and processes[processName]['hasSystematics']:
	                setattr(psetChannel, "template", cms.string(
                            (channels[channelName]['dqmDirectoryTemplate']
                           + channels[channelName]['meNameTemplateSys']) % (channels[channelName]['processes'][processName])
                        ))
	                setattr(psetChannel.normalization, "numEventsPassed", cms.string(
                            (channels[channelName]['dqmDirectoryTemplate']
                           + channels[channelName]['meNameNumEventsPassedSys']) % (channels[channelName]['processes'][processName])
                        ))	            
                        setattr(psetChannel, "systematics", cms.vstring(channels[channelName]['systematics']))  
	            else:
	                setattr(psetChannel, "template", cms.string(
                            (channels[channelName]['dqmDirectoryTemplate']
                           + channels[channelName]['meNameTemplate']) % (channels[channelName]['processes'][processName])
                        ))
	                setattr(psetChannel.normalization, "numEventsPassed", cms.string(
                            (channels[channelName]['dqmDirectoryFilterStat']
                           + channels[channelName]['meNameNumEventsPassed']) % (channels[channelName]['processes'][processName])
                        ))
                    setattr(psetProcess.distributions, channelName, psetChannel)
        setattr(psetProcesses, processName, psetProcess)
 
    # add template shapes for Higgs mass-points
    # CV: take all Higgs templates from AHtoMuTau channel for now;
    #     adjust normalization by AHtoElecTau/AHtoMuTau (AHtoElecMu/AHtoMuTau) Ztautau efficiency ratio
    for higgsMassPoint in higgsMassPoints:
        for higgsType in [ "A", "bbA" ]:
            psetHiggsMassPoint = cms.PSet(
                distributions = cms.PSet(),
                xSection = cms.double(AHtoMuTauSpecific_RECO_SAMPLES['%s%d' % (higgsType, higgsMassPoint)]['x_sec']),
                outputFilePath = cms.string("m%03d" % higgsMassPoint),
                outputFileName = cms.string(processes[higgsType]['outputFileName'])
            )
	    for channelName in channels.keys():
                if channelName in channelsToExport:
                    psetChannel = cms.PSet(
	                normalization = cms.PSet(
	                    numEventsProcessed = cms.string(
                                (channels[channelName]['dqmDirectoryFilterStat'] 
                               + channels[channelName]['meNameNumEventsProcessed']) % ('%s%d' % (higgsType, higgsMassPoint))
                            )
                        )
                    )
	            if channels[channelName]['doExportSystematics'] and processes[higgsType]['hasSystematics']:
	                setattr(psetChannel, "template", cms.string(
                            (channels[channelName]['dqmDirectoryTemplate']
                           + channels[channelName]['meNameTemplateSys']) % ('%s%d' % (higgsType, higgsMassPoint))
                        ))
	                setattr(psetChannel.normalization, "numEventsPassed", cms.string(
                            (channels[channelName]['dqmDirectoryTemplate']
                           + channels[channelName]['meNameNumEventsPassedSys']) % ('%s%d' % (higgsType, higgsMassPoint))
                        ))            
                        setattr(psetChannel, "systematics", cms.vstring(channels[channelName]['systematics']))  
	            else:
	                setattr(psetChannel, "template", cms.string(
                            (channels[channelName]['dqmDirectoryTemplate']
                           + channels[channelName]['meNameTemplate']) % ('%s%d' % (higgsType, higgsMassPoint))
                        ))
	                setattr(psetChannel.normalization, "numEventsPassed", cms.string(
                            (channels[channelName]['dqmDirectoryFilterStat']
                           + channels[channelName]['meNameNumEventsPassed']) % ('%s%d' % (higgsType, higgsMassPoint))
                        ))
                    setattr(psetHiggsMassPoint.distributions, channelName, psetChannel)
            setattr(psetProcesses, "%s%d" % (higgsType, higgsMassPoint), psetHiggsMassPoint)
    
    exportAnalysisResultsModule = cms.EDAnalyzer("DQMExportAnalysisResults",

        channels = cms.VPSet(psetsChannels),

        outputFilePath = cms.string(outputFilePath),

        processes = psetProcesses,

        systematics = cms.PSet(
            central_value = cms.PSet(
    	        dqmDirectory = cms.string('CENTRAL_VALUE'),
     	        outputFilePath = cms.string("nomin")
            ),        
       	    elecEnUp = cms.PSet(
    	        dqmDirectory = cms.string('sysElecEnUp'),
     	        outputFilePath = cms.string("ees+1")
            ),
    	    elecEnDown = cms.PSet(
    	        dqmDirectory = cms.string('sysElecEnDown'),
    	        outputFilePath = cms.string("ees-1")
            ),
    	    muonPtUp = cms.PSet(
    	        dqmDirectory = cms.string('sysMuonPtUp'),
     	        outputFilePath = cms.string("mpt+1")
            ),
    	    muonPtDown = cms.PSet(
    	        dqmDirectory = cms.string('sysMuonPtDown'),
    	        outputFilePath = cms.string("mpt-1")
            ),                                                  
    	    tauJetEnUp = cms.PSet(
    	        dqmDirectory = cms.string('sysTauJetEnUp'),
     	        outputFilePath = cms.string("tes+1")
            ),
    	    tauJetEnDown = cms.PSet(
    	        dqmDirectory = cms.string('sysTauJetEnDown'),
    	        outputFilePath = cms.string("tes-1")
            ),
    	    jetEnUp = cms.PSet(
    	        dqmDirectory = cms.string('sysJetEnUp'),
     	        outputFilePath = cms.string("jes+1")
            ),
    	    jetEnDown = cms.PSet(
    	        dqmDirectory = cms.string('sysJetEnDown'),
    	        outputFilePath = cms.string("jes-1")
            ),
            ZllRecoilCorrectionUp = cms.PSet(
     	        dqmDirectory = cms.string('sysZllRecoilCorrectionUp'),
    	        outputFilePath = cms.string("met+1")
            ),
            ZllRecoilCorrectionDown = cms.PSet(
     	        dqmDirectory = cms.string('sysZllRecoilCorrectionDown'),
    	        outputFilePath = cms.string("met-1")
            )
        )
    )

    return exportAnalysisResultsModule
