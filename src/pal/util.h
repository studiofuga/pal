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

#ifndef _UTIL_H
#define _UTIL_H

#include <cmath>
#include <cstring>
#include <stddef.h>

#if defined(HAVE_GEOS)
#include <geos_c.h>
#endif

#include <pal/pal.h>

#include "pointset.h"
#include "rtree.hpp"

namespace pal {

class LabelPosition;
class Layer;
class Feature;

inline bool ptrFeatureCompare(Feature *a, Feature *b)
{
    return a == b;
}


/**
     * \brief For translating feature from GEOS to Pal
     */
typedef struct Feat {
    PalGeometry::Ptr geom;
    const char *id;

    int nbPoints;
    std::vector<double> x;
    std::vector<double> y;

    double minmax[4];// {xmin, ymin, xmax, ymax}

    int nbHoles;
    std::vector<PointSet> holes;
} Feat;


/**
     * \brief split GEOS geom (multilinestring, multipoint, multipolygon) => (point, linestring, polygone)
     */
std::shared_ptr<LinkedList<std::shared_ptr<Feat>>> splitGeom(PalGeometry::Ptr the_geom, const char *geom_id);

typedef struct _feats {
    Feature *feature;
    PointSet *shape;
    double priority;
    int nblp;
    LabelPosition **lPos;
} Feats;


typedef struct _elementary_transformation {
    int feat;
    int old_label;
    int new_label;
} ElemTrans;


#define EPSILON 1e-9


    inline int max (int a, int b)
    {return (a > b ? a : b);}

    inline double max (double a, double b)
    {return (a > b ? a : b);}

    inline int min (int a, int b)
    {return (a < b ? a : b);}

    inline double min (double a, double b)
    {return (a < b ? a : b);}

    inline double vabs (double x)
    { return x >= 0 ? x : -x; }



      
    inline double degree2meter (double delta_deg){
        double lat = delta_deg*0.5;
        const static double rads = (4.0*atan(1.0))/180.0;
        double a = cos(lat*rads);
        a = a*a;
        double c = 2.0*atan2(sqrt(a), sqrt(1.0 - a));
        const static double ra = 6378000; // [m]
        const static double e = 0.0810820288;
        double radius = ra*(1.0 - e*e) / pow(1.0 - e*e*sin(lat*rads)*sin(lat*rads ), 1.5 );
        double meters = (delta_deg) / 180.0 * radius * c; // [m]

        return meters;
    }

    inline double unit_convert (double x, Units from, Units to, int dpi, double scale, double delta_canvas_width)
    { 
        /* nothing to convert */
        if (from == to){
            return x;
        }

        switch (from){
            case pal::PIXEL:
                switch (to){
                    case pal::METER:
                        return ((x / double (dpi)) * 0.0254) * scale;
                    case pal::FOOT:
                        return ((x / double (dpi))*12) * scale;
                    case pal::DEGREE:
                        double iw = degree2meter(delta_canvas_width)*39.3700787;
                        return (x * delta_canvas_width * scale) / (iw * dpi);
                }
                break;
            case pal::METER:
                switch (to){
                    case pal::PIXEL:
                        return (x*dpi)/(2.54*scale);
                    case pal::FOOT:
                        return x/0.3048;
                    case pal::DEGREE:
                        double mw = degree2meter(delta_canvas_width);
                        return (x * delta_canvas_width) / mw;
                }
                break;
            case pal::FOOT:
                switch (to){
                    case pal::PIXEL:
                        return (x*dpi)/(12*scale);
                    case pal::METER:
                        return x*0.3048;
                    case pal::DEGREE:
                        double iw = degree2meter(delta_canvas_width)*39.3700787;
                        return (x * delta_canvas_width) / iw;
                }
                break;
            case pal::DEGREE:
                switch (to){
                    case pal::PIXEL:
                        fprintf (stderr, "Degree to pixel not yet implemented");
                        break;
                    case pal::METER:
                        fprintf (stderr, "Degree to meter not yet implemented");
                        break;
                    case pal::FOOT:
                        fprintf (stderr, "Degree to foot not yet implemented");
                        break;
                }
                break;
        }

        fprintf (stderr, "Unable to convert. Unknown units");
        return 0.0;
    }


    /* From meters to PostScript Point */
    inline void convert2pt(int *x, double scale, int dpi, Units from, double delta_canvas_width)
    {
        *x = (int) ((unit_convert(*x, from, pal::METER, dpi, scale, delta_canvas_width) / scale) * 39.3700787402 * dpi + 0.5);
    }


    inline int convert2pt(double x, double scale, int dpi, Units from, double delta_canvas_width)
    {
        return (int) ((unit_convert(x, from, pal::METER, dpi, scale, delta_canvas_width) / scale) * 39.3700787402 * dpi + 0.5);
    }


    void sort(double *heap, int *x, int *y, int N);


    inline bool intCompare(int a, int b)
    {
        return a == b;
    }

    inline bool strCompare(char *a, char *b)
    {
        return strcmp(a, b) == 0;
    }

    inline bool ptrLPosCompare(std::shared_ptr<LabelPosition> a, std::shared_ptr<LabelPosition> b)
    {
        return a == b;
    }

    inline bool ptrPSetCompare(std::shared_ptr<PointSet> a, std::shared_ptr<PointSet> b)
    {
        return a == b;
    }


    inline bool ptrFeatCompare(std::shared_ptr<Feat> a, std::shared_ptr<Feat> b)
    {
        return a == b;
    }

    inline bool ptrFeatsCompare(std::shared_ptr<Feats> a, std::shared_ptr<Feats> b)
    {
        return a == b;
    }

    inline bool ptrLayerCompare(Layer *a, Layer *b)
    {
        return a == b;
    }


    inline bool ptrETCompare(ElemTrans *a, ElemTrans *b)
    {
        return a == b;
    }

    /**
     * \brief Sort an array of pointers
     * \param items arays of pointers to sort
     * \param N number of items
     * \param greater function to compare two items
     **/
    template<typename T>
    void sortVector(std::vector<T> items, bool (*greater)(T const &l, T const &r))
    {
        auto N = items.size();
        if (N <= 0)
            return;

        unsigned int n = (unsigned int) N, i = n / 2, parent, child;

        T t;

        for (;;) {
            if (i > 0) {
                i--;
                t = items[i];
            } else {
                n--;
                if (n == 0) return;
                t = items[n];
                items[n] = items[0];
            }
            parent = i;
            child = i * 2 + 1;
            while (child < n) {
                if (child + 1 < n && greater(items[child + 1], items[child])) {
                    child++;
                }
                if (greater(items[child], t)) {
                    items[parent] = items[child];
                    parent = child;
                    child = parent * 2 + 1;
                } else {
                    break;
                }
            }
            items[parent] = t;
        }
    }


    void tabcpy(int n, const int *const x, const int *const y,
                const double *const prob, int *cx, int *cy, double *p);


    typedef struct {
        LabelPosition *lp;
        int *nbOv;
        double *cost;
        double *inactiveCost;
        //int *feat;
    } CountContext;

    /*
     * count overlap, ctx = p_lp
     */
    bool countOverlapCallback (LabelPosition *lp, void *ctx);

    bool countFullOverlapCallback (LabelPosition *lp, void *ctx);

} // namespace

#endif
