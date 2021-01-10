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

#ifndef LIBPAL_POINT_H
#define LIBPAL_POINT_H

#include "palgeometry.h"

namespace pal {
namespace geometry {
class Point : public PalGeometry {
    struct Impl;
    Impl *p;

public:
    Point();
    explicit Point(double x, double y);

    ~Point() override;

    Type type() const override;
    std::vector<const PalGeometry *> getSimpleGeometries() const override;
    size_t getNumPoints() const override;
    double getCoordX(size_t n) const override;
    double getCoordY(size_t n) const override;
    void getCoordsX(double *xarray) const override;
    void getCoordsY(double *yarray) const override;
    PalGeometry *getExteriorRing() const override;
    size_t numInternalRings() const override;
    PalGeometry *getInternalRing(size_t n) const override;
};
}// namespace geometry
}// namespace pal


#endif//LIBPAL_POINT_H
