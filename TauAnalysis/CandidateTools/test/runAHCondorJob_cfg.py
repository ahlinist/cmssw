#!/usr/bin/env cmsRun

# Wrapper for parallelizing SV ntuple production using multiple samples

from makeSVntuple_cfg import process, cms
import os
import FWCore.ParameterSet.VarParsing as VarParsing

if not os.path.exists('condor'):
    os.mkdir('condor')

if not os.path.exists('SecondaryVertexTests'):
    os.mkdir('SecondaryVertexTests')
    os.mkdir('SecondaryVertexTests/data_in')

options = VarParsing.VarParsing ('standard')

options.register ('job',
                  0, # default value
                  VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.VarParsing.varType.int,          # string, int, or float
                  "Batch job")

options.register ('sample',
                  "Ztautau", # default value
                  VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.VarParsing.varType.string,          # string, int, or float
                  "Sample to use")

options.parseArguments()

ztt_file_list = open('ztt.files', 'r')
att_file_list = open('att.files', 'r')
qcd_file_list = open('qcd.files', 'r')

ztt_files = (file.strip() for file in ztt_file_list.readlines())
att_files = (file.strip() for file in att_file_list.readlines())
qcd_files = (file.strip() for file in qcd_file_list.readlines())

files = {}
files['AH_tautau'] = [
    'file:%s' % file for file in ztt_files
    #glob.glob( "/mnt/hadoop/store/user/cms1227/bbAtautau_M120/friis/bbAtautau_M120/ZtoMuTauSkimTest4/60dace99b1523ca69748e499060d8ab3/*.root")
]

files['Ztautau'] = [
    'file:%s' % file for file in att_files
    #glob.glob( "/mnt/hadoop/store/user/cms1227/Ztautau/friis/Ztautau/ZtoMuTauSkimTest4/60dace99b1523ca69748e499060d8ab3/*root")
]

files['QCD'] = [
    'file:%s' % file for file in qcd_files
    #glob.glob( "/mnt/hadoop/store/user/cms1227/InclusiveMu15/friis/InclusiveMu15/ZtoMuTauSkimTest4/60dace99b1523ca69748e499060d8ab3/*.root")
]

process.source.fileNames = cms.untracked.vstring(
    files[options.sample][options.job] )

process.out.fileName = cms.untracked.string(
    'SecondaryVertexTests/data_in/sv_ntuple_%s_%i.root' % (options.sample, options.job)
)
