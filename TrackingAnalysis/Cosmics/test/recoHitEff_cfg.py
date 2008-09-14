import FWCore.ParameterSet.Config as cms

process = cms.Process("ReRec")
process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.load("CondCore.DBCommon.CondDBSetup_cfi")


process.maxEvents = cms.untracked.PSet(  input = cms.untracked.int32(100000) )
#process.extend("CRUZET4_RECO_v1_trackerPointing_cfi")

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/003E85DC-E270-DD11-A1E4-0017312B56F7.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/00450816-D772-DD11-8110-00304875A587.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/0068DAF0-3D73-DD11-8DF7-003048769E85.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/047F6874-3E73-DD11-8D10-003048767EF7.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/08035670-2273-DD11-9A31-001A92810ADE.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/0C530147-C072-DD11-8900-001A92971B04.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/0C6399D3-2173-DD11-9B8E-0018F3D0965C.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/0CF8B667-2173-DD11-B020-001A92810AEE.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/10400E69-2273-DD11-873D-001A92971B0E.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/10F2E972-1773-DD11-BEE4-0018F3D09612.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/123B556E-C072-DD11-98D7-0018F3D0969A.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/14253750-C572-DD11-B311-001731A281B1.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/16272167-2473-DD11-A8B3-001A92810AD2.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/18A93150-C072-DD11-95F6-001A92971AEC.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/18C9A647-C072-DD11-83BB-0018F3C3E3A6.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/1C125198-D272-DD11-A62E-001A92971B06.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/1E37BC9B-3773-DD11-897B-001731AF6A8D.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/1EDD5259-C072-DD11-9273-001A928116F0.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/203D4841-D172-DD11-A221-001731AF699D.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/20971E38-C872-DD11-AEEB-001731AF67E1.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/20B5CB54-D672-DD11-868D-001A92811748.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/221202D1-3773-DD11-B095-003048769E6D.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/22467A33-2473-DD11-9654-003048767D43.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/224E3C13-2573-DD11-8DD8-001A92971BD8.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/226550C8-E270-DD11-AA90-001A92810AEC.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/226D8DCD-2273-DD11-940E-003048767FB3.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/22C7FC76-2373-DD11-9A6D-003048767FB3.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/24BA0601-E370-DD11-A236-001A92971B1A.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/264CE65F-2273-DD11-9EB9-0018F3D0970E.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/28FA8227-2273-DD11-9073-001BFCDBD1BE.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/2A4A1C06-3F73-DD11-A0EC-003048767D6F.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/2C5B8AF0-BF72-DD11-AEE4-001731AF6781.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/2CAEDB0A-C072-DD11-8D24-0018F3D095FA.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/2E92201C-C072-DD11-8745-001A92811736.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/2EC4B168-D372-DD11-8D19-001A92971B36.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/321A0A24-C072-DD11-B942-001BFCDBD11E.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/326E9CD9-BF72-DD11-A073-001731AF66B7.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/328F340C-C072-DD11-B2FC-0018F3D09616.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/341E6563-C072-DD11-AE00-001A92971AA4.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/38849663-C072-DD11-A3C3-001A928116F0.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/3A081076-1773-DD11-B93B-0018F3D0967E.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/3A3CFABC-3973-DD11-8ED0-001731AF6877.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/3AF07B6C-2273-DD11-BB92-001A92971B9A.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/3CB9EBCF-2473-DD11-A60A-003048767D4F.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/3E5B0667-3873-DD11-A4CE-001731AF66F1.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/3E86E863-C072-DD11-967C-001A928116F0.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/3E8B5374-1773-DD11-8F19-001A92971B90.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/3ED91D40-C072-DD11-9971-001BFCDBD1BE.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/3EEEBA73-DA72-DD11-9660-001731AF66F7.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/428A8661-1773-DD11-84A2-001A92971AAA.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/42A10F2B-D672-DD11-86C0-0030487624FB.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/42A673FD-D672-DD11-8B00-0017319C97E0.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/442D0568-C072-DD11-A5C0-001A92971ACE.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/44BEC163-1773-DD11-8952-001731AF66B3.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/44C53F59-C072-DD11-A2D4-0018F3D096DA.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/44F3ACA9-3D73-DD11-94A0-00304875AAED.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/46029B55-C072-DD11-9808-0018F3D09626.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/4850F0EC-BF72-DD11-BCE1-001731AF6781.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/4A250645-DA72-DD11-BD95-00304876A0DB.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/4A3B300B-C072-DD11-A7F4-001A92810AD2.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/4A75E68F-4073-DD11-A746-00304876A0FF.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/4AFA266A-D372-DD11-93CC-0018F3D096AE.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/4C411E3D-C072-DD11-A3BE-001A92971B7E.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/4C5A2F7F-3A73-DD11-9C48-003048767EE5.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/4C830CC4-3F73-DD11-9376-003048767D4F.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/4E7D01AE-2373-DD11-9667-0018F3D0965C.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/50B2B32C-3E73-DD11-998F-001731AF692D.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/50C69EE5-7470-DD11-A259-0018F3D0968C.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/52DBBE4A-E270-DD11-B2CD-001731230C61.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/54932011-D672-DD11-92DA-001A92971B9A.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/5809EE97-3973-DD11-A285-001731AF692D.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/58AA3330-C072-DD11-ADE0-001A92971B5E.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/5AD93A57-C072-DD11-9C4F-0018F3D09702.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/5E23DFFA-3D73-DD11-8212-00304875AA13.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/625B1EED-DA72-DD11-9053-003048767FAB.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/629EE5F9-C872-DD11-BE07-001A92810AD2.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/64093A46-C072-DD11-B1E2-003048754F9D.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/68552307-DA72-DD11-A37B-001731AF67BF.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/6A5B6683-3A73-DD11-BE78-003048769E85.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/6CBB8C47-3E73-DD11-8FE1-001731AF66F7.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/6CD0F957-C072-DD11-A396-001A92811748.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/6EE7D549-C072-DD11-8F97-001BFCDBD100.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/70DFE3C6-C772-DD11-ACCB-0018F3D09682.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/721746D7-E372-DD11-ACFA-00304875AB57.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/724CE4FD-3E73-DD11-9E59-00304876A0EB.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/7251511A-C072-DD11-9F5F-001A92811728.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/728B06EF-BF72-DD11-8472-001731AF67BD.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/74AFCC70-C072-DD11-B900-0018F3D0969A.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/74DBFAFF-7470-DD11-991B-001A928116F0.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/763E74DB-E270-DD11-A602-001A92810A96.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/76E990FD-BF72-DD11-8C0B-001731AF66B9.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/788D2B17-CB71-DD11-8E14-001731AF66C2.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/78D0222A-2273-DD11-A3A9-0018F3D0965C.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/7CF13B5D-C072-DD11-970F-001A92971B36.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/802E52A1-3D73-DD11-B979-001731AF67EF.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/80474118-4173-DD11-83AA-001731AF66A7.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/809F6856-C072-DD11-AC52-001A92810AA8.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/823F7085-2373-DD11-B1CD-003048767D4F.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/8249084C-C572-DD11-BCD9-001A92971B8E.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/82735B8F-4073-DD11-9FB6-001731AF698F.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/82CACB3B-7570-DD11-892D-001BFCDBD160.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/8421AD2E-4073-DD11-96F2-001731AF66A5.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/843BF218-CB71-DD11-8DEA-001731AF6863.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/86218604-3F73-DD11-9CF6-001731A28BE1.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/8625A4F0-7470-DD11-921B-001A92971ACE.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/886B9866-C072-DD11-BC30-001BFCDBD19E.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/88F3F8BD-3973-DD11-9496-001731AF6943.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/88F55C62-2573-DD11-B955-00304875A9ED.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/8A694CDB-BF72-DD11-9EB2-001731AF6933.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/8AFACA22-D272-DD11-A274-00304875AAD3.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/8C75F62F-3A73-DD11-9923-001731AF65E7.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/8CFA9855-C072-DD11-91FB-0018F3D0960A.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/8E030714-C072-DD11-93F4-0018F3D096EA.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/8E20E04F-C072-DD11-9D47-001A92971AEC.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/8E409B7F-2373-DD11-B23B-001731AF66C2.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/8EADF5FC-3D73-DD11-BBDD-001731A28857.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/90576D0C-C072-DD11-B6EF-0018F3D0962E.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/929E76B6-2273-DD11-B53D-0018F3D09676.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/9434791F-C072-DD11-8EBC-001A92971B0E.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/944F6914-E372-DD11-8704-0017313F01E4.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/9618753D-C572-DD11-A065-001731AF67EF.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/96A164C6-2273-DD11-A61F-003048767DD9.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/98B3A862-C072-DD11-987C-001A92810AA6.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/9A9F8A46-F872-DD11-83F6-001731AF687F.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/9ACA4476-D972-DD11-8A58-00304875AB53.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/9CB0C152-C072-DD11-9088-001BFCDBD1BE.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/9E2C5CE0-BF72-DD11-B490-00304876A067.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/9E723A39-D472-DD11-B2CA-001731AF685B.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/A0BC211E-3E73-DD11-9DDA-003048769FE1.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/A2B1E213-2273-DD11-8194-0018F3D0965C.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/A6D56E53-C572-DD11-A748-001731AF6727.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/A8F5E262-C072-DD11-AED5-001A92971B8A.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/AA4D6536-C072-DD11-87FA-001A92810AA4.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/AEE8290B-C972-DD11-8823-001731AF6AE7.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/B06CF50A-7570-DD11-9AB4-001A928116B4.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/B086A95C-3E73-DD11-B4F3-001731A28999.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/B090F928-2273-DD11-B5A5-001A92810AA4.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/B434232A-4073-DD11-8322-00304876A0FF.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/B4E5E8FB-3D73-DD11-A759-001731AF685F.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/B6C8D71E-2473-DD11-B29A-001A92810AD2.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/B83EA164-C072-DD11-8A57-001A928116F0.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/BCAADC49-3E73-DD11-A589-001731AF66F7.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/BE3490B0-3D73-DD11-BFDA-00304875AADF.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/C047AB97-E270-DD11-B01F-001A92971B84.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/C05AA861-C072-DD11-B791-001A928116F0.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/C09A6175-1773-DD11-9554-0018F3D09600.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/C0CDECBC-3973-DD11-8BD2-001731AF6943.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/C0DC33FF-BF72-DD11-BEC3-003048755187.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/C2B0EE5E-C072-DD11-9094-001BFCDBD19E.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/C2E33C51-3873-DD11-A910-001731AF67EF.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/C62A6C63-C072-DD11-8FCB-001A92971B0C.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/C62D04E6-3D73-DD11-99BC-003048767ED1.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/C644A4B4-3F73-DD11-9280-001731AF65E9.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/C6724983-3E73-DD11-A5D0-001731AF6787.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/C87A50BA-3773-DD11-9634-001731AF684B.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/CA2BE81B-D772-DD11-9A28-0017312B5DA9.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/CAA81F3D-C072-DD11-802E-0018F3D0965C.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/CAF88C4D-C072-DD11-BFF7-001BFCDBD100.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/CC86274B-C072-DD11-97FD-001A92971B7E.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/CCE05471-3A73-DD11-AA36-001731AF65EF.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/CEEF4E09-C072-DD11-B2C1-0018F3D096EE.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/D0EE7F0E-C072-DD11-9DEA-001A92810AE0.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/D2459575-1773-DD11-A84A-0018F3D0969C.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/D40AA050-C072-DD11-8E9A-001A92971B68.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/D41AD383-3A73-DD11-8D48-001731AF6B77.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/D4F76941-2273-DD11-9254-003048767FB3.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/D4FA10C7-2473-DD11-9313-00304875A9ED.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/D6691ADB-D272-DD11-AB0F-00304876A139.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/D8067BE1-7470-DD11-BD7C-0017312A21EB.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/D8D4ACFA-3F73-DD11-B6AF-001731AF6BD3.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/D8F3BF6E-2173-DD11-8A49-003048767FB3.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/DA00BB66-C072-DD11-BB80-0018F3D09710.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/DA39B6EF-D072-DD11-82EB-00304875AADD.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/DAF2992C-C072-DD11-997D-001A92971AD0.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/DC07FA4B-C072-DD11-BE45-001A92811748.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/DC8FB708-7570-DD11-9984-001A92810AA6.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/DE223652-C072-DD11-BC60-0018F3D09710.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/DEB4851F-3E73-DD11-8375-003048769E6D.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/DEB8A2D7-3773-DD11-8BAD-00304876A067.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/E0214DDB-3773-DD11-9E9E-003048767EF7.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/E0926501-C072-DD11-9EE8-001731A28319.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/E2671FF8-BF72-DD11-B8F0-001A92971BB8.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/E29B91CA-2373-DD11-8306-003048767FB3.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/E412B64F-C072-DD11-9769-0018F3D0965C.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/E41B9379-2373-DD11-8156-0018F3D096F8.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/E429C225-3A73-DD11-A76A-00304876A06D.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/E4B83F57-3873-DD11-87D7-00304876A147.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/E4CC7D90-3973-DD11-942C-00304875A9C5.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/E638DA4E-C072-DD11-B2E0-001A92971B0E.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/E6DA51C2-2373-DD11-B98F-001731AF66C2.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/E80BC1F4-2173-DD11-B415-00304875AB55.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/E82F3278-2273-DD11-9FA3-001A92971B80.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/E8F17CBF-2373-DD11-82D4-0018F3D096F8.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/EA67AA45-3E73-DD11-8E27-00304875A585.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/EACD3CD6-3773-DD11-A582-001731AF692D.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/EE2E3295-D372-DD11-BE02-001731AF677F.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/F00C3A3D-2573-DD11-B5F6-001731A28857.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/F0374562-C072-DD11-88E3-001A928116C0.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/F07F6A4F-C072-DD11-8EE8-001731AF66FF.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/F095BE53-C072-DD11-A646-0018F3D096BC.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/F241E155-C072-DD11-A28C-001A92810AA8.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/F288B4BD-3D73-DD11-AA71-00304875A585.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/F2AE9D68-2473-DD11-B0E4-001A92971BD8.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/F402B7B2-3D73-DD11-8020-003048769D4B.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/F413A2C0-D972-DD11-8F23-001731AF66AD.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/F6293705-C072-DD11-AE98-00304876A0EB.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/F69EC5F9-3F73-DD11-A523-001731AF66F5.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/F6C87EEC-BF72-DD11-A337-001731A28543.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/F6FA3EB3-3D73-DD11-8F6E-00304876A0DB.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/F8FEE7CF-2173-DD11-B76A-0018F3D096F8.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/FAC131B4-3D73-DD11-8546-001731A28999.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/FAD65FFC-D072-DD11-BDDB-001731AF67BF.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/FC0A48B2-3D73-DD11-A909-00304875A9C5.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/FC16ED6A-C072-DD11-AC70-001A92971BD8.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0000/FCD84F5D-3A73-DD11-9384-00304876A067.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0001/1AE654DA-1E73-DD11-8273-0018F3D096E8.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0001/366669D9-1E73-DD11-A7C6-001BFCDBD1BE.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0001/50748CD9-1E73-DD11-8C95-001A92971B04.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0001/5CB9BED8-1E73-DD11-9A89-0018F3D09682.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0001/606AE7D2-1E73-DD11-BE7C-003048767DD7.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0001/6C1CC8D9-1E73-DD11-B7D4-001A92971B0E.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0001/7800D0DC-1E73-DD11-B8A6-001A928116B4.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0001/9401CAD9-1E73-DD11-816F-001A92971AD0.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0001/A4AABEDB-1E73-DD11-9538-001A92810AEA.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0001/AC6181D7-1E73-DD11-B062-0018F3D09678.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0001/C6CB95DA-1E73-DD11-A83D-0018F3D096EA.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0001/C8DB66D9-1E73-DD11-8BF7-001A92971B08.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0001/D4F309DB-1E73-DD11-8D6F-0018F3D0961A.root',
        'dcache:/pnfs/cms/WAX/11/store/data/CRUZET4_v1/Cosmics/RECO/CRZT210_V1_TrackerPointing_v1/0001/EAAC08DB-1E73-DD11-8C29-0018F3D096EE.root'
        )
)


# output module
#
process.load("Configuration.EventContent.EventContentCosmics_cff")

process.FEVT = cms.OutputModule("PoolOutputModule",
    process.FEVTEventContent,
    dataset = cms.untracked.PSet(dataTier = cms.untracked.string('RECO')),
    fileName = cms.untracked.string('promptrecoCosmics.root')
)

process.FEVT.outputCommands.append('keep CaloTowersSorted_calotoweroptmaker_*_*')
process.FEVT.outputCommands.append('keep CSCDetIdCSCALCTDigiMuonDigiCollection_muonCSCDigis_MuonCSCALCTDigi_*')
process.FEVT.outputCommands.append('keep CSCDetIdCSCCLCTDigiMuonDigiCollection_muonCSCDigis_MuonCSCCLCTDigi_*')
process.FEVT.outputCommands.append('keep CSCDetIdCSCComparatorDigiMuonDigiCollection_muonCSCDigis_MuonCSCComparatorDigi_*')
process.FEVT.outputCommands.append('keep CSCDetIdCSCCorrelatedLCTDigiMuonDigiCollection_csctfDigis_*_*')
process.FEVT.outputCommands.append('keep CSCDetIdCSCCorrelatedLCTDigiMuonDigiCollection_muonCSCDigis_MuonCSCCorrelatedLCTDigi_*')
process.FEVT.outputCommands.append('keep CSCDetIdCSCRPCDigiMuonDigiCollection_muonCSCDigis_MuonCSCRPCDigi_*')
process.FEVT.outputCommands.append('keep CSCDetIdCSCStripDigiMuonDigiCollection_muonCSCDigis_MuonCSCStripDigi_*')
process.FEVT.outputCommands.append('keep CSCDetIdCSCWireDigiMuonDigiCollection_muonCSCDigis_MuonCSCWireDigi_*')
process.FEVT.outputCommands.append('keep cscL1TrackCSCDetIdCSCCorrelatedLCTDigiMuonDigiCollectionstdpairs_csctfDigis_*_*')
process.FEVT.outputCommands.append('keep DTChamberIdDTLocalTriggerMuonDigiCollection_muonDTDigis_*_*')
process.FEVT.outputCommands.append('keep DTLayerIdDTDigiMuonDigiCollection_muonDTDigis_*_*')
process.FEVT.outputCommands.append('keep intL1CSCSPStatusDigisstdpair_csctfDigis_*_*')
process.FEVT.outputCommands.append('keep L1MuDTChambPhContainer_dttfDigis_*_*')
process.FEVT.outputCommands.append('keep L1MuDTChambThContainer_dttfDigis_*_*')
process.FEVT.outputCommands.append('keep L1MuDTTrackContainer_dttfDigis_DATA_*')
process.FEVT.outputCommands.append('keep PixelDigiedmDetSetVector_siPixelDigis_*_*')
process.FEVT.outputCommands.append('keep recoCandidatesOwned_caloTowersOpt_*_*')
process.FEVT.outputCommands.append('keep RPCDetIdRPCDigiMuonDigiCollection_muonRPCDigis_*_*')

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.17 $'),
    name = cms.untracked.string('$Source: /local/projects/CMSSW/rep/CMSSW/Configuration/GlobalRuns/python/recoT0DQM_EvContent_cfg.py,v $'),
    annotation = cms.untracked.string('CRUZET Prompt Reco with DQM with Mag field at 0T')
)
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) ) ## default is false


# Conditions (Global Tag is used here):
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.connect = "frontier://PromptProd/CMS_COND_21X_GLOBALTAG"
process.GlobalTag.globaltag = "CRUZET4_V2P::All"
#process.GlobalTag.globaltag = "CRUZET4_V4P::All"
process.prefer("GlobalTag")

# Magnetic fiuld: force mag field to be 0 tesla
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.GlobalRuns.ForceZeroTeslaField_cff")

#Geometry
process.load("Configuration.StandardSequences.Geometry_cff")

# Real data raw to digi
process.load("Configuration.StandardSequences.RawToDigi_Data_cff")

# reconstruction sequence for Cosmics
process.load("Configuration.StandardSequences.ReconstructionCosmics_cff")

# offline DQM
#process.load("DQMOffline.Configuration.DQMOfflineCosmics_cff")
#process.load("DQMServices.Components.MEtoEDMConverter_cff")

#L1 trigger validation
#process.load("L1Trigger.HardwareValidation.L1HardwareValidation_cff")
process.load("L1Trigger.Configuration.L1Config_cff")
process.load("L1TriggerConfig.CSCTFConfigProducers.CSCTFConfigProducer_cfi")
process.load("L1TriggerConfig.CSCTFConfigProducers.L1MuCSCTFConfigurationRcdSrc_cfi")

## workaround for tracker
process.load("CalibTracker.SiStripESProducers.SiStripQualityESProducer_cfi")
process.SiStripQualityESProducer.ListOfRecordToMerge = cms.VPSet(
     cms.PSet( record = cms.string("SiStripFedCablingRcd"), tag    = cms.string("") ),
     cms.PSet( record = cms.string("SiStripBadChannelRcd"), tag    = cms.string("") ),
     cms.PSet( record = cms.string("SiStripBadFiberRcd"),   tag    = cms.string("") )
)
process.prefer("SiStripQualityESProducer")

##Add configurable here
#define quality for clusterizer
#process.load("CalibTracker.SiStripESProducers.SiStripQualityFakeESSource_cfi")
  #include "CalibTracker/SiStripESProducers/data/SiStripQualityFakeESSource.cfi"
  #replace siStripQualityFakeESSource.appendToDataLabel="qualityforCluster"

#define quality for tk reconstruction (=> module efficiency study)
#process.load("CalibTracker.SiStripESProducers.SiStripQualityConfigurableFakeESSource_cfi")
#process.prefer("SiStripQualityConfigurableFakeESSource")
   #include "CalibTracker/SiStripESProducers/data/SiStripQualityConfigurableFakeESSource.cfi"
   #replace siStripQualityConfigurableFakeESSource.appendToDataLabel=""
   #es_prefer  siStripQualityConfigurableFakeESSource= SiStripQualityConfigurableFakeESSource {}

#setup the reco modules
   #replace siStripClusters.SiStripQualityLabel="qualityforCluster"
   #replace CTF_P5_MeasurementTracker.UseStripModuleQualityDB = true
   #replace CTF_P5_MeasurementTracker.UseStripAPVFiberQualityDB = false
   #replace CTF_P5_MeasurementTracker.UseStripStripQualityDB = false
   #replace CTF_P5_MeasurementTracker.DebugStripModuleQualityDB = true
   #replace CTF_P5_MeasurementTracker.DebugStripAPVFiberQualityDB = false
   #replace CTF_P5_MeasurementTracker.DebugStripStripQualityDB = false

process.ckfBaseTrajectoryFilterP5.filterPset.maxLostHits = 5
process.ckfBaseTrajectoryFilterP5.filterPset.maxConsecLostHits = 5
process.ckfBaseTrajectoryFilterP5.filterPset.minimumNumberOfHits =  12

#workaround for RPC and EE DQM modules
#process.muonCosmicMonitors_fix = cms.Sequence(process.muonTrackCosmicAnalyzers*process.dtSegmentsMonitor*process.cscMonitor*process.muonCosmicAnalyzer)

#process.ee_dqm_source_offline_fix = cms.Sequence(process.ecalEndcapMonitorModule*process.dqmInfoEE*process.ecalEndcapOccupancyTask*process.ecalEndcapIntegrityTask*process.ecalEndcapStatusFlagsTask*process.ecalEndcapPedestalOnlineTask*process.ecalEndcapCosmicTask*process.ecalEndcapClusterTask)

#process.ecal_dqm_source_offline_fix = cms.Sequence(process.ee_dqm_source_offline_fix*process.eb_dqm_source_offline) 

#process.DQMOfflineCosmics_fix = cms.Sequence(process.SiStripDQMTier0*process.ecal_dqm_source_offline_fix*process.muonCosmicMonitors_fix*process.jetMETAnalyzer*process.hcalOfflineDQMSource*process.l1tmonitor*process.siPixelOfflineDQM_source*process.egammaCosmicPhotonMonitors)

process.testTraj = cms.EDAnalyzer("TrajAnalyzer")

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('hiteff_layer10_test.root')
)


process.anEff = cms.EDAnalyzer("HitEff",
                               Debug = cms.bool(True),
                               Layer = cms.int32(10),
                               rphirecHits = cms.InputTag("siStripMatchedRecHits:rphiRecHit"),
                               stereorecHits    = cms.InputTag("siStripMatchedRecHits:stereoRecHit"),
                               combinatorialTracks = cms.InputTag("ctfWithMaterialTracksP5"),
                               trajectories   =   cms.InputTag("ctfWithMaterialTracksP5"),
                               combinatorialSeeds = cms.InputTag("combinatorialcosmicseedfinderP5")
                               )


#Paths
#process.allPath = cms.Path( process.RawToDigi_woGCT * process.reconstructionCosmics *  process.DQMOfflineCosmics_fix * process.MEtoEDMConverter)
#process.allPath = cms.Path( process.trackerCosmics )
#process.allPath = cms.Path( process.trackerCosmics * process.testTraj)
process.allPath = cms.Path( process.trackerCosmics * process.anEff )

#process.outpath = cms.EndPath(process.FEVT)
