import hashlib
import os
import sys
import time
import string
sys.setrecursionlimit(int(1e4))

'''

Tools for splitting up jobs into chunks.

Author: Evan K. Friis, UC Davis

'''


bsub_template = string.Template(
'''#!/bin/csh
#BSUB -J $jobName
#BSUB -q 8nh
$dependencies

echo "Increasing vmem"
limit vmem unlim
echo "Changing directory"
cd $dir
pwd
setenv SCRAM_ARCH $arch
echo "Setting up runtime"
cmsenv
cd -
pwd
echo "Copying input files"
$copy_command
echo "Checking copies succeeded"
ls $inputFileList
if ( "$$status" != 0 ) then
    echo "Failed to copy input files to local area!"
    echo "Expected:"
    echo $inputFileCommand
    echo "Local are:"
    ls -l *.root
    exit 100
endif
echo "Running script"
$merger_comand $outputFileCommand $inputFileCommand
setenv result $$status
echo Job finished with exit code: $$result
if ( "$$result" == 0 ) then
    echo "Removing existing file"
    rfrm $outputFile
    echo "Copying output"
    rfcp $outputFileName $outputFile
endif
exit 0
''')

def hash_files(files, add_time=True):
    " Get the hash of a set of file names "
    hash = hashlib.md5()
    map(hash.update, files)
    if add_time:
        hash.update(time.asctime())
    return hash.hexdigest()[:4]

def make_bsub_script(output_file, input_jobs_and_files,
                     log_dir, merge_method, pass_io_files=True):
    # Create a unique job name for our output file.  We leave the add_time
    # option set to true so that lxbatch jobs always have a unique name.
    # Otherwise when we add dependencies LXB gets confused if there were
    # previous (finished/failed) jobs with the same name.
    job_name = hash_files([output_file])
    job_dependencies = [ jobname for jobname, file in input_jobs_and_files
                        if jobname is not None ]
    dependencies = ''
    if job_dependencies:
        dependencies = "#BSUB -w '"
        # Make sure there is no space in the exit command
        dependencies += " && ".join('done("%s")' % job_name for
                                    job_name in job_dependencies)
        dependencies += "'"

    arch = os.environ['SCRAM_ARCH']
    dir = os.environ['CMSSW_BASE']
    input_files = [file for job, file in input_jobs_and_files]

    log_file = os.path.join(log_dir, job_name + ".log")

    # Copy all the files locally
    #print input_files
    copy_command = '\n'.join('rfcp %s . &' % file for file in input_files)
    copy_command += "\n wait\n"

    outputFileName = os.path.basename(output_file)

    inputFileCommand = " ".join(map(os.path.basename, input_files))
    inputFileList = inputFileCommand # just a list of the files
    outputFileCommand = outputFileName

    # If we customize the cfg to auto load the correct names, we don't need to
    # pass any command line options
    if not pass_io_files:
        inputFileCommand = ""
        outputFileCommand = ""

    # Return a tuple containing the output job name and the text of the script
    # file
    return (job_name, bsub_template.substitute(
        dir=dir,
        jobName=job_name,
        dependencies=dependencies,
        outputFile = output_file,
        inputFileCommand = inputFileCommand,
        inputFileList = inputFileList,
        outputFileCommand = outputFileCommand,
        outputFileName = outputFileName,
        logfile = log_file,
        copy_command = copy_command,
        merger_comand = merge_method,
        arch = arch,
    ))



def split(input_files, threshold, function):
    current_value = 0
    current_list = []
    while input_files:
        current_list.append(input_files.pop(0))
        current_value += function(current_list[-1])
        if current_value > threshold or not input_files:
            yield current_list
            current_value = 0
            current_list = []


def make_merger(name, split, output_dir, filetype=".root"):
    def name_function(njob, nlayer, current_merge):
        " Get a unique name containing a hash of the parent files "
        hash = hashlib.md5()
        map(hash.update, current_merge)
        return "_".join(
            [name, "layer", str(nlayer), "job",
             str(njob), hash.hexdigest()[:3]]) + filetype

    def merge(current_layer, current_merge, merge_stack, output_layers):
        if len(current_merge) < split and merge_stack:
            current_merge.append(merge_stack.pop(0))
            return merge(current_layer, current_merge,
                         merge_stack, output_layers)
        # Check if the current merge is done or we have run out of things to merge
        else:
            output_name = os.path.join(output_dir, name_function(
                len(current_layer), len(output_layers), current_merge))
            current_layer.append((output_name, current_merge))
            # Check if we have more merging to do in this layer
            if merge_stack:
                return merge(current_layer, [], merge_stack, output_layers)
            else:
                # Add the layer to the output layers
                output_layers.append(current_layer)
                # Use the outputs of the latest layer as the new merge stack
                if len(current_layer) == 1:
                    # Final exit
                    return output_layers
                else:
                    return merge(
                        [], [], [out for out, infiles in current_layer],
                        output_layers)
    return merge

def make_merge_dependency_tree(name, input_times_and_files,
                               output_dir, split=5):
    ''' Build a merge tree, where each branch merges [split] input files.
    You must specify the last modified times of all input files.
    Note that this function is path indpendent.
    '''

    # Sort by modified time
    input_times_and_files.sort()

    merger= make_merger(name, split, output_dir)
    result = merger([], [], [file for time, file in input_times_and_files], [])
    return result

if __name__ == "__main__":
    listicle = [ (i, "doot_%i.root" % i) for i in range(60) ]
    output = make_merge_dependency_tree("blah", listicle, "/data1000/friis")
    for i, layer in enumerate(output):
        print "layer", i
        print layer


