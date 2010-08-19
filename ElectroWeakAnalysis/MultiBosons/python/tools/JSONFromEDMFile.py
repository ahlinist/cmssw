# \Author Lindsey Gray ( U.W. Madison )
# \Author Loic Quertenmont ( UCL ), for the original bit of C++ code

import ROOT
import sys

from ElectroWeakAnalysis.MultiBosons.tools.LuminosityBlocks import LuminosityBlocks

from ROOT import TFile

#from a TChain make the JSON dict
def makeJSONFromList(files = list()):
    full_json = dict()
    for file in files:
#        print "Processing: ",file
        file=TFile.Open(file)
        temp_json = makeJSONFromFile(file)
        file.Close()
        for run in temp_json.keys():
            if run not in full_json:
                full_json.update({run: temp_json[run]})
            else:
                lumi_list = full_json[run]
                lumi_list.extend(temp_json[run])
                lumi_list.sort()
                full_json.update({run: lumi_list})                

    for run in full_json.keys():
        lastsize = -1
        while(lastsize != len(full_json[run])):
            lastsize = len(full_json[run])
            full_json[run] = list(mcranges(full_json[run]))

    return full_json

#from a TFile make a JSON dict
def makeJSONFromFile(file = TFile()):
    lumis = LuminosityBlocks(file)
    json_dict=dict()
    for ls in lumis:
        run = str(ls.luminosityBlockAuxiliary().run())
        lumi = int(ls.luminosityBlockAuxiliary().id().luminosityBlock())
        if run not in json_dict:
            json_dict.update({run: [lumi]})
        else:
            lumi_list = json_dict[run]
            lumi_list.append(lumi)            
            lumi_list.sort()
            json_dict.update({run: lumi_list})    

    for run in json_dict.keys():
        json_dict.update({run: list(cranges(json_dict[run]))})
    
    return json_dict


#collapse all ranges of numbers into their endpoints
crange = lambda r: [r[0],r[-1]]
def cranges(seq, step=1):
    seq.sort()
    current = []
    for x in seq:
        if not current or x-current[-1] == step:
            current.append(x)
        else:
            yield crange(current)
            current = [x]
    if current:
        yield crange(current)


mcrange = lambda rr: [min([a[0] for a in rr]),max([a[-1] for a in rr])]
def mcranges(seq,step=1):
    seq.sort()
    result=[]
    for x in seq:        
        if not result or x[0]-result[-1][-1] <= step:
            result.append(x)
        else:
            yield mcrange(result)
            result =[x]
    if result:
        yield mcrange(result)
