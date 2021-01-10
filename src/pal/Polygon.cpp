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

#include "pal/Polygon.h"

namespace pal {

struct geometry::Polygon::Impl {
};

geometry::Polygon::Polygon()
        : p(new Impl)
{
}

geometry::Polygon::~Polygon()
{
    delete p;
}

PalGeometry::Type geometry::Polygon::type() const
{
    return Type::Polygon;
}

std::vector<const PalGeometry *> geometry::Polygon::getSimpleGeometries() const
{
    return std::vector<const PalGeometry *>();
}

size_t geometry::Polygon::getNumPoints() const
{
    return 0;
}

double geometry::Polygon::getCoordX(size_t n) const
{
    return 0;
}

double geometry::Polygon::getCoordY(size_t n) const
{
    return 0;
}

void geometry::Polygon::getCoordsX(double *xarray) const
{
}

void geometry::Polygon::getCoordsY(double *yarray) const
{
}

PalGeometry *geometry::Polygon::getExteriorRing() const
{
    return nullptr;
}

size_t geometry::Polygon::numInternalRings() const
{
    return 0;
}

PalGeometry *geometry::Polygon::getInternalRing(size_t n) const
{
    return nullptr;
}

}// namespace pal
