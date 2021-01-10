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

#if defined(HAVE_BENCHMARKS)
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#endif

#include <catch2/catch.hpp>

#include "pal/layer.h"
#include "pal/pal.h"
#include "pal/palgeometry.h"
#include "pal/Point.h"

#include <iostream>
#include <sstream>

TEST_CASE("Pal Geometries", "Creation of Features from Geos")
{
    pal::Pal context;

    SECTION("Point Feature")
    {
        pal::geometry::Point p(10, 5);
        REQUIRE(p.type() == pal::PalGeometry::Type::Point);
        REQUIRE(p.getNumPoints() == 1);
        REQUIRE(p.getCoordX(0) == 10);
        REQUIRE(p.getCoordY(0) == 5);
        REQUIRE_THROWS_AS(p.getCoordX(1), std::out_of_range);
        REQUIRE_THROWS_AS(p.getCoordY(1), std::out_of_range);
        REQUIRE(p.getExteriorRing() == nullptr);
        REQUIRE(p.numInternalRings() == 0);
        REQUIRE_THROWS_AS(p.getInternalRing(0), std::out_of_range);
    }
}
