/*
 *   libpal - Automated Placement of Labels Library     http://pal.heig-vd.ch
 *
 *
 *   Copyright (C) 2007, 2008 MIS-TIC, HEIG-VD (University of Applied Sciences Western Switzerland)
 *   Copyright (C) 2008, 2009 IICT-SYSIN, HEIG-VD (University of Applied Sciences Western Switzerland)
 *   Copyright (C) 2020 Federico Fuga <fuga@studiofuga.com>
 *
 *
 * This file is part of libpal.
 *
 * libpal is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libpal is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libpal. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef _POINTSET_H
#define _POINTSET_H

#include "linkedlist.hpp"
#include "pal/palgeometry.h"
#include "rtree.hpp"

#if defined(HAVE_GEOS)
#include <geos_c.h>
#endif

#include <cfloat>
#include <cmath>
#include <cstddef>
#include <memory>

namespace pal {

class Pal;
class Feat;
class Feature;
class Projection;
class LabelPosition;

typedef struct _cross {
    int pt;
    double d;
    double x;
    double y;
    int seg;       // seg{0,1,2,3}
    int nextCorner;// pt{0,1,2,3}
    int way;

} Crossing;

class PointSet;

typedef struct _cHullBox {
    std::vector<double> x = std::vector<double>(4);
    std::vector<double> y = std::vector<double>(4);

    double alpha;

    double width;
    double length;
} CHullBox;


inline bool ptrCrossingCompare(std::shared_ptr<Crossing> a, std::shared_ptr<Crossing> b)
{
    return a == b;
}

inline bool crossingDist(std::shared_ptr<Crossing> const &a, std::shared_ptr<Crossing> const &b)
{
    return a->d > b->d;
}


class PointSet {
    friend class Feature;
    friend class Pal;
    friend class Layer;
    friend class LabelPosition;
    friend class PolygonCostCalculator;
    friend class Problem;
    friend bool pruneLabelPositionCallback(LabelPosition *lp, void *ctx);
    //friend Feat *splitButterflyPolygon (Feat *f, int pt_a, int pt_b, double cx, double cy);
    friend bool obstacleCallback(PointSet *feat, void *ctx);
    friend bool extractFeatCallback(Feature *, void *);
    friend void extractXYCoord(std::shared_ptr<Feat> f);
    friend std::shared_ptr<LinkedList<std::shared_ptr<Feat>>> splitGeom(PalGeometry::Ptr the_geom, const char *geom_id);
    friend void releaseAllInIndex(RTree<PointSet *, double, 2, double> *obstacles);
    friend bool releaseCallback(PointSet *pset, void *ctx);
    friend bool filteringCallback(PointSet *, void *);

protected:
    std::vector<double> x;
    std::vector<double> y;// points order is counterclockwise

    std::vector<double> status;// -1 means inside the bbox, +1 means outside and 0 is either in either out
    std::vector<int> cHull;
    int cHullSize = 0;

    PalGeometry::Type type = PalGeometry::Type::Undefined;

    //PointSet *parent;

    std::shared_ptr<PointSet> holeOf = nullptr;
    std::shared_ptr<PointSet> parent = nullptr;

    //public:
    double xmin;
    double xmax;
    double ymin;
    double ymax;

    int getPath(int start, int stop, int *path_val);

    std::shared_ptr<PointSet> extractPath(int path, int nbPtPath, int nbBboxPt, double bbx[4], double bby[4], std::shared_ptr<Crossing> start, std::shared_ptr<Crossing> stop, int startPt);

    std::shared_ptr<PointSet> extractShape(int nbPtSh, int imin, int imax, int fps, int fpe, double fptx, double fpty);

    std::shared_ptr<PointSet> createProblemSpecificPointSet(double bbx[4], double bby[4], bool *outside, bool *inside);

    CHullBox *compute_chull_bbox();

    /*
         *  \brief Take each line in shape_toProcess and only keep inside bbox line parts
         */
    static void reduceLine(std::shared_ptr<PointSet> line,
                           std::shared_ptr<LinkedList<std::shared_ptr<PointSet>>> shapes_final,
                           double bbx[4], double bby[4]);


    /**
         * \brief takes shapes from shapes_toProcess, compute intersection with bbox
         * and puts new shapes into shapes_final
         */
    static void reducePolygon(std::shared_ptr<PointSet> shape_toProcess,
                              LinkedList<std::shared_ptr<PointSet>> *shapes_final,
                              double bbx[4], double bby[4]);


    /*
         * split a concave shape into several convex shapes
         *
         */
    static void splitPolygons(std::shared_ptr<LinkedList<std::shared_ptr<PointSet>>> shapes_toProcess,
                              std::shared_ptr<LinkedList<std::shared_ptr<PointSet>>> shapes_final,
                              double xrm, double yrm, char *uid);


    /*
         * Iterate on line by real step of dl on x,y points
         * @param nbPoint # point in line
         * @param x x coord
         * @param y y coord
         * @param d ??
         * @param ad distance from pt0 to each point (ad0 = pt0->pt0)
         * @param dl ??
         * @param px current x coord on line
         * @param py current y coord on line
         */
    inline void getPoint(double *d, double *ad, double dl,
                         double *px, double *py)
    {
        int i;
        double dx, dy, di;
        double distr;
        auto nbPoints = x.size();

        i = 0;
        if (dl >= 0) {
            while (i < nbPoints && ad[i] <= dl) i++;
            i--;
        }

        if (i < nbPoints - 1) {
            if (dl < 0) {
                dx = x[nbPoints - 1] - x[0];
                dy = y[nbPoints - 1] - y[0];
                di = sqrt(dx * dx + dy * dy);
            } else {
                dx = x[i + 1] - x[i];
                dy = y[i + 1] - y[i];
                di = d[i];
            }

            distr = dl - ad[i];
            *px = x[i] + dx * distr / di;
            *py = y[i] + dy * distr / di;
        } else {// just select last point...
            *px = x[i];
            *py = y[i];
        }
    }


public:
    double getXmin() const
    {
        return xmin;
    }

    double getXmax() const
    {
        return xmax;
    }

    double getYmin() const
    {
        return ymin;
    }

    double getYmax() const
    {
        return ymax;
    }

    inline PalGeometry::Type getType() const
    {
        return type;
    }

    inline std::vector<double> const &getX() const
    {
        return x;
    }

    inline std::vector<double> const &getY() const
    {
        return y;
    }

    inline int getNbPoints() const
    {
        return x.size();
    }

    /**
         * \brief return the minimum distance bw this and the point (px,py)
         *
         * compute the minimum distance bw the point (px,py) and this.
         * Optionnaly, store the nearest point in (rx,ry) 
         *
         * @param px x coordinate of the point
         * @param py y coordinate of the points
         * @param rx pointer to x coorinates of the nearest point (can be NULL)
         * @param ry pointer to y coorinates of the nearest point (can be NULL)
         */
    double getDist(double px, double py, double *rx, double *ry);


    //double getDistInside(double px, double py);

    void getCentroid(double &px, double &py);

    PointSet(PalGeometry::Type type, size_t numpoints);

    PointSet(double x, double y);

    PointSet(PointSet const &) = default;
    PointSet(PointSet &&) = default;

    PointSet();
    PointSet(int nbPoints, double *x, double *y);

    ~PointSet() = default;

    PointSet &operator=(PointSet const &) = default;
    PointSet &operator=(PointSet &&) = default;
};

} // namespace pal

#endif

