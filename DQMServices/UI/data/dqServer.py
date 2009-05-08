#!/usr/bin/env python

import sys, os, optparse, SimpleXMLRPCServer
from DBSAPI.dbsException import *
from DBSAPI.dbsApiException import *
from DBSAPI.dbsApi import DbsApi
from DBSAPI.dbsDQFlag import DbsDQFlag
from DBSAPI.dbsRunLumiDQ import DbsRunLumiDQ
from DBSAPI.dbsConfig import DbsConfig

import os, re, string, xml.sax, xml.sax.handler
from xml.sax.saxutils import escape, unescape
from xml.sax import SAXParseException

class DbsDQOptionParser(optparse.OptionParser):
  """
     OptionParser is main class to parse options.
  """

  def __init__(self):

    optparse.OptionParser.__init__(self, usage="%prog --help or %prog --command [options]", version="%prog 0.0.1", conflict_handler="resolve")

    self.add_option("--url=",action="store", type="string", dest="url", help="specify URL, e.g. --url=http://cmssrv17.fnal.gov:8989/DBS/servlet/DBSServlet, If no url is provided default url from dbs.config is attempted")
    #self.add_option("--dataset=",action="store", type="string", dest="dataset", help="specify dataset, e.g. --dataset=/mcTestCeballos_z2jet_VBFHiggsTo2Taugen-alpgen/CMSSW_1_4_6-CSA07-2119/GEN-SIM")
    #self.add_option("--run", action="store", type="int", dest="run", help="specify a valid run number")
    #self.add_option("--tag", action="store", type="string", dest="tag", help="Quality Information Mask")
    #self.add_option("--value", action="store", type="string", dest="value", help="Value can be GOOD, BAD and UNKNOWN or a INTEGER value")

def get_datasets(run):
  datasets = []
  query="find dataset where run=" + str(run) + " and dataset like '%RAW'"
  results = api.executeQuery(query)
  class Handler (xml.sax.handler.ContentHandler):
    def startElement(self, name, attrs):
      if name == 'result':
        datasets.append(str(attrs['PATH']))
  xml.sax.parseString(results, Handler())
  return datasets

def insertdq(run, tags):
  try:

    print "Received: ", run, tags
    
    datasets = get_datasets(run)

    print "Datasets:", datasets

    for t in tags.keys():

      flag = DbsDQFlag (
        Name = t, 
        Value = tags[t]
      )

      run_dq = DbsRunLumiDQ (
        RunNumber = run,
        #LumiSectionNumber=lumisection,
        DQFlagList = [flag]
      )
    
      for dataset in datasets:

        curr = querydq(dataset, run, t)

        print "Run:", run, "Dataset:", dataset, "Tag:", t, "DBS value:", curr, "RR value:", tags[t], "Action:",
        
        if curr == 'NOTFOUND':
          print "insert"
          api.insertRunLumiDQ(dataset, [run_dq])
        else:
          if curr != tags[t]:
            print "update"
            api.updateRunLumiDQ(dataset, [run_dq])
          else:
            print "none"
	  
    return "OK"
		
  except DbsApiException, ex:
		
    msg = "Caught API Exception %s: %s "  % (ex.getClassName(), ex.getErrorMessage() )
	#if ex.getErrorCode() not in (None, ""):
	#  msg += "\nDBS Exception Error Code: ", ex.getErrorCode()
    return msg


def querydq(dataset, run, tag):
	
  run_dq_search_criteria = DbsRunLumiDQ (RunNumber=run)

  try:

    dqHierarchyList =  api.listRunLumiDQ(dataset, runLumiDQList=[run_dq_search_criteria]  )
    for aDQ in dqHierarchyList:
      for aSubDQ in aDQ['DQFlagList']:
        if aSubDQ['Name'] == tag: return aSubDQ['Value']
        for aSubSubDQ in aSubDQ['SubSysFlagList']:
          if aSubSubDQ['Name'] == tag: return aSubSubDQ['Value']
          for abSubSubDQ in aSubSubDQ['SubSysFlagList']:
            if abSubSubDQ['Name'] == tag: return abSubSubDQ['Value']
	
    return 'NOTFOUND'
		
  except DbsApiException, ex:
    msg = "Caught API Exception %s: %s "  % (ex.getClassName(), ex.getErrorMessage() )
    return msg

if __name__ == "__main__":

  optManager  = DbsDQOptionParser()
  (opts, args) = optManager.parse_args()
  opts = opts.__dict__

  if opts['url'] in ('', None, 'BADURL'):
    configDict = DbsConfig(opts)
    opts['url'] = str(configDict.url())

  #if opts['url'] in ('', None, 'BADURL'):
  #  print "You must specify a valid DBS URL, use --url= or --help"
  #  sys.exit(0)

  #if opts['dataset'] in ('', None):
  #  opts['dataset']="/mcTestCeballos_z2jet_VBFHiggsTo2Taugen-alpgen/CMSSW_1_4_6-CSA07-2119/GEN-SIM"

  api = DbsApi(opts)

  #print querydq(46727, "SiStrip_Global")

  server = SimpleXMLRPCServer.SimpleXMLRPCServer(('localhost', 9999))
  server.register_function(insertdq);
  #server.register_function(querydq);
  server.serve_forever()

