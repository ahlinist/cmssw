from array import array
import itertools
import FWCore.ParameterSet.Config as cms

tracks = ['Global', 'StAlone', 'TkOnly', 'TPFMS', 'Picky', 'TMR', 'SS', 'PMC', 'StaGlb', 'StaTko']
positions = ['upper', 'lower']

class Bin:
    min_pt  = 0.
    max_pt  = 2000.
    min_eta = 0.
    max_eta = 2.5
    min_phi = -4.
    max_phi = 4.
    min_dxy = 0.
    max_dxy = 100.
    min_dz  = 0.
    max_dz  = 100.
    run_bin = 0

    use_by_bin = True
    
    def __init__(self, name, **kwargs):
        self.name = name
        for k,v in kwargs.iteritems():
            setattr(self, k, v)

    def pset(self):
        pset = cms.PSet()
        # let items in self override those in cls
        for k,v in itertools.chain(self.__class__.__dict__.iteritems(), self.__dict__.iteritems()):
            if hasattr(pset, k):
                nv = v # PSets only take the first assignment if they
                       # are cms types. To replace, they must be
                       # native python types.
            else:
                if type(v) == type(0) and k == 'run_bin':
                    nv = cms.uint32(v)
                elif type(v) == type(0.):
                    nv = cms.double(v)
                elif type(v) == type(()):
                    nv = cms.vdouble(*v)
                elif type(v) == type(''):
                    nv = cms.string(v)
                else:
                    #print 'ignoring',k,v
                    continue
            setattr(pset, k, nv)
        return pset
        

def make_bins(bin_by, bin_max=None, return_raw=False, is_mc=False, no_bin_deletion=False, bin_350=False, wide_all_for_stalone=False, min_eta=None, max_eta=None, min_phi=None, max_phi=None):
    if bin_by == 'pt':
        bins = [
            Bin('pTall',      scale_factor=1.    , use_by_bin=False,          diff_scales=(  3, 0.005,  0.003, 0.003, 0.05, 0.1 ),   res_scales=(0.1,  0.1,  0.005, 0.005, 2, 2),   pull_scales=( 5,  5,  5,  5, 3.5, 3.5)),
            Bin('pT010',      scale_factor=7.27  , min_pt=0,    max_pt=10,    diff_scales=(  1, 0.007,  0.01,  0.004, 0.05, 0.1 ),   res_scales=(0.1,  0.1,  0.005, 0.005, 2, 2),   pull_scales=( 5,  5,  5,  5, 3.5, 3.5)),
            Bin('pT1020',     scale_factor=13.792, min_pt=10,   max_pt=20,    diff_scales=(  2, 0.005,  0.01,  0.003, 0.05, 0.1 ),   res_scales=(0.1,  0.1,  0.005, 0.005, 2, 2),   pull_scales=( 5,  5,  5,  5, 3.5, 3.5)),
            Bin('pT2030',     scale_factor=24.044, min_pt=20,   max_pt=30,    diff_scales=(  5, 0.003,  0.01,  0.003, 0.05, 0.1 ),   res_scales=(0.1,  0.1,  0.005, 0.005, 2, 2),   pull_scales=( 5,  5,  5,  5, 3.5, 3.5)),
            Bin('pT3040',     scale_factor=34.125, min_pt=30,   max_pt=40,    diff_scales=(  7, 0.0025, 0.01,  0.002, 0.05, 0.1 ),   res_scales=(0.1,  0.1,  0.005, 0.005, 2, 2),   pull_scales=( 5,  5,  5,  5, 3.5, 3.5)),
            Bin('pT4050',     scale_factor=44.166, min_pt=40,   max_pt=50,    diff_scales=( 10, 0.0025, 0.01,  0.002, 0.05, 0.1 ),   res_scales=(0.15, 0.1,  0.005, 0.005, 2, 2),   pull_scales=( 5,  5,  5,  5, 5,   5  )),
            Bin('pT5075',     scale_factor=60.380, min_pt=50,   max_pt=75,    diff_scales=( 12, 0.0025, 0.01,  0.002, 0.05, 0.1 ),   res_scales=(0.15, 0.15, 0.005, 0.005, 2, 2),   pull_scales=( 5,  5,  5,  5, 5,   5  )),
            Bin('pT75100',    scale_factor=85.751, min_pt=75,   max_pt=100,   diff_scales=( 25, 0.0025, 0.01,  0.002, 0.05, 0.1 ),   res_scales=(0.25, 0.2,  0.005, 0.005, 2, 2),   pull_scales=( 5,  5,  5,  5, 5,   5  )),
            Bin('pT100200',   scale_factor=135.94, min_pt=100,  max_pt=200,   diff_scales=(100, 0.0025, 0.01,  0.002, 0.05, 0.1 ),   res_scales=(0.4,  0.4,  0.005, 0.005, 2, 2),   pull_scales=( 6,  6,  6,  6, 5,   5  )),
            Bin('pT200350',   scale_factor=256.07, min_pt=200,  max_pt=350,   diff_scales=(500, 0.0025, 0.005, 0.002, 0.05, 0.07),   res_scales=(0.6,  0.6,  0.005, 0.005, 2, 2),   pull_scales=(10, 10, 10, 10, 5,   5  )),
            Bin('pT3502000',  scale_factor=579.36, min_pt=350,  max_pt=2000,  diff_scales=(500, 0.0025, 0.005, 0.002, 0.05, 0.07),   res_scales=(1.0,  1.0,  0.005, 0.005, 2, 2),   pull_scales=(10, 10, 10, 10, 5,   5  )),
#           Bin('pT2002000',  scale_factor=365.13, min_pt=200,  max_pt=2000,  diff_scales=(500, 0.0025, 0.005, 0.002, 0.05, 0.07),   res_scales=(0.5,  0.5,  0.005, 0.005, 2, 2),   pull_scales=(10, 10, 10, 10, 5,   5  )),
        ]

#        for b in bins[-1:]:
#            b.use_by_bin = False
        raw_bins = array('d', range(0, 50, 10) + range(50, 100, 25) + [100, 200, 350, 2000])

        if min_eta is not None:
            for b in bins:
                b.min_eta = min_eta
        if max_eta is not None:
            for b in bins:
                b.max_eta = max_eta
        if min_phi is not None:
            for b in bins:
                b.min_phi = min_phi
        if max_phi is not None:
            for b in bins:
                b.max_phi = max_phi
                
        #if wide_all_for_stalone:
        #    for bin in bins:
        #        bin.diff_scales = tuple([10*x for x in bin.diff_scales])
        #        bin.res_scales = tuple([10*x for x in bin.res_scales])

        #if return_raw and not no_bin_deletion:
        #    if bin_350:
        #        raw_bins = array('d', range(0, 50, 10) + range(50, 100, 25) + [100, 200, 350, 2000])
        #        del bins[-1]
        #    else:
        #        raw_bins = array('d', range(0, 50, 10) + range(50, 100, 25) + [100, 200, 2000])
        #        del bins[-3:-1]

        if is_mc: # Last two bins don't have enough stats.
            del raw_bins[-2]
            del bins[-1]
            bins[-1].name = 'pT2002000'
            bins[-1].max_pt = 2000
        
    elif bin_by in ['dxy', 'dz']:
        nbins = 10
        raw_bins = array('d', [i*bin_max/nbins for i in xrange(nbins+1)])
        bins = []
        for i in xrange(nbins):
            bin  = Bin('pT1002000%s%i' % (bin_by, i), min_pt=100, max_pt=2000, diff_scales=(500, 0.0025, 0.005, 0.002, 0.05, 0.07),   res_scales=(1,    1,    0.005, 0.005, 2, 2),   pull_scales=(10, 10, 10, 10, 5,   5  ))
            bin2 = Bin('pTall%s%i' % (bin_by, i), min_pt=0, max_pt=2000, diff_scales=(  3, 0.005,  0.01,  0.003, 0.05, 0.1 ),   res_scales=(0.1,  0.1,  0.005, 0.005, 2, 2),   pull_scales=( 5,  5,  5,  5, 3.5, 3.5))
            for b in (bin, bin2):
                setattr(b, 'min_%s' % bin_by, raw_bins[i])
                setattr(b, 'max_%s' % bin_by, raw_bins[i+1])
                bins.append(b)

    for b in bins:
        bin_min = getattr(b, 'min_%s' % bin_by)
        bin_max = getattr(b, 'max_%s' % bin_by)
        
    if return_raw:
        return raw_bins, bins
    else:
        return bins

pt_bins_avgs = [(0.0, 10.0, 7.27),
                (10.0, 20.0, 13.791575409333872),
                (20.0, 30.0, 24.04417443777194),
                (30.0, 40.0, 34.124693468809532),
                (40.0, 50.0, 44.165928348446556),
                (50.0, 75.0, 60.380296974385423),
                (75.0, 100.0, 85.751047029055059),
                (100.0, 200.0, 135.94392828431586),
#                (200.0, 2000.0, 365.13),
                (200.0, 350.0, 256.07105292402844),
                (350.0, 2000.0, 579.36031025264197)
                ]
