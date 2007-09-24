#!/usr/bin/env python

import sys, os
import getopt

import FWCore.ParameterSet.Config as cms
import pickle

from xml.dom.minidom import Document, Element, Text

def pickleParameterSet(parameter_set):
    """

    pickle parameter set

    """

    # load parameter-set
    try:
        process = cms.include(parameter_set)
    except Exception,ex:
        print ''
        print 'ParameterSet: ',parameter_set,'could not be converted to python dictionary, error msg:',str(ex)
        print ''
        sys.exit(1)

    try:
        relValPSet = getattr(process,'ReleaseValidation')
    except:
        print 'Parameter-Set:',parameter_set,'is missing the ReleaseValidation PSet'
        print ''
        print 'Please add a PSet named ReleaseValidation defining the following parameters:'
        print ''
        print 'untracked PSet ReleaseValidation = {'
        print '  untracked uint32 totalNumberOfEvents = 1000'
        print '  untracked uint32 eventsPerJob        = 25'
        print '  untracked string primaryDatasetName  = \'RelValExample\''
        print '}'
        sys.exit(1)

    try:
        relValTotEvents = getattr(relValPSet,'totalNumberOfEvents')
    except:
        print 'Parameter-Set:',parameter_set,'is missing the ReleaseValidation PSet'
        print ''
        print 'Please add a PSet named ReleaseValidation defining the following parameters:'
        print ''
        print 'untracked PSet ReleaseValidation = {'
        print '  untracked uint32 totalNumberOfEvents = 1000'
        print '  untracked uint32 eventsPerJob        = 25'
        print '  untracked string primaryDatasetName  = \'RelValExample\''
        print '}'
        sys.exit(1)

    try:
        relValTotEvents = getattr(relValPSet,'totalNumberOfEvents')
    except:
        print 'Parameter-Set:',parameter_set,'is missing the ReleaseValidation PSet'
        print ''
        print 'Please add a PSet named ReleaseValidation defining the following parameters:'
        print ''
        print 'untracked PSet ReleaseValidation = {'
        print '  untracked uint32 totalNumberOfEvents = 1000'
        print '  untracked uint32 eventsPerJob        = 25'
        print '  untracked string primaryDatasetName  = \'RelValExample\''
        print '}'
        sys.exit(1)

    try:
        relValTotEvents = getattr(relValPSet,'eventsPerJob')
    except:
        print 'Parameter-Set:',parameter_set,'is missing the ReleaseValidation PSet'
        print ''
        print 'Please add a PSet named ReleaseValidation defining the following parameters:'
        print ''
        print 'untracked PSet ReleaseValidation = {'
        print '  untracked uint32 totalNumberOfEvents = 1000'
        print '  untracked uint32 eventsPerJob        = 25'
        print '  untracked string primaryDatasetName  = \'RelValExample\''
        print '}'
        sys.exit(1)

    try:
        relValTotEvents = getattr(relValPSet,'primaryDatasetName')
    except:
        print 'Parameter-Set:',parameter_set,'is missing the ReleaseValidation PSet'
        print ''
        print 'Please add a PSet named ReleaseValidation defining the following parameters:'
        print ''
        print 'untracked PSet ReleaseValidation = {'
        print '  untracked uint32 totalNumberOfEvents = 1000'
        print '  untracked uint32 eventsPerJob        = 25'
        print '  untracked string primaryDatasetName  = \'RelValExample\''
        print '}'
        sys.exit(1)

    # pickle parameter-set by replacing cfg extension with pkl extension
    file = open(parameter_set.replace('.cfg','.pkl'),'w')
    pickle.dump(process,file)
    file.close()

def main(argv) :
    """
    
    prepareRelVal
    
    - read in RelVal sample parameter-set list from text file, each parameter-set contains RelVal PSet
    - prepare pickle files from RelVal samples
    - requires:
      - setup'ed CMSSW user project area
      - checkout of Configuration/ReleaseValidation
    - has to be executed in Configuration/ReleaseValidation/data
    - output
      - workflow to be injected into RelValInjector

    required parameters
    --samples      <textfile>   : list of RelVal sample parameter-sets in plain text file, one sample per line, # marks comment
    --cms-path     <path>       : path to CMS software area
    
    optional parameters         :
    --help (-h)                 : help
    --debug (-d)                : debug statements
    
    
    """

    # default
    try:
        version = os.environ.get("CMSSW_VERSION")
    except:
        print ''
        print 'CMSSW version cannot be determined from $CMSSW_VERSION'
        sys.exit(2)

    try:
        architecture = os.environ.get("SCRAM_ARCH")
    except:
        print ''
        print 'CMSSW architecture cannot be determined from $SCRAM_ARCH'
        sys.exit(2)

    samples      = ''
    debug        = 0
    cms_path     = ''

    try:
        opts, args = getopt.getopt(argv, "", ["help", "debug", "samples=", "cms-path="])
    except getopt.GetoptError:
        print main.__doc__
        sys.exit(2)

    # check command line parameter
    for opt, arg in opts :
        if opt == "--help" :
            print main.__doc__
            sys.exit()
        elif opt == "--debug" :
            debug = 1
        elif opt == "--samples" :
            samples = arg
        elif opt == "--cms-path" :
            cms_path = arg

    if samples == '' or cms_path == '':
        print main.__doc__
        sys.exit(2)

    # read in samples
    cfgs = []
    try:
        file = open(samples)
    except IOError:
        print 'file with list of parameter-sets cannot be opened!'
        sys.exit(1)
    for line in file.readlines():
        line = line.strip()
        if len(line) > 0 and line[0] != '#':
            if len(line.strip().split()) > 1 :
                print 'Please specify only one parameter-set per line, following line has more than one:',line
                sys.exit(1)
            else :
                cfgs.append(line.strip())

    # pickle parameter-sets
    for cfg in cfgs:
        print 'Store python representation of parameter-set:',cfg,'in pickle file',cfg.replace('.cfg','.pkl')
        pickleParameterSet(cfg)

    # write workflow xml
    try:
        topNode = Element("RelValSpec")

        for cfg in cfgs:
            element = Element("RelValTest")
            element.setAttribute("Name", cfg.replace('.cfg',''))
            
            PickleFileElement = Element("PickleFile")
            PickleFileElement.setAttribute("Value", os.path.join(os.getcwd(), cfg.replace('.cfg','.pkl')))
            element.appendChild(PickleFileElement)
            
            PickleFileElement = Element("CMSPath")
            PickleFileElement.setAttribute("Value", cms_path)
            element.appendChild(PickleFileElement)
            
            PickleFileElement = Element("CMSSWArchitecture")
            PickleFileElement.setAttribute("Value", architecture)
            element.appendChild(PickleFileElement)
            
            PickleFileElement = Element("CMSSWVersion")
            PickleFileElement.setAttribute("Value", version)
            element.appendChild(PickleFileElement)
            
            topNode.appendChild(element)
    
        doc = Document()
        doc.appendChild(topNode)
        handle = open("relval_workflows.xml", "w")
        handle.write(doc.toprettyxml())
        handle.close()
    except Exception,ex:
        print 'Preparation of ProdAgent workflow: relval_workflows.xml failed with message:',ex
        sys.exit(1)
        
    print ''
    print 'Prepared ProdAgent workflow: relval_workflows.xml to be injected into the RelValInjector by:'
    print 'python2.4 publish.py RelValInjector:Inject',os.path.join(os.getcwd(), 'relval_workflows.xml')

if __name__ == '__main__' :
    main(sys.argv[1:])
