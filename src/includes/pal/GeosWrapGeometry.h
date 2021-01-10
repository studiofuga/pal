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
#ifndef LIBPAL_GEOSWRAPGEOMETRY_H
#define LIBPAL_GEOSWRAPGEOMETRY_H

#if !defined(HAVE_GEOS)
#error "GeosWrapGeometry depends on GEOS but the library was not enabled"
#endif

#include "pal/label.h"
#include "pal/palgeometry.h"

#include <list>

struct GEOSGeom_t;
typedef struct GEOSGeom_t GEOSGeometry;

namespace pal {

class GeosWrapGeometry : public PalGeometry {
    struct Impl;

    Impl *p = nullptr;

private:
    explicit GeosWrapGeometry(GEOSGeometry *);

public:
    explicit GeosWrapGeometry(const char *wkb);

    GeosWrapGeometry(GeosWrapGeometry &&) = default;
    GeosWrapGeometry(GeosWrapGeometry const &) = delete;

    GeosWrapGeometry &operator=(GeosWrapGeometry &&) = default;
    GeosWrapGeometry &operator=(GeosWrapGeometry const &) = delete;

    virtual ~GeosWrapGeometry();

    /**
     * \brief get the geometry in WKB hexa format
     * This method is called by Pal each time it needs a geom's coordinates
     * \return WKB Hex buffer
     */
    GEOSGeometry *getGeosGeometry();

    /**
     * \brief Called by Pal when it doesn't need the coordinates anymore
     * @param the_geom is the geos geom  from PalGeometry::getGeosGeometry()
     */
    void releaseGeosGeometry();
    Type type() const override;
    std::vector<PalGeometry *> getSimpleGeometries() const override;
    size_t getNumPoints() const override;
    double getCoordX(size_t n) const override;
    double getCoordY(size_t n) const override;
    void getCoordsX(double *xarray) const override;
    void getCoordsY(double *yarray) const override;
    PalGeometry *getExteriorRing() const override;
    size_t numInternalRings() const override;
    PalGeometry *getInternalRing(size_t n) const override;
};

}

#endif//LIBPAL_GEOSWRAPGEOMETRY_H
