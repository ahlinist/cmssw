import itertools
import FWCore.ParameterSet.Config as cms

class Bin:
    main_var = ''
    use_by_bin = True
    abscissa = None
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
    nbins_scale = 1
    
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
                if type(v) == type(0):
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

    def main_var_range(self):
        if self.main_var == 'run':
            return self.run_bin

        a = getattr(self, 'min_' + self.main_var)
        b = getattr(self, 'max_' + self.main_var)
        x = (b+a)/2 if self.abscissa is None else self.abscissa
        
        return a,b,x

    def __repr__(self):
        return 'Bin("%s", main_var="%s", range=%s)' % (self.name, self.main_var, repr(self.main_var_range()))

def make_bins(bin_by, bin_extra=None):
    if bin_by == 'pt':
        bins = [
            Bin('pT010',      abscissa=7.27, min_pt=0.,    max_pt=10.,    diff_scales=(  1., 0.007,  0.01,  0.004, 0.05, 0.1 ),   res_scales=(0.1,  0.1,  0.005, 0.005, 2., 2.),   pull_scales=( 5.,  5.,  5.,  5., 3.5, 3.5)),
            Bin('pT1020',     abscissa=13.8, min_pt=10.,   max_pt=20.,    diff_scales=(  2., 0.005,  0.01,  0.003, 0.05, 0.1 ),   res_scales=(0.1,  0.1,  0.005, 0.005, 2., 2.),   pull_scales=( 5.,  5.,  5.,  5., 3.5, 3.5)),
            Bin('pT2030',     abscissa=24.0, min_pt=20.,   max_pt=30.,    diff_scales=(  5., 0.003,  0.01,  0.003, 0.05, 0.1 ),   res_scales=(0.1,  0.1,  0.005, 0.005, 2., 2.),   pull_scales=( 5.,  5.,  5.,  5., 3.5, 3.5)),
            Bin('pT3040',     abscissa=34.1, min_pt=30.,   max_pt=40.,    diff_scales=(  7., 0.0025, 0.01,  0.002, 0.05, 0.1 ),   res_scales=(0.1,  0.1,  0.005, 0.005, 2., 2.),   pull_scales=( 5.,  5.,  5.,  5., 3.5, 3.5)),
            Bin('pT4050',     abscissa=44.2, min_pt=40.,   max_pt=50.,    diff_scales=( 10., 0.0025, 0.01,  0.002, 0.05, 0.1 ),   res_scales=(0.1,  0.1,  0.005, 0.005, 2., 2.),   pull_scales=( 5.,  5.,  5.,  5., 5.,  5. )),
            Bin('pT5075',     abscissa=60.4, min_pt=50.,   max_pt=75.,    diff_scales=( 12., 0.0025, 0.01,  0.002, 0.05, 0.1 ),   res_scales=(0.15, 0.15, 0.005, 0.005, 2., 2.),   pull_scales=( 5.,  5.,  5.,  5., 5.,  5. )),
            Bin('pT75100',    abscissa=85.8, min_pt=75.,   max_pt=100.,   diff_scales=( 25., 0.0025, 0.01,  0.002, 0.05, 0.1 ),   res_scales=(0.15, 0.15, 0.005, 0.005, 2., 2.),   pull_scales=( 5.,  5.,  5.,  5., 5.,  5. )),
            Bin('pT100150',   abscissa=121., min_pt=100.,  max_pt=150.,   diff_scales=(100., 0.0025, 0.01,  0.002, 0.05, 0.1 ),   res_scales=(0.25, 0.25, 0.005, 0.005, 2., 2.),   pull_scales=( 6.,  6.,  6.,  6., 5.,  5. )),
            Bin('pT150200',   abscissa=172., min_pt=150.,  max_pt=200.,   diff_scales=(100., 0.0025, 0.01,  0.002, 0.05, 0.1 ),   res_scales=(0.25, 0.25, 0.005, 0.005, 2., 2.),   pull_scales=( 6.,  6.,  6.,  6., 5.,  5. )),
            Bin('pT200350',   abscissa=256., min_pt=200.,  max_pt=350.,   diff_scales=(500., 0.0025, 0.005, 0.002, 0.05, 0.07),   res_scales=(0.25, 0.25, 0.005, 0.005, 2., 2.),   pull_scales=(10., 10., 10., 10., 5.,  5. )),
            Bin('pT350500',   abscissa=411., min_pt=350.,  max_pt=500.,   diff_scales=(500., 0.0025, 0.005, 0.002, 0.05, 0.07),   res_scales=(0.75, 0.75, 0.005, 0.005, 2., 2.),   pull_scales=(10., 10., 10., 10., 5.,  5. )),
            Bin('pT5002000',  abscissa=755., min_pt=500.,  max_pt=2000.,  diff_scales=(500., 0.0025, 0.005, 0.002, 0.05, 0.07),   res_scales=(0.75, 0.75, 0.005, 0.005, 2., 2.),   pull_scales=(10., 10., 10., 10., 5.,  5. )),
        ]

        bins += [
            Bin('pTall',      use_by_bin=False,                             diff_scales=(  3., 0.005,  0.003, 0.003, 0.05, 0.1 ),   res_scales=(0.1,  0.1,  0.005, 0.005, 2., 2.),   pull_scales=( 5.,  5.,  5.,  5., 3.5, 3.5)),
            Bin('pTover2000', use_by_bin=False, min_pt=2000., max_pt=1e99,  diff_scales=(500., 0.0025, 0.005, 0.002, 0.05, 0.07),   res_scales=(2.0,  2.0,  0.005, 0.005, 2., 2.),   pull_scales=(10., 10., 10., 10., 5.,  5. )),
            ]

    elif bin_by in ['eta', 'phi', 'dxy', 'dz']:
        if bin_extra is None:
            raise ValueError('for bin_by=eta, dxy, or dz, bin_extra must be list of bin edges, including lowest and highest')
        raw_bins = bin_extra
        bins = []
        for i in xrange(len(raw_bins)-1):
            bin  = Bin('pT2030%s%i'    % (bin_by, i), min_pt=20.,   max_pt=30.,    diff_scales=(  5., 0.003,  0.01,  0.003, 0.05, 0.1 ),   res_scales=(0.1,  0.1,  0.005, 0.005, 2., 2.),   pull_scales=( 5.,  5.,  5.,  5., 3.5, 3.5))
            bin2 = Bin('pT5002000%s%i' % (bin_by, i), min_pt=500.,  max_pt=2000.,  diff_scales=(500., 0.0025, 0.005, 0.002, 0.05, 0.07),   res_scales=(0.75, 0.75, 0.005, 0.005, 2., 2.),   pull_scales=(10., 10., 10., 10., 5.,  5. ))
            for b in (bin, bin2):
                setattr(b, 'min_%s' % bin_by, raw_bins[i])
                setattr(b, 'max_%s' % bin_by, raw_bins[i+1])
                bins.append(b)

    elif bin_by == 'run':
        runs = bin_extra
        bins = []
        for run in runs:
            bin  = Bin('pT1020run%i'   % run, min_pt=10,  max_pt=20,  diff_scales=(  2., 0.005,  0.01,  0.003, 0.05, 0.1 ),   res_scales=(0.1,  0.1,  0.005, 0.005, 2., 2.),   pull_scales=( 5.,  5.,  5.,  5., 3.5, 3.5))
            bin2 = Bin('pT100200run%i' % run, min_pt=100, max_pt=200, diff_scales=(100., 0.0025, 0.01,  0.002, 0.05, 0.1 ),   res_scales=(0.25, 0.25, 0.005, 0.005, 2., 2.),   pull_scales=( 6.,  6.,  6.,  6., 5.,  5. ))
            for b in (bin, bin2):
                b.run_bin = run
                b.nbins_scale = 5
                bins.append(b)

    for b in bins:
        b.main_var = bin_by

    return bins
