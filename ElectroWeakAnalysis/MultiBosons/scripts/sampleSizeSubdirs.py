#!/usr/bin/env python
"""
Takes a directory on CASTOR hardcoded as the variable `base' and gets the 
dataset size statistics for each of its subdirectories. Assumes that each
subdirectory contains EDM root files. Prints results to STDOUT.
"""
# base = "/castor/cern.ch/user/v/veverka/mc/Spring10/START3X_V26-v1/PAT/"
base = "/castor/cern.ch/user/l/lgray/mc/Spring10/VGammaSkim_v2"
# base = "/castor/cern.ch/user/l/lgray/data/36X/VGammaSkim_v2"

import os
from sampleSize import sampleSize

datasets = os.popen("nsls " + base).read().split()

if base[-1] == "/": separator = ""
else: separator = "/"

for ds in datasets:
  sampleSize(base + separator + ds)
  print "--"

