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

#include "pal/label.h"

#if defined(HAVE_GEOS)
#include <geos_c.h>
#endif

#include "internalexception.h"

#include <list>
#include <memory>
#include <numeric>
#include <vector>

namespace pal {

    /**
     * \brief
     */
    class PalGeometry {
    public:
        using Ptr = std::shared_ptr<PalGeometry>;

        enum class Type {
            Undefined = -1,
            Point=0, LineString, Polygon,
            MultiPoint, MultiLineString, MultiPolygon
        };

        struct Point { double x = 0.0; double y = 0.0;};

    private:
        Type mType = Type::Undefined;

        using Pointlist = std::vector<Point>;
        using Featurelist = std::vector<Pointlist>;

        Featurelist mFeatures;

#if defined(HAVE_GEOS)
        GEOSGeometry *geometry = nullptr;
#endif

    public:
        explicit PalGeometry(std::vector<Point> const & pointlist) {
            mFeatures.emplace_back(pointlist);
        }

        PalGeometry(PalGeometry const &) = default;
        PalGeometry(PalGeometry &&) = default;

        PalGeometry &operator =(PalGeometry const &) = default;
        PalGeometry &operator =(PalGeometry &&) = default;

        ~PalGeometry() = default;

        auto type() const { return mType; }

        auto getNumGeometries() const { return mFeatures.size();}

        auto getGeometry(size_t index) const {
            return PalGeometry(mFeatures[index]);
        }

        auto getExteriorRing() const {
            if (type() != Type::Polygon)
                throw InternalException::WrongGeometry();
            return getGeometry(0);
        }

        auto getNumInteriorRings() const {
            if (type() != Type::Polygon)
                throw InternalException::WrongGeometry();
            return mFeatures.size()-1;
        }

        auto getInteriorRing(int ring) const {
            if (type() != Type::Polygon)
                throw InternalException::WrongGeometry();
            if (getNumInteriorRings() < 1)
                throw InternalException::Empty();
            return getGeometry(ring+1);
        }

        auto getNumCoordinates() const {
            return std::accumulate(mFeatures.begin(), mFeatures.end(), 0,
                                   [](size_t r, std::vector<Point> const &v){
                return r+v.size();
            });
        }

        auto getNumCoordinates(int index) const {
            return mFeatures[index].size();
        }

        void copyCoordinatesSequence(int i, std::vector<double> &x, std::vector<double> &y)const {
            auto sz = getNumCoordinates(i);
            x.resize(sz);
            y.resize(sz);

            size_t ndx = 0;
            for (auto const &p : mFeatures[i]) {
                x[ndx] = p.x;
                y[ndx] = p.y;
            }
        }

#if defined(HAVE_GEOS)
        /**
         * \brief get the GEOSGeometry of the feature
         * This method is called by Pal each time it needs a geom's coordinates
         *
         * @return GEOSGeometry * a pointer the the geos geom
         */
        GEOSGeometry* getGeosGeometry() const {
            return geometry;
        }

        /**
         * \brief Called by Pal when it doesn't need the coordinates anymore
         * @param the_geom is the geoms geom  from PalGeometry::getfeomGeometry()
         */
        void releaseGeosGeometry (GEOSGeometry *the_geom) {

        };
#endif
    };

} // end namespace pal

#endif
