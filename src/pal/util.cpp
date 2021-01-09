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

#include "pal/layer.h"

#include "internalexception.h"

#include "util.h"
#include "labelposition.h"
#include "feature.h"
#include "geomfunction.h"

#include <cstddef>
#include <sstream>
#include <iostream>

#include <cfloat>
#include <cstdarg>
#include <ctime>

namespace pal {

    void sort (double* heap, int* x, int* y, int N) {
        unsigned int n = N, i = n / 2, parent, child;
        double t;
        int tx;
        int ty;
        for (;;) {
            if (i > 0) {
                i--;
                t = heap[i];
                tx = x[i];
                ty = y[i];
            } else {
                n--;
                if (n == 0) return;
                t = heap[n];
                tx = x[n];
                ty = y[n];
                heap[n] = heap[0];
                x[n] = x[0];
                y[n] = y[0];
            }
            parent = i;
            child = i * 2 + 1;
            while (child < n) {
                if (child + 1 < n  &&  heap[child + 1] > heap[child]) {
                    child++;
                }
                if (heap[child] > t) {
                    heap[parent] = heap[child];
                    x[parent] = x[child];
                    y[parent] = y[child];
                    parent = child;
                    child = parent * 2 + 1;
                } else {
                    break;
                }
            }
            heap[parent] = t;
            x[parent] = tx;
            y[parent] = ty;
        }
    }

    void tabcpy (int n, const int* const x, const int* const y,
                 const double* const prob, int *cx, int *cy, double *p) {
        int i;

        for (i = 0;i < n;i++) {
            cx[i] = x[i];
            cy[i] = y[i];
            p[i] = prob[i];
        }
    }


    void sort (void** items, int N, bool (*greater) (void *l, void *r)) {

        if (N <= 0)
            return;

        unsigned int n = (unsigned int) N, i = n / 2, parent, child;

        void *t = NULL;

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
                if (child + 1 < n  &&  greater (items[child + 1], items[child])) {
                    child++;
                }
                if (greater (items[child], t)) {
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



    bool countOverlapCallback (LabelPosition *lp, void *ctx) {
        LabelPosition *lp2 = (LabelPosition*) ctx;

        if (lp2->isInConflict (lp)) {
            lp2->nbOverlap++;
        }

        return true;
    }

    bool countFullOverlapCallback (LabelPosition *lp, void *ctx) {
        LabelPosition *lp2 = ( (CountContext*) ctx)->lp;
        double *cost = ( (CountContext*) ctx)->cost;
        //int *feat = ((CountContext*)ctx)->feat;
        int *nbOv = ( (CountContext*) ctx)->nbOv;
        double *inactiveCost = ( (CountContext*) ctx)->inactiveCost;
        if (lp2->isInConflict (lp)) {
#ifdef _DEBUG_FULL_
            std::cout <<  "count overlap : " << lp->id << "<->" << lp2->id << std::endl;
#endif
            (*nbOv) ++;
            *cost += inactiveCost[lp->probFeat] + lp->cost;

        }

        return true;
    }


    inline bool ptrGeomEq (const PalGeometry *l, const PalGeometry *r) {
        return l == r;
    }

    auto unmulti (PalGeometry *the_geom) {
        auto queue = new  LinkedList<const PalGeometry*> (ptrGeomEq);
        auto final_queue = new  LinkedList<const PalGeometry*> (ptrGeomEq);

        queue->push_back (the_geom);
        int nGeom;
        int i;

        while (queue->size() > 0) {
            auto geom = queue->pop_front();
            switch (geom->type()) {
            case PalGeometry::Type::MultiPoint:
            case PalGeometry::Type::MultiLineString:
            case PalGeometry::Type::MultiPolygon: {
                auto simplifiedGeometries = geom->getSimpleGeometries();
                for (auto x : simplifiedGeometries) {
                    queue->push_back(x);
                }
/*
                std::copy(simplifiedGeometries.begin(), simplifiedGeometries.end(),
                          std::back_inserter(queue));
*/
            }
/*

                nGeom = GEOSGetNumGeometries (geom);
                for (i = 0;i < nGeom;i++) {
                    queue->push_back (GEOSGetGeometryN (geom, i));
                }
*/
                break;
            case PalGeometry::Type::Point:
            case PalGeometry::Type::LineString:
            case PalGeometry::Type::Polygon:
                final_queue->push_back (geom);
                break;
            default:
                throw InternalException::UnknownGeometry();
            }
        }
        delete queue;

        return final_queue;
    }

    /*
     * \brief read coordinates from a GEOS geom
     */
    void extractXYCoord (Feat *f) {
        int i, j;

        double phi, lambda;

        const auto geom = f->geom;
        const PalGeometry *r_geom;

        switch (geom->type()) {
        case PalGeometry::Type::Point:
        case PalGeometry::Type::LineString:
        case PalGeometry::Type::Polygon:
            f->type = geom->type();
            break;
        default:
            throw std::logic_error("Wrong geometry !!");
        }

        if (f->type == PalGeometry::Type::Polygon) {
            r_geom = geom->getExteriorRing();
            f->nbHoles = geom->numInternalRings();
            if (f->nbHoles > 0) {
                f->holes = new PointSet*[f->nbHoles];
#ifdef _DEBUG_FULL_
                std::cout << f->nbHoles << " obstacles !" << std::endl;
#endif
                for (i = 0;i < f->nbHoles;i++) {
                    f->holes[i] = new PointSet();
                    f->holes[i]->holeOf = NULL;

                    auto interior =  geom->getInternalRing(i);
                    f->holes[i]->nbPoints = interior->getNumPoints();
                    f->holes[i]->x = new double[f->holes[i]->nbPoints];
                    f->holes[i]->y = new double[f->holes[i]->nbPoints];

                    f->holes[i]->xmin = DBL_MAX;
                    f->holes[i]->xmax = -DBL_MAX;
                    f->holes[i]->ymin = DBL_MAX;
                    f->holes[i]->ymax = -DBL_MAX;

                    for (j = 0;j < f->holes[i]->nbPoints;j++) {
                        f->holes[i]->x[j] = interior->getCoordX(j);
                        f->holes[i]->y[j] = interior->getCoordY(j);

                        f->holes[i]->xmax = f->holes[i]->x[j] > f->holes[i]->xmax ? f->holes[i]->x[j] : f->holes[i]->xmax;
                        f->holes[i]->xmin = f->holes[i]->x[j] < f->holes[i]->xmin ? f->holes[i]->x[j] : f->holes[i]->xmin;

                        f->holes[i]->ymax = f->holes[i]->y[j] > f->holes[i]->ymax ? f->holes[i]->y[j] : f->holes[i]->ymax;
                        f->holes[i]->ymin = f->holes[i]->y[j] < f->holes[i]->ymin ? f->holes[i]->y[j] : f->holes[i]->ymin;
                    }
                    //delete coordSeq;
                    reorderPolygon (f->holes[i]->nbPoints, f->holes[i]->x, f->holes[i]->y);
                }
            }
        } else {
            r_geom = geom;
            f->nbHoles = 0;
        }


        f->nbPoints = r_geom->getNumPoints();
//        coordSeq = GEOSGeom_getCoordSeq (r_geom);

        double xmin = DBL_MAX;
        double xmax = -DBL_MAX;
        double ymin = DBL_MAX;
        double ymax = -DBL_MAX;

        f->x = new double[f->nbPoints];
        f->y = new double[f->nbPoints];

        int *pts = new int [f->nbPoints];


#ifdef _DEBUG_FULL_
        std::cout << "ExtractXY (" << f->nbPoints << " points)" << std::endl;
#endif
        for (i = 0;i < f->nbPoints;i++) {
            f->x[i] = r_geom->getCoordX(i);
            f->y[i] = r_geom->getCoordY(i);

            xmax = f->x[i] > xmax ? f->x[i] : xmax;
            xmin = f->x[i] < xmin ? f->x[i] : xmin;

            ymax = f->y[i] > ymax ? f->y[i] : ymax;
            ymin = f->y[i] < ymin ? f->y[i] : ymin;

            pts[i] = i;

#ifdef _DEBUG_FULL_
            std::cout << f->x[i] << ";" << f->y[i] << std::endl;
#endif
        }

        f->minmax[0] = xmin;
        f->minmax[1] = ymin;
        f->minmax[2] = xmax;
        f->minmax[3] = ymax;


        // TODO make a function with that and add simplify() process
        int new_nbPoints = f->nbPoints;
        bool *ok = new bool[new_nbPoints];

        for (i = 0;i < f->nbPoints;i++) {
            ok[i] = true;
            j = (i + 1) % f->nbPoints;
            if (i == j)
                break;
            if (vabs (f->x[i] - f->x[j]) < 0.0000001 && vabs (f->y[i] - f->y[j]) < 0.0000001) {
                new_nbPoints--;
                ok[i] = false;
            }
        }

        if (new_nbPoints < f->nbPoints) {
#ifdef _DEBUG_FULL_
            std::cout << "Sans Doublon: (" << new_nbPoints << ")" << std::endl;
#endif
            double *new_x = new double[new_nbPoints];
            double *new_y = new double[new_nbPoints];
            for (i = 0, j = 0;i < f->nbPoints;i++) {
                if (ok[i]) {
                    new_x[j] = f->x[i];
                    new_y[j] = f->y[i];
#ifdef _DEBUG_FULL_
                    std::cout << new_x[j] << ";" << new_y[j] << std::endl;
#endif
                    j++;
                }
            }
            delete[] f->x;
            delete[] f->y;
            f->x = new_x;
            f->y = new_y;
            f->nbPoints = new_nbPoints;
        }

        delete[] ok;


        //delete coordSeq;
        delete[] pts;
    }



    LinkedList<Feat*> * splitGeom (PalGeometry *the_geom, const char *geom_id) {
        LinkedList <Feat*> *fCoordQueue = new LinkedList<Feat*> (ptrFeatCompare);
        LinkedList <Feat*> *finalQueue = new LinkedList<Feat*> (ptrFeatCompare);

        auto simpleGeometries = unmulti (the_geom);

        int i, j, k, l, j2, l2;

        int pt_a = -1;
        int pt_b = -1;
        double cX, cY;
        double tmpX, tmpY;

        Feat *f;

        while (simpleGeometries->size() > 0) {
            const auto geom = simpleGeometries->pop_front();
            //std::cout << "    split->typeid : " << geom->getGeometryTypeId() << std::endl;
            switch (geom->type()) {
                case PalGeometry::Type::MultiPoint:
            case PalGeometry::Type::MultiLineString:
            case PalGeometry::Type::MultiPolygon:
                throw std::logic_error("MUTLI geometry should never occurs here");
            case PalGeometry::Type::Point:
            case PalGeometry::Type::LineString:
                f = new Feat();
                f->geom = geom;
                f->id = geom_id;
                f->type = geom->type();
                extractXYCoord (f);
                fCoordQueue->push_back (f);
                break;
            case PalGeometry::Type::Polygon:
                f = new Feat();
                f->geom = geom;
                f->id = geom_id;
                f->type = PalGeometry::Type::Polygon;
                extractXYCoord (f);

                // BUGFIX #8 by maxence -- 11/03/2008
                if (f->nbPoints >= 3) {
#ifdef _DEBUG_FULL_
                    std::cout << "new polygon for " << geom_id << " (" << f->nbPoints << " pts)" << std::endl;
                    for (i = 0;i < f->nbPoints;i++) {
                        std::cout << f->x[i] << " ; " << f->y[i] << std::endl;
                    }
#endif
                    if (reorderPolygon (f->nbPoints, f->x, f->y) == 0) {
#ifdef _DEBUG_FULL_
                        std::cout << "reordered: " << geom_id << " (" << f->nbPoints << " pts)" << std::endl;
                        for (i = 0;i < f->nbPoints;i++) {
                            std::cout << f->x[i] << " ; " << f->y[i] << std::endl;
                        }
#endif
                        fCoordQueue->push_back (f);
                    } else {
                        std::cout << __FILE__ << ":" << __LINE__ << " Unable to reorder the polygon ..." << std::endl;
                        for (i = 0;i < f->nbHoles;i++)
                            delete f->holes[i];
                        delete f->holes;
                        delete f->x;
                        delete f->y;
                        delete f;
                    }
                } else {
                    std::cout << "Geometry " << geom_id << " is invalid (less than 3 real points)" << std::endl;
                    for (i = 0;i < f->nbHoles;i++)
                        delete f->holes[i];
                    delete f->holes;
                    delete[] f->x;
                    delete[] f->y;
                    delete f;
                }
                break;
            default:
                throw InternalException::UnknownGeometry();
            }
        }

        delete simpleGeometries;

        cX = 0.0;
        cY = 0.0;

        while (fCoordQueue->size() > 0) {
            f = fCoordQueue->pop_front();

            if (f->type == PalGeometry::Type::Polygon) {
#ifdef _DEBUG_FULL_
                std::cout << "New feature coordinates:" << std::endl;
                for (i = 0;i < f->nbPoints;i++)
                    std::cout << f->x[i] << ";" << f->y[i] << std::endl;
#endif

                // Butterfly detector
                //
                //   3____0
                //    \  /
                //     \/  <--- not allowed
                //     /\
                //   1/__\2
                //
                //   1____0
                //    \  /
                //    2\/5  <--- allowed
                //     /\
                //   3/__\4
                //
                pt_a = -1;
                pt_b = -1;
                for (i = 0;i < f->nbPoints - 2;i++) {
                    j = i + 1;
                    j2 = (j + 1) % f->nbPoints;
                    for (k = i + 2;k < f->nbPoints - (i == 0);k++) {
                        l = (k + 1) % f->nbPoints;
                        l2 = (l + 1) % f->nbPoints;

                        //std::cout << "   " << i << "->" << j << "     " << k << "->" << l << std::endl;
                        if (computeSegIntersectionExt (f->x[i], f->y[i],
                                                       f->x[j], f->y[j],
                                                       f->x[j2], f->y[j2],
                                                       f->x[k], f->y[k],
                                                       f->x[l], f->y[l],
                                                       f->x[l2], f->y[l2],
                                                       &tmpX, &tmpY)) {
#ifdef _DEBUG_FULL_
                            std::cout << i << "->" << j << "  intersect " << k << "->" << l << std::endl;
#endif
                            pt_a = i;
                            pt_b = k;
                            cX = tmpX;
                            cY = tmpY;
                            i = k = f->nbPoints;
                        }
                    }
                }

                if (pt_a == -1 && pt_b == -1) {
                    finalQueue->push_back (f);
                } else {
                    //fCoordQueue->push_back(splitButterflyPolygon (f, (pt_a+1)%f->nbPoints, (pt_b+1)%f->nbPoints, cX, cY));
                    //fCoordQueue->push_back(splitButterflyPolygon (f, (pt_b+1)%f->nbPoints, (pt_a+1)%f->nbPoints, cX, cY));
                    for (i = 0;i < f->nbHoles;i++)
                        delete f->holes[i];
                    delete f->holes;
                    delete[] f->x;
                    delete[] f->y;
                    delete f;
                }
            } else {
                finalQueue->push_back (f);
            }

        }
        delete fCoordQueue;
        //delete geosGeom;
        return finalQueue;
    }

} // namespace



