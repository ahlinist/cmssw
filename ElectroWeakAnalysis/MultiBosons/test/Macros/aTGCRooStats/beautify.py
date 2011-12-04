import ROOT

class beautify:
    def fix(self,hist):
        nbins = hist.GetNbinsX()
        value1 = hist.GetBinContent(nbins+1)
        if value1!=0:
            print "OVERFLOW BIN HAS ", value1, " events"
            value2 = hist.GetBinContent(nbins)
            print "last bin has ", value2, " events"
            err1 = hist.GetBinError(nbins+1)
            err2 = hist.GetBinError(nbins)
            hist.SetBinContent(nbins+1, 0)
            hist.SetBinContent(nbins, value1 + value2)
            hist.SetBinError(nbins, ROOT.TMath.Sqrt(err1*err1 + err2*err2))
            hist.SetBinError(nbins+1,0)

        value1 = hist.GetBinContent(0)
        if value1!=0:
            print "UNDERFLOW BIN HAS ", value1, " events"
            value2 = hist.GetBinContent(1)
            print "first bin has ", value2, " events"
            err1 = hist.GetBinError(1)
            err2 = hist.GetBinError(0)
            hist.SetBinContent(0, 0)
            hist.SetBinContent(1, value1 + value2)
            hist.SetBinError(1, ROOT.TMath.Sqrt(err1*err1 + err2*err2))
            hist.SetBinError(0,0)

    def beautifyAxis(self,ax):
        ax.SetLabelFont(132)
        ax.SetTitleFont(132)
        ax.SetLabelSize(0.05)
        ax.SetTitleSize(0.06)

    def beautifyLegend(self,leg):
        leg.SetTextFont(132)
        leg.SetFillColor(0)
        leg.SetBorderSize(0)

    def SetTitles(self,hist,Name,Title,xTitle,yTitle,zTitle):
        if not Name==''  :  hist.SetName(Name)
        if not Title=='' :  hist.SetTitle(Title)
        if not xTitle=='':  hist.GetXaxis().SetTitle(xTitle)
        if not yTitle=='':  hist.GetYaxis().SetTitle(yTitle)
        if not zTitle=='':  hist.GetZaxis().SetTitle(zTitle)

    def SetTitleOffsets(self,hist,xOffset,yOffset,zOffset):
        if not xOffset=='': hist.GetXaxis().SetTitleOffset(xOffset)
        if not yOffset=='': hist.GetYaxis().SetTitleOffset(yOffset)
        if not zOffset=='': hist.GetZaxis().SetTitleOffset(zOffset)
       
    def SetLinesAndMarkers(self,hist,lineColor,lineWidth,markerColor,markerStyle,markerSize,fillColor,fillStyle):
        if not lineColor  =='': hist.SetLineColor(  lineColor)
        if not lineWidth  =='': hist.SetLineWidth(  lineWidth)
        if not markerColor=='': hist.SetMarkerColor(markerColor)
        if not markerStyle=='': hist.SetMarkerStyle(markerStyle)
        if not markerSize =='': hist.SetMarkerSize (markerSize )
        if not fillColor=='': hist.SetFillColor(fillColor)
        if not fillStyle=='': hist.SetFillStyle(fillStyle)

        #beautify().beautifyAxis(hist.GetXaxis())
        #beautify().beautifyAxis(hist.GetYaxis())

    def SetNdivisions(self,hist,nDivisions):
        hist.GetXaxis().SetNdivisions(nDivisions)
        hist.GetYaxis().SetNdivisions(nDivisions)
        hist.GetZaxis().SetNdivisions(nDivisions)

    def SetRange(self,hist,xMin,xMax,yMin,yMax,zMin,zMax):
        if not xMin=='' or not xMax=='': hist.GetXaxis().SetRangeUser(xMin,xMax)
        if not yMin=='' or not yMax=='': hist.GetYaxis().SetRangeUser(yMin,yMax)
        if not zMin=='' or not zMax=='': hist.GetZaxis().SetRangeUser(zMin,zMax)
        
