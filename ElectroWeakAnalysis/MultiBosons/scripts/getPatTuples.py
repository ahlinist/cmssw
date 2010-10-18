#!/usr/bin/env python

import os
import shutil
import glob
import sys
from optparse import OptionParser
try:
    import multiprocessing
    from Queue import Empty
    use_multiprocessing = True
except ImportError:
    use_multiprocessing = False

parser = OptionParser()
options, args = parser.parse_args()

if len(args) != 2:
    print "Please provide a source directory of text files with locations and "
    print "a destination directory"
    exit
source_dir = os.path.realpath(os.path.expanduser(args[0]))
destination_dir = os.path.realpath(os.path.expanduser(args[1]))
sourcename = os.path.basename(source_dir)

def plotting_worker(queue):
    while True:
        try:
            command_to_process = queue.get(timeout=5)
        except (Empty, IOError):
            break
        os.system(command_to_process)
        queue.task_done()

shcommands = []
for listfile in [x for x in os.listdir(source_dir) if x.endswith('.txt')]:
    samplename = listfile[:-4]
    print samplename
    sampletype = samplename[samplename.rfind('-') + 1:]
    sample_destination_dir = os.path.join(destination_dir, sourcename,
                                          samplename)
    if not os.path.exists(sample_destination_dir):
        os.makedirs(sample_destination_dir)
    for line in open('%s/%s' % (source_dir, listfile), 'r').readlines():
        rootfile = line[:-1]
        destination_path = os.path.join(sample_destination_dir,
                                        os.path.basename(rootfile))
        if os.path.exists(destination_path):
            print destination_path, "already exists, skipping..."
        else:
            shcommands.append('rfcp %s %s' %
                              (rootfile, destination_path))

if use_multiprocessing:
    queue = multiprocessing.JoinableQueue()
    for command in shcommands:
        queue.put(command)
    for i in range(4):
        p = multiprocessing.Process(target=plotting_worker,
                                    args=(queue,), name="worker-%i"%i)
        p.daemon = True
        p.start()
    queue.join()
else:
    for command in shcommands:
        os.system(command)
