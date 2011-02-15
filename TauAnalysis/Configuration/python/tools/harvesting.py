'''

Tools for managing harvesting of TauAnalysis plots.

Author: Evan K. Friis, UC Davis

'''

import os
import TauAnalysis.Configuration.tools.castor as castor
import TauAnalysis.Configuration.tools.crab as crab
import re

crab_extractor = re.compile(
    "(?P<prefix>\w*?)_(?P<job>\d*)_(?P<retry>\d*)_(?P<code>[0-9a-zA-Z]*).root")

def get_crab_info(file_name):
    " Extract crab information from a file name "
    file_name = os.path.basename(file_name)
    match = crab_extractor.match(file_name)
    if not match:
        raise ValueError("Can't parse crab id from file", file_name)
    return match.groupdict()

def get_crab_id(file_name):
    " Get a unique tuple for a given crab file - (job, retry, code) "
    crab_info = get_crab_info(file_name)
    return tuple([crab_info[crabitem] for crabitem in ['job', 'retry', 'code']])

def castor_source(directory):
    " Build a generator that lists file in a castor directory, sorted by time "
    print "<castor_source>", directory
    # First sort by time
    files = list(castor.nslsl(directory))
    # Sort by time
    files.sort(key = lambda x: x['time'])
    for file_info in files:
        if not file_info['size']:
            print "Warning <castor_source>: file %s has size 0" % \
                    file_info['path']
        yield file_info

def clean_by_crab_id(file_infos):
    ''' Given a list of files, determine any overlapping crab_ids
    and take only the newest files
    '''
    good_ids = {}
    for file_info in file_infos:
        crab_info = get_crab_info(file_info['path'])
        file_id = (
            crab_info['prefix'],
            int(crab_info['job'])
        )
        if file_id not in good_ids:
            good_ids[file_id] = file_info
        else:
            # Overlap! check which one is newer!
            print "crab file overlap!", \
                    good_ids[file_id]['file'], \
                    "(size: %s)" % good_ids[file_id]['size'], \
                    file_info['file'], \
                    "(size: %s)" % file_info['size'], \
                    " taking newer!"
            if file_info['time'] > good_ids[file_id]['time']:
                good_ids[file_id] = file_info
                print "took", good_ids[file_id]['file']

    # Return the clean files sorted by time.
    return sorted(good_ids.values(), key = lambda x: x['time'])

def crabdir_source(directory):
    print "Getting list of files from crab dir:", directory
    crab_files = list(crab.map_lfns_to_castor(crab.lfns(directory)))
    good_ids = set(get_crab_id(file) for file in crab_files)
    # Get good crab 'ids' - a tuple of the crab job, retry, and random code
    # Figure out what castor directory we are in so we can get all the
    # information.
    print "getting all files"
    if crab_files:
        castor_dir = os.path.dirname(crab_files[0]) + '/'
        print castor_dir
        castor_files_info = castor.nslsl(castor_dir)
        for file_info in castor_files_info:
            if get_crab_id(file_info['file']) in good_ids:
                yield file_info

def crabdir_sources(*directories):
    for directory in directories:
        for file_info in crabdir_source(directory):
            yield file_info

