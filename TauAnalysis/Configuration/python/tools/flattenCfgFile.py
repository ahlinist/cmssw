#!/usr/bin/env python

''' 
Given a CMSSW cfg.py file as input, load the <process> object, and dump the
flattened python via dumpPython() to the specified output file

Author: Evan K. Friis, UC Davis

'''

import sys
import os

if __name__ == "__main__":
    file_to_dump = sys.argv[1]
    output_file = sys.argv[2]
    path_to_file = os.path.dirname(os.path.abspath(file_to_dump))
    # Add the directory containing the file to the PYTHON_PATH
    sys.path.append(path_to_file)
    cfg_module_name = os.path.basename(file_to_dump).replace('.py', '')
    __import__(cfg_module_name)
    cfg = sys.modules[cfg_module_name]
    output = open(output_file, 'w')
    output.write(cfg.process.dumpPython())
    output.close()
