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

#include "pal/LineString.h"

#include <stdexcept>

namespace pal {

struct geometry::LineString::Impl {
    std::vector<double> x, y;
};

geometry::LineString::LineString()
        : p(new Impl)
{
}

geometry::LineString::~LineString()
{
    delete p;
}

void geometry::LineString::addPoint(double x, double y)
{
    p->x.push_back(x);
    p->y.push_back(y);
}

PalGeometry::Type geometry::LineString::type() const
{
    return Type::LineString;
}

std::vector<const PalGeometry *> geometry::LineString::getSimpleGeometries() const
{
    return std::vector<const PalGeometry *>{this};
}

size_t geometry::LineString::getNumPoints() const
{
    if (p->x.size() != p->y.size()) {
        throw std::logic_error("x and y coordinates array must have the same size.");
    }
    return p->x.size();
}

double geometry::LineString::getCoordX(size_t n) const
{
    return p->x.at(n);
}

double geometry::LineString::getCoordY(size_t n) const
{
    return p->y.at(n);
}

void geometry::LineString::getCoordsX(double *xarray) const
{
    std::copy(p->x.begin(), p->x.end(), xarray);
}

void geometry::LineString::getCoordsY(double *yarray) const
{
    std::copy(p->y.begin(), p->y.end(), yarray);
}

PalGeometry *geometry::LineString::getExteriorRing() const
{
    return nullptr;
}

size_t geometry::LineString::numInternalRings() const
{
    return 0;
}

PalGeometry *geometry::LineString::getInternalRing(size_t n) const
{
    throw std::out_of_range("LineString geometry has no internal rings");
}

}// namespace pal
