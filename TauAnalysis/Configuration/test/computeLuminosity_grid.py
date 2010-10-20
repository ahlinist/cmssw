#!/usr/bin/env python

import os
import subprocess
import time
import csv

from RecoLuminosity.LumiDB import argparse

if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description = "Compute total luminosity of CRAB jobs given a list of"
        " crab directories and an HLT path"
    )
    parser.add_argument('crabdirs', metavar='dir', nargs='+',
                        help='List of CRAB directories')
    parser.add_argument('--hltpath', dest='hltpath', nargs=1, default='')

    options=parser.parse_args()

    dirs = options.crabdirs

    print "Getting JSON report for all CRAB directories"
    for dir in dirs:
        subprocess.call(["crab", "-report", "-c", "%s" % dir])

    print "Submitting lumiCalc.py requests"
    requests = []
    for dir in dirs:
        output_file_name = os.path.join(dir, 'lumiCalc.csv')
        command = ['lumiCalc.py',
                   '-c', 'frontier://LumiProd/CMS_LUMI_PROD', 
                   '-i', os.path.join(dir, 'res', 'lumiSummary.json'),
                   '--nowarning', 
                   'recorded', 
                   '-o', output_file_name
                  ]
        if options.hltpath:
            command.append('-hltpath')
            command.append(options.hltpath[0])
        print command
        proc = subprocess.Popen(
            command,
            stdout=subprocess.PIPE)
        requests.append(proc)

    while any(proc.poll() is None for proc in requests):
        print "Waiting for lumiCalc requests..."
        time.sleep(3)
    

    # Parse the lumiCalc output
    total_lumi = 0.0
    for dir in dirs:
        print "Finding lumi for", dir
        sample_lumi = 0.0
        reader = csv.DictReader(open(os.path.join(dir, 'lumiCalc.csv')))
        for row in reader:
            if row['recorded'] != "N/A":
                lumi = float(row['recorded'])
                sample_lumi += lumi
        print "Dir:", dir, "   LUMI: ", sample_lumi
        total_lumi += sample_lumi
    print "TOTAL INTEGRATED LUMINOSITY: ", total_lumi, " MICROBARNS"

