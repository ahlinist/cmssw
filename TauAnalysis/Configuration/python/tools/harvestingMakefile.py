import itertools
import os
import glob
import hashlib
import sys

'''

harvestingMakefile

Author: Evan K. Friis, UC Davis

Tools to build a Makefile to harvests DQM histograms in parallel.
Each harvest job merges up to three input DQM .root files.  If necessary,
the output of this harvest job is used as an input for an additional DQM
merge job.

See the docstring of buildMakefile(...) for an exmaple of use.

'''


def group(items, items_per_group):
    current_group = []
    for item in items:
        current_group.append(item)
        if len(current_group) == items_per_group:
            yield tuple(current_group)
            current_group = []
    # Yield any stragglers in a smaller group
    if current_group:
        yield tuple(current_group)


def buildMergeTree(files, output_filename, intermediate_dir, merge_per_job=3, verbose=False):
    ''' Build a make file to harvest DQM histograms

    Build a dependency tree of cascading merge jobs.  Each merge job will
    attempt to merge three files.
    '''

    layers = []
    # We don't mess around with the path for this one
    input_files = (file for file in files)

    done = False
    while not done:
        new_layer = []
        # Keep track of jobs that have only one file to merge.  Send these
        # unmodified to the next layer
        leftover_files = []
        for job, files_to_merge in enumerate(group(input_files, merge_per_job)):
            # Check if this is the only file in the merge job
            if len(files_to_merge) == 1:
                leftover_files.append(files_to_merge[0])
                continue
            # Compute a hash of the input filenames to ensure that nothing
            # tricky happens when we update the list of input files
            hash = hashlib.md5()
            map(hash.update, files_to_merge)
            hash_str = hash.hexdigest()[:6]
            # Otherwise merge this set
            output_file_name = os.path.join(
                intermediate_dir,
                output_filename.replace('.root', '_layer%i_job%i_%s.root' %
                                        (len(layers), job, hash_str))
            )
            new_layer.append( (output_file_name, files_to_merge ) )

        # Make sure we have some files to merge
        if not new_layer:
            if not len(leftover_files):
                print "WARNING: Output file %s has no input files!" % output_filename
            done = True
            continue
        # Add this layer to our set of jobs
        layers.append(new_layer)
        # Make a new generator of input files using the outputs of the new layer
        input_files = itertools.chain(( job[0] for job in new_layer ), leftover_files)
        # Check and see if we are done - only one job in this layer, no leftovers
        if len(new_layer) == 1 and not len(leftover_files):
            done = True

    if verbose:
        for i, layer in enumerate(layers):
            for job, (output, inputs) in enumerate(layer):
                print i, job, output, inputs

    return layers

def writeMakefileLocalCopy(castor_file, working_dir, makefile):
    basename = os.path.basename(castor_file)
    local_path = os.path.join(working_dir, basename)
    target_line = "%s:\n" % local_path
    rfcp_line = "\trfcp %s %s\n" % (castor_file, local_path)
    makefile.write(target_line)
    makefile.write(rfcp_line)
    return local_path

def copyLocalAndWrite(castor_files, working_dir, makefile):
    for castor_file in castor_files:
        castor_file = castor_file.replace('rfio:', '')
        yield writeMakefileLocalCopy(castor_file, working_dir, makefile)

def writeMakefileCommands(mergeTree, outputFile, makefile):
    for level, layer in enumerate(mergeTree):
        makefile.write("\n# Merge layer %i \n" % level)
        for job in layer:
            output_file = job[0]
            input_files = [file for file in job[1]]
            input_files_no_castor = [
                file for file in input_files if not file.find('castor')!=-1]
            target_line = '%s: %s\n' % (
                output_file, ' '.join(input_files_no_castor))
            makefile.write(target_line)
            run_line = '\tnice ${HARVEST} %s %s\n' % (
                 output_file, ' '.join(input_files))
            makefile.write(run_line)
            if input_files_no_castor:
                pass
                # Delete the last layer
                #rm_line = '\trm %s\n' % ' '.join(input_files_no_castor)
                #makefile.write(rm_line)
            makefile.write('\n')
    # Make a rule to copy our final level file
    if mergeTree:
        final_merge_output = mergeTree[-1][0][0]
        makefile.write('# Copy final output to desired location\n')
        makefile.write('%s: %s\n' % (outputFile, final_merge_output))
        makefile.write('\tcp %s %s\n' % (final_merge_output, outputFile))

def buildMakefile(merge_jobs, working_dir, makefilename,
                  copy_first=True, merge_per_job=3, ana_defs=None,
                  plot_defs=None, plotters=None,
                  harvest_tool = 'genericHarvester.py'):
    ''' Build a Makefile to merge DQM histograms

    [merge_jobs] is a list of tuples of the form
    [ (alias1, output1, [input1_1.root, input1_2.root, ... ] ),
      (alias2, output2, [input2_1.root, input2_2.root, ... ] ),
      ...  ]

    [working_dir] is a local directory to store the temporary
    files.  [makefilename] specifies the name of the output
    Makefile

    [harvest_tool] is the name of the script in
    TauAnalysis/Configuration/python/tools/
    that can merge the desired type of files.
    '''

    # Check if intermediate working directory exists and create it if not.
    if not os.path.exists(working_dir):
        print "Creating working directory:", working_dir
        os.makedirs(working_dir)
    # Make sure it isn't an existing file
    if not os.path.isdir(working_dir):
        print "Can't access (or create) the working directory:",\
                working_dir, " Please fix!"

    # Setup makefile
    makefile = open(makefilename, 'w')
    path_to_harvester_script = os.path.join(
        os.environ['CMSSW_BASE'], 'src', 'TauAnalysis/Configuration/',
        'python/tools', harvest_tool)
    makefile.write("# Path to harvester script\n")
    makefile.write("HARVEST = %s\n\n" % path_to_harvester_script)

    makefile.write("# List of targets to merge\n")
    makefile.write("all: analysis %s\n\n" % ' '.join(merge_job[0] for merge_job in merge_jobs if merge_job[2]))

    makefile.write("# Alias to filename mappings:\n")
    for alias, output_file in [ (merge_job[0], merge_job[1]) for merge_job in merge_jobs if merge_job[2]]:
        makefile.write("%s: %s\n" % (alias, output_file))

    for alias, output_file, to_merge in merge_jobs:
        print "Building makefile section for %s" % output_file
        makefile.write("\n#########################################################\n")
        makefile.write("# Merging %s\n" % output_file)
        makefile.write("#########################################################\n")

        to_merge = copyLocalAndWrite(to_merge, working_dir, makefile)

        writeMakefileCommands(
            buildMergeTree(
                to_merge,
                os.path.basename(output_file),
                working_dir,
                merge_per_job
            ),
            output_file,
            makefile
        )

    if not ana_defs:
        makefile.write("analysis:\n")
        makefile.write('\t echo "No analysis jobs specified."\n')
    else:
        __import__(ana_defs)
        ana_module = sys.modules[ana_defs]
        path_to_scaler_script = os.path.join(
            os.environ['CMSSW_BASE'], 'src', 'TauAnalysis/Configuration/',
            'python/tools', 'genericScaler.py')

        # Keep track of what file to get a given source from.  This gets updated
        # as the histograms get merged, scaled, etc.
        histo_source = {}
        # our initial source is the merging of the indivudal job outputs
        for sample, merge_out, merge_in in merge_jobs:
            # Make sure there were input files
            if merge_in:
                histo_source[sample] = merge_out

        makefile.write("\n#########################################################\n")
        makefile.write("# Analysis jobs \n")
        makefile.write("#########################################################\n")

        makefile.write("\n#########################################################\n")
        makefile.write("# Scale histos \n")
        makefile.write("#########################################################\n")
        makefile.write("SCALE = %s\n\n" % path_to_scaler_script)

        # Scale all of our input histograms
        scale_jobs = dict(
            (sample, (merge_out, merge_out.replace('.root', '_scaled.root')))
            for sample, merge_out in histo_source.iteritems() )

        for sample, (input_histo, scale_histo) in scale_jobs.iteritems():
            print "Generating scaling code for", sample
            makefile.write("\n")
            makefile.write(scale_histo+": " + input_histo + "\n")
            makefile.write("\t${SCALE} %s %s %s %s\n\n"
                           % (scale_histo, input_histo, sample, ana_defs))
            # The scaled histogram is now the source for this sample
            histo_source[sample] = scale_histo

        path_to_factorize_script = os.path.join(
            os.environ['CMSSW_BASE'], 'src', 'TauAnalysis/Configuration/',
            'python/tools', 'genericFactorizer.py')

        makefile.write("\n#########################################################\n")
        makefile.write("# Factorize Histos \n")
        makefile.write("#########################################################\n")
        makefile.write("FACTORIZE = %s\n\n" % path_to_factorize_script)

        # Build the jobs for actorization
        factorize_jobs = dict(
            (sample,
             (scaled_file, scaled_file.replace('.root', '_factorized.root')))
             for sample, scaled_file in histo_source.iteritems() if
            ana_module.RECO_SAMPLES[sample]['factorize']
        )

        for sample, (scaled_file, factorized_file) in factorize_jobs.iteritems():
            print "Generating factorization code for", sample
            makefile.write(factorized_file+": " + scaled_file + "\n")
            makefile.write("\t${FACTORIZE} %s %s %s\n\n" %
                           (factorized_file, scaled_file, sample))
            # Update the 'official' histo_source
            histo_source[sample] = factorized_file

        path_to_add_script = os.path.join(
            os.environ['CMSSW_BASE'], 'src', 'TauAnalysis/Configuration/',
            'python/tools', 'genericAdder.py')
        makefile.write("\n#########################################################\n")
        makefile.write("# Add Histos \n")
        makefile.write("#########################################################\n")
        makefile.write("ADDER = %s\n\n" % path_to_add_script)

        # Build dependency tree
        merge_jobs = {}
        for merged_sample, merge_info in ana_module.MERGE_SAMPLES.iteritems():
            subsamples = merge_info['samples']
            print "Generating merge code for", merged_sample, " merging: ", " ".join(subsamples)
            # Find the sources for the subsamples
            subsample_files = {}
            for sample in subsamples:
                subsample_files[sample] = histo_source[sample]
            # Get directory
            directory = os.path.dirname(subsample_files.values()[0])
            output_file = os.path.join(directory, 'merged_%s.root' % merged_sample)
            merge_jobs[merged_sample] = (subsample_files, output_file)

        for sample, (input_files, output_file ) in merge_jobs.iteritems():
            # Make target with prerequisites
            makefile.write(output_file + ": " + " ".join(input_files.values()) + "\n")
            makefile.write("\t${ADDER} outputFile=%s dirout=harvested/%s "
                           % (output_file, sample))
            for subsample, input_file in input_files.iteritems():
                # We might need to change the input folder if this is a
                # factorized sample
                subsample_folder = subsample
                if subsample in factorize_jobs:
                    subsample_folder = subsample + "_factorized"
                makefile.write(" dirin=harvested/%s inputFiles=%s"
                               % (subsample_folder, input_file))
            makefile.write("\n\n")
            # Update the histo source
            histo_source[sample] = output_file

        # At this point, we can drop stuff that we no longer need for plotting.
        # The individual sample files A200Sum, Ztautau, still have all the
        # information.
        makefile.write("\n##################################################\n")
        makefile.write("# Drop crap \n")
        makefile.write("####################################################\n")
        path_to_trimmer_script = os.path.join(
            os.environ['CMSSW_BASE'], 'src', 'TauAnalysis/Configuration/',
            'python/tools', 'genericTrimmer.py')

        makefile.write("# Path to trimmer script\n")
        makefile.write("TRIM = %s\n\n" % path_to_trimmer_script)

        # Now we need to figure out what will be plotted, and keep that.  We do
        # this by looking at the draw jobs in the plotXXXX_cff file.
        __import__(plot_defs)
        plot_cff = sys.modules[plot_defs]
        # Keep track of the plots we want to keep
        keepers = []
        for plotter in plotters:
            # Get the DQMHistPlotter
            plot_module = getattr(plot_cff, plotter)
            draw_jobs = plot_module.drawJobs
            # Loop over the draw jobs
            for draw_job in draw_jobs.parameters_().values():
                for histo_src in draw_job.plots.dqmMonitorElements:
                    histo_src = os.path.dirname(
                        (str(histo_src).replace('#PROCESSDIR#','')))
                    keepers.append(histo_src)

        for sample in histo_source.keys():
            print "Generating trim code for", sample
            source_file = histo_source[sample]
            output_file = source_file.replace('.root', '_trim.root')
            keep_statements = ['"drop harvested/*"',
                               '"keep */FilterStatistics"']
            for keep in keepers:
                sample_name = sample
                if sample in factorize_jobs:
                    sample_name += "_factorized"
                keep_statements.append('"keep %s"' % os.path.normpath(
                    os.path.join( "harvested", "%s/%s" % (sample_name, keep))))
            makefile.write(output_file + ": " + source_file + "\n")
            makefile.write("\t${TRIM} $@ $< " + " ".join(keep_statements))
            makefile.write("\n\n")
            # Update our histo source
            histo_source[sample] = output_file

        makefile.write("\n#########################################################\n")
        makefile.write("# Merge all \n")
        makefile.write("#########################################################\n")

        # Necessary samples = SAMPLES to plot + SAMPLES_TO_PRINT
        ALL_SAMPLES = set()
        ALL_SAMPLES.update(ana_module.SAMPLES_TO_PRINT)
        ALL_SAMPLES.update(ana_module.SAMPLES_TO_PLOT)
        # Get the final list of files to put in plots_All.root
        final_files_to_merge = [ histo_source[sample] for sample in ALL_SAMPLES]

        # Make the final output file
        final_file = os.path.join(os.path.dirname(histo_source.values()[0]),
                                  "plots_all.root")
        makefile.write(
            final_file + ": " + " ".join(final_files_to_merge) + "\n")
        makefile.write("\tnice ${HARVEST} %s %s\n\n" %
                       (final_file, " ".join(final_files_to_merge)))

        # Make our analysis target
        makefile.write("analysis: %s" % final_file)


if __name__ == "__main__":
    files1 = [ 'file%i.root' % i for i in range(19) ]
    files2 = glob.glob('/data1/friis/tmp_data/*root')
    buildMakefile([
        ('ztt.root', files1),
        ('att.root', files2)],
        '/tmp/friis',
        'Makefile.harvesting'
    )

