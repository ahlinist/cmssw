import FWCore.ParameterSet.Config as cms

from math import pi

## Define some utilities to declare bins easily
##--- n bins spaced evenly in range [min, max[
def _nBins(n,min,max): return cms.vdouble(*[min + (max-min)/float(n)*i for i in range(n+1)])
##--- n bins of size 1 centered on 0, 1, ... (n-1)
def _iBins(n): return _nBins(n,-0.5,n-0.5)
##--- even bins of size delta from min to max
def _evenBins(min,max,delta): 
    ret = cms.vdouble(min)
    x = min
    while x < max - 1e-4: # need a small hint otherwise for some numbers it will overstep due to numerical resolution
        x += delta
        ret.append(x)
    return ret 

def makeInclusiveMuonPlots(rebinFactor=1):
    return cms.PSet(        # ---- Muons per event ----
        nMuonsBins = _iBins(7),
        # ---- Kinematics ----
        ptBins = _evenBins( 0, 30, 0.25 * rebinFactor),
        pBins  = _evenBins( 0, 50, 0.5  * rebinFactor),
        etaBins = _evenBins( -2.6, 2.6, 0.2 * rebinFactor),
        phiBins = _evenBins(-pi, pi, pi/16 * rebinFactor),
        chargeBins = cms.vdouble(-2,0,2),
        qptBins = _evenBins( -30, 30, 0.25 * rebinFactor),
        qpBins  = _evenBins( -50, 50, 0.5  * rebinFactor),
        # ---- Vertex ----
        dxyFineBins = _evenBins(-0.2, 0.2, 0.005), #  50um
        dzFineBins  = _evenBins(-0.5, 0.5, 0.010), # 100um
        dxyCoarseBins = _evenBins( -4,  4, 0.1), # 1mm
        dzCoarseBins  = _evenBins(-10, 10, 0.1), # 1mm
        tipBins  = _evenBins(0, 0.1, 0.0025), #  25um
        ip3dBins = _evenBins(0, 0.2, 0.0050), #  50um
        tipSigBins  = _evenBins(0, 10, 0.25),
        ip3dSigBins = _evenBins(0, 15, 0.25),
        # ---- Tracks ----
        pixelHitsBins       = _iBins(8),
        pixelLayersBins     = _iBins(5),
        trackerHitsBins     = _nBins(33,0,33),
        trackerLostHitsBins = _nBins(10,0,10),
        muonHitsBins        = _nBins(25,0,50),
        muonBadHitsBins     = _nBins(20,0,20),
        globalHitsBins      = _nBins(40,0,80),
        trackerChi2nBins = _evenBins(0, 10, 0.2 * rebinFactor),
        muonChi2nBins    = _evenBins(0, 10, 0.2 * rebinFactor),
        globalChi2nBins  = _evenBins(0, 10, 0.2 * rebinFactor),
        # ---- Isolation ----
        isolationBins = _evenBins(0,  5, .25  * rebinFactor),
        relIsoBins    = _evenBins(0, .5, .025 * rebinFactor),
        # ---- Muon ID ----
        muonStationsBins   = _iBins(5), 
        segmentMatchesBins = _iBins(8),
        segmentCompatBins  = _nBins(20/rebinFactor, 0, 1.001), 
        caloCompatBins     = _nBins(20/rebinFactor, 0, 1.001),
        # ---- Time ----
        timeAtIpInOutBins    = _evenBins(-100., 100., 5.0),
        timeAtIpInOutSigBins = _nBins(50/rebinFactor, -25, 25),
        # ---- Phi at MB/ME 1 Surface ----
        trkPhiAtSurfaceBins = _nBins(37,-3.23,3.23)
    )

inclusiveMuonPlots = cms.EDAnalyzer("InclusiveMuonPlots",
    makeInclusiveMuonPlots(),
    muons     = cms.InputTag('muons'),
    selection = cms.string("isTrackerMuon && muonID('TMLastStationAngTight')"),
    onlyLeadingMuon = cms.bool(False),
    primaryVertices = cms.InputTag("offlinePrimaryVertices"),
    #normalization   = cms.InputTag("countCollisionEvents"), ## read normalization from output of cms.EDProducer("EventCountProducer") 
)

