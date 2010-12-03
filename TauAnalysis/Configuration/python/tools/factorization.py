import FWCore.ParameterSet.Config as cms
import os

from TauAnalysis.DQMTools.tools.composeSubDirectoryName import \
        composeSubDirectoryName, getCutName

def _make_process_adder(process):
    def adder(object, name):
        " Return an [object] with name to the process. Returns the object. "
        if hasattr(process, name):
            raise ValueError("Process already has an item called [%s]" % name)
        setattr(process, name, object)
        return getattr(process, name)
    return adder

def factorize(process, input_dir=None, output_dir=None,
              analyzers=None, sequence=None):
    """  Build factorization sequence

    Tight and loose analyzers must be a list of dictionaries containing:

    cuts = list of strings corresponding to cuts (evtSelGenPhaseSpace, etc)
    analyzer = ahMuTauAnalyzer_woBtag, zMuTauAnalyzer, etc
    (only one tight/loose needs to specify numerator and denominator)

    """
    #sequence = process.factorize

    # Build a function that registers objects in the process
    register_module = _make_process_adder(process)
    sequencer = [sequence]

    input_dir_name = input_dir.replace('/', '_').strip('_')
    output_dir_name = output_dir.replace('/', '_').strip('_')

    def sequence_adder(object, names):
        full_name = "_".join(names)
        sequencer[0] += register_module(object, full_name)

    for output_analyzer, analyzer_info in analyzers.iteritems():
        print "Building factorization sequence for:"
        print " -- input dir:", input_dir
        print " -- tight analyzer:", analyzer_info['tight_analyzer']
        print " -- loose analyzer:", analyzer_info['loose_analyzer']
        # Build event selection cut lists.  Note you must load the analyze
        # sequence into the event before calling this function
        tight_cuts = [ getattr(process, cut)
                      for cut in analyzer_info['tight_cuts'] ]
        loose_cuts = [ getattr(process, cut)
                      for cut in analyzer_info['loose_cuts'] ]
        all_cuts = set(tight_cuts + loose_cuts)

        dqm_dir_in_tight = os.path.join(
            input_dir, analyzer_info['tight_analyzer'])
        dqm_dir_in_loose = os.path.join(
            input_dir, analyzer_info['loose_analyzer'])

        dqm_dir_out = os.path.join(output_dir, output_analyzer)
        # First do the easy part.  Copy all histograms that before there are any
        # differences is the cuts.
        tight_cuts_for_pairs = [ None ] + tight_cuts + [loose_cuts[0]]
        easy_copier = cms.EDAnalyzer(
            "DQMHistScaler",
            dqmDirectory_input = cms.string(dqm_dir_in_tight),
            dqmSubDirectories_input = cms.vstring(list(
                composeSubDirectoryName(before, after)
                for before, after in zip(
                    tight_cuts_for_pairs[0:-1], tight_cuts_for_pairs[1:]))
            ),
            scaleFactor = cms.double(1.),
            dqmDirectory_output = cms.string(dqm_dir_out)
        )
        # Add to our factorization sequence
        sequence_adder(
            easy_copier, ["copy_prefactorized", output_analyzer,
                          input_dir_name, output_dir_name ]
        )
        # configure a DQMHistScaler that copies FilterStatistics objects filled
        # from the tight branch.
        easy_copier_stats = cms.EDAnalyzer(
            "DQMHistScaler",
            dqmDirectory_input = cms.string(
                dqm_dir_in_tight + "/FilterStatistics/"),
            dqmSubDirectories_input = cms.vstring(
               [x.pluginName.value() for x in all_cuts]
            ),
            scaleFactor = cms.double(1.),
            dqmDirectory_output = cms.string(dqm_dir_out + "/FilterStatistics/")
        )
        # Add to sequence
        sequence_adder(
            easy_copier_stats, ["copy_filterstats", output_analyzer,
                                input_dir_name, output_dir_name ])
        # Okay, now we need to make the histograms after the cut where the loose
        # and tight selections differ. The normalization from the *with* muon
        # isolation branch, and the *shape* from the without muon isolation
        # branch.
        loose_cuts_for_pairs = loose_cuts + [ None ]
        # Figure out how the cuts pair together
        loose_cut_pairs = zip(loose_cuts_for_pairs[0:-1],
                              loose_cuts_for_pairs[1:])

        me_name = lambda cut : "#".join([cut, 'a1', 's1'])
        # FIXME we may get out of sync by one cut here.  Need to double check.
        scalers = []
        for loose_cut_pair in loose_cut_pairs:
            cut_directory = composeSubDirectoryName(*loose_cut_pair)
            #print " Building DQM scaler for cut %s in analyzer: %s" % (
                #cut_directory, tight_info['analyzer'])
            # Build the base for the scaler that will manage this cut
            cut_name = getCutName(loose_cut_pair[0])
            scaler = cms.EDAnalyzer(
                "DQMHistScaler",
                # Copy the histograms from the loose cut path
                dqmDirectory_input = cms.string(dqm_dir_in_loose),
                # Copy only the histograms for the current cut level
                dqmSubDirectories_input = cms.vstring(cut_directory),
                # Copy to the final output directory
                dqmDirectory_output = cms.string(dqm_dir_out),
                # The locations of the filter statistics
                dqmDirectory_factorizedLooseSel = cms.string(
                    dqm_dir_in_loose + "/FilterStatistics/"),
                dqmDirectory_factorizedTightSel = cms.string(
                    dqm_dir_in_tight + "/FilterStatistics/"),
                # Compute scale factor.
                # Numerator = number of passed events for a given loose/tight
                #    chain
                # Denominator = number of input events (i.e. genPhaseSpace)
                meName_numerator = cms.string(
                    me_name(cut_name+"/passed_cumulative_numWeighted")
                ),
                meName_denominator = cms.string(
                    #me_name(cut_name+"/passed_cumulative_numWeighted")
                    me_name("genPhaseSpaceCut/passed_cumulative_numWeighted")
                ),
                meType = cms.string("real"),
            )
            scalers.append((loose_cut_pair, scaler))
            name_in_process = "factorize" + cut_directory + \
                    analyzer_info['tight_analyzer']
            # Add the scaler to the sequence
            sequence_adder(scaler,
                           ['factorize', cut_directory,
                            analyzer_info['tight_analyzer'],
                            input_dir_name, output_dir_name])
