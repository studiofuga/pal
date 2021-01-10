//
// Created by happycactus on 30/12/20.
//

#if defined(HAVE_BENCHMARKS)
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#endif

#include <catch2/catch.hpp>

#include "pal/GeosWrapGeometry.h"
#include "pal/layer.h"
#include "pal/pal.h"
#include "pal/palgeometry.h"

#include <iostream>
#include <sstream>

TEST_CASE("Geos Geometries", "Creation of Features from Geos")
{
    pal::Pal context;

    SECTION("Point Feature")
    {
        pal::GeosWrapGeometry g("POINT(10 5)");
        GEOSGeometry *geometry;
        REQUIRE((geometry = g.getGeosGeometry()) != nullptr);
        g.releaseGeosGeometry();
    }

    SECTION("Line Feature")
    {
        pal::GeosWrapGeometry g("LINESTRING(10 5, 10 -5)");
        GEOSGeometry *geometry;
        REQUIRE((geometry = g.getGeosGeometry()) != nullptr);
        g.releaseGeosGeometry();
    }
}

static const double dx = 10;
static const double dy = 10;
static const double lx = 2.5;
static const double ly = 1.0;

pal::Layer *buildLayer(pal::Pal *pal, pal::Layer *, int num)
{
    pal::Layer *layer = pal->addLayer("main", -1, -1, pal::P_FREE,
                                      pal::METER, 0.5, false, true, true);

    for (int y = 0; y < num; ++y) {
        for (int x = 0; x < num; ++x) {
            std::ostringstream wkt;
            std::ostringstream id;

            id << "G:" << (y * num + x);
            wkt << "POLYGON((";
            wkt << (x * dx) << " " << (y * dy);
            wkt << "," << ((x + 1) * dx - 0.5) << " " << (y * dy);
            wkt << "," << ((x + 1) * dx - 0.5) << " " << ((y + 1) * dy - 0.5);
            wkt << "," << (x * dx) << " " << ((y + 1) * dy - 0.5);
            wkt << "," << (x * dx) << " " << (y * dy);
            wkt << "))";

            auto geom = new pal::GeosWrapGeometry(wkt.str().c_str());
            layer->registerFeature(id.str().c_str(), geom, lx, ly);
        }
    }

    return layer;
}

std::tuple<std::list<pal::Label *> *, pal::PalStat *> makeLabelling(pal::Pal *pal, pal::Layer *layer, int num)
{
    pal::PalStat *stats;

    auto xmin = num * dx / 4;
    auto xmax = 3 * xmin;
    auto ymin = num * dx / 4;
    auto ymax = 3 * ymin;

    double bbox[4] = {xmin, ymin, xmax, ymax};

    std::list<pal::Label *> *labels = pal->labeller(1, bbox, &stats, false);

    return std::make_tuple(labels, stats);
}

void destroy(pal::Pal *pal, pal::Layer *layer, std::tuple<std::list<pal::Label *> *, pal::PalStat *> tpl)
{
    auto &labels = std::get<0>(tpl);
    auto &stats = std::get<1>(tpl);

    while (labels->size() > 0) {
        delete labels->front();
        labels->pop_front();
    }

    delete stats;
    delete labels;

    pal->removeLayer(layer);
}

void testSquares(pal::Pal *pal, pal::Layer *, int num)
{
    auto layer = buildLayer(pal, nullptr, num);
    auto tpl = makeLabelling(pal, layer, num);

    destroy(pal, layer, tpl);
}

TEST_CASE("Geos Labelling")
{
    pal::Pal pal;
    pal.setSearch(pal::CHAIN);
    pal.setMapUnit(pal::METER);

    auto layer = buildLayer(&pal, nullptr, 3);
    auto tpl = makeLabelling(&pal, layer, 3);
    auto &labels = std::get<0>(tpl);
    auto &stats = std::get<1>(tpl);

    REQUIRE(labels->size() > 0);
    for (auto label : *labels) {
        std::cout << "Label: " << label->getFeatureId() << " Position " << label->getOrigX() << ","
                  << label->getOrigY() << " deg " << label->getRotation() << "\n";
    }

    destroy(&pal, layer, tpl);
}

#if defined(HAVE_BENCHMARKS)

TEST_CASE("Geos Labelling", "Geos labelling Benchmark")
{
    pal::Pal pal;
    pal.setSearch(pal::CHAIN);
    pal.setMapUnit(pal::METER);

    BENCHMARK("Test 2x2 labelling")
    {
        return testSquares(&pal, nullptr, 2);
    };
    BENCHMARK("Test 5x5 labelling")
    {
        return testSquares(&pal, nullptr, 5);
    };
    BENCHMARK("Test 10x10 labelling")
    {
        return testSquares(&pal, nullptr, 10);
    };
}

#endif
