class TauNtupleSelection(object):
   def __init__(self, cut):
      self.cut = cut
   def negate(self):
      return TauNtupleSelection("!(%s)" % self.cut)
   def __add__(self, other):
      return TauNtupleSelection("(%s) || (%s)" % (self.cut, other.cut))
   def __mul__(self, other):
      return TauNtupleSelection("(%s) && (%s)" % (self.cut, other.cut))
   def __str__(self):
      return self.cut

cuts = {}

#Individual discriminators
cuts['ByLeadTrackPt']    =  TauNtupleSelection("$ByLeadTrackPt")
cuts['ByLeadPionPt']     =  TauNtupleSelection("$ByLeadPionPt")
cuts['ByIsolation']      =  TauNtupleSelection("$ByIsolation")
cuts['AgainstMuon']      =  TauNtupleSelection("$AgainstMuon")
cuts['AgainstElectron']  =  TauNtupleSelection("$AgainstElectron")
cuts['ByCharge']         =  TauNtupleSelection("$charge == 1")
cuts['ByNumTracks']      =  TauNtupleSelection("$nTrks == 1 || $nTrks == 3")
cuts['ByTaNCfrOne']      =  TauNtupleSelection('$ByTaNCfrOne')
cuts['ByTaNCfrHalf']     =  TauNtupleSelection('$ByTaNCfrHalf')
cuts['ByTaNCfrQuarter']  =  TauNtupleSelection('$ByTaNCfrQuarter')
cuts['ByTaNCfrTenth']    =  TauNtupleSelection('$ByTaNCfrTenth')

# Combo discriminators
cuts['ByChargeAndTracks']             =  cuts['ByCharge']*cuts['ByNumTracks']

cuts['ByStandardChain']               =  cuts['ByLeadPionPt']*cuts['ByIsolation']*cuts['ByChargeAndTracks']
cuts['ByStandardChainNoMuon']         =  cuts['ByStandardChain']*cuts['AgainstMuon']
cuts['ByStandardChainNoElectron']     =  cuts['ByStandardChain']*cuts['AgainstElectron']

cuts['ByTaNCChainOne']                =  cuts['ByChargeAndTracks']*cuts['ByTaNCfrOne']
cuts['ByTaNCChainHalf']               =  cuts['ByChargeAndTracks']*cuts['ByTaNCfrHalf']
cuts['ByTaNCChainQuarter']            =  cuts['ByChargeAndTracks']*cuts['ByTaNCfrQuarter']
cuts['ByTaNCChainTenth']              =  cuts['ByChargeAndTracks']*cuts['ByTaNCfrTenth']

cuts['ByTaNCChainOneNoMuon']          =  cuts['ByChargeAndTracks']*cuts['ByTaNCfrOne']*cuts['AgainstMuon']
cuts['ByTaNCChainHalfNoMuon']         =  cuts['ByChargeAndTracks']*cuts['ByTaNCfrHalf']*cuts['AgainstMuon']
cuts['ByTaNCChainQuarterNoMuon']      =  cuts['ByChargeAndTracks']*cuts['ByTaNCfrQuarter']*cuts['AgainstMuon']
cuts['ByTaNCChainTenthNoMuon']        =  cuts['ByChargeAndTracks']*cuts['ByTaNCfrTenth']*cuts['AgainstMuon']

cuts['ByTaNCChainOneNoElectron']      =  cuts['ByChargeAndTracks']*cuts['ByTaNCfrOne']*cuts['AgainstElectron']
cuts['ByTaNCChainHalfNoElectron']     =  cuts['ByChargeAndTracks']*cuts['ByTaNCfrHalf']*cuts['AgainstElectron']
cuts['ByTaNCChainQuarterNoElectron']  =  cuts['ByChargeAndTracks']*cuts['ByTaNCfrQuarter']*cuts['AgainstElectron']
cuts['ByTaNCChainTenthNoElectron']    =  cuts['ByChargeAndTracks']*cuts['ByTaNCfrTenth']*cuts['AgainstElectron']
