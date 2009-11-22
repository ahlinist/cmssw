import FWCore.ParameterSet.Config as cms
import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt

process = cms.Process('TEST')
process.load('JetMETAnalysis.PromptAnalysis.ntuple_cff')

process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.Services_cff')
process.add_( cms.Service( "TFileService",
                           fileName = cms.string( 'testTree.root' ),
                           closeFileFast = cms.untracked.bool(True)  ) )

#baseLocation="/store/data/Commissioning09/Calo/RECO/v8/000/116/736"

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )
process.source = cms.Source (
    "PoolSource",
    fileNames = cms.untracked.vstring(
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/F8B46F58-29D6-DE11-8949-001D09F24934.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/F03C8BB3-23D6-DE11-BFE0-0019B9F72BAA.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/EEF282BA-2AD6-DE11-AF0D-003048D37456.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/EEBCA549-22D6-DE11-BE94-001D09F292D1.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/EE7F7C62-29D6-DE11-B9F2-0030487A322E.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/EE1D6B56-30D6-DE11-8988-000423D952C0.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/E4A463F6-2ED6-DE11-A35F-001D09F2438A.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/DEF2E005-23D6-DE11-B3C6-001D09F24D67.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/DEA949A7-2FD6-DE11-BD83-0030487D0D3A.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/DCF98C80-1FD6-DE11-BB6B-001D09F2426D.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/CE2CBAC0-28D6-DE11-919A-001D09F26C5C.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/CC83E8BF-28D6-DE11-A833-000423D98634.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/CA4E6EDB-2CD6-DE11-92D3-001D09F27003.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/C4AB27C0-28D6-DE11-A9F8-001D09F2960F.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/C2E76275-2BD6-DE11-91F9-001D09F25208.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/C230A13B-2ED6-DE11-A53E-001D09F2906A.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/BECF4B1D-25D6-DE11-880E-001D09F2AD4D.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/BE8055F1-27D6-DE11-831B-001D09F24FBA.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/BCD47DE0-2CD6-DE11-92A4-001D09F25041.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/AE7E06F6-2ED6-DE11-B1DD-001D09F23F2A.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/AA159280-26D6-DE11-B0FA-001D09F2924F.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/A07589DC-2CD6-DE11-B634-001D09F28EA3.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/A03500C3-1ED6-DE11-9878-0019B9F72BFF.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/9EFC737D-26D6-DE11-A64E-003048D37538.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/9EFB76E5-20D6-DE11-9637-001D09F24D8A.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/9832F4A0-2FD6-DE11-9E33-0019DB29C5FC.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/98200C7C-26D6-DE11-B5BD-001D09F292D1.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/92E1EBE1-20D6-DE11-812E-001D09F2905B.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/8C68EE3C-2ED6-DE11-BDFB-001D09F2438A.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/8AAF231C-25D6-DE11-9E25-0019B9F72BFF.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/8A4CDB75-2BD6-DE11-A15F-001D09F2512C.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/8687367F-26D6-DE11-A1D0-001D09F24D4E.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/7EE258DB-2CD6-DE11-AF0B-001D09F2983F.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/7A70407F-26D6-DE11-9926-001D09F28755.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/7445F2ED-27D6-DE11-859D-001D09F251BD.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/70072DBD-28D6-DE11-A726-003048D2BED6.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/6C9384E5-20D6-DE11-BF46-001D09F2527B.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/6C75C382-26D6-DE11-B1BD-001D09F34488.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/66BB1649-22D6-DE11-922F-001D09F232B9.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/6655EA88-1FD6-DE11-A417-001D09F2516D.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/640C31BE-2AD6-DE11-9321-001D09F2438A.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/6288F4EF-2ED6-DE11-AC4A-001617DBD556.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/608E1CDF-2CD6-DE11-8129-001D09F253FC.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/6022B781-1FD6-DE11-8BEF-001D09F2437B.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/5E62A0E5-20D6-DE11-B6B2-001D09F25393.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/56C6957F-26D6-DE11-A5D2-001D09F24F65.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/50EED17B-26D6-DE11-BF11-0019B9F72F97.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/509247ED-27D6-DE11-950D-0019B9F707D8.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/4CDFA9C1-2AD6-DE11-AD31-001D09F2910A.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/3CF66675-2BD6-DE11-8907-001D09F24E39.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/3CCA572F-20D6-DE11-B543-001D09F276CF.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/32F1841C-25D6-DE11-8050-001D09F2532F.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/32A9C87E-26D6-DE11-A21A-0030487D0D3A.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/2EDE86E5-20D6-DE11-A5A0-001D09F253C0.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/2CF33E85-1FD6-DE11-8CD4-001D09F24E39.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/2A5F0E49-22D6-DE11-8272-001D09F23944.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/2613F105-23D6-DE11-BD3D-001D09F23944.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/222B061C-25D6-DE11-8B89-001D09F24763.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/20F07848-22D6-DE11-AE3C-001D09F2A49C.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/203E03ED-27D6-DE11-B543-001D09F2527B.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/1EE5A0AE-23D6-DE11-8655-001D09F2AD7F.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/184E931B-25D6-DE11-BF4B-001D09F24D67.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/126005B3-23D6-DE11-9E17-001D09F26509.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/0C3D08C3-2AD6-DE11-A4C0-001D09F251B8.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/0A520AE5-20D6-DE11-9591-001D09F23174.root",
"/store/express/BeamCommissioning09/ExpressPhysics/FEVT/v2/000/121/964/047C5FF1-27D6-DE11-A9EE-001D09F28E80.root"),

    #    "/store/relval/CMSSW_3_1_4/RelValTTbar/GEN-SIM-RECO/MC_31X_V3-v1/0005/901ABD8A-E5B0-DE11-8AE6-000423D98DD4.root"),
    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
    
    secondaryFileNames = cms.untracked.vstring())

process.MessageLogger = cms.Service("MessageLogger",
                                    default = cms.untracked.PSet(
    reportEvery = cms.untracked.int32(100)
    )
                                    )
