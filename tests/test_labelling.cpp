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
#include "pal/LineString.h"

#include <iostream>
#include <sstream>

static const double lx = 2.5;
static const double ly = 1.0;

TEST_CASE("Labelling Points")
{
    pal::Pal context;
    context.setSearch(pal::CHAIN);
    context.setMapUnit(pal::METER);

    auto p1 = new pal::geometry::Point(0, 5);
    auto p2 = new pal::geometry::Point(10, 5);
    auto p3 = new pal::geometry::Point(20, 5);

    auto layer = context.addLayer("points", -1, -1, pal::P_FREE,
                                  pal::METER, 0.5, false, true, true);

    layer->registerFeature("p1", p1, lx, ly);
    layer->registerFeature("p2", p2, lx, ly);
    layer->registerFeature("p3", p3, lx, ly);

    double bbox[4] = {-10, -10, 30, 10};

    pal::PalStat *stats;
    std::list<pal::Label *> *labels = context.labeller(1, bbox, &stats, false);

    REQUIRE(labels->size() == 3);
    for (auto label : *labels) {
        std::cout << "Label: " << label->getFeatureId() << " Position " << label->getOrigX() << ","
                  << label->getOrigY() << " deg " << label->getRotation() << "\n";
    }

}

TEST_CASE("Labelling LineStrings")
{
    pal::Pal context;
    context.setSearch(pal::CHAIN);
    context.setMapUnit(pal::METER);

    auto l1 = new pal::geometry::LineString(0, 5, 0, 10);
    auto l2 = new pal::geometry::LineString(10, 5, 10, 10);
    auto l3 = new pal::geometry::LineString(20, 5, 20, 10);

    auto layer = context.addLayer("linestrings", -1, -1, pal::P_FREE,
                                  pal::METER, 0.5, false, true, true);

    layer->registerFeature("l1", l1, lx, ly);
    layer->registerFeature("l2", l2, lx, ly);
    layer->registerFeature("l3", l3, lx, ly);

    double bbox[4] = {-10, -10, 30, 20};

    pal::PalStat *stats;
    std::list<pal::Label *> *labels = context.labeller(1, bbox, &stats, false);

    REQUIRE(labels->size() == 3);
    for (auto label : *labels) {
        std::cout << "Label: " << label->getFeatureId() << " Position " << label->getOrigX() << ","
                  << label->getOrigY() << " deg " << label->getRotation() << "\n";
    }

}
