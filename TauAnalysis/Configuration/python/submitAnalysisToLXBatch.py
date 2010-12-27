import os
import TauAnalysis.Configuration.tools.castor as castor
import TauAnalysis.Configuration.tools.jobtools as jobtools
from TauAnalysis.Configuration.tools.harvestingLXBatch import \
        write_comment_header
import TauAnalysis.Configuration.userRegistry as reg

from TauAnalysis.Configuration.prepareConfigFile2 import getNewConfigFileName, prepareConfigFile

def submitAnalysisToLXBatch(configFile = None, channel = None, samples = None,
                            samplesToAnalyze = None, samplesToSkip = None,
                            disableFactorization = False,
                            disableSysUncertainties = False,
                            script_directory=None,
                            cfgdir = 'lxbatch',
                            inputFileMap = None, outputFileMap = None,
                            outputDirectory = None,
                            enableEventDumps = False,
                            enableFakeRates = False,
                            processName = None,
                            changeTauId = None,
                            saveFinalEvents = False):

    """
    Submit analysis job (event selection, filling of histogram)
    to local machine
    """

    # check that configFile, channel, samples and jobId
    # parameters are defined and non-empty
    for param in ["configFile", "channel", "samples",
                  "outputDirectory"]:
        if locals()[param] is None:
            raise ValueError("Undefined '%s' parameter!!" % param)

    jobId = reg.getJobId(channel)

    # If not specified take script directory from user preferences.
    if script_directory is None:
        script_directory = reg.getHarvestScriptLocation()

    # Make sure our output file for the scripts is okay
    if not os.path.exists(script_directory):
        os.makedirs(script_directory)

    # Get all the files in our output directory that have non-zero size
    tmp_files = set(x['file'] for x in castor.nslsl(outputDirectory)
                    if x['size'])

    # Keep track of the files we care about
    relevant_files = set([])

    submit_file_name = 'submit_lxbatch_analysis_' + jobId + '.sh'
    with open(submit_file_name, 'w') as submit_file:
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

            # Now build the scripts to feed to bsub
            # Find the input files
            input_files = list(inputFileMap(channel, sample, jobId))

            if len(input_files) > 0:
                print("Submitting %s in %i part(s)" % (sample, len(input_files)))
            else:
                print("No local input files for %s found !!" % sample)

            for job, file in enumerate(input_files):

                input_files = [file]
                # The None in the tuple indicates this file has no dependencies in
                # the batch job.
                input_files_and_jobs = [ (None, file) for file in input_files ]
                # Need to prepend file:, and strip off the directory since we
                # always have bsub rfcp the input files to the working
                # directory.
                input_files_for_cfgOptions = [
                    'file:' + os.path.basename(file) for file in input_files]


                output_file = outputFileMap(channel, sample, jobId)
                input_file_hash = jobtools.hash_files(
                    input_files, add_time=False)
                # Add the hash of the input file so we know the provenance of all
                # files
                output_file = os.path.join(outputDirectory, output_file.replace(
                    '.root', '_' + str(job) + '_' + input_file_hash + '.root'))

                relevant_files.add(os.path.basename(output_file))

                # Uncomment to skip rerunning of old jobs
                #if os.path.basename(output_file) in tmp_files:
                    #print " done; skipping", output_file
                    #continue

                # First, prepare the configuration file
                newConfigFile = getNewConfigFileName(
                    configFile, cfgdir, sample,
                    jobId, index = job, label = "@lxbatch")

                write_comment_header(submit_file, " cfg: " + newConfigFile)
                #--------------------------------------------------------------------
                # CV: temporary "hack" for producing (ED)Ntuples/skims for tau id. efficiency measurement
                jobCustomizations = []
                jobCustomizations.append("if hasattr(process, 'ntupleOutputModule'):")
                jobCustomizations.append("    process.ntupleOutputModule.fileName = '%s'" % output_file)
                jobCustomizations.append("if hasattr(process, 'skimOutputModule'):")
                jobCustomizations.append("    process.skimOutputModule.fileName = '%s'" % output_file)
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
                    configFile = configFile, jobInfo = jobInfo,
                    newConfigFile = newConfigFile,
                    sample_infos = samples,
                    disableFactorization = disableFactorization,
                    disableSysUncertainties = disableSysUncertainties,
                    # We always copy the input files to the local directory
                    # before running cmsRun, so just take the basname
                    input_files = input_files_for_cfgOptions,
                    output_file = os.path.basename(output_file),
                    enableEventDumps = enableEventDumps, enableFakeRates = enableFakeRates,
                    processName = processName,
                    saveFinalEvents = saveFinalEvents,
                    changeTauId = changeTauId,
                    customizations = jobCustomizations)

                #print("job %i:" % (job + 1))

                # Build our batch job
                jobname, script = jobtools.make_bsub_script(
                    output_file, input_files_and_jobs, '/tmp/log',
                    "cmsRun %s" % newConfigFile, pass_io_files = False)

                bsub_script_file = os.path.join(
                    script_directory, "_".join([
                        'analyze', sample, 'job',
                        str(job), input_file_hash]) + '.sh')
                with open(bsub_script_file, 'w') as bsub_script:
                    bsub_script.write(script)
                # Add this bsub to our submission script
                submit_file.write("bsub < %s\n" % bsub_script_file)

        print len(tmp_files)
        garbage = tmp_files - relevant_files
        print len(garbage)
        if garbage:
            print "Found %i files not generated by this job!!" % len(garbage)
            print " You should really run:"
            print " cat ana_garbage.txt | xargs -n 1 -P 10 rfrm"
            with open('ana_garbage.txt', 'w') as garbage_script:
                for file in garbage:
                    garbage_script.write(
                        '%s\n' % os.path.join(outputDirectory, file))
        print "Run ./%s to submit jobs" % submit_file_name
        os.chmod(submit_file_name, 0755)
