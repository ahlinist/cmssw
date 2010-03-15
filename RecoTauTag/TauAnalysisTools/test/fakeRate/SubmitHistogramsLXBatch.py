#!/usr/bin/env python

'''

Script to submit histogram production to LXBatch

Author: Evan Friis

'''

import os
import string
import subprocess
from sources import sources

samples_to_submit = sources.keys()

cmssw_base = os.environ['CMSSW_BASE']
working_dir = os.path.join(cmssw_base, 'src/RecoTauTag/TauAnalysisTools/test/fakeRate')

job_template = string.Template('''
#!/bin/bash
cd $working_dir
eval `scram ru -sh`
./batch_histograms.py -n $maxJobs -j $jobID -s $sample
''')

jobs_per_sample=50

for sample in samples_to_submit:
    for jobID in range(jobs_per_sample):
        job = job_template.substitue(working_dir=working_dir,
                                     maxJobs=jobs_per_sample,
                                     jobID=jobID, 
                                     sample=sample)
        temp_job_path = os.path.join('/tmp', "%s_%i.sh" % (sample, jobID))
        temp_job_file = file(
            temp_job_path, 'w')
        temp_job_file.write(job)
        temp_job_file.close()
        job_proc = subprocess.Popen(['bsub', '-q', '8nh', '<', temp_job_path])

