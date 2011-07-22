#!/usr/bin/env python
import getpass
import os
import re
import sys

from optparse import OptionParser

parser = OptionParser(description = "%prog : Parse the output of multicrab -status, and produce various reports.",
    usage = "parseMulticrabStatus.py [status.out]")
(options, args) = parser.parse_args()

if len(args) == 0:
    source = sys.stdin
elif len(args) == 1 and args[0] == "-":
    source = sys.stdin
elif len(args) == 1 and args[0] != "-":
    source = open(args[0], "r")
else:
    parser.print_help()
    sys.exit(1)

multiStatus = source.read()
allJobsOk = []
toPublish = []
allPublished = []

for text in re.split(r"crab:  Version \d+\.\d+\.\d+ running on", multiStatus):
    ## Get the dataset name
    m = re.search(r"\tworking directory\s+(/.+)", text)
    if not m: continue
    path = m.groups()[0]
    head, dataset = os.path.split(path)
    if dataset == "":
        head, dataset = os.path.split(head)

    ## Get the total number of jobs
    m = re.search(r"crab:   (\d+) Total Jobs", text)
    if not m:
        raise RuntimeError, 'Couldn\'t parse "Total Jobs"'
    totalJobs = int(m.groups()[0])

    toResubmit = []
    exitCodeSummary = {}
    pattern = r""">>>>>>>>> (\d+) Jobs with Wrapper Exit Code : (\d+).*
.*List of jobs: (\d.*)\s
"""
    for nJobs, exitCode, jobList in re.findall(pattern, text):
        exitCodeSummary[exitCode] = (int(nJobs), jobList)
        if int(exitCode) != 0:
            if not toResubmit: print "##", dataset
            toResubmit.append(jobList)
            print "##   %s exit code %s: %s" % (nJobs, exitCode, jobList)

    if "0" in exitCodeSummary.keys() and totalJobs == exitCodeSummary["0"][0]:
        allJobsOk.append(dataset)

    pattern = r""" >>>>>>>>> (\d+) Jobs Aborted
\s*You can resubmit them specifying JOB numbers: crab -resubmit <List of jobs>
\s*List of jobs: (\d.*)\s
"""
    for nJobs, jobList in re.findall(pattern, text):
        if not toResubmit: print "##", dataset
        toResubmit.append(jobList)
        print "##   %s aborted: %s" % (nJobs, jobList)

    if toResubmit:
        version = 1
        while os.path.exists(os.path.join(dataset, "log/resubmit_%d.out" % version)):
            version = version + 1
        log = os.path.join(dataset, "log/resubmit_%d.out" % version)
        print """nohup crab -c %s \\
        -resubmit %s \\
        >& %s &
        """ % (dataset, ",".join(toResubmit), log)

    ## Get the working directory
    workdirRE = re.compile(r"^\s*working directory\s+(/.+)", re.MULTILINE)
    m = re.search(workdirRE, text)
    if m: workdir = m.groups()[0]
    if os.path.exists(os.path.join(workdir, "log", "crab.log")):
        log = file(os.path.join(workdir, "log", "crab.log"), "r").read()
        m = re.search(r"-CMSSW.datasetpath=(/\S+)\s", log)
        if m: datasetpath = m.groups()[0].split("/")[1]
        else:
            print "Error parsing", os.path.join(workdir, "log", "crab.log")
            continue
        m = re.search(r"-USER.publish_data_name=(\S+)\s", log)
        if m: publishDataName = m.groups()[0]
        else:
            print "Error parsing", os.path.join(workdir, "log", "crab.log")
            continue
        pattern = "/" + datasetpath + "/" + getpass.getuser() + "-" + publishDataName + "-*" + "/USER"
        dbsCommand = """dbs search \
        --url=http://cmsdbsprod.cern.ch/cms_dbs_ph_analysis_01/servlet/DBSServlet \
        --query="find dataset where dataset like %s" | grep USER
        """ % pattern
        dbsNames = os.popen(dbsCommand).read().split()
        if len(dbsNames) == 0:
            print "## %s not found in DBS under %s" % (dataset, pattern)
        elif len(dbsNames) == 1:
            dbsName = dbsNames[0]
        else:
            print "## Looking for %s found %d instances matching %s" % (dataset, len(dbsNames), pattern)

        dbsCommand = """dbs search \
        --url=http://cmsdbsprod.cern.ch/cms_dbs_ph_analysis_01/servlet/DBSServlet \
        --query="find file where dataset like %s" | grep root | wc -l
        """ % pattern

        publishedJobs = int(os.popen(dbsCommand).read())
        if dataset in allJobsOk:
            if publishedJobs < totalJobs:
                toPublish.append(dataset)
                print "## %s ready to publish!" % dataset
            elif publishedJobs == totalJobs:
                allPublished.append(dataset)
                print "## %s all published under" % dataset
                print "##   %s" % pattern
            else:
                print "## WARNING: %s: published %d of %d jobs!" % (dataset, publishedJobs, totalJobs)

## Summary
toPublish.sort()
print "## Ready to publish (all jobs retrieved with exit status 0)"
for name in toPublish:
    if name in allPublished: continue
    ## Check the number of published files
    version = 1
    while os.path.exists(os.path.join(name, "log/publish_%d.out" % version)):
        version = version + 1
    log = os.path.join(name, "log/publish_%d.out" % version)
    print "nohup crab -publish -c %s >& %s &" % (name, log)

print "############################################################################"
print "## All jobs published"
allPublished.sort()
for name in allPublished:
    print "##    ", name

if __name__ == "__main__": import user
