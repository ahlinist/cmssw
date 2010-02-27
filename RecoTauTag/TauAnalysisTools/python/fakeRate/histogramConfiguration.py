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

def makeCuts(denominator="1"):
    " Build the set of cuts, given a denominator defintion "
    cuts = {}

    # Denominator defined by standard EWK skim defintion
    denominator = TauNtupleSelection(denominator)
    cuts['denominator'] = denominator

    #Individual discriminators
    cuts['ByLeadTrackPt']    =  TauNtupleSelection("$ByLeadTrackPt")*denominator
    cuts['ByLeadPionPt']     =  TauNtupleSelection("$ByLeadPionPt")*denominator
    cuts['ByIsolation']      =  TauNtupleSelection("$ByIsolation")*denominator
    cuts['ByTrackIsolation'] =  TauNtupleSelection("$ByTrackIsolation")*denominator
    cuts['ByEcalIsolation']  =  TauNtupleSelection("$ByEcalIsolation")*denominator
    cuts['AgainstMuon']      =  TauNtupleSelection("$AgainstMuon")*denominator
    cuts['AgainstElectron']  =  TauNtupleSelection("$AgainstElectron")*denominator
    cuts['ByCharge']         =  TauNtupleSelection("abs($charge) == 1")*denominator
    cuts['ByNumTracks']      =  TauNtupleSelection("$nCharged == 1 || $nCharged == 3")*denominator
    cuts['ByTaNCfrOne']      =  TauNtupleSelection('$ByTaNCfrOne')*denominator
    cuts['ByTaNCfrHalf']     =  TauNtupleSelection('$ByTaNCfrHalf')*denominator
    cuts['ByTaNCfrQuarter']  =  TauNtupleSelection('$ByTaNCfrQuarter')*denominator
    cuts['ByTaNCfrTenth']    =  TauNtupleSelection('$ByTaNCfrTenth')*denominator

    # Combo discriminators
    cuts['ByChargeAndTracks']             =  cuts['ByCharge']*cuts['ByNumTracks']

    cuts['ByTrackIsolationSeq']           = cuts['ByTrackIsolation']*denominator
    cuts['ByEcalIsolationSeq']            = cuts['ByEcalIsolation']*cuts['ByTrackIsolationSeq']
    cuts['ByNTracksSeq']                  = cuts['ByNumTracks']*cuts['ByEcalIsolationSeq']
    cuts['ByChargeSeq']                   = cuts['ByCharge']*cuts['ByNTracksSeq']

    cuts['ByStandardChain']               =  cuts['ByLeadTrackPt']*cuts['ByIsolation']*cuts['ByChargeAndTracks']
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

    return cuts
