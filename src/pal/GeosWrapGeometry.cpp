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

#include "pal/GeosWrapGeometry.h"

#include <geos_c.h>

#include <stdexcept>

namespace pal {

struct GeosWrapGeometry::Impl {
    GEOSGeometry *geosGeom = nullptr;
    int nb = 0;

    std::vector<PalGeometry *> simpleGeometries;
    std::vector<double> x, y;

    PalGeometry *externalRing = nullptr;
    std::vector<PalGeometry *> internalRings;

    PalGeometry::Type type() const
    {
        auto t = GEOSGeomTypeId(geosGeom);
        switch (t) {
            case GEOS_POINT:
                return PalGeometry::Type::Point;
            case GEOS_LINESTRING:
                return PalGeometry::Type::LineString;
            case GEOS_LINEARRING:
                return PalGeometry::Type::LineArray;
            case GEOS_POLYGON:
                return PalGeometry::Type::Polygon;
            case GEOS_MULTIPOINT:
                return PalGeometry::Type::MultiPoint;
            case GEOS_MULTILINESTRING:
                return PalGeometry::Type::MultiLineString;
            case GEOS_MULTIPOLYGON:
                return PalGeometry::Type::MultiPolygon;
            default:
                return PalGeometry::Type::Unknown;
        }
    }

    void buildSimpleGeometries()
    {
        simpleGeometries.clear();
        int nGeom = GEOSGetNumGeometries(geosGeom);
        simpleGeometries.reserve(nGeom);

        for (auto i = 0; i < nGeom; i++) {
            auto n = new GeosWrapGeometry(const_cast<GEOSGeometry *>(GEOSGetGeometryN(geosGeom, i)));
            simpleGeometries.push_back(n);
        }
    }

    void extractCoordinates()
    {
        auto n = GEOSGetNumCoordinates(geosGeom);
        x.resize(n);
        y.resize(n);
        auto coordSeq = GEOSGeom_getCoordSeq(geosGeom);

        for (auto i = 0; i < n; ++i) {
            GEOSCoordSeq_getX(coordSeq, i, &x[i]);
            GEOSCoordSeq_getY(coordSeq, i, &y[i]);
        }
    }

    void extractRings()
    {
        auto r = new GeosWrapGeometry(const_cast<GEOSGeometry *>(GEOSGetExteriorRing(geosGeom)));
        externalRing = r;

        auto nr = GEOSGetNumInteriorRings(geosGeom);

        for (auto i = 0; i < nr; i++) {
            auto interior = GEOSGetInteriorRingN(geosGeom, i);
            auto ir = new GeosWrapGeometry(const_cast<GEOSGeometry *>(interior));

            internalRings.push_back(ir);
        }
    }

    void init(PalGeometry *parent)
    {
        if (type() == PalGeometry::Type::MultiPolygon ||
            type() == PalGeometry::Type::MultiLineString ||
            type() == PalGeometry::Type::MultiPoint) {
            buildSimpleGeometries();
        } else {
            simpleGeometries = {parent};
            if (type() == PalGeometry::Type::Polygon) {
                extractRings();
            } else {
                extractCoordinates();
            }
        }
    }
};

GeosWrapGeometry::GeosWrapGeometry(GEOSGeometry *g)
    : PalGeometry(),
      p(new Impl)
{
    p->geosGeom = g;
    p->init(this);
}

GeosWrapGeometry::GeosWrapGeometry(const char *wkb)
    : PalGeometry(),
      p(new Impl)
{
    GEOSWKTReader *reader = GEOSWKTReader_create();

    p->geosGeom = GEOSWKTReader_read(reader, wkb);
    GEOSWKTReader_destroy(reader);

    p->init(this);
}

GeosWrapGeometry::~GeosWrapGeometry()
{
    if (p->nb > 0)
        throw std::runtime_error("Trying to destroy a GeosWrapGeometry with unreleased GeosGeometry");

    if (type() == PalGeometry::Type::MultiPolygon ||
        type() == PalGeometry::Type::MultiLineString ||
        type() == PalGeometry::Type::MultiPoint) {
        for (auto x : p->simpleGeometries)
            delete x;
    }
    p->simpleGeometries.clear();

    GEOSGeom_destroy(p->geosGeom);
}

GEOSGeometry *GeosWrapGeometry::getGeosGeometry()
{
    ++p->nb;
    return p->geosGeom;
}


void GeosWrapGeometry::releaseGeosGeometry()
{
    --p->nb;
}

PalGeometry::Type GeosWrapGeometry::type() const
{
    return p->type();
}

std::vector<PalGeometry *> GeosWrapGeometry::getSimpleGeometries() const
{
    return p->simpleGeometries;
}

size_t GeosWrapGeometry::getNumPoints() const
{
    return GEOSGetNumCoordinates(p->geosGeom);
}

double GeosWrapGeometry::getCoordX(size_t n) const
{
    return p->x[n];
}

double GeosWrapGeometry::getCoordY(size_t n) const
{
    return p->y[n];
}

void GeosWrapGeometry::getCoordsX(double *xarray) const
{
    std::copy(p->x.begin(), p->x.end(), xarray);
}

void GeosWrapGeometry::getCoordsY(double *yarray) const
{
    std::copy(p->y.begin(), p->y.end(), yarray);
}

PalGeometry *GeosWrapGeometry::getExteriorRing() const
{
    return p->externalRing;
}

size_t GeosWrapGeometry::numInternalRings() const
{
    return p->internalRings.size();
}

PalGeometry *GeosWrapGeometry::getInternalRing(size_t n) const
{
    return p->internalRings[n];
}

}// namespace pal