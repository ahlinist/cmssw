# Define our standard binning
#standard_bins = {
#    'x_bins' : [0, 10, 20, 30, 50, 80, 120], # pt
#    'y_bins' : [0, 0.5, 1.0, 1.2, 1.5, 2.0, 2.5], # eta
#    'z_bins' : [0, 0.01, 0.02, 0.03, 0.04] # jet width
#}

standard_bins = {
    'x_bins' : [0, 10, 20, 25, 30, 40, 60, 120], # pt                                                                                                                                                        
    'y_bins' : [0, 0.5, 1.0, 1.2, 1.5, 2.0, 2.5], # eta                                                                                                                                                      
    'z_bins' : [0, 0.0125, 0.025, 0.05, 0.075, 0.10, 0.15, 0.20, 0.30, 0.4 ] # jet width    
}

#denominator = "$ByLeadTrackPt && abs($eta) < 2.1 && $pt > 20 && $AgainstMuon"
denominator = "$ByLeadTrackPt && abs($eta) < 2.1 && $pt > 20"

sources = {}
# Define our sources
sources['ztt'] = {
    # Source defintion (crab_output, xSection (pb), filter eff)
    'sources': {'Ztautau/res': (1944, 1.0)},
    'output_file': "histograms/ztt_histograms.root",
    'ntuple_name': "shrinkingConePFTau",
    'ntuple_setup': lambda ntuple: ntuple.set_collection("matched"),
}
sources['ztt'].update(standard_bins)

sources['ppmux'] = {
    'sources': {'InclusiveMu15/res': ((0.5091*1e9), 2.881e-4)}, # 0.5091 mb 
    'output_file': 'histograms/ppmux_histograms.root',
    'ntuple_name': 'shrinking',
    'ntuple_setup': lambda ntuple: ntuple.set_collection("unmatched"),
}
sources['ppmux'].update(standard_bins)

# Case for highest pt jet probe
sources['dijet_first'] = {
    'sources': {
        'Dijet20_30/res': (400982000., 1.0), # in pb
        'Dijet30_50/res': (94702500., 1.0),
        'Dijet50_80/res': (12195900., 1.0),
        'Dijet80_120/res': (1617240., 1.0),
        'Dijet120_170/res': (255987., 1.0) },
    'output_file': 'histograms/dijet_highpt_histograms.root',
    'ntuple_name': 'highestPtJet',
    'ntuple_setup': lambda ntuple: ntuple.set_collection('all'),
}
sources['dijet_first'].update(standard_bins)

# Case for second pt jet probe
sources['dijet_second'] = {
    'sources': {
        'Dijet20_30/res': (400982000., 1.0), # in pb
        'Dijet30_50/res': (94702500., 1.0),
        'Dijet50_80/res': (12195900., 1.0),
        'Dijet80_120/res': (1617240., 1.0),
        'Dijet120_170/res': (255987., 1.0) },
    'output_file': 'histograms/dijet_secondpt_histograms.root',
    'ntuple_name': 'secondPtJet',
    'ntuple_setup': lambda ntuple: ntuple.set_collection('all'),
}
sources['dijet_second'].update(standard_bins)

sources['wjets'] = {
    'sources': {'WJets/res': (30000., 0.36)},
    'output_file': 'histograms/wjets_histograms.root',
    'ntuple_name': 'shrinking',
    'ntuple_setup': lambda ntuple: ntuple.set_collection("unmatched"),
}
sources['wjets'].update(standard_bins)
