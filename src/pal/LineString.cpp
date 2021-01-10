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


namespace pal {

struct geometry::LineString::Impl {
};

geometry::LineString::LineString()
        : p(new Impl)
{
}

geometry::LineString::~LineString()
{
    delete p;
}

PalGeometry::Type geometry::LineString::type() const
{
    return Type::LineString;
}

std::vector<const PalGeometry *> geometry::LineString::getSimpleGeometries() const
{
    return std::vector<const PalGeometry *>();
}

size_t geometry::LineString::getNumPoints() const
{
    return 0;
}

double geometry::LineString::getCoordX(size_t n) const
{
    return 0;
}

double geometry::LineString::getCoordY(size_t n) const
{
    return 0;
}

void geometry::LineString::getCoordsX(double *xarray) const
{
}

void geometry::LineString::getCoordsY(double *yarray) const
{
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
    return nullptr;
}

}// namespace pal
