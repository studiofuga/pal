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

#include "pal/label.h"

#include <geos_c.h>
#include <list>

namespace pal {

class GeosWrapGeometry {
public:
    virtual ~GeosWrapGeometry() {}

    /**
     * \brief get the GEOSGeometry of the feature
     * This method is called by Pal each time it needs a geom's coordinates
     *
     * @return GEOSGeometry * a pointer the the geos geom
     */
    virtual GEOSGeometry *getGeosGeometry() = 0;

    /**
     * \brief Called by Pal when it doesn't need the coordinates anymore
     * @param the_geom is the geoms geom  from PalGeometry::getfeomGeometry()
     */
    virtual void releaseGeosGeometry(GEOSGeometry *the_geom) = 0;
};

}

#endif//LIBPAL_GEOSWRAPGEOMETRY_H
