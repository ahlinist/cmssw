#!/usr/bin/env python

import urllib,urllib2

class DBSFetch:
    base_files = "http://cmsweb.cern.ch/dbs_discovery/getLFNsForSite?"
    base_dataset = "http://cmsweb.cern.ch/dbs_discovery/aSearch?"

    @staticmethod
    def getDataset(searchstring,**kwargs):
        searchstring=searchstring.strip()
        print "Looking for datasets matching %s" % (searchstring)
        opt = {
            "dbsInst":"cms_dbs_prod_global",
            "caseSensitive":"on",
            "method":"dbsapi",
            "html":"0",
            "userInput":searchstring,
            "details":"0",
            "pagerStep":"-1",
            "cff":"1"
            }
        for key,value in kwargs.iteritems():
            opt[key]=value

        optstring = urllib.unquote(urllib.urlencode(opt))

        response = urllib2.urlopen(DBSFetch.base_dataset+optstring)
        datasetlist = response.read().strip().split('\n')
        return datasetlist[2:]

    @staticmethod
    def auto(searchstring):
        results = DBSFetch.getDataset(searchstring)
        if len(results)==1:
            print "Search found one matching dataset, %s" % (results[0])
            return DBSFetch.getFiles(results[0])
        else:
            print "Search returned multiple results"
            for i,r in enumerate(results):
                print "[%s] %s" % (i,r)
            user=""
            while not (user.isdigit() and int(user)<len(results)):
                user=raw_input(">")
            filelist = DBSFetch.getFiles(results[int(user)])
            return filelist
                
    @staticmethod
    def getFiles(dataset,**kwargs):
        dataset = dataset.strip()
        print "Getting file list for dataset %s" % (dataset)
        opt = {
            "dbsInst":"cms_dbs_prod_global",
            "site":"None",
            "datasetPath":dataset,
            "what":"txt"
            }
        for key,value in kwargs.iteritems():
            opt[key]=value

        optstring = urllib.unquote(urllib.urlencode(opt))
        response = urllib2.urlopen(DBSFetch.base_files+optstring)
        filelist = response.read().strip().split('\n')
        return filelist

    #__call__ = staticmethod(__call__)
    #getFiles = staticmethod(getFiles)
    #getDataset = staticmethod(getDataset)


if __name__=='__main__':
    import sys
    if len(sys.argv)==2:
        DBSFetch.auto(sys.argv[1])
    else:
        print "Please enter a search term or dataset name"
        search = raw_input(">")
        DBSFetch.auto(search)
        
