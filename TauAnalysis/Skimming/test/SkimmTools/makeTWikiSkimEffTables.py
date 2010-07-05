#!/usr/bin/env python

import sys, re, os
    
##############
def getEffFromTrigReport(selectionPath,skimType,fileList,opt):
    NEvents = 0
    NPassedAll = 0
    ### Init counters ###
    NPassedPath = [0] * 10
    filters = []
    #########################
    counterPrefix = skimType
    counterPathName = counterPrefix+"Counter"
    counterName = counterPrefix+"Filter"
    #########################
    fullEntryPattern = re.compile('------ Modules in End-Path: '+counterPathName+' ------------[\t\n\r\f\v\W\w]+TrigReport ---------- Module')   
    ppat = 'TrigReport [\W\w]+'
    p = []
    p.append(re.compile(ppat))
    ########################
    fullEntryPatternPath = re.compile('TrigReport ---------- Modules in Path: '+selectionPath+' ------------(\nTrigReport [\ \# \w]+)+')
    ppat = 'TrigReport [\W\w]+ '
    pPath = []
    ########################
    # Loop over all files
    if(len(fileList)<1):
        print "No crab output files present!"
        return
    for filen in fileList:
        input = file(filen)
        fullTxt = input.read() # read full txt
        ######Total efficiency        
        m0 = fullEntryPattern.search(fullTxt) # extract region
        if (m0):
            lines = re.split('\n',m0.group())
            for line in lines:
                if len(line.split())==8 and line.split()[1]=='0':
                    expr = re.compile(ppat+line.split()[7])
                    for i in range(0,len(p)):
                        m = p[i].search(line)
                        if (m):
                            tabs = re.split('[\s]+',m.group())
                            if tabs[7]==counterName:
                                NEvents = NEvents + int(tabs[3])
                                NPassedAll = NPassedAll + int(tabs[4])        
        #############################################
        ####Efficiency for each filter       
        m1 = fullEntryPatternPath.search(fullTxt) # extract region
        if (m1):
            lines = re.split('\n',m1.group())
            for line in lines:
                if len(line.split())==8 and line.split()[1]=='1':
                    expr = re.compile(ppat+line.split()[7])
                    if pPath.count(expr)==0:
                       pPath.append(expr)
                       filters.append(line.split()[7])
                for i in range(0,len(pPath)):
                    m = pPath[i].search(line)
                    if (m):
                        tabs = re.split('[\s]+',m.group())
                        NPassedPath[i] = NPassedPath[i] + int(tabs[4])
        #############################################                        
        input.close()


#####
    outputString = ""
    if(opt==0):
        outputString += str(NEvents)+"</td> <td>"+str(NPassedAll)+"</td> <td>"+'%5.3f'%(float(NPassedAll)/NEvents)+"</td>"
        return outputString

    outputString += "       "+selectionPath+" Efficiencies"
    outputString += "<table border=\"1\" bgcolor=\"black\"> <tbody> "
    outputString += "<tr> "
    outputString +="<td>Filter Passed</td> <td>Passed</td> <td>Efficiency</td> <td>Cumul.Efficiency</td> "
    outputString += "</tr> "
    eff = -1.
    # Treat first entry
    if (float(NEvents!=0)):
        eff = float(NPassedPath[0])/float(NEvents)
    print '%24s: %8d     %5.3f       %5.3f'%(filters[0],NPassedPath[0],eff,float(NPassedPath[0])/float(NEvents))
    outputString += "<tr> "
    outputString+="<td>"+filters[0]+"</td> <td>"+str(NPassedPath[0])+"</td> <td>"+'%5.3f'%(eff)+"</td> <td>"+'%5.3f'%(float(NPassedPath[0])/float(NEvents))+"</td> "
    outputString += "</tr> "    
    for i in range(1,len(filters)):
        if (float(NPassedPath[i-1]==0)):
            eff = -1.
        else:
            eff = float(NPassedPath[i])/float(NPassedPath[i-1])
        print '%24s: %8d     %5.3f       %5.3f'%(filters[i],NPassedPath[i],eff,float(NPassedPath[i])/float(NEvents))
        outputString += "<tr> "
        outputString+="<td>"+filters[i]+"</td> <td>"+str(NPassedPath[i])+"</td> <td>"+'%5.3f'%(eff)+"</td> <td>"+'%5.3f'%(float(NPassedPath[i])/float(NEvents))+"</td> "
        outputString += "</tr> "
    print '---------------------------------------------------------------'
    last = len(filters)-1
    print '\n*** Total events processed/passed: %8d %8d'%(NEvents,NPassedAll)

    outputString +="</tbody> </table>"
    return outputString
#####
    
    exit
##################
##################
def getReportForSample(path,skimType):
    for crabDir in os.listdir(path):
        if re.search("crab_0_",crabDir)!=None:
            print "----------------------------------------------"
            print "Checking: ",crabDir
        ##############
            crabLogPath = path+crabDir+"/log/crab.log"
            crabLog = open(crabLogPath,"r")
            publishName=""
            dashboardLink=""
            for line in crabLog:
                if re.search("<User Dataset Name>",line)!=None and re.search(skimType,line)!=None:
                    publishName = line.split("=")[1].rstrip("\n")
                if re.search("CMS Dashboard:",line)!=None:
                    dashboardLink = line.split(" ")[2].rstrip("\n")
            crabCfgPath = path+crabDir+"/share/crab.cfg"
            crabCfg = open(crabCfgPath,"r")
            pset = ""
            datasetpath = ""
            storage_element = ""
            publish_data_name = ""
            for line in crabCfg:
                if re.search("pset",line)!=None:
                    pset = line.split("=")[1].rstrip("\n")
                if re.search("datasetpath",line)!=None:
                    datasetpath = line.split("=")[1].rstrip("\n")
                if re.search("storage_element",line)!=None:
                    storage_element = line.split("=")[1].rstrip("\n")
                if re.search("publish_data_name",line)!=None:
                    publish_data_name = line.split("=")[1].rstrip("\n")
                if re.search("dbs_url_for_publication",line)!=None:
                    tmp = line.split("=")[1].rstrip("\n")
                    dbsInst = tmp.split("/")[3]
                    tmp1 = dbsInst[:dbsInst.rfind("_writer")]
                    dbsInst = tmp1
            print "datasetpath: ",datasetpath," pset: ",pset," SE: ",storage_element
            print "publish_data_name: ",publish_data_name
            dbsDatasetName="/"+datasetpath.split("/")[1]+"/akalinow-"+publish_data_name
            print "DBS Instance: ",dbsInst
            print "DBS Link: ","https://cmsweb.cern.ch/dbs_discovery/aSearch?caseSensitive=on&userMode=user&sortOrder=desc&sortName=&grid=1&method=dbsapi&dbsInst="+dbsInst+"&userInput=find+dataset+where+dataset+like+*"+dbsDatasetName+"*"+skimType+"*+and+dataset.status+like+VALID*"
        ##############
            selectionPaths = ['']
            if  re.search("muTau",skimType)!=None:
                selectionPaths = ['muPFTauSkimPath','muCaloTauSkimPath']    
            if  re.search("diTau",skimType)!=None:
                selectionPaths = ['caloTauSkimPath','pfTauSkimPath','mixedTauSkimPathI','mixedTauSkimPathII']
            if  re.search("elecTau",skimType)!=None:
                selectionPaths = ['elecPFTauSkimPath','elecCaloTauSkimPath']
            if  re.search("elecMu",skimType)!=None:
                selectionPaths = ['elecMuSkimPath']
            if  re.search("wTauNu",skimType)!=None:
                selectionPaths = ['PFTauNuSkimPath','CaloTauNuSkimPath']
                                            
            fileListTmp = os.listdir(path+"/"+crabDir+"/res/")
            fileList = []
            for tmp in fileListTmp:
                if re.search("stdout",tmp)!=None:
                    jobNumber = tmp.split("_")[1].split(".")[0]
                    stdoutFileName = "CMSSW_"+str(jobNumber)+".stdout"
                    stdoutFile = open(path+"/"+crabDir+"/res/"+stdoutFileName,"r")
                    exitCode = 9999
                    for line in stdoutFile:
                        if re.search("JobExitCode",line)!=None:
                            exitCode = line.split("=")[1].rstrip("\n")
                    if int(exitCode)==0:
                        fileList.append(path+"/"+crabDir+"/res/"+tmp)
                    if int(exitCode)!=0:
                        print "Will skip job number:",jobNumber," with exitCode= ",exitCode
                    pass
                              
            outputString=""
            outputString += getEffFromTrigReport(selectionPaths[0],skimType,fileList,0)
            outputString += "<td> \n %TREEBROWSER{\"thread\" noroot=\"on\" noindent=\"on\" autotoggle=\"on\" style=\"dmenu\"}%\n"
            outputString += "   * Click for details \n"

            for selectionPath in selectionPaths:
                outputString += getEffFromTrigReport(selectionPath,skimType,fileList,1)


            print "datasetpath: ",datasetpath," pset: ",pset," SE: ",storage_element
            print "publish_data_name: ",publish_data_name
            dbsDatasetName="/"+datasetpath.split("/")[1]+"/akalinow-"+publish_data_name
            print "DBS Instance: ",dbsInst
            print "dbsDatasetName: ",dbsDatasetName
            print "publishName: ",publishName
            print "dashboardLink: ",dashboardLink
            fileName = "TWiki_"+skimType+".txt"
            f = open(fileName,"a")
            
            f.write("<tr> <td>[[https://cmsweb.cern.ch/dbs_discovery/aSearch?caseSensitive=on&userMode=user&sortOrder=desc&sortName=&grid=1&method=dbsapi&dbsInst="+dbsInst+"&userInput=find+dataset+where+dataset+like+*"+dbsDatasetName+"*"+skimType+"*+and+dataset.status+like+VALID*]["+publishName+"]]"
                    + "</td> <td>" + "[["+dashboardLink+"][Link]]"
                    + "</td> <td>" + "[[https://cmsweb.cern.ch/filemover/]["+storage_element+"]]"
                    + "</td> <td>" + outputString
                    + " </td> </tr> \n \n \n")
            f.close()
##################
##################

rootPath = "/home/akalinow/scratch/CMS/ZtauTauSkimms/Spring10/Crab/"
###
'''
path = rootPath+"Ztautau/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/"
getReportForSample(path,"muTau")
getReportForSample(path,"diTau")
getReportForSample(path,"elecTau")
getReportForSample(path,"elecMu")

path = rootPath+"ZJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/"
getReportForSample(path,"muTau")
getReportForSample(path,"diTau")
getReportForSample(path,"elecTau")
getReportForSample(path,"elecMu")

path = rootPath+"WJets-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/"
getReportForSample(path,"muTau")
getReportForSample(path,"diTau")
getReportForSample(path,"elecTau")
getReportForSample(path,"elecMu")

path = rootPath+"Zee/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/"
getReportForSample(path,"muTau")
getReportForSample(path,"diTau")
getReportForSample(path,"elecTau")
getReportForSample(path,"elecMu")

path = rootPath+"Zmumu/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/"
getReportForSample(path,"muTau")
getReportForSample(path,"diTau")
getReportForSample(path,"elecTau")
getReportForSample(path,"elecMu")

path = rootPath+"TTbarJets_Tauola-madgraph/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/"
getReportForSample(path,"muTau")
getReportForSample(path,"diTau")
getReportForSample(path,"elecTau")
getReportForSample(path,"elecMu")

path = rootPath+"Wtaunu/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/"
getReportForSample(path,"muTau")
getReportForSample(path,"diTau")
getReportForSample(path,"elecTau")
getReportForSample(path,"elecMu")

path = rootPath+"QCD_BCtoE_Pt20to30/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/"
getReportForSample(path,"muTau")
getReportForSample(path,"diTau")
getReportForSample(path,"elecTau")
getReportForSample(path,"elecMu")

path = rootPath+"QCD_BCtoE_Pt30to80/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/"
getReportForSample(path,"muTau")
getReportForSample(path,"diTau")
getReportForSample(path,"elecTau")
getReportForSample(path,"elecMu")

path = rootPath+"QCD_BCtoE_Pt80to170/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/"
getReportForSample(path,"muTau")
getReportForSample(path,"diTau")
getReportForSample(path,"elecTau")
getReportForSample(path,"elecMu")

path = rootPath+"QCD_EMEnriched_Pt20to30/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/"
getReportForSample(path,"muTau")
getReportForSample(path,"elecTau")
getReportForSample(path,"elecMu")

path = rootPath+"QCD_EMEnriched_Pt30to80/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/"
getReportForSample(path,"muTau")
getReportForSample(path,"diTau")
getReportForSample(path,"elecTau")
getReportForSample(path,"elecMu")

path = rootPath+"QCD_EMEnriched_Pt80to170/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/"
getReportForSample(path,"muTau")
getReportForSample(path,"diTau")
getReportForSample(path,"elecTau")
getReportForSample(path,"elecMu")

path = rootPath+"PhotonJet_Pt0to15/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/"
getReportForSample(path,"muTau")
getReportForSample(path,"diTau")
getReportForSample(path,"elecTau")
getReportForSample(path,"elecMu")

path = rootPath+"PhotonJet_Pt15to20/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/"
getReportForSample(path,"muTau")
getReportForSample(path,"diTau")
getReportForSample(path,"elecTau")
getReportForSample(path,"elecMu")

path = rootPath+"PhotonJet_Pt20to30/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/"
getReportForSample(path,"muTau")
getReportForSample(path,"diTau")
getReportForSample(path,"elecTau")
getReportForSample(path,"elecMu")

path = rootPath+"PhotonJet_Pt30to50/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/"
getReportForSample(path,"muTau")
getReportForSample(path,"diTau")
getReportForSample(path,"elecTau")
getReportForSample(path,"elecMu")

path = rootPath+"PhotonJet_Pt50to80/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/"
getReportForSample(path,"muTau")
getReportForSample(path,"diTau")
getReportForSample(path,"elecTau")
getReportForSample(path,"elecMu")

path = rootPath+"PhotonJet_Pt80to120/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/"
getReportForSample(path,"muTau")
getReportForSample(path,"diTau")
getReportForSample(path,"elecTau")
getReportForSample(path,"elecMu")

path = rootPath+"PhotonJet_Pt120to170/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/"
getReportForSample(path,"muTau")
getReportForSample(path,"diTau")
getReportForSample(path,"elecTau")
getReportForSample(path,"elecMu")

path = rootPath+"PhotonJet_Pt170to300/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/"
getReportForSample(path,"muTau")
getReportForSample(path,"diTau")
getReportForSample(path,"elecTau")
getReportForSample(path,"elecMu")

path = rootPath+"InclusiveMu15/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/"
getReportForSample(path,"muTau")
getReportForSample(path,"diTau")
getReportForSample(path,"elecTau")
getReportForSample(path,"elecMu")

path = rootPath+"ppMuX/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/"
getReportForSample(path,"muTau")
getReportForSample(path,"diTau")
getReportForSample(path,"elecTau")
getReportForSample(path,"elecMu")

path = rootPath+"QCD_Pt-20_MuEnrichedPt10_7TeV-pythia6/Spring10-START3X_V26-v1/GEN-SIM-RECO/"
getReportForSample(path,"muTau")
getReportForSample(path,"elecTau")
getReportForSample(path,"elecMu")

path = rootPath+"WtaunuSkimm/Wtaunu/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/"
getReportForSample(path,"wTauNu")

path = rootPath+"WtaunuSkimm/QCD_Pt15_BiasedTau/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/"
getReportForSample(path,"wTauNu")
'''




