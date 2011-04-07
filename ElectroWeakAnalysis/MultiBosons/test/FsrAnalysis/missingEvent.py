from ROOT import *

def scan(tree, names, quantities):
    for x in quantities.split():
      expr = ":".join([name + "." + x + "()" for name in names.split()])
      print "==", expr, "=="
      tree.Scan(expr)

def main():
    gSystem.Load("libFWCoreFWLite.so")
    AutoLibraryLoader.enable()

#     file1 = TFile("/tmp/veverka/InterestingEvents.root")
#     file2 = TFile("/tmp/veverka/merge_15_1_8jf.root")
    file2 = TFile("/mnt/tier2/store/user/veverka/Mu/" + \
                  "Run2010B-DiLeptonMu-Dec22ReReco_v2_DimuonVGammaSkim_v4/" + \
                  "f81f6ec801d687f509099b4fa4a3c90c/merge_15_1_8jf.root")

    ## Preselection
    Events.Draw(">>elist", "EventAuxiliary.id().event()==105502694", "entrylist")
    Events.SetEntryList(gDirectory.Get("elist"))

    ## Aliases
    Events.SetAlias("reco", "recoPhotons_photons__RECO.obj")
    Events.SetAlias("rereco", "recoPhotons_photons__PAT.obj")
    Events.SetAlias("pat", "patPhotons_cleanPatPhotonsTriggerMatch__PAT.obj")

#     scan(Events, "reco rereco pat", "energy pt eta phi")
#     scan(Events, "reco rereco pat", "vx vy vz")
#     scan(Events, "reco rereco pat", "caloPosition().x caloPosition().y caloPosition().z")

#     scan(Events, "reco rereco", "energy pt eta phi")
#     scan(Events, "reco rereco", "vx vy vz")
#     scan(Events, "reco rereco", "caloPosition().x caloPosition().y caloPosition().z")
    scan(Events, "reco rereco", "r9")



if __name__ == "__main__":
    import user
    main()