#!/usr/bin/env python

import os
import subprocess
import time
import csv
import glob

from RecoLuminosity.LumiDB import argparse

if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description = "Compute total luminosity of CRAB jobs given a list of"
        " crab directories and an HLT path"
    )
    parser.add_argument('--crabdirs', metavar='dir', nargs='+',
                        help='List of CRAB directories')
    parser.add_argument('--hltpath', dest='hltpath', nargs='+', default='')
    parser.add_argument('--noupdate', action='store_true',
                        help="Don't run crab -report",
                        dest='noupdate', default=False)

    options=parser.parse_args()

    dirs = options.crabdirs

    if not options.noupdate:
        print "Getting JSON report for all CRAB directories"
        for dir in dirs:
            subprocess.call(["crab", "-report", "-c", "%s" % dir])

    print "Submitting lumiCalc.py requests"
    requests = []
    for dir in dirs:
        for hlt in options.hltpath:
            output_file_name = os.path.join(dir, 'lumiCalc_%s.csv' % hlt)
            command = ['lumiCalc.py',
                       '-c', 'frontier://LumiProd/CMS_LUMI_PROD',
                       '-i', os.path.join(dir, 'res', 'lumiSummary.json'),
                       '--nowarning',
                       'recorded',
                       '-o', output_file_name
                      ]
            command.append('-hltpath')
            command.append(hlt)
            #print command
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
        lumi_csvs = glob.glob(os.path.join(dir, '*.csv'))
        print "o Found lumi files", lumi_csvs
        sample_lumi = 0.0
        n_lumi_files = 0
        for file in lumi_csvs:
            file_lumi = 0.0
            n_lumi_files += 1
            reader = csv.DictReader(open(file, 'r'))
            for row in reader:
                if row['recorded'] != "N/A":
                    lumi = float(row['recorded'])
                    file_lumi += lumi
            print "o File", file, "has lumi", file_lumi
            sample_lumi += file_lumi
        avg_lumi = sample_lumi/n_lumi_files
        total_lumi += avg_lumi
        print "o Dir:", dir, "   LUMI: ", sample_lumi, "/", n_lumi_files, " = ", avg_lumi
    print "TOTAL INTEGRATED LUMINOSITY: ", total_lumi, " MICROBARNS"

