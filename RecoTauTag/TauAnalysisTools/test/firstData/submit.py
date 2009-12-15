import os
import sys
import string
from optparse import OptionParser

parser = OptionParser()

parser.add_option("-s", "--source")
parser.add_option("-d", "--destination")
parser.add_option("-c", "--cfg_file")
parser.add_option("-o", "--output_filename")
parser.add_option("-n", "--dry_run", action="store_true", dest="dry_run")

(options, args) = parser.parse_args()

print options

bsub_command_template = string.Template('''
#!/bin/bash
cd $working_dir
eval `scram ru -sh`
cd - 
cmsRun $cfg_file files=$src_file
rfcp $out_file $castor_location
''')

source = __import__(options.source)
file_list = source.source.fileNames.value()

full_cfg_file = os.path.abspath(options.cfg_file)

for index, file in enumerate(file_list):
    print "Submitting:", file,
    indexed_file = options.output_filename
    indexed_file = indexed_file.replace(".root", "_%i.root" % index)
    destination = options.destination + "/" + indexed_file
    to_run = bsub_command_template.substitute(
        working_dir=os.path.dirname(full_cfg_file),
        cfg_file=full_cfg_file,
        src_file = file,
        out_file=options.output_filename,
        castor_location=destination
    )
    temp_file = open('temp.txt', 'w')
    temp_file.write(to_run)
    temp_file.close()
    if options.dry_run is None:
        os.system('bsub -q 8nh < temp.txt')
        print "...submitted"
