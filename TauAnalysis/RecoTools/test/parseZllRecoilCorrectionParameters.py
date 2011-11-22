#!/usr/bin/env python

import math
import os
import re

inputFilePath = "/data1/veelken/tmp/ZllRecoilCorrection/v4_3_1/2011RunA/"

tablesToPrint = {
    'rawPFMET' : {
        'Data'    : 'fittedZllRecoilCorrectionParameters_Data_pfMEt_qT_central_cfi.py',
        'MC'      : 'fittedZllRecoilCorrectionParameters_simDYtoMuMu_pfMEt_qT_central_cfi.py',
        'MC_corr' : 'fittedZllRecoilCorrectionParameters_simDYtoMuMu_pfMEtSmeared_qT_%s_cfi.py'
    },
    'Type1corrPFMET' : {
        'Data'    : 'fittedZllRecoilCorrectionParameters_Data_pfMEtTypeIcorrected_qT_central_cfi.py',
        'MC'      : 'fittedZllRecoilCorrectionParameters_simDYtoMuMu_pfMEtTypeIcorrected_qT_central_cfi.py',
        'MC_corr' : 'fittedZllRecoilCorrectionParameters_simDYtoMuMu_pfMEtTypeIcorrectedSmeared_qT_%s_cfi.py'
    }
}

central = 'central'

shifts = [
    'jetEnUp',
    'jetEnDown',
    'jetResUp',
    'jetResDown'
]

central_or_shifts = [ central ]
central_or_shifts.extend(shifts)

header_regex   = re.compile(r"\s*recoZllRecoilCorrectionParameters\s*=\s*cms.PSet\(\s*")
varName_regex  = re.compile(r"\s*(?P<varName>[a-zA-Z0-9]+)\s*=\s*cms.PSet\(\s*")
varValue_regex = re.compile(r"\s*value\s*=\s*cms.double\((?P<varValue>[0-9.e+-]+)\),*\s*")
varError_regex = re.compile(r"\s*uncertainty\s*=\s*cms.double\((?P<varError>[0-9.e+-]+)\),*\s*")

varValues = {}
varErrors = {}

for tableName, tableOptions in tablesToPrint.items():
    varValues[tableName] = {}
    varErrors[tableName] = {}
    for columnName, columnOptions in tableOptions.items():
        varValues[tableName][columnName] = {}
        varErrors[tableName][columnName] = {}
        for central_or_shift in central_or_shifts:
            
            inputFileName = os.path.join(inputFilePath, columnOptions)
            if inputFileName.find("_%s_") != -1:
                inputFileName = inputFileName % central_or_shift
            elif central_or_shift != central:
                continue
            inputFile = open(inputFileName, 'r')
            lines = (line.strip() for line in inputFile.readlines())

            currentVarName = None
            for line in lines:
                if header_regex.match(line):
                    continue
                elif varName_regex.match(line):
                    currentVarName = str(varName_regex.match(line).group('varName'))
                    if not currentVarName in varValues[tableName][columnName].keys():
                        varValues[tableName][columnName][currentVarName] = {}
                    if not currentVarName in varErrors[tableName][columnName].keys():
                        varErrors[tableName][columnName][currentVarName] = {}
                elif varValue_regex.match(line):
                    if currentVarName != 'k1':
                        varValues[tableName][columnName][currentVarName][central_or_shift] = \
                          float(varValue_regex.match(line).group('varValue'))
                    else:
                        varValues[tableName][columnName][currentVarName][central_or_shift] = \
                          -float(varValue_regex.match(line).group('varValue'))
                elif varError_regex.match(line):    
                    varErrors[tableName][columnName][currentVarName][central_or_shift] = \
                      float(varError_regex.match(line).group('varError'))

columnNames = [ 'Data', 'MC', 'MC_corr' ]

for tableName, tableOptions in tablesToPrint.items():
    print "table: %s" % tableName
    # CV: comma at end of print statement prevents new-line
    print "%-20s" % "parameter",
    for columnName in columnNames:
        print "          %-20s" % columnName,
    print ""    
    for varName in [ 'k1', 'k2', 'k3', 'sigma1', 'b1', 'c1', 'sigma2', 'b2', 'c2' ]:
        print "%-20s" % varName,
        for columnName in columnNames:
            if len(varValues[tableName][columnName][varName].keys()) > 1:
                # sum systematic uncertainties in quadrature
                central_value = varValues[tableName][columnName][varName][central]
                error2 = 0.
                for central_or_shift in central_or_shifts:
                    if central_or_shift != central:
                        error2 += math.pow(central_value - varValues[tableName][columnName][varName][central_or_shift], 2)
                # average up/down shifts
                error2 /= 2.            
                # add statistical uncertainty in quadrature
                error2 += math.pow(varErrors[tableName][columnName][varName][central], 2)
                if varValues[tableName][columnName][varName][central] > 1.e-2:
                    print "%11.4f +/- %11.4f   " % \
                      (varValues[tableName][columnName][varName][central],
                       math.sqrt(error2)),
                else:                    
                    print "%11.4e +/- %11.4e   " % \
                      (varValues[tableName][columnName][varName][central],
                       math.sqrt(error2)),
            else:
                # statistical uncertainties only
                if varValues[tableName][columnName][varName][central] > 1.e-2:
                    print "%11.4f +/- %11.4f   " % \
                      (varValues[tableName][columnName][varName][central],
                       varErrors[tableName][columnName][varName][central]),
                else:
                    print "%11.4e +/- %11.4e   " % \
                      (varValues[tableName][columnName][varName][central],
                       varErrors[tableName][columnName][varName][central]),
        print ""
    print ""
