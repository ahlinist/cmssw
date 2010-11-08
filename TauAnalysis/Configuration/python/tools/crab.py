from xml.dom import minidom
import os
import glob

'''

Tools for interfacing the TauAnalysis software with CRAB

Author: Evan K. Friis (UC Davis)

'''

def lfns(crab_dir):
    ''' Return the valid output LFNS for a crab job. Sorted by modified time '''
    job_xml_files = [(os.path.getmtime(file), file) for file in
                     glob.glob(os.path.join(crab_dir, 'res', '*.xml'))]
    job_xml_files.sort()
    count = 0
    for mtime, job_xml in job_xml_files:
        job_report = minidom.parse(job_xml)
        framework_report = job_report.firstChild

        # Check to make sure the job completed successfully
        exit_statuses = framework_report.getElementsByTagName("FrameworkError")
        exit_status = {}
        for exit_code in exit_statuses:
            exit_status[exit_code.getAttribute("Type")] = \
                    int(exit_code.getAttribute("ExitStatus"))
        isOk = True
        for key, value in exit_status.iteritems():
            if value:
                isOk = False
                print "Skipping %s - status: %s = %s" % (file, key, value)
        if not isOk:
            continue

        outputFiles = framework_report.getElementsByTagName("AnalysisFile")
        file_lfn = outputFiles[0].getElementsByTagName("LFN")[0]
        file = file_lfn.getAttribute("Value")
        yield file
        if count > 2:
            # For debugging
            pass
            #break
        count += 1

def map_lfn_to_castor(lfn):
    return '/castor/cern.ch' + lfn

def map_lfns_to_castor(lfns):
    for lfn in lfns:
        yield map_lfn_to_castor(lfn)

if __name__ == "__main__":
    path = os.path.join(
        '/afs/cern.ch/user/f/friis/scratch0/',
        'HiggsAnalysis38/src/TauAnalysis/Configuration/test',
        'crab',
        'crabdir_runAHtoMuTau_AHtoMuTau_Zmumu_Run10')
    for lfn in map_lfns_to_castor(lfns(path)):
        print lfn


