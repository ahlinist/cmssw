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
    skipped_files = 0
    for mtime, job_xml in job_xml_files:
        #print "parsing", job_xml
        try:
            job_report = minidom.parse(job_xml)
        except:
            print "Couldn't parse xml file %s, skipping!!!" % job_xml
            continue
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
                print "Skipping %s - status: %s = %s" % (job_xml, key, value)
        if not isOk:
            continue

        edmFiles = framework_report.getElementsByTagName("File")
        outputFiles = framework_report.getElementsByTagName("AnalysisFile")
        files = []
        if outputFiles:
            file_name = outputFiles[0].getElementsByTagName("LFN")[0].\
                getAttribute("Value")
            #print "Adding %s plot file" % (file_name)
            files.append(file_name)
        if edmFiles:
            edmFile = edmFiles[0].getElementsByTagName("LFN")[0].firstChild.data.strip()
            edmEvents = int(edmFiles[0].getElementsByTagName(
                "TotalEvents")[0].firstChild.data)
            # Keep all, so we do skim eff correctly.
            files.append(edmFile)
        # Yield all files
        for file in files:
            yield file
        if count > 2:
            # For debugging
            pass
            #break
        count += 1
    print "Parsed %i files in %s, and skipped %i since they had no events." % (
        count, crab_dir, skipped_files)

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
        'crabdir_runAHtoMuTau_A100_Run23')
    for lfn in map_lfns_to_castor(lfns(path)):
        print lfn


