import sys, os
from array import array
sys.argv.append('-b')       # Start ROOT in batch mode;
import ROOT; ROOT.TCanvas   # make sure libGui gets initialized while '-b' is specified;
assert sys.argv[-1] == '-b' # and don't mess up sys.argv.
sys.argv.pop()

def binomial_clopper_pearson(n_on, n_tot, alpha=1-0.6827, equal_tailed=True):
    if equal_tailed:
        alpha_min = alpha/2
    else:
        alpha_min = alpha

    lower = 0
    upper = 1

    if n_on > 0:
        lower = ROOT.Math.beta_quantile(alpha_min, n_on, n_tot - n_on + 1)
    if n_tot - n_on > 0:
        upper = ROOT.Math.beta_quantile_c(alpha_min, n_on + 1, n_tot - n_on)

    if n_on == 0 and n_tot == 0:
        return 0, lower, upper
    else:
        return float(n_on)/n_tot, lower, upper

def binomial_divide(h1, h2, confint=binomial_clopper_pearson):
    nbins = h1.GetNbinsX()
    xax = h1.GetXaxis()
    if h2.GetNbinsX() != nbins: # or xax2.GetBinLowEdge(1) != xax.GetBinLowEdge(1) or xax2.GetBinLowEdge(nbins) != xax.GetBinLowEdge(nbins):
        raise ValueError('incompatible histograms to divide')
    x = []
    y = []
    exl = []
    exh = []
    eyl = []
    eyh = []
    xax = h1.GetXaxis()
    for ibin in xrange(1, nbins+1):
        s,t = h1.GetBinContent(ibin), h2.GetBinContent(ibin)
        if t == 0:
            assert(s == 0)
            continue

        p_hat = float(s)/t
        rat, a,b = confint(s,t)
        #print ibin, s, t, a, b

        _x  = xax.GetBinCenter(ibin)
        _xw = xax.GetBinWidth(ibin)/2
        
        x.append(_x)
        exl.append(_xw)
        exh.append(_xw)

        y.append(p_hat)
        eyl.append(p_hat - a)
        eyh.append(b - p_hat)
    eff = ROOT.TGraphAsymmErrors(len(x), *[array('d', obj) for obj in (x,y,exl,exh,eyl,eyh)])
    return eff

def core_gaussian(hist, factor, i=[0]):
    """Make a Gaussian TF1 with range set to hist.mean +/- factor *
    hist.rms.
    """
    core_mean  = hist.GetMean()
    core_width = factor*hist.GetRMS()
    f = ROOT.TF1('core%i' % i[0], 'gaus', core_mean - core_width, core_mean + core_width)
    i[0] += 1
    return f

def fit_gaussian(hist, factor=None, fit_options='qr', cache=[]):
    """Fit a Gaussian to the histogram, and return a dict with fitted
    parameters and errors. If factor is supplied, fit only to range in
    hist.mean +/- factor * hist.rms.
    """

    if factor is not None:
        fcn = core_gaussian(hist, factor)
        cache.append(fcn)
        hist.Fit(fcn, fit_options)
    else:
        hist.Fit('gaus', fit_options)
        fcn = hist.GetFunction('gaus')
        
    return {
        'fcn': fcn,
        'constant': (fcn.GetParameter(0), fcn.GetParError(0)),
        'mu':       (fcn.GetParameter(1), fcn.GetParError(1)),
        'sigma':    (fcn.GetParameter(2), fcn.GetParError(2)),
        }

def detree(t, branches='run:lumi:event', cut='', xform=lambda x: tuple(int(y) for y in x)):
    """Dump specified branches from tree into a list of tuples, via an
    ascii file. By default all vars are converted into integers. The
    xform parameter specifies the function transforming the tuple of
    strings into the desired format."""
    
    tmp_fn = os.tmpnam()
    t.GetPlayer().SetScanRedirect(True)
    t.GetPlayer().SetScanFileName(tmp_fn)
    t.Scan(branches, cut, 'colsize=50')
    t.GetPlayer().SetScanRedirect(False)
    l = len(branches.split(':')) + 2
    ret = []
    for line in open(tmp_fn):
        if ' * ' in line and 'Row' not in line:
            ret.append(xform(line.split('*')[2:l]))
    return ret

def differentiate_stat_box(hist, movement=1, new_color=None):
    """Move hist's stat box and change its line/text color. If
    movement is just an int, that number specifies how many units to
    move the box downward. If it is a 2-tuple of ints (m,n), the stat
    box will be moved to the left m units and down n units. A unit is
    the width or height of the stat box.

    Call TCanvas::Update first (and use TH1::Draw('sames') if
    appropriate) or else the stat box will not exist."""

    s = hist.FindObject('stats')

    if new_color is not None:
        s.SetTextColor(new_color)
        s.SetLineColor(new_color)

    if type(movement) == int:
        movement = (0,movement)
    m,n = movement
    
    x1,x2 = s.GetX1NDC(), s.GetX2NDC()
    y1,y2 = s.GetY1NDC(), s.GetY2NDC()
    s.SetX1NDC(x1 - (x2-x1)*m)
    s.SetX2NDC(x2 - (x2-x1)*m)
    s.SetY1NDC(y1 - (y2-y1)*n)
    s.SetY2NDC(y2 - (y2-y1)*n)

def integral(hist, xlo, xhi=None, return_error=False, include_last_bin=True):
    """For the given histogram, return the integral of the bins
    corresponding to the values xlo to xhi, along with its error if
    not integral_only.
    """

    binlo = hist.FindBin(xlo) if xlo is not None else 0
    binhi = hist.FindBin(xhi) if xhi is not None else hist.GetNbinsX()+1

    if not include_last_bin:
        binhi -= 1

    sum = hist.Integral(binlo, binhi)
    if not return_error:
        return sum

    wsq = 0
    for i in xrange(binlo, binhi+1):
        wsq += hist.GetBinError(i)**2
    return sum, wsq**0.5

def mkdir(dir, path):
    """In newer versions of ROOT, TDirectory::mkdir will make a
    hierarchy of directories if the path has slashes in it, but for
    now it can't. Emulate this."""

    for p in path.split('/'):
        if p in [x.GetName() for x in dir.GetListOfKeys()] and dir.Get(p).Class().GetName() == 'TDirectoryFile':
            dir = dir.Get(p)
        else:
            dir = dir.mkdir(p)
    return dir

def offset_x_tgae(g, f):
    """Shift all the central points by a relative fraction f
    (e.g. f=1.05 means shift 50 to 52.5, 200 to 210). Error bar
    endpoints are kept fixed."""
    
    for i in xrange(g.GetN()):
        x,y = ROOT.Double(), ROOT.Double()
        g.GetPoint(i,x,y)
        exh = g.GetErrorXhigh(i)
        exl = g.GetErrorXlow(i)
        g.SetPoint(i, ROOT.Double(x*f), y)
        dx = (f-1)*x
        g.SetPointEXhigh(i, exh - dx)
        g.SetPointEXlow (i, exl + dx)

class plot_saver:
    i = 0
    
    def __init__(self, plot_dir=None, html=True, log=True, root=True, pdf=False, pdf_log=False, C=False, C_log=False, size=(820,630), index_fn='index.html'):
        self.make_canvas(size)
        self.saved = []
        self.html = html
        self.set_plot_dir(plot_dir)
        self.log = log
        self.root = root
        self.pdf = pdf
        self.pdf_log = pdf_log
        self.C = C
        self.C_log = C_log
        self.index_fn = index_fn

    def __del__(self):
        self.write_index()

    def make_canvas(self, size):
        self.c = ROOT.TCanvas('c%i' % plot_saver.i, '', *size)
        plot_saver.i += 1
        
    def anchor_name(self, fn):
        return fn.replace('.', '_').replace('/', '_')
    
    def write_index(self):
        if not self.saved or not self.html:
            return
        html = open(os.path.join(self.plot_dir, self.index_fn), 'wt')
        html.write('<html><body><pre>\n')
        html.write('<a href="..">.. (parent directory)</a>\n')
        for i, save in enumerate(self.saved):
            if type(save) == str:
                if save == '__sep__':
                    html.write('-'*80 + '\n')
                else:
                    # a directory link
                    html.write('<a href="%s">%10i%32s%s</a>\n' % (save, i, 'change directory: ', save))
                continue

            fn, log, root, pdf, pdf_log, C, C_log = save
            bn = os.path.basename(fn)
            html.write('<a href="#%s">%10i</a> ' % (self.anchor_name(fn), i))
            if log:
                html.write(' <a href="%s">log</a>' % os.path.basename(log))
            else:
                html.write('    ')
            if root:
                html.write(' <a href="%s">root</a>' % os.path.basename(root))
            else:
                html.write('     ')
            if pdf:
                html.write(' <a href="%s">pdf</a>' % os.path.basename(pdf))
            else:
                html.write('     ')
            if pdf_log:
                html.write(' <a href="%s">pdf_log</a>' % os.path.basename(pdf_log))
            else:
                html.write('     ')
            if C:
                html.write(' <a href="%s">C</a>' % os.path.basename(C))
            else:
                html.write('     ')
            if C_log:
                html.write(' <a href="%s">C_log</a>' % os.path.basename(C_log))
            else:
                html.write('     ')
            html.write('  <a href="%s">%s</a>' % (bn, bn))
            html.write('\n')
        html.write('<br><br>')
        for i, save in enumerate(self.saved):
            if type(save) == str:
                if save == '__sep__':
                    html.write('<hr>\n')
                continue # skip dir entries
            fn, log, root, pdf, pdf_log, C, C_log = save
            bn = os.path.basename(fn)
            html.write('<h4 id="%s">%s</h4><br>\n' % (self.anchor_name(fn), bn.replace('.png', '')))
            if log:
                html.write('<img src="%s"><img src="%s"><br><br>\n' % (bn, os.path.basename(log)))
            else:
                html.write('<img src="%s"><br><br>\n' % bn)
        html.write('</pre></body></html>\n')
        
    def set_plot_dir(self, plot_dir):
        self.write_index()
        self.saved = []
        if plot_dir is not None and '~' in plot_dir:
            plot_dir = os.path.expanduser(plot_dir)
        self.plot_dir = plot_dir
        if plot_dir is not None:
            os.system('mkdir -p %s' % self.plot_dir)

    def save_dir(self, n):
        if self.plot_dir is None:
            raise ValueError('save_dir called before plot_dir set!')
        self.saved.append(n)

    def add_sep(self):
        self.saved.append('__sep__')

    def save(self, n, log=None, root=None, pdf=None, pdf_log=None, C=None, C_log=None):
        log = self.log if log is None else log
        root = self.root if root is None else root
        pdf = self.pdf if pdf is None else pdf
        pdf_log = self.pdf_log if pdf_log is None else pdf_log
        C = self.C if C is None else C
        C_log = self.C_log if C_log is None else C_log
        
        if self.plot_dir is None:
            raise ValueError('save called before plot_dir set!')
        self.c.SetLogy(0)
        fn = os.path.join(self.plot_dir, n + '.png')
        self.c.SaveAs(fn)
        if root:
            root = os.path.join(self.plot_dir, n + '.root')
            self.c.SaveAs(root)
        if log:
            self.c.SetLogy(1)
            log = os.path.join(self.plot_dir, n + '.log.png')
            self.c.SaveAs(log)
            self.c.SetLogy(0)
        if pdf:
            pdf = os.path.join(self.plot_dir, n + '.pdf')
            self.c.SaveAs(pdf)
        if pdf_log:
            self.c.SetLogy(1)
            pdf_log = os.path.join(self.plot_dir, n + '.log.pdf')
            self.c.SaveAs(pdf_log)
            self.c.SetLogy(0)
        if C:
            C = os.path.join(self.plot_dir, n + '.C')
            self.c.SaveAs(C_fn)
        if C_log:
            self.c.SetLogy(1)
            C_log = os.path.join(self.plot_dir, n + '.log.C')
            self.c.SaveAs(C_log)
            self.c.SetLogy(0)
        self.saved.append((fn, log, root, pdf, pdf_log, C, C_log))

def tdr_style(_cache=[]):
    if _cache:
        _cache[0].cd()
        return

    s = ROOT.TStyle("tdrStyle", "Style for P-TDR")
    _cache.append(s)

    s.SetFillColor(ROOT.kWhite)
    s.SetCanvasBorderMode(0)
    s.SetCanvasColor(ROOT.kWhite)
    s.SetCanvasDefH(600)
    s.SetCanvasDefW(600)
    s.SetCanvasDefX(0)
    s.SetCanvasDefY(0)
    s.SetPadBorderMode(0)
    s.SetPadColor(ROOT.kWhite)
    s.SetPadGridX(False)
    s.SetPadGridY(False)
    s.SetGridColor(0)
    s.SetGridStyle(3)
    s.SetGridWidth(1)
    s.SetFrameBorderMode(0)
    s.SetFrameBorderSize(1)
    s.SetFrameFillColor(0)
    s.SetFrameFillStyle(0)
    s.SetFrameLineColor(1)
    s.SetFrameLineStyle(1)
    s.SetFrameLineWidth(1)
    s.SetHistLineColor(1)
    s.SetHistLineStyle(0)
    s.SetHistLineWidth(1)
    s.SetEndErrorSize(2)
    s.SetMarkerStyle(20)
    s.SetOptFit(1)
    s.SetFitFormat("5.4g")
    s.SetFuncColor(2)
    s.SetFuncStyle(1)
    s.SetFuncWidth(1)
    s.SetOptDate(0)
    s.SetOptFile(0)
    s.SetStatColor(ROOT.kWhite)
    s.SetStatFont(42)
    s.SetStatFontSize(0.025)
    s.SetStatTextColor(1)
    s.SetStatFormat("6.4g")
    s.SetStatBorderSize(1)
    s.SetStatH(0.1)
    s.SetStatW(0.15)
    s.SetPadTopMargin(0.05)
    s.SetPadBottomMargin(0.13)
    s.SetPadLeftMargin(0.16)
    s.SetPadRightMargin(0.02)
    s.SetOptTitle(0)
    s.SetTitleFont(42)
    s.SetTitleColor(1)
    s.SetTitleTextColor(1)
    s.SetTitleFillColor(10)
    s.SetTitleFontSize(0.05)
    s.SetTitleColor(1, "XYZ")
    s.SetTitleFont(42, "XYZ")
    s.SetTitleSize(0.06, "XYZ")
    s.SetTitleXOffset(0.9)
    s.SetTitleYOffset(1.25)
    s.SetLabelColor(1, "XYZ")
    s.SetLabelFont(42, "XYZ")
    s.SetLabelOffset(0.007, "XYZ")
    s.SetLabelSize(0.05, "XYZ")
    s.SetAxisColor(1, "XYZ")
    s.SetStripDecimals(True)
    s.SetTickLength(0.03, "XYZ")
    s.SetNdivisions(510, "XYZ")
    s.SetPadTickX(1)
    s.SetPadTickY(1)
    s.SetOptLogx(0)
    s.SetOptLogy(0)
    s.SetOptLogz(0)
    s.SetPaperSize(20.,20.)
    s.SetPalette(1)

    s.cd()

    ROOT.gErrorIgnoreLevel = 1001 # Suppress TCanvas::SaveAs messages.

def tfile_iterator(dir, path='', pathsep='/'):
    """An iterator to loop over all histograms in a file/dir, allowing
    nested folders."""
    for key in dir.GetListOfKeys():
        obj = key.ReadObj()
        subpath = path + (pathsep if path else '') + obj.GetName()
        if obj.Class().GetName() == 'TDirectoryFile':
            for yld in tfile_iterator(obj, subpath):
                yield yld
        else:
            yield obj, subpath

def ttree_iterator(tree, return_tree=False):
    for jentry in xrange(tree.GetEntriesFast()):
        if tree.LoadTree(jentry) < 0: break
        if tree.GetEntry(jentry) <= 0: continue
        if return_tree:
            yield jentry, tree
        else:
            yield jentry

__all__ = [
    'ROOT',
    'binomial_clopper_pearson',
    'binomial_divide',
    'core_gaussian',
    'detree',
    'differentiate_stat_box',
    'fit_gaussian',
    'integral',
    'mkdir',
    'offset_x_tgae',
    'plot_saver',
    'tdr_style',
    'tfile_iterator',
    'ttree_iterator',
    ]
