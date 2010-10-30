import itertools
import os
import glob
import hashlib

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
            run_line = '\t${HARVEST} %s %s\n' % (
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

def buildMakefile(merge_jobs, working_dir, makefilename, copy_first=True, merge_per_job=3):
    ''' Build a Makefile to merge DQM histograms

    [merge_jobs] is a list of tuples of the form
    [ (alias1, output1, [input1_1.root, input1_2.root, ... ] ),
      (alias2, output2, [input2_1.root, input2_2.root, ... ] ),
      ...  ]

    [working_dir] is a local directory to store the temporary
    files.  [makefilename] specifies the name of the output
    Makefile
    '''

    # Setup makefile
    makefile = open(makefilename, 'w')
    path_to_harvester_script = os.path.join(
        os.environ['CMSSW_BASE'], 'src', 'TauAnalysis/Configuration/', 
        'python/tools', 'genericHarvester.py')
    makefile.write("# Path to harvester script\n")
    makefile.write("HARVEST = %s\n\n" % path_to_harvester_script)

    makefile.write("# List of targets to merge\n")
    makefile.write("all: %s\n\n" % ' '.join(merge_job[0] for merge_job in merge_jobs if merge_job[2])) 

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

if __name__ == "__main__":
    files1 = [ 'file%i.root' % i for i in range(19) ]
    files2 = glob.glob('/data1/friis/tmp_data/*root')
    buildMakefile([
        ('ztt.root', files1),
        ('att.root', files2)],
        '/tmp/friis',
        'Makefile.harvesting'
    )

