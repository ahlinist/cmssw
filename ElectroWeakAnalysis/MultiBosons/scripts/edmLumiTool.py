#!/usr/bin/env python
import glob
import sys
import os

from RecoLuminosity.LumiDB import argparse
from FWCore.PythonUtilities.LumiList import LumiList
from ElectroWeakAnalysis.MultiBosons.tools.JSONFromEDMFile import makeJSONFromList
from ElectroWeakAnalysis.MultiBosons.tools.LumiDBWrapper import LumiDBWrapper

parser = argparse.ArgumentParser(prog=os.path.basename(sys.argv[0]),description='edmLumiTool')
#basic options for manipulating a JSON made from edm files
parser.add_argument('-e','--edmFiles',dest='inputEDMFiles',action='store', required=True,
                    help='Input EDM files to extract lumi info from.')
parser.add_argument('-j','--jsonFiles',dest='inputJSONFiles',action='store',required=False,
                    help='Input JSON files, operation agaist generated JSON is specified by further options.')
parser.add_argument('-o','--outputJSON',dest='outputJSON',action='store',required=False,
                    help='Name of JSON file to output. If not specified doesn\'t output a file.')
parser.add_argument('-p','--printJSON',dest='printJSON',action='store_true',
                    help='Prints JSON to the screen.')
parser.add_argument('-S','--printCMSSWString',dest='printCMSSWString',action='store_true',
                    help='Prints JSON to the screen in CMSSW String format.')
parser.add_argument('-u','--union',dest='union',action='store_true',
                    help='Make union of input JSONs with created JSON')
parser.add_argument('-s','--subtract',dest='subtract',action='store_true',
                    help='Subtract input JSONS from created JSON')
parser.add_argument('-i','--intersect',dest='intersect',action='store_true',
                    help='Make intersection of input JSONs with created JSON')
#lumi DB related arguments
parser.add_argument('-l','--lumiDatabase',dest='lumiDatabase',action='store_true',
                    help='Query the lumi database with the resulting JSON. Stores delivered and rec\'d lumi.')
parser.add_argument('-c','--connectString',dest='connectString',action='store',required=False,
                    help='connect string to lumiDB (default is: frontier://LumiProd/CMS_LUMI_PROD)')
parser.add_argument('-P','--authPath',dest='authPath',action='store',
                    help='Path to authentication file.')
parser.add_argument('-n','--normalization',dest='normalization',action='store',
                    help='normalization factor (optional, default to 1.0)')
parser.add_argument('--lumiVersion',dest='lumiVersion',action='store',
                    help='lumi data version, optional for all, default 0001')
parser.add_argument('--siteConfPath',dest='siteConfPath',action='store',
                    help='specific path to site-local-config.xml file, default to $CMS_PATH/SITECONF/local/JobConfig, if path undefined, fallback to cern proxy&server')
parser.add_argument('--verbose',dest='verbose',action='store_true',help='verbose mode for printing' )
parser.add_argument('--nowarning',dest='nowarning',action='store_true',help='suppress bad for lumi warnings' )
parser.add_argument('--debug',dest='debug',action='store_true',help='debug')

args = parser.parse_args()

inputEDMFiles = None
inputJSONFiles = None
lumiCalc = None

if args.inputEDMFiles:
    inputEDMFiles = sorted(glob.glob(args.inputEDMFiles))
if args.inputJSONFiles:
    inputJSONFiles = sorted(glob.glob(args.inputJSONFiles))
if args.outputJSON:
    outputJSON = args.outputJSON
if args.lumiDatabase:
    argslist = dict()
    
    if args.connectString:
        argslist['connectstring']=args.connectString
    if args.authPath:
        os.environ['CORAL_AUTH_PATH']=args.authPath
    if args.siteConfPath:
        argslist['siteconfpath']=args.siteConfPath
    if args.verbose:
        argslist['verbose']=True
    if args.nowarning:
        argslist['nowarning']=True
    if args.debug:
        argslist['debug']=True
    if args.normalization:
        argslist['normfactor']=args.normalization
    if args.lumiVersion:
        argslist['lumiversion']=args.lumiVersion
    
    lumiCalc = LumiDBWrapper(**argslist)

print "Building LumiList from EDM Files..."
edmLumis = LumiList(compactList=makeJSONFromList(inputEDMFiles))
impLumis = None

if inputJSONFiles is not None:
    impLumis = LumiList(filename=inputJSONFiles.pop())
    for j in inputJSONFiles:
        impLumis = impLumis | LumiList(filename=j)

if impLumis is not None:
    if args.union:
        edmLumis = edmLumis | impLumis
    if args.subtract:
        edmLumis = edmLumis - impLumis
    if args.intersect:
        edmLumis = edmLumis & impLumis

reclumiData=None
dellumiData=None
if lumiCalc is not None:
    print "Accessing LumiDB... can take a while..."
    dellumiData=lumiCalc.deliveredLumiForRange(edmLumis.getCompactList())
    reclumiData=lumiCalc.recordedLumiForRange(edmLumis.getCompactList())    
    totalRec = 0.0
    totalDel = 0.0
    for dpr in dellumiData:
        if dpr[2] != 'N/A':
            totalDel += float(dpr[2])
    for dpr in reclumiData:
        totalRec += lumiCalc.calculateTotalRecorded(dpr[2])
    print "Delivered Luminosity: ",totalDel
    print "Recorded Luminosity: ",totalRec

if args.outputJSON:
    edmLumis.writeJSON(outputJSON)

if args.printJSON:
    print edmLumis.getCompactList()

if args.printCMSSWString:
    print edmLumis.getCMSSWString()

if lumiCalc is not None:
    del lumiCalc
