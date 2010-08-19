import ROOT

class beautify:
    def SetTitles(self,hist,Name,Title,xTitle,yTitle,zTitle):
        if not Name=='': hist.SetName(Name)
        hist.SetTitle(Title)
        hist.GetXaxis().SetTitle(xTitle)
        hist.GetYaxis().SetTitle(yTitle)
        hist.GetZaxis().SetTitle(zTitle)

    def SetTitleOffsets(self,hist,xOffset,yOffset,zOffset):
        if not xOffset=='': hist.GetXaxis().SetTitleOffset(xOffset)
        if not yOffset=='': hist.GetYaxis().SetTitleOffset(yOffset)
        if not zOffset=='': hist.GetZaxis().SetTitleOffset(zOffset)
       
    def SetNdivisions(self,hist,nDivisions):
        hist.GetXaxis().SetNdivisions(nDivisions)
        hist.GetYaxis().SetNdivisions(nDivisions)
        hist.GetZaxis().SetNdivisions(nDivisions)

    def SetRange(self,hist,xMin,xMax,yMin,yMax,zMin,zMax):
        if not xMin=='' or not xMax=='': hist.GetXaxis().SetRangeUser(xMin,xMax)
        if not yMin=='' or not yMax=='': hist.GetYaxis().SetRangeUser(yMin,yMax)
        if not zMin=='' or not zMax=='': hist.GetZaxis().SetRangeUser(zMin,zMax)
        
