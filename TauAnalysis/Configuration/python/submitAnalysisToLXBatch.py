import os
import TauAnalysis.Configuration.tools.castor as castor
import TauAnalysis.Configuration.tools.jobtools as jobtools
from TauAnalysis.Configuration.tools.harvestingLXBatch import \
        write_comment_header

from TauAnalysis.Configuration.prepareConfigFile2 import getNewConfigFileName, prepareConfigFile

def submitAnalysisToLXBatch(configFile = None, channel = None, samples = None, jobId = None,
                            samplesToAnalyze = None, samplesToSkip = None,
                            disableFactorization = False,
                            disableSysUncertainties = False,
                            script_directory='/tmp/harvest_scripts',
                            cfgdir = 'lxbatch',
                            inputFileMap = None, outputFileMap = None,
                            outputDirectory = None,
                            enableEventDumps = False,
                            enableFakeRates = False,
                            processName = None,
                            saveFinalEvents = False):

    """
    Submit analysis job (event selection, filling of histogram)
    to local machine
    """

    # check that configFile, channel, samples and jobId
    # parameters are defined and non-empty
    for param in ["configFile", "jobId", "channel", "samples",
                  "outputDirectory"]:
        if locals()[param] is None:
            raise ValueError("Undefined '%s' parameter!!" % param)

    # Get all the files in our output directory that have non-zero size
    tmp_files = set(x['file'] for x in castor.nslsl(outputDirectory)
                    if x['size'])

    # Keep track of the files we care about
    relevant_files = set([])

    with open('submit_lxbatch_analysis.sh', 'w') as submit_file:
        # Loop over the samples to be analyzed
        for sample in samples['SAMPLES_TO_ANALYZE']:
            write_comment_header(submit_file, " Sample: " + sample)
            # Skip submitting crab job in case
            #  o list of samples for which crab jobs are to be submitted has been
            #    explicitely specified
            #  o sample has explicitely been requested to be skipped
            if samplesToAnalyze:
                if sample not in samplesToAnalyze:
                    print "Skipping", sample
                    continue
            if samplesToSkip:
                if sample in samplesToSkip:
                    print "Skipping", sample
                    continue

            sample_info = samples['RECO_SAMPLES'][sample]

            # Make job info
            jobInfo = {
                'channel' : channel,
                'sample' : sample,
                'id' : jobId
            }

            # First, prepare the configuration file
            newConfigFile = getNewConfigFileName(configFile, cfgdir, sample, jobId, label = "@lxbatch")
            write_comment_header(submit_file, " cfg: " + newConfigFile)
            #--------------------------------------------------------------------
            # CV: temporary "hack" for producing (ED)Ntuples/skims for tau id. efficiency measurement
            jobCustomizations = []
            jobCustomizations.append("if hasattr(process, 'ntupleOutputModule'):")
            jobCustomizations.append("    process.ntupleOutputModule.fileName = options.outputFile")
            jobCustomizations.append("if hasattr(process, 'skimOutputModule'):")
            jobCustomizations.append("    process.skimOutputModule.fileName = options.outputFile")
            HLTprocessName = 'HLT'
            if 'hlt' in samples['RECO_SAMPLES'][sample].keys():
                HLTprocessName = samples['RECO_SAMPLES'][sample]['hlt'].getProcessName()
            jobCustomizations.append("if hasattr(process, 'hltMu'):")
            jobCustomizations.append("    process.hltMu.selector.src = cms.InputTag('TriggerResults::%s')" % HLTprocessName)
            jobCustomizations.append("process.patTrigger.processName = '%s'" % HLTprocessName)
            jobCustomizations.append("process.patTriggerEvent.processName = '%s'" % HLTprocessName)
            jobCustomizations.append("if hasattr(process, 'prePatProductionSequence'):")
            jobCustomizations.append("    process.prePatProductionSequence.remove(process.prePatProductionSequenceGen)")
            if samples['RECO_SAMPLES'][sample]['type'] == 'Data':
                jobCustomizations.append("if hasattr(process, 'ntupleProducer'):")
                jobCustomizations.append("    delattr(process.ntupleProducer.sources, 'tauGenJets')")
                jobCustomizations.append("    delattr(process.ntupleProducer.sources, 'genJets')")
                jobCustomizations.append("    delattr(process.ntupleProducer.sources, 'genPhaseSpaceEventInfo')")
                jobCustomizations.append("    delattr(process.ntupleProducer.sources, 'genPileUpEventInfo')")
            #jobCustomizations.append("print process.dumpPython()")
            #--------------------------------------------------------------------

            prepareConfigFile(
              configFile = configFile, jobInfo = jobInfo, newConfigFile = newConfigFile,
              sample_infos = samples,
              disableFactorization = disableFactorization,
              disableSysUncertainties = disableSysUncertainties,
              #input_files = input_files, output_file = output_file,
              enableEventDumps = enableEventDumps, enableFakeRates = enableFakeRates,
              processName = processName,
              saveFinalEvents = saveFinalEvents,
              customizations = jobCustomizations)

            # Now build the scripts to feed to bsub
            # Find the input files
            input_files = list(inputFileMap(channel, sample, jobId))

            if len(input_files) > 0:
                print("Submitting %s in %i part(s)" % (sample, len(input_files)))
            else:
                print("No local input files for %s found !!" % sample)

            for job, file in enumerate(input_files):
                #print("job %i:" % (job + 1))

                # The None in the tuple indicates this file has no dependencies in
                # the batch job.
                input_files = [ (None, file) ]

                output_file = outputFileMap(channel, sample, jobId)
                input_file_hash = jobtools.hash_files(
                    map(lambda x:x[1], input_files), add_time=False)
                # Add the hash of the input file so we know the provenance of all
                # files
                output_file = output_file.replace(
                    '.root', '_' + input_file_hash + '.root')

                # Build our batch job
                jobname, script = jobtools.make_bsub_script(
                    output_file, input_files, '/tmp/log',
                    "cmsRun %s" % newConfigFile, use_VarParsing = True)

                bsub_script_file = os.path.join(
                    script_directory, "_".join([
                        'analyze', sample, 'job',
                        str(job), input_file_hash]) + '.sh')
                with open(bsub_script_file, 'w') as bsub_script:
                    bsub_script.write(script)
                # Add this bsub to our submission script
                submit_file.write("bsub < %s\n" % bsub_script_file)
