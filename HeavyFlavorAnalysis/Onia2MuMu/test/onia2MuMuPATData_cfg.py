# for the list of used tags please see:
# https://twiki.cern.ch/twiki/bin/view/CMS/Onia2MuMuSamples

import FWCore.ParameterSet.Config as cms

process = cms.Process("Onia2MuMuPAT")

from HeavyFlavorAnalysis.Onia2MuMu.onia2MuMuPAT_cff import *

onia2MuMuPAT(process, GlobalTag="GR_R_38X_V8::All", MC=False, HLT="HLT", Filter=True)

process.source.fileNames = cms.untracked.vstring(
	'/store/data/Run2010A/MuOnia/RECO/v4/000/140/401/FC6A5623-1993-DF11-B37D-0030487C7828.root',
	'/store/data/Run2010A/MuOnia/RECO/v4/000/140/401/F202B792-1093-DF11-889A-001617C3B70E.root',
	'/store/data/Run2010A/MuOnia/RECO/v4/000/140/401/D8023811-0D93-DF11-8ECA-0030487CAF5E.root',
	'/store/data/Run2010A/MuOnia/RECO/v4/000/140/401/CC367D91-1093-DF11-B545-001617C3B6CC.root',
	'/store/data/Run2010A/MuOnia/RECO/v4/000/140/401/C0C63A7A-1393-DF11-9C57-003048F11114.root',
	'/store/data/Run2010A/MuOnia/RECO/v4/000/140/401/88C873F1-1193-DF11-BBFD-0030487A18F2.root',
	'/store/data/Run2010A/MuOnia/RECO/v4/000/140/401/7E56E31E-1993-DF11-B4C5-000423D98950.root',
	'/store/data/Run2010A/MuOnia/RECO/v4/000/140/401/66C7120F-1493-DF11-B150-0030487CAEAC.root',
	'/store/data/Run2010A/MuOnia/RECO/v4/000/140/401/343F2303-0693-DF11-BBE3-003048F1BF68.root',
	'/store/data/Run2010A/MuOnia/RECO/v4/000/140/401/3041B3C8-1B93-DF11-9C44-003048F110BE.root',
	'/store/data/Run2010A/MuOnia/RECO/v4/000/140/401/22CEE4C7-1B93-DF11-B7C7-003048F1182E.root',
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )

