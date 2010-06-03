# helpers go here


# =========================
# for histograms
# =========================
def beautify(histo, color, Title, Xtitle, Ytitle):
    histo.SetLineWidth(2)
    histo.SetLineColor(color)
    histo.SetTitle(Title)
    histo.GetXaxis().SetTitle(Xtitle)
    histo.GetYaxis().SetTitle(Ytitle)

# =========================
# deltaR, deltaR2, deltaPhi
# =========================
def delPhiCalculator(phi1, phi2):
    result = phi1 - phi2;
    while result >   math.pi:
        result = result - 2. * math.pi
    while result <= -math.pi:
        result = result + 2. * math.pi
    return result

def delR2calculator( eta1, phi1, eta2, phi2 ):
    deta = eta1 - eta2
    dphi = delPhiCalculator(phi1, phi2)
    return deta*deta + dphi*dphi;
  
def delRcalculator( eta1, phi1, eta2, phi2 ):
    return math.sqrt(delR2calculator (eta1, phi1, eta2, phi2))
