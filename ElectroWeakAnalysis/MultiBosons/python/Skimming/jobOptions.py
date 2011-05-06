import copy
import sys
from ElectroWeakAnalysis.MultiBosons.Skimming.options import options as defaultOptions


## IDEA: manipulate the command line arguments in sys.argv to parse only the
## jobType first and then parse the rest of the options

def applyJobOptions(options):
  """
  Set multiple options defined by the jobType argument.
  To be called after options.parseArguments()
  """
  options.parseArguments()

  ## Attach the crabOptions to the command line arguments
  if options.crabOptions: sys.argv += options.crabOptions.split(",")

  jobOptions = copy.deepcopy(defaultOptions)

  if options.jobType == "testMC":
    jobOptions.outEvents = 100
    jobOptions.inputFiles = [
      "/store/relval/CMSSW_3_8_7/RelValZMM/GEN-SIM-RECO/START38_V13-v1/" +
      file for file in """
      0017/E81C0282-93FC-DF11-B15D-00261894390A.root
      0017/50641150-9DFC-DF11-AC76-001A92971B54.root
      0016/AEA8DD07-8BFC-DF11-A505-003048678FE4.root
      0016/A4A2387F-8CFC-DF11-BCBF-00304867C136.root
      0016/86090D81-8BFC-DF11-AB45-003048679228.root
      0016/5418F1FD-89FC-DF11-9610-002618943852.root
      0016/124ACA84-8AFC-DF11-8D9B-0030486791F2.root
      """.split()
    ]
    jobOptions.globalTag = "START38_V14::All"
    jobOptions.reportEvery = 1
    jobOptions.isRealData = False
    jobOptions.use35XInput = False
    jobOptions.wantSummary = False
    jobOptions.skimType = "MuonPhoton"
  elif options.jobType == "testMC414":
    jobOptions.outEvents = 100
    jobOptions.inputFiles = [
      "/store/relval/CMSSW_4_1_4/RelValZMM/GEN-SIM-RECO/START311_V2-v1/" +
      file for file in """
      0013/40E72E1E-8F60-E011-A5A1-002618FDA211.root      
      """.split()
    ]
    jobOptions.globalTag = "START311_V2::All"
    jobOptions.reportEvery = 1
    jobOptions.isRealData = False
    jobOptions.use35XInput = False
    jobOptions.wantSummary = False
    jobOptions.skimType = "MuonPhoton"
  elif options.jobType == "testPOWHEG":
    jobOptions.maxEvents = -1
    jobOptions.inputFiles = [
      "file:/scratch/lgray/skimming/CMSSW_3_8_4_patch3/"+
      "src/ElectroWeakAnalysis/MultiBosons/" +
      file for file in """
        3A6643C1-D17F-DF11-8B44-001A92971B0C.root
        36FD5DFF-D67F-DF11-9C85-0026189438A9.root
      """.split()
    ]
    jobOptions.globalTag = "START38_V14::All"
    jobOptions.reportEvery = 1
    jobOptions.isRealData = False
    jobOptions.use35XInput = False
    jobOptions.wantSummary = False
    jobOptions.hltProcessName = "REDIGI36X"
    jobOptions.skimType = "MuonPhoton"

  # end of testMC options <-------------------------------------

  elif options.jobType == "testRealData":
    jobOptions.outEvents = 500
    jobOptions.inputFiles = [
      "/store/data/Run2011A/SingleMu/AOD/PromptReco-v2/"+
      file for file in """
      000/163/664/BA4E582E-3374-E011-AB7D-0030487C6A66.root
      000/163/664/8E33DE82-1374-E011-89AE-003048D2C01E.root
      000/163/664/8AA8987F-1374-E011-AC71-000423D98B6C.root
      000/163/664/76AB5BD9-2074-E011-8D1B-001617C3B66C.root
      000/163/661/BCEE4EBA-0E74-E011-92D3-001D09F250AF.root
      000/163/660/D8062937-FC73-E011-826D-001D09F25401.root
      000/163/660/5E39E2C1-0E74-E011-BC04-003048D2BCA2.root
      000/163/658/483857D2-7D73-E011-BFF0-000423D33970.root
      000/163/657/F46C481E-B073-E011-AFA3-000423D996C8.root
      000/163/657/D4DBB7A2-A973-E011-A5E4-0030487C608C.root
      000/163/657/A0FE2C77-AD73-E011-8963-001D09F24F65.root
      000/163/657/247A95AF-C673-E011-824B-003048D2C0F2.root
      000/163/657/2288F9F3-AD73-E011-B8B0-003048F024FA.root
      000/163/655/4073681D-BC73-E011-B2B0-0030487CBD0A.root
      000/163/630/DEFF64B5-9373-E011-BF38-001D09F2525D.root
      000/163/630/929C7AE5-7573-E011-B6B1-000423D996C8.root
      000/163/630/8466AF01-7873-E011-9BBF-001617C3B5E4.root
      000/163/596/46641959-5B73-E011-B446-001D09F2545B.root
      000/163/593/3453C686-5873-E011-89A8-0030487CAF5E.root
      000/163/592/988C8007-4473-E011-A2B9-003048F118C4.root
      000/163/592/52A94462-5873-E011-BEE3-003048F024E0.root
      000/163/592/12DA80AF-3D73-E011-887C-001D09F2447F.root
      000/163/591/129910A2-3D73-E011-8FB6-001617E30F50.root
      000/163/589/FAE2BA34-3373-E011-969A-0030487CD6B4.root
      000/163/589/A436ADFB-3073-E011-80D6-001617DBD472.root
      000/163/589/720AF300-F873-E011-87AF-0030487D1BCC.root
      000/163/587/403931B1-AD73-E011-9929-001D09F23F2A.root
      000/163/587/26B7AB8D-0273-E011-A96B-0030487A3232.root
      000/163/586/8211944D-FE72-E011-A6AC-0030487CD180.root
      000/163/586/7A8509F8-FE72-E011-BC51-001D09F2516D.root
      000/163/586/2AD36113-1573-E011-97FC-003048D2C16E.root
      000/163/585/6C7BB385-E873-E011-BC3E-0030486780AC.root
      000/163/584/FAF91016-1273-E011-AE04-0030487A1990.root
      000/163/584/10C6D3BC-F872-E011-B8E2-00304879FA4C.root
      000/163/583/EAC69AA0-B873-E011-8AD6-00304879BAB2.root
      000/163/583/CEAEBF32-F572-E011-8A8A-001617C3B710.root
      000/163/583/AA569404-F872-E011-BD07-0030487C912E.root
      000/163/583/9A8017E7-F572-E011-9463-000423D33970.root
      000/163/583/78FDE7B8-F872-E011-A70E-001D09F242EF.root
      000/163/583/3A5B34BB-F872-E011-B4CB-001D09F231B0.root
      000/163/583/0E796304-F872-E011-8A79-001D09F2516D.root
      000/163/582/8E632D3D-0873-E011-9337-003048D37514.root
      000/163/581/7248974F-B972-E011-8F73-0030487C912E.root
      000/163/580/18C2CF17-B772-E011-AA21-001D09F242EF.root
      000/163/483/E636CC89-9772-E011-8E94-0030487CD14E.root
      000/163/482/903F5F8F-9772-E011-9475-0030487CD906.root
      000/163/481/66803430-7E72-E011-BBBA-001617E30E28.root
      000/163/480/BA0E648F-3473-E011-A5D7-003048F024FE.root
      000/163/480/86B8696E-7B72-E011-B023-003048F1BF66.root
      000/163/480/7282816E-7B72-E011-AFA8-003048F11CF0.root
      000/163/480/5E8B5013-7C72-E011-9D1B-003048F117B4.root
      000/163/479/D2ACD951-7972-E011-9341-0030487A18A4.root
      000/163/479/40E04CA3-5972-E011-863E-001D09F292D1.root
      000/163/479/14E4677B-5772-E011-A597-001D09F24489.root
      000/163/479/0821182B-5872-E011-B1D1-003048D37538.root
      000/163/478/DC67A823-3772-E011-9706-003048F1C58C.root
      000/163/476/D83CC344-3972-E011-AB2C-0030487CD180.root
      000/163/476/C61C46C0-3572-E011-8E30-003048F11DE2.root
      000/163/476/96A88397-3872-E011-8E76-001617DC1F70.root
      000/163/476/4AF55B45-3972-E011-B087-0030487CD6E6.root
      000/163/476/402E4CBC-5272-E011-AF3B-003048D3750A.root
      000/163/475/F2C0032C-3272-E011-8E7D-001617C3B6C6.root
      000/163/475/E667F8F0-6E72-E011-9CDD-001D09F29597.root
      000/163/475/BE564733-3072-E011-A523-000423D94494.root
      000/163/475/A8668BED-3272-E011-8195-001617C3B6C6.root
      000/163/475/6ACC8F95-2C72-E011-814D-000423D9A2AE.root
      000/163/475/68808B8D-2C72-E011-A94B-001617DBD5AC.root
      000/163/475/3A09B36F-6372-E011-837D-000423D9A2AE.root
      000/163/475/32A47786-3172-E011-B797-001617E30F4C.root
      000/163/402/FCD2F7EA-8171-E011-9B47-001617C3B66C.root
      000/163/402/C05E6582-8771-E011-A71C-000423D94908.root
      000/163/402/A8B4F912-7F71-E011-A5C5-001617E30CC8.root
      000/163/402/A0FFBAF7-7C71-E011-8E48-001617E30F48.root
      000/163/402/A0676709-8471-E011-A761-003048D2BF1C.root
      000/163/402/9EF99D0B-8471-E011-B43F-003048D2C174.root
      000/163/402/9E1DB67F-7971-E011-8F25-000423D94494.root
      000/163/402/9CB468EE-8171-E011-8C4E-001617C3B5D8.root
      000/163/402/8C8DFEF8-7C71-E011-8268-000423D996C8.root
      000/163/402/7A962A7F-7971-E011-A089-003048F1BF66.root
      000/163/402/76F8B1F7-7F71-E011-AAFD-003048F11C58.root
      000/163/402/76153876-9F71-E011-9C7B-003048F117B4.root
      000/163/402/6ABFC6F5-7F71-E011-9B8E-003048F1C832.root
      000/163/402/6455BDE9-A071-E011-A307-003048F1C424.root
      000/163/402/641541F7-7C71-E011-BE10-001617C3B6DE.root
      000/163/402/628328F9-0372-E011-811A-001D09F24024.root
      000/163/402/2CE6C80C-9271-E011-B964-003048F117B6.root
      000/163/402/14020736-8171-E011-B839-001D09F2423B.root
      000/163/387/A42C02EC-5671-E011-89F7-003048F024DE.root
      000/163/387/A2F90559-3471-E011-94C6-003048D374F2.root
      000/163/387/5E50DB57-3471-E011-8AA1-001D09F241B9.root
      000/163/387/566C1878-3671-E011-8D47-001D09F24EAC.root
      000/163/387/32992624-3771-E011-85BA-003048F1183E.root
      000/163/385/CE5F2FBC-2E71-E011-A623-001617C3B706.root
      000/163/385/C477EDC3-D271-E011-AD13-003048F118D2.root
      000/163/385/B6889379-4271-E011-9DE1-0030487CAF5E.root
      000/163/385/A84D0F87-2571-E011-A930-001617C3B70E.root
      000/163/385/749E61E9-2B71-E011-97C6-003048D2BE08.root
      000/163/385/2C5B2FD1-2971-E011-B9D3-001617DBD316.root
      000/163/385/22992B9E-2C71-E011-B8E8-003048F117EA.root
      000/163/385/1ADDDD3D-2B71-E011-9859-003048F024C2.root
      000/163/385/18801CE2-4771-E011-BC8C-0030487CD716.root
      000/163/378/DC4A29BA-F770-E011-BFF8-0019B9F72CE5.root
      000/163/378/9ECB7B37-F970-E011-970B-001617E30F50.root
      000/163/378/9E941AC5-FE70-E011-B635-003048F1110E.root
      000/163/378/9A344E35-0071-E011-91B3-0030486780B4.root
      000/163/378/90D581EE-F970-E011-872C-0019B9F72CE5.root
      000/163/378/5EFFE557-FD70-E011-874B-003048D2C01A.root
      000/163/378/4C93BFA2-0171-E011-B71C-001D09F251B8.root
      000/163/378/48332255-4072-E011-95D4-001617E30D52.root
      000/163/378/04790F34-0071-E011-B94E-0030487CD6B4.root
      000/163/377/A2F1A4E8-CC70-E011-83C8-001D09F2983F.root
      000/163/376/BCD9DDD4-E670-E011-B5E1-001617C3B5D8.root
      000/163/376/5EDF72C7-6072-E011-BF1E-001D09F29146.root
      000/163/376/4E6CBA3F-0771-E011-80C4-001D09F26C5C.root
      000/163/376/16A704EB-F570-E011-BE7E-001D09F251BD.root
      000/163/375/A04F391B-F070-E011-8F7F-001617C3B66C.root
      000/163/374/FEE0E6EB-E170-E011-A9C1-001D09F244BB.root
      000/163/374/F0EB638B-C870-E011-9FB5-001D09F25401.root
      000/163/374/DAED269F-1471-E011-B4FD-001617E30D00.root
      000/163/374/D24759E9-E170-E011-B244-001D09F2546F.root
      000/163/374/D014AE83-D270-E011-8058-001D09F24D8A.root
      000/163/374/C459732A-D370-E011-86AF-001D09F29169.root
      000/163/374/AAE8CD3C-CE70-E011-91B8-001D09F276CF.root
      000/163/374/A611F741-D570-E011-BCE2-003048F118DE.root
      000/163/374/9063A8F6-7272-E011-9769-003048D2C0F0.root
      000/163/374/90178A92-D470-E011-BD83-003048F11C28.root
      000/163/374/8ABA0558-C470-E011-B566-003048D2C108.root
      000/163/374/720EE0E4-DF70-E011-86F9-003048F117B4.root
      000/163/374/62A500E8-DA70-E011-BF82-003048F1C836.root
      000/163/374/3E858886-C670-E011-9EFA-001D09F24498.root
      000/163/374/14CAC982-D270-E011-A8C2-001D09F24600.root
      000/163/372/B870A7CF-8372-E011-8791-0030487CD840.root
      000/163/372/6A274170-B070-E011-B5A2-001D09F2915A.root
      000/163/371/EE9EFD93-7872-E011-94F1-001617C3B5E4.root
      000/163/371/B4D51C3B-AD70-E011-964C-0030487A1990.root
      000/163/371/A2327AD1-AA70-E011-AD7D-0030486733B4.root
      000/163/371/A07539A4-AF70-E011-808D-001D09F24399.root
      000/163/371/94E7FDA1-B470-E011-AC7D-0030487A1884.root
      000/163/371/8818FDA5-AF70-E011-933C-001D09F2426D.root
      000/163/371/686B1A55-B070-E011-AE64-0030487CD716.root
      000/163/371/3EA7BD54-B070-E011-A8E5-0030487CD6B4.root
      000/163/370/B0A4FFBF-8970-E011-8D8C-001617C3B654.root
      000/163/370/AE59EE0F-B170-E011-AD8C-001D09F232B9.root
      000/163/370/9667CF30-B470-E011-BD72-0030487C8E02.root
      000/163/370/926BADEA-8372-E011-8605-001D09F24353.root
      000/163/369/FA67A91C-B270-E011-926A-003048D37560.root
      000/163/369/F0433F0D-C870-E011-B772-003048F11C28.root
      000/163/369/7858EF10-B470-E011-87DE-0030487CD7B4.root
      000/163/368/54F89C18-4270-E011-9846-0030487CD6D8.root
      000/163/358/804B2B43-B970-E011-8E3F-001D09F242EA.root
      000/163/340/F0DC3C88-3970-E011-A10B-001617E30CD4.root
      000/163/340/E23F0F4D-4370-E011-B771-0030487CD7EA.root
      000/163/340/CE244A77-0B72-E011-956C-003048F118AA.root
      000/163/340/C648D575-2B70-E011-929F-003048CFB40C.root
      000/163/340/7AE32078-3470-E011-B80C-0030487CBD0A.root
      000/163/340/4CA3B9C6-3370-E011-9D3A-001617E30F4C.root
      000/163/340/224728A6-2570-E011-9708-0030487CD7EE.root
      000/163/340/10B82062-5870-E011-AFB5-0030486780A8.root
      000/163/339/F4E034EB-2170-E011-B8A2-001617E30D52.root
      000/163/339/AE8F9B2C-7F70-E011-8A4A-0030487A1884.root
      000/163/339/8693F25C-2670-E011-A5E7-003048F110BE.root
      000/163/338/6E0EA318-1B70-E011-B4EE-0030487CF41E.root
      000/163/338/4E68E45A-2170-E011-A658-0030487C6090.root
      000/163/338/04742A7C-C070-E011-8618-003048D375AA.root
      000/163/337/F8E5A568-1170-E011-B9F8-003048F1C832.root
      000/163/337/F4C0B21F-1270-E011-87C4-001617C3B5F4.root
      000/163/337/EC0F49FF-0E70-E011-97CA-001D09F24353.root
      000/163/337/C2C51F27-0C70-E011-9523-001617E30CD4.root
      000/163/337/A6E4DFEE-2170-E011-A156-001617DBD472.root
      000/163/337/604BFD84-0770-E011-B664-001D09F231C9.root
      000/163/337/4CF6DFBF-C570-E011-9302-001D09F29146.root
      000/163/337/2A609DB1-1A70-E011-BFE4-001D09F2A690.root
      000/163/334/FEC0B4E3-F570-E011-9761-001D09F29619.root
      000/163/334/D488863E-6370-E011-B698-003048F1C424.root
      000/163/334/D04839BE-1A70-E011-884F-003048F01E88.root
      000/163/334/9CC7CEE0-DB6F-E011-AFA8-003048F024C2.root
      000/163/334/9C028B7F-DC6F-E011-9F92-003048F117B4.root
      000/163/334/8AEF5881-DC6F-E011-8C80-001D09F242EA.root
      000/163/334/8816ECE6-0E70-E011-9A35-003048F1183E.root
      000/163/334/66B34AD7-1270-E011-B131-003048D2C0F0.root
      000/163/334/5E5511EF-0870-E011-B54D-001D09F29849.root
      000/163/334/485588B5-FF6F-E011-B8C5-003048D2C0F0.root
      000/163/334/0A2BC7AC-0E70-E011-B108-0030487CD7E0.root
      000/163/333/C0FA78A7-0470-E011-BD66-003048D2BE06.root
      000/163/333/665DB648-4F70-E011-84E5-003048F117EC.root
      000/163/332/F815360E-0670-E011-93CA-0030487C90EE.root
      000/163/332/F42E10F8-1D70-E011-89AF-003048D37538.root
      000/163/332/F05D1228-6972-E011-A5F1-001D09F2424A.root
      000/163/332/C805A857-FE6F-E011-94F2-0030487C6062.root
      000/163/332/BC505FF4-F76F-E011-9CC3-001D09F282F5.root
      000/163/332/BA0ABE06-3870-E011-B9B9-0030487CD6F2.root
      000/163/332/B67FC994-2370-E011-A1EF-001D09F253FC.root
      000/163/332/B262E594-2370-E011-A5C0-001D09F28F0C.root
      000/163/332/8035116E-2D70-E011-9315-0030487CD716.root
      000/163/332/729F2C45-1A71-E011-BEB8-001617DBD556.root
      000/163/332/666783AD-E570-E011-B8C5-003048D2BF1C.root
      000/163/332/5CF59EDC-2170-E011-9A8D-003048F1C420.root
      000/163/332/4E20BCA3-C66F-E011-839D-001D09F24D67.root
      000/163/332/46BB9AF6-2470-E011-8996-001617DBCF90.root
      000/163/332/2CC9432B-1270-E011-85C4-000423D94494.root
      000/163/332/2AD44BF8-C56F-E011-B907-001D09F2910A.root
      000/163/332/22C8E5F6-1D70-E011-9012-003048F117B6.root
      000/163/332/20C2D694-2370-E011-914C-0019B9F72D71.root
      000/163/332/2032B4CD-2E70-E011-843F-003048F01E88.root
      000/163/308/B8465E3E-D16F-E011-9C6F-001D09F2447F.root
      000/163/302/82E73FC1-A76F-E011-A8AD-0030486780B4.root
      000/163/302/3E0BD818-7D70-E011-876E-0030487D1BCC.root
      000/163/302/16553BC3-586F-E011-B302-001D09F2438A.root
      000/163/301/EA6E1BB2-4B6F-E011-8145-003048F1C836.root
      000/163/301/BA22CBCA-456F-E011-A807-003048F117B4.root
      000/163/301/845C8B70-8F70-E011-B55C-001D09F23A84.root
      000/163/300/FC5FFA8B-3A6F-E011-88E0-001617DBCF6A.root
      000/163/300/D8B203D5-776F-E011-A077-001D09F2426D.root
      000/163/300/9E7F775C-7570-E011-9096-003048F117B4.root
      000/163/300/8A77C905-516F-E011-BBE2-003048F1183E.root
      000/163/300/80EEEA3E-436F-E011-8ACC-001D09F24F65.root
      000/163/300/8039BA97-4D6F-E011-95E5-003048F1183E.root
      000/163/300/2A724C25-5F6F-E011-AC76-001D09F29849.root
      000/163/300/1C60589F-626F-E011-8BB4-001617DBCF6A.root
      000/163/300/14C2E9CB-456F-E011-AC1A-0030487A322E.root
      000/163/300/12E73DF0-3B6F-E011-81AC-000423D9A2AE.root
      000/163/300/0443BFCC-766F-E011-B423-0030487CD710.root
      000/163/297/D6510AB7-566F-E011-AAE3-001D09F2546F.root
      000/163/297/9C841787-3D71-E011-9DE4-001D09F2438A.root
      000/163/297/48475FFA-6F6F-E011-84F6-001D09F244BB.root
      000/163/297/004EB90D-C16F-E011-A02A-0030487C7828.root
      000/163/296/FEBC635B-2A6F-E011-B8EB-003048F1C424.root
      000/163/296/EA8B3ECE-2B6F-E011-B24B-001617DBCF6A.root
      000/163/296/CC34CC12-6A6F-E011-85EF-001D09F2305C.root
      000/163/296/A4AAD948-AF70-E011-983F-001617E30D0A.root
      000/163/296/985DE64E-6A6F-E011-84BF-001617C3B6E2.root
      000/163/296/88FEF3F2-0E6F-E011-ABFB-001D09F297EF.root
      000/163/296/82F39649-7B6F-E011-B5DC-001D09F2906A.root
      000/163/296/6480FBB6-5D6F-E011-A988-001D09F2AD4D.root
      000/163/296/643D5E43-2F6F-E011-ADEA-001D09F23A20.root
      000/163/296/600A3B05-226F-E011-A665-003048D374F2.root
      000/163/296/261739C3-7E6F-E011-A43B-003048D375AA.root
      000/163/296/18416F48-F76F-E011-8B64-001D09F2A690.root
      000/163/289/A65F06C3-D56E-E011-B9B4-00304879FC6C.root
      000/163/289/9603C47C-CA6E-E011-993F-001617E30F4C.root
      000/163/289/3C9DC179-D66E-E011-96E0-001D09F2B30B.root
      000/163/289/289D8989-D16E-E011-B062-001617E30D0A.root
      000/163/289/066A134F-CD6E-E011-A218-001617E30CD4.root
      000/163/289/00797419-5472-E011-9071-001D09F24F65.root
      000/163/286/EE2EB91C-116F-E011-ACE1-001D09F290BF.root
      000/163/286/D0E87FFB-156F-E011-85C5-003048D2BBF0.root
      000/163/286/AEDD4F1D-7E6F-E011-B4B6-001D09F290CE.root
      000/163/286/5610F8D8-266F-E011-A717-001617C3B778.root
      000/163/286/164950F5-D26E-E011-8C23-001617C3B6DE.root
      000/163/275/EE661AAB-446E-E011-9E2A-003048D37538.root
      000/163/270/D8FD7BCD-A36E-E011-AD33-001617DBD472.root
      000/163/270/D427264E-996E-E011-9749-001617C3B6E2.root
      000/163/270/D22A36DA-976E-E011-B6C6-0019B9F72D71.root
      000/163/270/CC4F4ADB-906E-E011-98A3-000423D94E70.root
      """.split()
    ]
    jobOptions.globalTag = "GR_R_311_V2::All"
    jobOptions.reportEvery = 1
    jobOptions.isRealData = True
    jobOptions.use35XInput = False
    jobOptions.wantSummary = False
    jobOptions.skimType = "MuonPhoton"

  # end of testRealData options <-----------------------------------

  elif options.jobType == "testJetRealData":
    jobOptions.maxEvents = -1
    jobOptions.inputFiles = [
      "/store/data/Run2010B/Jet/RECO/PromptReco-v2/000/" +
      file for file in """
        147/757/1428A995-A4D6-DF11-8499-0030487C2B86.root
        147/757/360DEEC4-A1D6-DF11-9680-003048F11CF0.root
        147/757/748A647E-A2D6-DF11-AF11-001617C3B6CE.root
        147/757/B8DDD485-A2D6-DF11-9276-001617DBCF6A.root
        147/757/D4F5A10B-ADD6-DF11-A99F-0030487C7828.root
        147/755/0E36619B-98D6-DF11-98B4-001D09F2A465.root
        147/755/3E51D9A2-8CD6-DF11-9A05-000423D9997E.root
        147/755/5C885813-98D6-DF11-AA8A-001617E30CC8.root
        147/755/6063353F-97D6-DF11-A69C-001D09F24664.root
        147/755/B6D62927-9CD6-DF11-966D-003048F0258C.root
        147/755/DC73547A-8DD6-DF11-849E-0030487C8CB8.root
        147/755/E865F520-98D6-DF11-8BAC-001617C3B6CE.root
      """.split()
    ]
    jobOptions.globalTag = "GR_R_311_V2::All"
    jobOptions.reportEvery = 1
    jobOptions.isRealData = True
    jobOptions.use35XInput = False
    jobOptions.wantSummary = False
    jobOptions.skimType = "Jet"
    jobOptions.hltProcessName = "HLT"
  # end of testRealData options <-----------------------------------

  elif options.jobType == "testSummer10":
    jobOptions.inputFiles = [
        "rfio:/castor/cern.ch/user/z/zkliu/TestRECO/36x_ZJet_Madgraph_tauola_TestRECO.root"
        ]
    jobOptions.skimType = "ElectronPhoton"
    jobOptions.globalTag = "START38_V14::All"
    jobOptions.reportEvery = 1
    jobOptions.isRealData = False
    jobOptions.use35XInput = False
    jobOptions.wantSummary = True
    jobOptions.hltProcessName = "REDIGI36X"

  elif options.jobType == "testFall10":
    jobOptions.inputFiles = """
        /store/mc/Fall10/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/GEN-SIM-RECO/START38_V12-v2/0006/E2250407-B3E4-DF11-941F-E0CB4E29C4C6.root
        /store/mc/Fall10/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/GEN-SIM-RECO/START38_V12-v2/0006/6E58848F-87E4-DF11-81C0-001F2965D25E.root
        /store/mc/Fall10/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/GEN-SIM-RECO/START38_V12-v2/0005/CCE7C256-03E4-DF11-BD14-00261834B5BA.root
        /store/mc/Fall10/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/GEN-SIM-RECO/START38_V12-v2/0005/C8C7A3EB-ACE3-DF11-8D6D-00D0680B8852.root
        /store/mc/Fall10/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/GEN-SIM-RECO/START38_V12-v2/0005/A8CA5933-BDE3-DF11-961D-A4BADB3CEBCE.root
        /store/mc/Fall10/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/GEN-SIM-RECO/START38_V12-v2/0005/90E09BA7-AEE3-DF11-A22D-001EC9EAFEBE.root
        """.split()
    jobOptions.skimType = "ElectronPhoton"
    jobOptions.globalTag = "START38_V14::All"
    jobOptions.reportEvery = 1
    jobOptions.isRealData = False
    jobOptions.wantSummary = True
    jobOptions.hltProcessName = "HLT"

  elif options.jobType == "testWinter10":
    jobOptions.inputFiles =  ["/store/mc/Winter10/" + \
      "DYToMuMu_M-20_CT10_TuneZ2_7TeV-powheg-pythia/" + \
      "GEN-SIM-RECO/E7TeV_ProbDist_2010Data_BX156_START39_V8-v1/0033/" + \
      "FE75602D-4810-E011-B662-1CC1DE051038.root"
      ]
    jobOptions.skimType = "Dimuon"
    jobOptions.globalTag = "START39_V8::All"
    jobOptions.reportEvery = 1
    jobOptions.isRealData = False
    jobOptions.wantSummary = True
    jobOptions.hltProcessName = "REDIGI39X"

  elif options.jobType == "Winter10":
    jobOptions.inputFiles =  ["/store/mc/Winter10/" + \
      "DYToMuMu_M-20_CT10_TuneZ2_7TeV-powheg-pythia/" + \
      "GEN-SIM-RECO/E7TeV_ProbDist_2010Data_BX156_START39_V8-v1/0033/" + \
      "FE75602D-4810-E011-B662-1CC1DE051038.root"
      ]
    jobOptions.skimType = "Dimuon"
    jobOptions.globalTag = "START39_V8::All"
    jobOptions.reportEvery = 1000
    jobOptions.isRealData = False
    jobOptions.wantSummary = True
    jobOptions.hltProcessName = "REDIGI39X"

  elif options.jobType == "Dec22ReReco":
    jobOptions.inputFiles =  ["/store/data/Run2010B/Mu/RECO/" + \
      "Dec22ReReco_v1/0025/60BB3418-F60F-E011-8952-90E6BA0D09CB.root"
      ]
    jobOptions.skimType = "Dimuon"
    jobOptions.globalTag = "GR_R_39X_V6::All"
    jobOptions.reportEvery = 1000
    jobOptions.isRealData = True
    jobOptions.wantSummary = True
    jobOptions.hltProcessName = "HLT"

  elif options.jobType == "MUPHPromptReco36X":
    jobOptions.globalTag = "GR10_P_V7::All"
    jobOptions.isRealData = True
    jobOptions.use35XInput = False
    jobOptions.maxEvents = -1
    jobOptions.skimType = "Muon"
    jobOptions.wantSummary = True

  elif options.jobType == "EPHPromptReco36X":
    jobOptions.globalTag = "GR10_P_V7::All"
    jobOptions.isRealData = True
    jobOptions.use35XInput = False
    jobOptions.maxEvents = -1
    jobOptions.skimType = "Electron"
    jobOptions.wantSummary = True

  elif options.jobType == "JETPromptReco36X":
    jobOptions.globalTag = "GR10_P_V7::All"
    jobOptions.isRealData = True
    jobOptions.use35XInput = False
    jobOptions.maxEvents = -1
    jobOptions.skimType = "Jet"
    jobOptions.wantSummary = True

  elif options.jobType == "PHMETPromptReco36X":
    jobOptions.globalTag = "GR10_P_V7::All"
    jobOptions.isRealData = True
    jobOptions.use35XInput = False
    jobOptions.maxEvents = -1
    jobOptions.skimType = "PhotonMET"
    jobOptions.wantSummary = True

  elif options.jobType == "MUPHPromptReco38X":
    jobOptions.globalTag = "GR10_P_V9::All"
    jobOptions.isRealData = True
    jobOptions.use35XInput = False
    jobOptions.maxEvents = -1
    jobOptions.skimType = "Muon"
    jobOptions.wantSummary = True

  elif options.jobType == "MUPHPromptReco414":
    jobOptions.globalTag = "GR_R_311_V2::All"
    jobOptions.isRealData = True
    jobOptions.use35XInput = False
    jobOptions.maxEvents = -1
    jobOptions.skimType = "MuonPhoton"
    jobOptions.wantSummary = True
    jobOptions.isAOD=True

  elif options.jobType == "MUPHReReco414":
    jobOptions.globalTag = "GR_R_311_V2::All"
    jobOptions.isRealData = True
    jobOptions.use35XInput = False
    jobOptions.maxEvents = -1
    jobOptions.skimType = "MuonPhoton"
    jobOptions.wantSummary = True
    jobOptions.isAOD=True

  elif options.jobType == "MUPHMCSpring11":
    jobOptions.globalTag = "START311_V2:All"
    jobOptions.isRealData = False
    jobOptions.use35XInput = False
    jobOptions.maxEvents = -1
    jobOptions.skimType = "MuonPhoton"
    jobOptions.wantSummary = True
    jobOptions.isAOD=True

  elif options.jobType == "EPHPromptReco38X":
    jobOptions.globalTag = "GR10_P_V9::All"
    jobOptions.isRealData = True
    jobOptions.use35XInput = False
    jobOptions.maxEvents = -1
    jobOptions.skimType = "Electron"
    jobOptions.wantSummary = True

  elif options.jobType == "JETPromptReco38X":
    jobOptions.globalTag = "GR10_P_V9::All"
    jobOptions.isRealData = True
    jobOptions.use35XInput = False
    jobOptions.maxEvents = -1
    jobOptions.skimType = "Jet"
    jobOptions.wantSummary = True

  elif options.jobType == "PHMETPromptReco38X":
    jobOptions.globalTag = "GR10_P_V9::All"
    jobOptions.isRealData = True
    jobOptions.use35XInput = False
    jobOptions.maxEvents = -1
    jobOptions.skimType = "PhotonMET"
    jobOptions.wantSummary = True

  elif options.jobType == "MUPHReReco36X":
    jobOptions.globalTag = "GR_R_36X_V12B::All"
    jobOptions.isRealData = True
    jobOptions.use35XInput = False
    jobOptions.maxEvents = -1
    jobOptions.skimType = "Muon"
    jobOptions.wantSummary = True

  elif options.jobType == "EPHReReco36X":
    jobOptions.globalTag = "GR_R_36X_V12B::All"
    jobOptions.isRealData = True
    jobOptions.use35XInput = False
    jobOptions.maxEvents = -1
    jobOptions.skimType = "Electron"
    jobOptions.wantSummary = True

  elif options.jobType == "JETReReco36X":
    jobOptions.globalTag = "GR_R_36X_V12B::All"
    jobOptions.isRealData = True
    jobOptions.use35XInput = False
    jobOptions.maxEvents = -1
    jobOptions.skimType = "Jet"
    jobOptions.wantSummary = True

  elif options.jobType == "PHMETReReco36X":
    jobOptions.globalTag = "GR_R_36X_V12B::All"
    jobOptions.isRealData = True
    jobOptions.use35XInput = False
    jobOptions.maxEvents = -1
    jobOptions.skimType = "PhotonMET"
    jobOptions.wantSummary = True

  elif options.jobType == "MUPHReReco38X":
    jobOptions.globalTag = "GR_R_38X_V13A::All"
    jobOptions.isRealData = True
    jobOptions.use35XInput = False
    jobOptions.maxEvents = -1
    jobOptions.skimType = "Muon"
    jobOptions.hltPaths = ["HLT_Mu9","HLT_Mu11"]
    jobOptions.wantSummary = True

  elif options.jobType == "EPHReReco38X":
    jobOptions.globalTag = "GR_R_38X_V13A::All"
    jobOptions.isRealData = True
    jobOptions.use35XInput = False
    jobOptions.maxEvents = -1
    jobOptions.skimType = "Electron"
    jobOptions.hltPaths = ["HLT_Ele15_LW_L1R", "HLT_Ele15_SW_L1R"]
    jobOptions.wantSummary = True

  elif options.jobType == "JETReReco38X":
    jobOptions.globalTag = "GR_R_38X_V13A::All"
    jobOptions.isRealData = True
    jobOptions.use35XInput = False
    jobOptions.maxEvents = -1
    jobOptions.skimType = "Jet"
    jobOptions.wantSummary = True

  elif options.jobType == "PHMETReReco38X":
    jobOptions.globalTag = "GR_R_38X_V13A::All"
    jobOptions.isRealData = True
    jobOptions.use35XInput = False
    jobOptions.maxEvents = -1
    jobOptions.skimType = "PhotonMET"
    jobOptions.wantSummary = True

  elif options.jobType == "MUPHMC36XSpring10":
    jobOptions.globalTag = "START36_V10::All"
    jobOptions.isRealData = False
    jobOptions.use35XInput = True
    jobOptions.maxEvents = -1
    jobOptions.skimType = "Muon"
    jobOptions.hltPaths = ["HLT_Mu9","HLT_Mu11"]
    jobOptions.wantSummary = True
    jobOptions.hltProcessName = "REDIGI"

  elif options.jobType == "MUPHMC38XSpring10":
    jobOptions.globalTag = "START38_V12::All"
    jobOptions.isRealData = False
    jobOptions.use35XInput = True
    jobOptions.maxEvents = -1
    jobOptions.skimType = "Muon"
    jobOptions.hltPaths = ["HLT_Mu9","HLT_Mu11"]
    jobOptions.wantSummary = True
    jobOptions.hltProcessName = "REDIGI"

  elif options.jobType == "MUPHMC36XSummer10":
    jobOptions.globalTag = "START36_V10::All"
    jobOptions.isRealData = False
    jobOptions.use35XInput = False
    jobOptions.maxEvents = -1
    jobOptions.skimType = "Muon"
    jobOptions.hltPaths = ["HLT_Mu9","HLT_Mu11"]
    jobOptions.wantSummary = True
    jobOptions.hltProcessName = "REDIGI36X"

  elif options.jobType == "MUPHMC38XSummer10":
    jobOptions.globalTag = "START38_V12::All"
    jobOptions.isRealData = False
    jobOptions.use35XInput = False
    jobOptions.maxEvents = -1
    jobOptions.skimType = "Muon"
    jobOptions.hltPaths = ["HLT_Mu9","HLT_Mu11"]
    jobOptions.wantSummary = True
    jobOptions.hltProcessName = "REDIGI36X"

  elif options.jobType == "EPHMC38XSpring10":
    jobOptions.globalTag = "START38_V12::All"
    jobOptions.isRealData = False
    jobOptions.use35XInput = True
    jobOptions.maxEvents = -1
    jobOptions.skimType = "Electron"
    jobOptions.hltPaths = ["HLT_Ele15_LW_L1R", "HLT_Ele15_SW_L1R"]
    jobOptions.wantSummary = True
    jobOptions.hltProcessName = "REDIGI"

  elif options.jobType == "EPHMC38XSummer10":
    jobOptions.globalTag = "START38_V12::All"
    jobOptions.isRealData = False
    jobOptions.use35XInput = True
    jobOptions.maxEvents = -1
    jobOptions.skimType = "Electron"
    jobOptions.hltPaths = ["HLT_Ele15_LW_L1R", "HLT_Ele15_SW_L1R"]
    jobOptions.wantSummary = True
    jobOptions.hltProcessName = "REDIGI36X"

  elif options.jobType == "JETMC38X":
    jobOptions.globalTag = "START38_V12::All"
    jobOptions.isRealData = False
    jobOptions.use35XInput = True
    jobOptions.maxEvents = -1
    jobOptions.skimType = "Muon"
    jobOptions.hltPaths = ["HLT_Mu9","HLT_Mu11"]
    jobOptions.wantSummary = True
    jobOptions.hltProcessName = "REDIGI"

  elif options.jobType == "PHMETMC38X":
    jobOptions.globalTag = "START38_V12::All"
    jobOptions.isRealData = False
    jobOptions.use35XInput = True
    jobOptions.maxEvents = -1
    jobOptions.skimType = "Muon"
    jobOptions.hltPaths = ["HLT_Mu9","HLT_Mu11"]
    jobOptions.wantSummary = True
    jobOptions.hltProcessName = "REDIGI"

  elif options.jobType == "MC36X":
    jobOptions.globalTag = "START3X_V26::All"
    jobOptions.isRealData = False
    jobOptions.use35XInput = True
    jobOptions.maxEvents = -1

  elif options.jobType != "":
    raise RuntimeError, "Unknown jobType option `%s'" % options.jobType

  ## Set all the non-default option values equal to jobOptions to preserve options set
  ##+ in the cfg file
  for name, value in options._singletons.items() + options._lists.items():
    if value != getattr(defaultOptions, name):
      ## If it's a list, clear it first
      if name in options._lists.keys(): jobOptions.clearList(name)
      setattr(jobOptions, name, value)

  jobOptions.parseArguments()

  ## Set all the options equal to jobOptions
  for name, value in jobOptions._singletons.items() + jobOptions._lists.items():
    ## If it's a list, clear it first
    if name in options._lists.keys(): options.clearList(name)
    setattr(options, name, value)

  ## Remove duplicated input files (from e.g. using the command line
  ##+ option inputFiles_load=list.txt
  duplicates = []
  for index in range(len(options.inputFiles)):
    file = options.inputFiles[index]
    if file in options.inputFiles[index+1:]:
      ## Found a duplicated file, store its index
      duplicates.append(options.inputFiles.index(file, index+1))
  ## Now remove the duplicates in the reverse order to keep the original index
  ##+ association
  duplicates.reverse()
  for index in duplicates: del options.inputFiles[index]

# applyMultiOptionTag(options) <----------------------------------------------


