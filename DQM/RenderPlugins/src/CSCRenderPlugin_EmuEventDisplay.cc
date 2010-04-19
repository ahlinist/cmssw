/*
 * =====================================================================================
 *
 *       Filename:  EmuEventDisplay.cc
 *
 *    Description:  draw event display based on encoded histogram 
 *
 *        Version:  1.0
 *        Created:  12/12/2009 08:57:49 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Valdas Rapsevicius (VR), Valdas.Rapsevicius@cern.ch
 *        Company:  CERN, CH
 *
 * =====================================================================================
 */

#include "CSCRenderPlugin_EmuEventDisplay.h"

/**
 * @brief  Constructor
 */
EmuEventDisplay::EmuEventDisplay() {

    histo_zr = new TH2F("h1", "Events in #Zeta-R projection", 22, -11.0, 11.0, 14, -7.0, 7.0);
    histo_zr->GetXaxis()->SetTitle("#Zeta * 1000");
    histo_zr->GetXaxis()->SetTitleOffset(1.2);
    histo_zr->GetXaxis()->CenterTitle(true);
    histo_zr->GetXaxis()->SetLabelSize(0.02);
    histo_zr->GetXaxis()->SetTicks("+-");
    histo_zr->GetYaxis()->SetTitle("R * 1000");
    histo_zr->GetYaxis()->SetTitleOffset(-1.2);
    histo_zr->GetYaxis()->SetTicks("+-");
    histo_zr->GetYaxis()->CenterTitle(true);
    histo_zr->GetYaxis()->SetLabelSize(0.02);
    histo_zr->SetStats(kFALSE);

    histo_zphi = new TH2F("h2", "Events in #Zeta-#phi projection", 22, -11.0, 11.0, (int) (PI * 2 + 0.2), -0.2, PI * 2);
    histo_zphi->GetXaxis()->SetTitle("#Zeta * 1000");
    histo_zphi->GetXaxis()->SetTitleOffset(1.2);
    histo_zphi->GetXaxis()->CenterTitle(true);
    histo_zphi->GetXaxis()->SetLabelSize(0.02);
    histo_zphi->GetXaxis()->SetTicks("+-");
    histo_zphi->GetYaxis()->SetTitle("#phi");
    histo_zphi->GetYaxis()->SetTitleOffset(-1.2);
    histo_zphi->GetYaxis()->SetTicks("+-");
    histo_zphi->GetYaxis()->CenterTitle(true);
    histo_zphi->GetYaxis()->SetLabelSize(0.02);
    histo_zphi->SetStats(kFALSE);

    histo_xy = new TH2F("h3", "Events in X-Y projection", 19, -9.33, 9.33, 14, -7.0, 7.0);
    histo_xy->GetXaxis()->SetTitle("X * 1000");
    histo_xy->GetXaxis()->SetTitleOffset(1.2);
    histo_xy->GetXaxis()->CenterTitle(true);
    histo_xy->GetXaxis()->SetLabelSize(0.02);
    histo_xy->GetXaxis()->SetTicks("+-");
    histo_xy->GetYaxis()->SetTitle("Y * 1000");
    histo_xy->GetYaxis()->SetTitleOffset(-1.2);
    histo_xy->GetYaxis()->SetTicks("+-");
    histo_xy->GetYaxis()->CenterTitle(true);
    histo_xy->GetYaxis()->SetLabelSize(0.02);
    histo_xy->SetStats(kFALSE);

    for (unsigned int side = 1; side <= N_SIDES; side++) {
        for (unsigned int station = 1; station <= N_STATIONS; station++) {
            for (unsigned int ring = 1; ring <= detector.NumberOfRings(station); ring++) {

                double rd = (detector.NumberOfStrips(station, ring) * detector.stripDPhiRad(station, ring)) / 2;

                for (unsigned int chamber = 1; chamber <= detector.NumberOfChambers(station, ring); chamber++) {

                    if (detector.isChamberInstalled(side, station, ring, chamber)) {

                        for (int ipart = 1; ipart <= detector.NumberOfChamberParts(station, ring); ipart++) {
                            std::string part = detector.ChamberPart(ipart);

                            double z1 = detector.Z_mm(side, station, ring, chamber, 1);
                            double z2 = detector.Z_mm(side, station, ring, chamber, N_LAYERS);

                            { // Z-R plane elements

                                double r1 = detector.R_mm(side, station, ring, part, 1, 1, 1);
                                double r2 = detector.R_mm(side, station, ring, part, N_LAYERS, detector.NumberOfHalfstrips(station, ring, part), detector.NumberOfWiregroups(station, ring));

                                // Switch lower chambers to minus projection
                                if (chamber > (detector.NumberOfChambers(station, ring) / 2)) {
                                    r1 = -r1;
                                    r2 = -r2;
                                }

                                EmuChamber* ch = new EmuChamber();
                                ch->setX(0, z1)->setX(1, z1)->setX(2, z2)->setX(3, z2);
                                ch->setY(0, r1)->setY(1, r2)->setY(2, r2)->setY(3, r1);

                                zrChambers.push_back(ch);

                            }

                            { // Z-Phi plane elements

                                double r = detector.PhiRadChamberCenter(station, ring, chamber);

                                EmuChamber* ch = new EmuChamber();
                                ch->setX(0, z1)->setX(1, z1)->setX(2, z2)->setX(3, z2);
                                ch->setY(0, r + rd, false)->setY(1, r - rd, false)->setY(2, r - rd, false)->setY(3, r + rd, false);

                                zpChambers.push_back(ch);

                            }

                            { // X-Y plane elements


                                EmuChamber* ch = new EmuChamber();

                                double x[4], y[4];
                                detector.chamberBoundsXY(side, station, ring, chamber, part, x, y);
                                ch->setX(0, x[0])->setX(1, x[1])->setX(2, x[2])->setX(3, x[3]);
                                ch->setY(0, y[0])->setY(1, y[1])->setY(2, y[2])->setY(3, y[3]);

                                xyChambers.push_back(ch);

                            }

                        }

                    }

                }
            }
        }
    }

    {
        TLine *line = new TLine(-11.0, 0, 11.0, 0);
        line->SetLineColor(kRed - 10);
        line->SetLineStyle(kDashed);
        line->SetLineWidth(1);
        zrLines.push_back(line);
    }

    {
        TLine *line = new TLine(0, -7.0, 0, 7.0);
        line->SetLineColor(kRed - 10);
        line->SetLineStyle(kDashed);
        line->SetLineWidth(1);
        zrLines.push_back(line);
    }

}

/**
 * @brief  Destructor
 */
EmuEventDisplay::~EmuEventDisplay() {

    delete histo_zr;
    delete histo_zphi;
    delete histo_xy;

    deleteItems(zrChambers);
    deleteItems(zpChambers);
    deleteItems(xyChambers);

    deleteItems(zrHits);
    deleteItems(zpHits);
    deleteItems(xyHits);

    deleteItems(zrLines);

}

/**
 * @brief  Draw ZPhi event display map
 * @param  data Data histogram
 * @return 
 */
void EmuEventDisplay::drawEventDisplay_ZR(TH2* data) {

    deleteItems(zrHits);

    histo_zr->Draw();
    drawItems(zrChambers);
    drawItems(zrLines);

    int chIndex = 1;
    for (unsigned int side = 1; side <= N_SIDES; side++) {
        for (unsigned int station = 1; station <= N_STATIONS; station++) {
            for (unsigned int ring = 1; ring <= detector.NumberOfRings(station); ring++) {
                for (unsigned int chamber = 1; chamber <= detector.NumberOfChambers(station, ring); chamber++) {
                    if (detector.isChamberInstalled(side, station, ring, chamber)) {

                        EmuRawHits hits;
                        if (readHistogramHits(data, station, ring, chIndex, hits)) {

                            std::vector<EmuLayerHits> layerHits;
                            if (hits.getLayerHits(detector, station, ring, layerHits)) {
                                for (unsigned int i = 0; i < layerHits.size(); i++) {

                                    EmuLayerHits layerHit = layerHits[i];
                                    double z = detector.Z_mm(side, station, ring, chamber, layerHit.layer + 1);
                                    double r = detector.R_mm(side, station, ring, layerHit.part, layerHit.layer + 1, layerHit.hs, layerHit.wg);

                                    // Lower chamber numbers go down
                                    if (chamber > (detector.NumberOfChambers(station, ring) / 2)) {
                                        r = -r;
                                    }

                                    EmuHit* hit = new EmuHit(z, r);
                                    zrHits.push_back(hit);

                                }
                            }

                        }

                        chIndex += 1;

                    }
                }
            }
        }
    }

    drawItems(zrHits);

}

void EmuEventDisplay::drawEventDisplay_ZPhi(TH2* data) {

    deleteItems(zpHits);

    histo_zphi->Draw();
    drawItems(zpChambers);

    int chIndex = 1;
    for (unsigned int side = 1; side <= N_SIDES; side++) {
        for (unsigned int station = 1; station <= N_STATIONS; station++) {
            for (unsigned int ring = 1; ring <= detector.NumberOfRings(station); ring++) {
                for (unsigned int chamber = 1; chamber <= detector.NumberOfChambers(station, ring); chamber++) {

                    if (detector.isChamberInstalled(side, station, ring, chamber)) {

                        double r = detector.PhiRadChamberCenter(station, ring, chamber);

                        for (int strip = 1; strip <= detector.NumberOfStrips(station, ring); strip++) {
                            int layerBitset = (int) data->GetBinContent(chIndex, strip);
                            if (layerBitset > 0) {
                                for (int layer = 0; layer < N_LAYERS; layer++) {
                                    if (layerBitset & (1 << layer)) {
                                        double z = detector.Z_mm(side, station, ring, chamber, layer + 1);
                                        double pd = detector.LocalPhiRadStripToChamberCenter(side, station, ring, layer + 1, strip);

                                        EmuHit* hit = new EmuHit(z, r + pd, true, false);
                                        zpHits.push_back(hit);

                                    }
                                }
                            }
                        }

                        chIndex += 1;

                    }
                }
            }
        }
    }

    drawItems(zpHits);

}

void EmuEventDisplay::drawEventDisplay_XY(TH2* data) {

    deleteItems(xyHits);

    histo_xy->Draw();
    drawItems(xyChambers);

    int chIndex = 1;
    for (unsigned int side = 1; side <= N_SIDES; side++) {
        for (unsigned int station = 1; station <= N_STATIONS; station++) {
            for (unsigned int ring = 1; ring <= detector.NumberOfRings(station); ring++) {
                for (unsigned int chamber = 1; chamber <= detector.NumberOfChambers(station, ring); chamber++) {
                    if (detector.isChamberInstalled(side, station, ring, chamber)) {

                        EmuRawHits hits;
                        if (readHistogramHits(data, station, ring, chIndex, hits)) {

                            std::vector<EmuLayerHits> layerHits;
                            if (hits.getLayerHits(detector, station, ring, layerHits)) {
                                for (unsigned int i = 0; i < layerHits.size(); i++) {

                                    EmuLayerHits layerHit = layerHits[i];
                                    double cx = detector.X_mm(side, station, ring, layerHit.part, chamber, layerHit.layer + 1, layerHit.hs, layerHit.wg);
                                    double cy = detector.Y_mm(side, station, ring, layerHit.part, chamber, layerHit.layer + 1, layerHit.hs, layerHit.wg);

                                    EmuHit* hit = new EmuHit(cx, cy);
                                    xyHits.push_back(hit);
                                }
                            }
                        }

                        chIndex += 1;

                    }
                }
            }
        }
    }

    drawItems(xyHits);

}

bool EmuEventDisplay::readHistogramHits(TH2* data, unsigned int station, unsigned int ring, int chIndex, EmuRawHits& hits) {
    bool is_wgs = false, is_hss = false;

    for (int wg = 1; wg <= detector.NumberOfWiregroups(station, ring); wg++) {
        int wgBitset = (int) data->GetBinContent(chIndex, wg);
        if (wgBitset > 0) {
            for (int layer = 0; layer < N_LAYERS; layer++) {
                if (wgBitset & (1 << layer)) {
                    hits.addWgHit(layer, wg);
                    is_wgs = true;
                }
            }
        }
    }

    for (int hs = 1; hs <= detector.NumberOfHalfstrips(station, ring); hs++) {
        int hsBitset = (int) data->GetBinContent(chIndex, 160 + hs);
        if (hsBitset > 0) {
            for (int layer = 0; layer < N_LAYERS; layer++) {
                if (hsBitset & (1 << layer)) {
                    hits.addHsHit(layer, hs);
                    is_hss = true;
                }
            }
        }
    }

    return (is_wgs && is_hss);

}