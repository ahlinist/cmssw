#!/usr/bin/env python
from TauAnalysis.Configuration.recoSampleDefinitionsAHtoMuTau_7TeV_grid_cfi \
        import recoSampleDefinitionsAHtoMuTau_7TeV as samples

import TauAnalysis.Configuration.submitAnalysisToLXBatch as submit
import TauAnalysis.Configuration.userRegistry as reg
import TauAnalysis.Configuration.tools.castor as castor
import os

channel = 'AHtoMuTau'
#configFile = 'runAHtoMuTau_OldTaNC_cfg.py'
configFile = 'runAHtoMuTau_cfg.py'

#reg.overrideJobId(channel, 'Run37skimForMike')
#reg.overrideJobId(channel, 'Run37sysTanc')
#reg.overrideJobId(channel, 'Run37sys')
#reg.overrideJobId(channel, 'Run39FR')
#reg.overrideJobId(channel, 'Run45FR')
#reg.overrideJobId(channel, 'Run45plain')
#reg.overrideJobId(channel, 'Run42OldTaNCfinal')
#reg.overrideJobId(channel, 'Run42OldTaNCfinal')
#for jobId in ['Run39FRTanc', 'Run39plainTanc', 'Run45FRTanc', 'Run45plainTanc']:
#for jobId in ['Run39FRTanc']:
for jobId in ['Run37sysTanc']:
    reg.overrideJobId(channel, jobId)

    powheg_samples = [sample for sample in samples['SAMPLES_TO_ANALYZE']
                      if sample.find('POWHEG') != -1 ]

    fake_rate_samples = [sample for sample in samples['SAMPLES_TO_ANALYZE']
                         if samples['RECO_SAMPLES'][sample]['enableFakeRates']]

    factorized_samples = [sample for sample in samples['SAMPLES_TO_ANALYZE']
                         if samples['RECO_SAMPLES'][sample]['factorize']]

    samples_for_mike = ['WplusJets_madgraph',
                        'PPmuXptGt20Mu10',
                        'PPmuXptGt20Mu15',
                        'data_Mu_Run2010A_Nov4ReReco',
                        'data_Mu_Run2010B_Nov4ReReco' ]

    # If this is a list, only the items in the list will be analyzed.
    samplesToAnalyze = []
    #samplesToAnalyze = fake_rate_samples

    # Where we will send the output on castor
    outputPath = reg.getAnalysisFilePath(channel)
    jobId = reg.getJobId(channel)
    # Figure out where our root files were stored for the desired skim
    skimPath = reg.getSkimEvents(channel)

    # Get all the skim files from the castor directory
    skim_files = [os.path.join(skimPath, file) for file in
        filter(lambda x: x.startswith('skim_'), (
        file_info['file'] for file_info in castor.nslsl(skimPath)))]

    def inputFileMapper(channel, sample, jobId):
        for file in skim_files:
            if file.find('_' + sample + '_') != -1:
                yield file

    enableFakeRates = False
    enableSystematics = False
    changeTauId = None
    saveFinalEvents = False
    eventList = None

    if jobId == 'Run39FR' or jobId == 'Run39plain':
        for sample in samples['SAMPLES_TO_ANALYZE']:
            if samples['RECO_SAMPLES'][sample]['factorize']:
                print "Disabling factorization for", sample
                samples['RECO_SAMPLES'][sample]['factorize'] = False
        #samplesToAnalyze = ['PPmuXptGt20Mu15']
        samplesToAnalyze = [
            'PPmuXptGt20Mu15',
            'data_Mu_Run2010A_Nov4ReReco',
            'data_Mu_Run2010B_Nov4ReReco',
            'ZtautauPU156bx',
        ]
        if jobId == 'Run39FR':
            enableFakeRates = True

    if jobId == 'Run39FRTanc' or jobId == 'Run39plainTanc':
        for sample in samples['SAMPLES_TO_ANALYZE']:
            if samples['RECO_SAMPLES'][sample]['factorize']:
                print "Disabling factorization for", sample
                samples['RECO_SAMPLES'][sample]['factorize'] = False
        #samplesToAnalyze = ['PPmuXptGt20Mu15']
        samplesToAnalyze = [
            'PPmuXptGt20Mu15',
            'data_Mu_Run2010A_Nov4ReReco',
            'data_Mu_Run2010B_Nov4ReReco',
            'ZtautauPU156bx',
        ]
        changeTauId = "tauID('byTaNCloose') > 0.5"
        if jobId == 'Run39FRTanc':
            enableFakeRates = True

    if jobId == "Run45FRTanc" or jobId == "Run45plainTanc":
        for sample in samples['SAMPLES_TO_ANALYZE']:
            if samples['RECO_SAMPLES'][sample]['factorize']:
                print "Disabling factorization for", sample
                samples['RECO_SAMPLES'][sample]['factorize'] = False
        samplesToAnalyze = [
            'PPmuXptGt20Mu10',
            'WplusJets_madgraph',
            'Zmumu_powheg',
            'ZtautauPU156bx',
            'TTplusJets_madgraph',
        ]
        changeTauId = "tauID('byTaNCloose') > 0.5"
        if jobId == 'Run45FRTanc':
            enableFakeRates = True

    if jobId == "Run45FR" or jobId == "Run45plain":
        for sample in samples['SAMPLES_TO_ANALYZE']:
            if samples['RECO_SAMPLES'][sample]['factorize']:
                print "Disabling factorization for", sample
                samples['RECO_SAMPLES'][sample]['factorize'] = False
        samplesToAnalyze = [
            'PPmuXptGt20Mu10',
            'WplusJets_madgraph',
            'ZtautauPU156bx',
            'TTplusJets_madgraph',
        ]
        if jobId == 'Run45FR':
            enableFakeRates = True

    if jobId == "Run37sys":
        enableSystematics = True

    if jobId == "Run37sysTanc":
        enableSystematics = True
        changeTauId = "tauID('byTaNCloose') > 0.5"

    if jobId == "Run37skimForMike":
        #saveFinalEvents = True
        #samplesToAnalyze = samples_for_mike
        samplesToAnalyze = [
            'data_Mu_Run2010A_Nov4ReReco',
            'data_Mu_Run2010B_Nov4ReReco' ]
        eventList = 'uw_events.txt'

    #for sample in samples['SAMPLES_TO_ANALYZE']:
        #print "Disabling weights"
        #samples['RECO_SAMPLES'][sample]['applyMuonTriggerEfficiencyCorrection'] = False
        #samples['RECO_SAMPLES'][sample]['applyMuonIsolationEfficiencyCorrection'] = False
        #samples['RECO_SAMPLES'][sample]['applyVertexMultiplicityReweighting'] = False

    submit.submitAnalysisToLXBatch(
        configFile=configFile,
        channel=channel,
        samples=samples,
        samplesToAnalyze = samplesToAnalyze,
        outputDirectory = outputPath,
        disableSysUncertainties = not enableSystematics,
        enableFakeRates = enableFakeRates,
        inputFileMap = inputFileMapper,
        eventList = eventList,
        changeTauId = changeTauId,
        processName = 'lxbatch',
        saveFinalEvents = False,
    )
