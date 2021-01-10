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

#include "pal/Point.h"

#include <stdexcept>

namespace pal {

struct geometry::Point::Impl {
    double x = 0;
    double y = 0;
};

geometry::Point::Point()
        : p(new Impl)
{

}

geometry::Point::Point(double x, double y)
        : p(new Impl)
{
    p->x = x;
    p->y = y;
}

geometry::Point::~Point()
{
    delete p;
}

PalGeometry::Type geometry::Point::type() const
{
    return Type::Point;
}

std::vector<const PalGeometry *> geometry::Point::getSimpleGeometries() const
{
    return std::vector<const PalGeometry *>{this};
}

size_t geometry::Point::getNumPoints() const
{
    return 1;
}

double geometry::Point::getCoordX(size_t n) const
{
    if (n > 0) {
        throw std::out_of_range("Invalid index in coord for point (must be 0)");
    }
    return p->x;
}

double geometry::Point::getCoordY(size_t n) const
{
    if (n > 0) {
        throw std::out_of_range("Invalid index in coord for point (must be 0)");
    }
    return p->y;
}

void geometry::Point::getCoordsX(double *xarray) const
{
    xarray[0] = p->x;
}

void geometry::Point::getCoordsY(double *yarray) const
{
    yarray[0] = p->y;
}

PalGeometry *geometry::Point::getExteriorRing() const
{
    return nullptr;
}

size_t geometry::Point::numInternalRings() const
{
    return 0;
}

PalGeometry *geometry::Point::getInternalRing(size_t n) const
{
    throw std::out_of_range("Point geometry has no internal rings");
}

}// namespace pal
