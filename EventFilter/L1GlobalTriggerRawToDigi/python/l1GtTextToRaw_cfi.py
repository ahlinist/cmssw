import FWCore.ParameterSet.Config as cms

#
# cfi file for a module to produce raw GT DAQ starting from a text (ASCII) file
#
l1GtTextToRaw = cms.EDFilter("L1GtTextToRaw",
    # FED Id for GT DAQ record 
    # default value defined in DataFormats/FEDRawData/src/FEDNumbering.cc
    DaqGtFedId = cms.untracked.int32(813),
    # name of the text file to be packed
    # the module is using a EmptySource source
    TextFileName = cms.untracked.string('testGt_TextToRaw_source.txt'),
    # FED raw data size (in 8bits units, including header and trailer)
    # If negative value, the size is retrieved from the trailer.    
    RawDataSize = cms.untracked.int32(872),
    # type of the text file
    TextFileType = cms.untracked.string('VmeSpyDump')
)


