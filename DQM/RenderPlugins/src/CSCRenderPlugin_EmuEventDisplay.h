#ifndef CSCRenderPlugin_EmuEventDisplay_H
#define CSCRenderPlugin_EmuEventDisplay_H

/*
 * =====================================================================================
 *
 *       Filename:  CSCRenderPlugin_EmuEventDisplay.h
 *
 *    Description:  CSC Histogram Rendering Plugin
 *
 *        Version:  1.0
 *        Created:  12/02/2010 07:50:48 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Valdas Rapsevicius (VR), Valdas.Rapsevicius@cern.ch
 *        Company:  CERN, CH
 *
 * =====================================================================================
 */

#define CSC_RENDER_PLUGIN

#ifdef DQMLOCAL
#include "DQM/CSCMonitorModule/interface/CSCDQM_Detector.h"
#else
#include "CSCDQM_Detector.h"
#endif 

#include <string>
#include <iostream>
#include <vector>
#include <TH1.h>
#include <TH2.h>
#include <TBox.h>
#include <TText.h>
#include <TPRegexp.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TPolyLine.h>
#include <TLine.h>

#define CSC_HIT_DELTA 0.01
#define CSC_HIT_SCALE 0.001

/**
 * @class EmuDrawable
 * @brief Interface to object that can be Draw'ed
 */
class EmuDrawable {

    virtual void Draw(const char* option) = 0;
    
};

/**
 * @class EmuHit
 * @brief Class that holds hit data: coordinates and TObject
 */
class EmuHit : public EmuDrawable {

    private:

        double x;
        double y;
        TBox* hit;

    public:

        EmuHit(double x_, double y_, bool x_scale = true, bool y_scale = true) {
            x = x_ * (x_scale ? CSC_HIT_SCALE : 1.0);
            y = y_ * (y_scale ? CSC_HIT_SCALE : 1.0);
            hit = new TBox(x + CSC_HIT_DELTA, y + CSC_HIT_DELTA, x - CSC_HIT_DELTA, y - CSC_HIT_DELTA);
            hit->SetLineColor(1);
            hit->SetLineStyle(0);
            hit->SetLineWidth(0);
            hit->SetFillColor(1);
        }

        virtual ~EmuHit() {
            delete hit;
        }

        void Draw(const char* option) {
            hit->Draw(option);
        }

};

struct EmuLayerHits {
    std::string part;
    int layer;
    int hs;
    int wg;
};

/**
 * @class EmuRawHit
 * @brief Class that holds raw hits and computes intersections
 */
class EmuRawHits {

    private:

        std::vector<int> wgs[N_LAYERS];
        std::vector<int> hss[N_LAYERS];

    public:

        void addWgHit(int layer, int wg) {
            wgs[layer].push_back(wg + layer);
        }

        void addHsHit(int layer, int hs) {
            hss[layer].push_back(hs + layer);
        }

        // Get intersecting hits
        bool getLayerHits(cscdqm::Detector& detector, unsigned int station, unsigned int ring, std::vector<EmuLayerHits>& layerHits) {
            bool result = false;
            for (int ipart = 1; ipart <= detector.NumberOfChamberParts(station, ring); ipart++) {
                std::string part = detector.ChamberPart(ipart);
                for (int layer = 0; layer < N_LAYERS; layer++) {
                    if (!hss[layer].empty() && !wgs[layer].empty()) {
                        for (unsigned int i = 0; i < wgs[layer].size(); i++) {
                            int wg = wgs[layer][i];
                            for (unsigned int j = 0; j < hss[layer].size(); j++) {
                                EmuLayerHits hits;
                                hits.part = part;
                                hits.layer = layer;
                                hits.wg = wg;
                                hits.hs = hss[layer][j];
                                layerHits.push_back(hits);
                                result = true;
                            }
                        }
                    }
                }
            }
            return result;
        }

};

/**
 * @class EmuChamber
 * @brief Class that holds chamber bounds: coordinates and object
 */
class EmuChamber : public EmuDrawable {

    private:

        static const int POINTS = 5;

        double x[POINTS];
        double y[POINTS];
        TPolyLine* bounds;

        void setValue(double* points, int i, double d, bool scale) {
            if (i >= 0 && i < (POINTS - 1)) {
                points[i] = d * (scale ? CSC_HIT_SCALE : 1.0);
                if (i == 0) {
                    points[POINTS - 1] = points[0];
                }
            }
        }

    public:

        EmuChamber() : bounds(0) { }

        EmuChamber* setX(int i, double d, bool scale = true) {
            setValue(x, i, d, scale);
            return this;
        }

        EmuChamber* setY(int i, double d, bool scale = true) {
            setValue(y, i, d, scale);
            return this;
        }

        virtual ~EmuChamber() {
            if (bounds) {
                delete bounds;
            }
        }

        void Draw(const char* option) {
            if (bounds == 0) {
                bounds = new TPolyLine(5, x, y);
                bounds->SetLineColor(kRed - 10);
                bounds->SetLineStyle(1);
                bounds->SetLineWidth(1);
            }
            bounds->Draw(option);
        }

        bool hitInBounds(double hx, double hy) const {
            int SIDES = POINTS - 1, j = SIDES - 1;
            bool result = false;

            for (int i = 0; i < SIDES; i++) {
                if ((y[i] < hy && y[j] >= hy) || (y[j] < hy && y[i] >= hy)) {
                    if (x[i] + (hy - y[i]) / (y[j] - y[i]) * (x[j] - x[i]) < hx) {
                        result =! result;
                    }
                }
                j = i;
            }

            return result;
        }

};


/**
 * @class EmuEventDisplay
 * @brief Class that draws CSC Map diagram
 */
class EmuEventDisplay {
  
  private:

    cscdqm::Detector detector;

    TH2F* histo_zr;
    TH2F* histo_zphi;
    TH2F* histo_xy;

    std::vector<EmuChamber*> zrChambers;
    std::vector<EmuChamber*> zpChambers;
    std::vector<EmuChamber*> xyChambers;

    std::vector<TLine*> zrLines;

    std::vector<EmuHit*> zrHits;
    std::vector<EmuHit*> zpHits;
    std::vector<EmuHit*> xyHits;

    template<class T>
    void deleteItems(std::vector<T*> &v) {
      for (unsigned int i = 0; i < v.size(); i++) {
        T* t = v.at(i);
        delete t;
      }
      v.clear();
    }

    template<class T>
    void drawItems(std::vector<T*> &v, const std::string& options = "l") {
      for (unsigned int i = 0; i < v.size(); i++) {
        T* t = v.at(i);
        t->Draw(options.c_str());
      }
    }

    bool readHistogramHits(TH2* data, unsigned int station, unsigned int ring, int chIndex, EmuRawHits& hits);

  public:
    
    EmuEventDisplay();
    ~EmuEventDisplay();

    void drawEventDisplay_ZR(TH2* data);
    void drawEventDisplay_ZPhi(TH2* data);
    void drawEventDisplay_XY(TH2* data);

};

#endif
