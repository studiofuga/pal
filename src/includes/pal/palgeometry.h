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

#ifndef _PAL_GEOMETRY_H
#define _PAL_GEOMETRY_H

#include <vector>

namespace pal {

/**
 * \brief Interface that allow Pal to acces user's geometries
 */
class PalGeometry {
public:
    enum class Type {
        None = 0,
        Unknown,
        Point,
        LineString,
        LineArray,
        Polygon,
        MultiPoint,
        MultiLineString,
        MultiPolygon,
        GeometryCollection
    };

    virtual ~PalGeometry() = default;

    /**
     * @brief Returns the exact type of the geometry (or geometries) contained in the object
     * @return a Type enum for the type.
     */
    virtual Type type() const  = 0;

    /**
     * @brief Returns the collection of simple geometries contained in the geometry. In case of "Simple"
     * geometry (Poin, Line, Polygon) returns a vector with just one element, otherwise (for Multi*),
     * returns all the Point/LineString/Polygon.
     * @return
     */
    virtual std::vector<PalGeometry*> getSimpleGeometries() const = 0;

    /**
     * @brief Returns the number of coordinate pairs of the geometry. It's 1 for Point, 2 for Line and so on
     * @return
     */
    virtual size_t getNumPoints() const = 0;

    /**
     * @brief Returns the X coordinate of the nth point
     * @param n point seq
     * @return
     */
    virtual double getCoordX(size_t n) const = 0;

    /**
     * @brief Returns the Y coordinate of the nth point
     * @param n point seq
     * @return
     */
    virtual double getCoordY(size_t n) const = 0;

    /**
     * @brief Copy all the X coords to the pointed array. the array must be already allocated
     * and should have enouth space to fit all the coords. See getNumPoints()
     * @param xarray
     */
    virtual void getCoordsX (double *xarray) const = 0;

    /**
     * @brief Copy all the Y coords to the pointed array. the array must be already allocated
     * and should have enouth space to fit all the coords. See getNumPoints()
     * @param yarray
     */
    virtual void getCoordsY (double *yarray) const = 0;

    /**
     * @brief Returns the pointer to the exterior ring Polygon Geometry.
     * The returned object pointer is owned by the object itself, so it must not be freed or
     * modified, but the life cycle is managed by the object itself.
     * @return A pointer to the exterior ring, if any. nullptr if not a polygon.
     */
    virtual PalGeometry *getExteriorRing() const = 0;

    /**
     * @brief Returns the number of internal rings of the polygon geometry
     * @return the number of internal rings. if any. 0 if there are no internal rings or if geometry is not
     * a polygon
     */
    virtual size_t numInternalRings() const = 0;

    /**
     * @brief Returns the pointer to the nth internal ring Polygon Geometry.
     * The returned object pointer is owned by the object itself, so it must not be freed or
     * modified, but the life cycle is managed by the object itself.
     * @return A pointer to the nth internal ring, if any. nullptr if not a polygon.
     * @throws std::out_of_range is n is greater or equal to numInternalRings().
     */
    virtual PalGeometry *getInternalRing(size_t n) const = 0;

};

} // end namespace pal

#endif
