import FWCore.ParameterSet.Config as cms

process = cms.Process("HcalChanMon")

#process.load("MagneticField.Engine.uniformMagneticField_cfi")
process.load("L1Trigger.Configuration.L1Config_cff")
process.load("L1TriggerConfig.L1GtConfigProducers.Luminosity.lumi1x1032.L1Menu_CRUZET200805_gr7_muon_cff")
process.load('FWCore/MessageService/MessageLogger_cfi')

process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(2))

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(       
#81156 bad for sure for HO 144 344
#    '/store/data/Commissioning09/Calo/RAW/v1/000/081/156/D6E11A46-8324-DE11-A2F2-000423D98E30.root'
    
#98534 ok or not?   
#    '/store/data/CRUZET09/Calo/RAW/v1/000/098/534/CCB6A6D1-1D52-DE11-9BDA-000423D9517C.root',
#    '/store/data/CRUZET09/Calo/RAW/v1/000/098/534/7EA0341C-1D52-DE11-BDD0-000423D98BC4.root'

#98532 ok or not?
#    '/store/data/HAPPYHAPPYWARMFUZZY_T0TEST_WITHBUNNIESDANCINGAROUND/Calo/RAW/premwgrtest-v1/000/098/532/EC808F98-6C55-DE11-9A14-000423D6C8E6.root',
#    '/store/data/HAPPYHAPPYWARMFUZZY_T0TEST_WITHBUNNIESDANCINGAROUND/Calo/RAW/premwgrtest-v1/000/098/532/D8758D4A-6D55-DE11-B587-000423D6AF24.root',
#    '/store/data/HAPPYHAPPYWARMFUZZY_T0TEST_WITHBUNNIESDANCINGAROUND/Calo/RAW/premwgrtest-v1/000/098/532/8AC8FB96-6C55-DE11-BE62-000423D992DC.root',
#    '/store/data/HAPPYHAPPYWARMFUZZY_T0TEST_WITHBUNNIESDANCINGAROUND/Calo/RAW/premwgrtest-v1/000/098/532/7ACE5E19-6E55-DE11-B7F3-001617E30F50.root',
#    '/store/data/HAPPYHAPPYWARMFUZZY_T0TEST_WITHBUNNIESDANCINGAROUND/Calo/RAW/premwgrtest-v1/000/098/532/06D80A98-6C55-DE11-AF59-000423D992A4.root'

#96891
#	 '/store/data/Commissioning09/Calo/RAW/v1/000/096/891/FEE4F44A-1D44-DE11-ABC2-000423D6006E.root',
#	 '/store/data/Commissioning09/Calo/RAW/v1/000/096/891/FCE7420D-2544-DE11-A2FF-000423D94524.root',
#	 '/store/data/Commissioning09/Calo/RAW/v1/000/096/891/F20D8C94-2444-DE11-8FE3-001D09F2983F.root',
#	 '/store/data/Commissioning09/Calo/RAW/v1/000/096/891/E6E19088-1D44-DE11-8AA6-001D09F2AF96.root',
#	 '/store/data/Commissioning09/Calo/RAW/v1/000/096/891/E4119CB1-1A44-DE11-8135-001D09F2932B.root',
#	 '/store/data/Commissioning09/Calo/RAW/v1/000/096/891/DCFEDD0B-2144-DE11-A12C-0019DB29C614.root',
#	 '/store/data/Commissioning09/Calo/RAW/v1/000/096/891/D8738C93-2444-DE11-8583-001D09F251D1.root',
#	 '/store/data/Commissioning09/Calo/RAW/v1/000/096/891/D427E5BF-2144-DE11-A707-001D09F29114.root',
#	 '/store/data/Commissioning09/Calo/RAW/v1/000/096/891/CED7CD8E-2444-DE11-BBB3-001617E30CE8.root',
#	 '/store/data/Commissioning09/Calo/RAW/v1/000/096/891/C821C506-2644-DE11-96B7-001D09F29146.root',
#	 '/store/data/Commissioning09/Calo/RAW/v1/000/096/891/C03969C0-2144-DE11-9183-001D09F2A465.root',
#	 '/store/data/Commissioning09/Calo/RAW/v1/000/096/891/B6CB85A0-1F44-DE11-B792-000423D98B08.root',
#	 '/store/data/Commissioning09/Calo/RAW/v1/000/096/891/B23525C1-2644-DE11-A0C3-000423D9880C.root',
#	 '/store/data/Commissioning09/Calo/RAW/v1/000/096/891/AEAB192C-2344-DE11-ADC1-001D09F26C5C.root',
#	 '/store/data/Commissioning09/Calo/RAW/v1/000/096/891/A4802A4E-2544-DE11-913B-000423D985E4.root',
#	 '/store/data/Commissioning09/Calo/RAW/v1/000/096/891/A0CC9688-1D44-DE11-84AA-001D09F2960F.root',
#	 '/store/data/Commissioning09/Calo/RAW/v1/000/096/891/9AD96E9D-1F44-DE11-93B6-001617C3B79A.root',
#	 '/store/data/Commissioning09/Calo/RAW/v1/000/096/891/98D7ABE2-2344-DE11-A578-001D09F26C5C.root',
#	 '/store/data/Commissioning09/Calo/RAW/v1/000/096/891/8677FB20-1C44-DE11-8312-001D09F231B0.root',
#	 '/store/data/Commissioning09/Calo/RAW/v1/000/096/891/865786F9-1944-DE11-8823-000423D94A04.root',
#	 '/store/data/Commissioning09/Calo/RAW/v1/000/096/891/86534F23-1C44-DE11-9AC8-00304879FBB2.root',
#	 '/store/data/Commissioning09/Calo/RAW/v1/000/096/891/84064BF6-2344-DE11-BED3-000423D94990.root',
#	 '/store/data/Commissioning09/Calo/RAW/v1/000/096/891/7C8E83D4-2844-DE11-B7F6-000423D9880C.root',
#	 '/store/data/Commissioning09/Calo/RAW/v1/000/096/891/64A55387-2944-DE11-8E05-001D09F2AD4D.root',
#	 '/store/data/Commissioning09/Calo/RAW/v1/000/096/891/528FCDA3-1F44-DE11-AD7D-000423D6B42C.root',
#	 '/store/data/Commissioning09/Calo/RAW/v1/000/096/891/500ACE14-2144-DE11-8622-001D09F24303.root',
#	 '/store/data/Commissioning09/Calo/RAW/v1/000/096/891/48016E96-2444-DE11-9608-000423D6CA42.root',
#	 '/store/data/Commissioning09/Calo/RAW/v1/000/096/891/46847FC0-2144-DE11-A3DC-001D09F24691.root',
#	 '/store/data/Commissioning09/Calo/RAW/v1/000/096/891/3C1F5F23-1C44-DE11-A3B8-0030487C608C.root',
#	 '/store/data/Commissioning09/Calo/RAW/v1/000/096/891/301D5FB3-2644-DE11-96A7-001D09F28D4A.root',
#	 '/store/data/Commissioning09/Calo/RAW/v1/000/096/891/0C288193-2444-DE11-8F92-001D09F2932B.root',
#        '/store/data/Commissioning09/Calo/RAW/v1/000/096/891/0C0F42C1-2144-DE11-9D77-001D09F2AD7F.root',
#        '/store/data/Commissioning09/Calo/RAW/v1/000/096/891/0A4C8B64-2744-DE11-83E8-001617C3B778.root',
#        '/store/data/Commissioning09/Calo/RAW/v1/000/096/891/043F2DC7-1C44-DE11-88E2-000423D94A04.root',
#        '/store/data/Commissioning09/Calo/RAW/v1/000/096/891/02AFBE9C-1F44-DE11-A6D6-001617E30CE8.root'
#      96888	
#        '/store/data/Commissioning09/Calo/RAW/v1/000/096/888/FEE55020-0444-DE11-90C3-001D09F23E53.root',
#        '/store/data/Commissioning09/Calo/RAW/v1/000/096/888/FA1DDE73-0344-DE11-920E-001D09F2A49C.root',
#        '/store/data/Commissioning09/Calo/RAW/v1/000/096/888/EC9E11A3-0044-DE11-B98B-001D09F2A465.root',
#        '/store/data/Commissioning09/Calo/RAW/v1/000/096/888/DCA6F033-FD43-DE11-8B34-001D09F28F25.root',
#        '/store/data/Commissioning09/Calo/RAW/v1/000/096/888/DAC32507-0244-DE11-8E5C-001D09F2525D.root',
#        '/store/data/Commissioning09/Calo/RAW/v1/000/096/888/CA65C9D0-FD43-DE11-BED5-000423D98634.root',
#        '/store/data/Commissioning09/Calo/RAW/v1/000/096/888/C4B50DB3-FB43-DE11-8C36-001D09F29169.root',
#        '/store/data/Commissioning09/Calo/RAW/v1/000/096/888/BCEF2ED1-FD43-DE11-B873-000423D99264.root',
#        '/store/data/Commissioning09/Calo/RAW/v1/000/096/888/B08564B7-0244-DE11-944B-001D09F295A1.root',
#        '/store/data/Commissioning09/Calo/RAW/v1/000/096/888/A66D2528-FD43-DE11-8C43-001D09F291D7.root',
#        '/store/data/Commissioning09/Calo/RAW/v1/000/096/888/9A603175-0344-DE11-909B-001D09F29524.root',
#        '/store/data/Commissioning09/Calo/RAW/v1/000/096/888/8E8DF70C-0244-DE11-AA5A-001D09F24303.root',
#        '/store/data/Commissioning09/Calo/RAW/v1/000/096/888/8C9ED665-FC43-DE11-A6CE-0019B9F70607.root',
#        '/store/data/Commissioning09/Calo/RAW/v1/000/096/888/88E47102-3244-DE11-AFA0-0016177CA778.root',
#        '/store/data/Commissioning09/Calo/RAW/v1/000/096/888/7A4E6B35-FF43-DE11-9CD3-000423D9863C.root',
#        '/store/data/Commissioning09/Calo/RAW/v1/000/096/888/6A4734B4-FB43-DE11-BA41-001D09F2532F.root',
#        '/store/data/Commissioning09/Calo/RAW/v1/000/096/888/46763EE6-FF43-DE11-828C-001D09F253C0.root',
#        '/store/data/Commissioning09/Calo/RAW/v1/000/096/888/402ECC63-FC43-DE11-8101-001D09F2915A.root',
#        '/store/data/Commissioning09/Calo/RAW/v1/000/096/888/349908D4-0444-DE11-A5F5-001D09F2924F.root',
#        '/store/data/Commissioning09/Calo/RAW/v1/000/096/888/30B9C47A-FE43-DE11-B019-000423D944DC.root',
#        '/store/data/Commissioning09/Calo/RAW/v1/000/096/888/3051F5A0-0044-DE11-BF66-001D09F252E9.root',
#        '/store/data/Commissioning09/Calo/RAW/v1/000/096/888/1C4AFE57-0144-DE11-B524-000423D992A4.root',
#        '/store/data/Commissioning09/Calo/RAW/v1/000/096/888/1663A204-0744-DE11-9614-000423D999CA.root'
#      80963
#	 '/store/data/Commissioning09/Calo/RAW/v1/000/080/963/DE464328-2A24-DE11-BDB1-00161757BF42.root',
#	 '/store/data/Commissioning09/Calo/RAW/v1/000/080/963/D0AAA92A-2A24-DE11-A48F-001617DBCF6A.root',
#	 '/store/data/Commissioning09/Calo/RAW/v1/000/080/963/C2A9DDE6-2524-DE11-91D7-001617E30D38.root',
#	 '/store/data/Commissioning09/Calo/RAW/v1/000/080/963/C221588A-2B24-DE11-92A8-000423D987FC.root',
#	 '/store/data/Commissioning09/Calo/RAW/v1/000/080/963/84313EB7-2824-DE11-8D97-001617DBD332.root',
#	 '/store/data/Commissioning09/Calo/RAW/v1/000/080/963/70574769-2924-DE11-BB2E-000423D94990.root',
#	 '/store/data/Commissioning09/Calo/RAW/v1/000/080/963/682976D3-2A24-DE11-8531-001617C3B778.root',
#	 '/store/data/Commissioning09/Calo/RAW/v1/000/080/963/66EFF3B6-2824-DE11-ACA5-000423D98BC4.root',
#	 '/store/data/Commissioning09/Calo/RAW/v1/000/080/963/6676DA9B-2624-DE11-B3C1-001617E30D38.root',
#	 '/store/data/Commissioning09/Calo/RAW/v1/000/080/963/64353C9B-2624-DE11-83B8-001617E30F56.root',
#	 '/store/data/Commissioning09/Calo/RAW/v1/000/080/963/56E04D81-3224-DE11-ADD6-001617E30F4C.root',
#	 '/store/data/Commissioning09/Calo/RAW/v1/000/080/963/485DD64E-2724-DE11-A5F3-000423D98E30.root',
#	 '/store/data/Commissioning09/Calo/RAW/v1/000/080/963/0E1FD227-2A24-DE11-A772-001617DBCF1E.root',
#	 '/store/data/Commissioning09/Calo/RAW/v1/000/080/963/04E88867-2924-DE11-B5C0-000423D99B3E.root'
# 110846
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/FC920182-FD87-DE11-8A03-001D09F2424A.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/F8399F19-E987-DE11-B8F0-000423D98B28.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/F4B8D0C4-F587-DE11-A0FD-001D09F24F1F.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/F06D181B-FC87-DE11-AC60-001D09F24600.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/F03AF106-0188-DE11-B669-0019B9F72CE5.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/EE9F1438-FE87-DE11-899F-001D09F25393.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/E2FA4D6C-FB87-DE11-92BB-001D09F2960F.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/E2D68803-EE87-DE11-B6D9-001D09F291D7.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/E2832B47-0588-DE11-BC8A-0019B9F70468.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/E09FBB43-E687-DE11-BE3E-001D09F25393.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/DECAC3AD-FF87-DE11-8D78-0019B9F707D8.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/DECA5042-F287-DE11-9926-001D09F2B30B.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/DE881127-E487-DE11-87F2-001D09F23D04.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/DE3F8CE3-EB87-DE11-861D-001D09F2503C.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/D8FC4A9E-0B88-DE11-8288-001D09F2516D.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/D8535A42-F287-DE11-8827-001D09F28F1B.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/D614BE63-F987-DE11-920E-001D09F2AD84.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/D4E90D90-0488-DE11-8034-001D09F250AF.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/D4C416F6-0588-DE11-A201-001D09F23A6B.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/D432865E-E887-DE11-BE81-000423D98F98.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/D4044C23-F087-DE11-979C-000423D94AA8.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/D2E24E06-0188-DE11-AB56-001D09F24664.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/D23204DC-E487-DE11-A579-0019B9F72BAA.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/D0FD6DE3-EB87-DE11-BBD0-001D09F232B9.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/CE720ACC-E987-DE11-B03D-000423D99F1E.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/CE064CA6-0288-DE11-9EF6-001D09F28E80.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/CC059BA5-F887-DE11-BCB2-001D09F2AF96.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/C88B49D8-0388-DE11-B53B-001D09F241B4.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/C800980F-F587-DE11-B22E-001D09F24047.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/C4D8FB83-FD87-DE11-B881-001D09F29114.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/C497E0EF-0188-DE11-8B7C-001D09F2B2CF.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/C2C5F450-ED87-DE11-9AC9-001D09F28D4A.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/C0F232CF-0F88-DE11-BD6D-001D09F290CE.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/C019BC6C-F987-DE11-B9FC-0019B9F72CC2.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/BCCCE7C9-E987-DE11-9AD5-000423D98FBC.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/BCC39BF9-E687-DE11-9B2F-000423D94990.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/BC93F0DB-E487-DE11-8CB9-001D09F2462D.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/BC5E456B-EF87-DE11-9DBA-000423D99BF2.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/BC016490-E587-DE11-A297-001D09F24303.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/BAC18A0F-F587-DE11-BC7C-001D09F24637.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/BA86E134-EB87-DE11-90F4-000423D94534.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/BA42E2AC-0688-DE11-BA93-001D09F2546F.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/B621A3E4-0A88-DE11-BAF4-001D09F241F0.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/B432F8F0-FE87-DE11-9E57-001D09F231C9.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/B224E198-EC87-DE11-BB8A-000423D990CC.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/B0F90905-EE87-DE11-BAA4-001D09F2523A.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/B0DAB439-0A88-DE11-91DA-000423D991D4.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/B0B9A168-FB87-DE11-9761-001D09F29321.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/B05BC568-F487-DE11-9969-001D09F23174.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/B00D462F-0A88-DE11-AEE2-001D09F28F0C.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/AEAC5956-0088-DE11-85BE-001D09F276CF.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/AE5512D8-0388-DE11-8E49-001D09F2905B.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/AC69AB7D-EA87-DE11-9EFB-000423D99160.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/AC23EEB1-FA87-DE11-A764-001D09F2423B.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/AA09EF27-FA87-DE11-A79D-001D09F24498.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/A8CE7426-0388-DE11-B3B6-001D09F251BD.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/A6FE9A82-FD87-DE11-BBAF-001D09F253FC.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/A6B30385-F187-DE11-ABC0-000423D98B6C.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/A6A85C2D-0A88-DE11-973B-000423D98634.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/A244E85C-0C88-DE11-81FD-001D09F292D1.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/A202AEE2-EE87-DE11-84EB-000423D944F8.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/A00E27AC-0688-DE11-A897-001D09F24448.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/9AFEA38D-0488-DE11-9FDD-001D09F23A07.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/9A3A8FC5-F587-DE11-B497-001D09F253D4.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/962339AC-F387-DE11-9A07-001D09F248F8.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/92177C52-ED87-DE11-AADA-001D09F29146.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/903F9F6B-EF87-DE11-B8D0-000423D94524.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/8E5E5BD0-FC87-DE11-A8AF-001D09F25041.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/8CD27554-0088-DE11-9D6A-001D09F232B9.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/8C2BD108-0188-DE11-B44A-001D09F2525D.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/8C2B9599-0B88-DE11-83B4-001D09F251D1.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/86F23BB8-EE87-DE11-B378-001D09F28EA3.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/8643F3F6-F787-DE11-970A-001D09F29849.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/861A40AD-FF87-DE11-86E6-000423D98868.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/8606DF90-0488-DE11-96C8-001D09F241B9.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/84A6E796-F887-DE11-BFC8-001D09F244DE.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/82F7FBD1-F087-DE11-AC36-000423D99658.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/82CAD9AC-E787-DE11-9334-000423D991F0.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/82BBDA56-0088-DE11-99C0-001D09F2A690.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/807C5C98-EC87-DE11-84D0-000423D98844.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/7E231644-0588-DE11-9EE1-001D09F25460.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/7C72B802-EE87-DE11-B76F-000423D98E30.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/7C616469-FB87-DE11-83B1-001D09F2437B.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/7C5335E8-EB87-DE11-BFE9-000423D94A04.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/7C2A60A7-0288-DE11-B080-001D09F252F3.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/7AB9DC44-F787-DE11-A36B-001D09F25442.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/76D06B0E-F587-DE11-B0B0-001D09F25109.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/74624EB6-EE87-DE11-8519-001D09F25208.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/70E84720-F087-DE11-A3E6-000423D94E1C.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/705C9DAF-E787-DE11-BB36-000423D98DD4.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/701C5ECD-E987-DE11-8743-000423D996C8.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/6E9CF319-E987-DE11-8764-000423D99AAE.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/6E77BF3E-EB87-DE11-9D62-000423D60FF6.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/6A417938-FE87-DE11-9333-001D09F24D4E.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/6A2FC673-F687-DE11-960C-001D09F295A1.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/68F47EAA-F387-DE11-819B-001D09F242EF.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/689CFA7B-EA87-DE11-8555-000423D8FA38.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/68301E3F-F787-DE11-ADE2-001D09F290CE.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/646E261A-E987-DE11-82FD-000423D99660.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/644919AB-F387-DE11-83C3-001D09F2AD4D.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/62DD7390-E587-DE11-918C-0019B9F709A4.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/60EC480D-FA87-DE11-9161-001D09F2910A.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/606A12B6-FA87-DE11-93B4-001D09F28755.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/5EE15E43-F287-DE11-89B9-001D09F24763.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/5C0CCE4B-ED87-DE11-8DFB-000423D94E70.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/5AA89A68-FB87-DE11-8DC9-001D09F252F3.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/5A5F5B5C-F487-DE11-AFF1-001D09F248FD.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/5673FBCA-E987-DE11-BC63-000423D987FC.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/56281FD2-F087-DE11-9013-000423D94908.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/5491751F-F087-DE11-8B50-000423D9890C.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/521DDDB4-FA87-DE11-A576-001D09F2906A.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/503EDEB2-FA87-DE11-82CC-001D09F24FEC.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/4EF3D1CD-FC87-DE11-A1B7-001D09F2841C.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/4EC0CF9E-0B88-DE11-AA1E-001D09F29321.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/4AB82E30-EB87-DE11-BA49-000423D998BA.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/48F7E20A-0188-DE11-83C0-001D09F25438.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/48673BAD-FF87-DE11-A674-001D09F2543D.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/46B8A892-E587-DE11-BCC7-001D09F27003.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/4654822F-0A88-DE11-8776-001D09F291D2.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/46241EFB-E687-DE11-8416-0019B9F72D71.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/449D5B5D-0C88-DE11-BF18-001D09F2462D.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/445A5A28-0388-DE11-9B55-001D09F24EE3.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/406F6497-EC87-DE11-9BE5-000423D944F0.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/3CCB4FD0-FC87-DE11-891E-001D09F2AD7F.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/3C548F99-0B88-DE11-909D-001D09F242EF.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/38DB6EC5-F587-DE11-A854-001D09F29169.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/386C31E4-0A88-DE11-BBBD-001D09F2915A.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/36978FF5-FE87-DE11-9174-001D09F2D426.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/36097876-F687-DE11-B82B-0019B9F72BAA.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/34A065F7-F787-DE11-B1B5-001D09F24EC0.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/347EAAAB-F387-DE11-8093-001D09F292D1.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/344505AF-E787-DE11-ACAE-000423D98EC4.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/32AD84F5-F287-DE11-8AAC-0019B9F72BFF.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/3263D918-FC87-DE11-97DC-000423D987E0.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/30788420-F087-DE11-8525-000423D99614.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/2CF2B4D3-F087-DE11-8CB6-000423D8F63C.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/2CEA585E-E887-DE11-A7BF-000423D991D4.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/2C3216F4-F287-DE11-BD57-0019B9F704D6.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/2A5C4FAC-0688-DE11-8059-001D09F253C0.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/2A57D623-0388-DE11-93AC-000423D98EC4.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/2A5070F1-0188-DE11-AC49-001D09F23D1D.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/2A276B45-E687-DE11-8B52-001D09F24399.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/24E12E86-F187-DE11-9C24-001D09F23E53.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/2432B94A-F787-DE11-8091-001D09F23A02.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/220FCAF2-FE87-DE11-A186-001D09F2A465.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/20C8DB2C-F787-DE11-894C-0019B9F70607.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/1E51DB63-0788-DE11-80C1-001D09F2A49C.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/1C12D3D2-F087-DE11-A1A3-000423D99A8E.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/1A770C38-FE87-DE11-B255-001D09F24D8A.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/18C4FF5F-E887-DE11-91F0-000423D99B3E.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/16E6B894-F887-DE11-9A15-001D09F24691.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/168829DC-E487-DE11-862E-001D09F23D1D.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/1643F4F8-0588-DE11-A518-001D09F2545B.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/148207F8-F787-DE11-B234-001D09F24682.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/1216BD6D-F987-DE11-975C-001D09F2516D.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/10A7A258-F487-DE11-B682-001D09F295FB.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/10A50BF1-FE87-DE11-AA2E-001D09F251E0.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/105C1D7E-EA87-DE11-B982-001D09F2437B.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/0ECE3FE6-0388-DE11-98D7-001D09F24399.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/0EAD7964-F987-DE11-AA5A-001D09F25401.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/0E5EAE77-F687-DE11-A890-001D09F25217.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/0CBC7AA7-0288-DE11-9A15-001D09F282F5.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/0AC41195-0488-DE11-85C5-001D09F251FE.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/0A4B2A5C-0C88-DE11-9999-001D09F24FBA.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/08FFDECD-FC87-DE11-9997-0019B9F72F97.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/08FBB1A3-0288-DE11-B966-0019B9F7312C.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/08B01A97-EC87-DE11-8D08-000423D98920.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/088A0AAF-E787-DE11-B121-000423D98868.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/06E2AD43-E687-DE11-AD7A-001D09F24664.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/06D5F240-F287-DE11-9216-000423D9A212.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/06D25652-0C88-DE11-9C03-001D09F24DA8.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/04E03FF5-0588-DE11-BCF3-001D09F248FD.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/04969644-E687-DE11-A30C-001D09F2305C.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/02A731BA-EE87-DE11-B5F4-000423D952C0.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/02575465-0788-DE11-9BDF-001D09F23174.root',
        '/store/data/CRAFT09/Calo/RAW/v1/000/110/846/00CED845-0588-DE11-B480-001D09F24637.root'
# end 
    )
)

process.hcalchanmon = cms.EDAnalyzer('HcalChanMon',
# subDet HB=1, HE=2, HO=3, HF=4
# 144 344 233 HO was the demonstration list

       ChannelList    = cms.untracked.VPSet(
         cms.PSet(
           subDet = cms.untracked.uint32(3),
	   ieta   = cms.untracked.int32(-14),
	   iphi   = cms.untracked.int32(37),
	   depth  = cms.untracked.int32(4)
	 ),
        cms.PSet(
           subDet = cms.untracked.uint32(3),
	   ieta   = cms.untracked.int32(7),
	   iphi   = cms.untracked.int32(59),
	   depth  = cms.untracked.int32(4)
	 ),
	 cms.PSet(
           subDet = cms.untracked.uint32(3),
	   ieta   = cms.untracked.int32(12),
	   iphi   = cms.untracked.int32(49),	   
	   depth  = cms.untracked.int32(4)
	 ),
	 # GOOD CHANNEL WAIT TO SEE
        cms.PSet(
           subDet = cms.untracked.uint32(3), 
           ieta   = cms.untracked.int32(12),
           iphi   = cms.untracked.int32(50),
           depth  = cms.untracked.int32(4)
         ),

       )

)

process.TFileService = cms.Service("TFileService",
#    fileName = cms.string('Hcal_ChanMon_81156.root')
#    fileName = cms.string('Hcal_ChanMon_98532.root')
#    fileName = cms.string('Hcal_ChanMon_96888.root')
#    fileName = cms.string('Hcal_ChanMon_96891.root')
#    fileName = cms.string('Hcal_ChanMon_80963.root')
     fileName = cms.string('Hcal_ChanMon_110846_test.root')  
)

# process.load("Configuration.StandardSequences.FakeConditions_cff")
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.es_prefer_GlobalTag = cms.ESPrefer('PoolDBESSource','GlobalTag')
process.GlobalTag.globaltag = 'GR09_31X_V4P::All'

#Magnetic Field
process.load("Configuration.StandardSequences.MagneticField_0T_cff")
# process.load('Configuration/StandardSequences/MagneticField_38T_cff')

#Geometry
process.load("Configuration.StandardSequences.GeometryPilot2_cff")
#process.load("Configuration.StandardSequences.Geometry_cff")

# Real data raw to digi and reco
process.load("Configuration.StandardSequences.RawToDigi_Data_cff")
process.load("Configuration.StandardSequences.ReconstructionCosmics_cff")

process.getDigi = cms.Path(process.RawToDigi_woGCT)
process.getReco = cms.Path(process.reconstructionCosmics)

process.p1 = cms.Path(process.hcalchanmon)

process.schedule = cms.Schedule(process.getDigi, process.getReco, process.p1)
