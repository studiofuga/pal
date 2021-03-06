//
// Created by happycactus on 30/12/20.
//

#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <catch2/catch.hpp>

#include "Geom.h"
#include "pal/palgeometry.h"
#include "pal/pal.h"
#include "pal/layer.h"

#include <sstream>
#include <iostream>

TEST_CASE("Geos Geometries", "Creation of Features from Geos")
{
    pal::Pal context;

    SECTION("Point Feature") {
        Geom g("POINT(10 5)");
        GEOSGeometry *geometry;
        REQUIRE((geometry = g.getGeosGeometry()) != nullptr);
    }

    SECTION("Line Feature") {
        Geom g("LINESTRING(10 5, 10 -5)");
        GEOSGeometry *geometry;
        REQUIRE((geometry = g.getGeosGeometry()) != nullptr);
    }
}

void testSquares (pal::Pal *pal, pal::Layer *layer, int num)
{
    const double dx = 10;
    const double dy = 10;
    const double lx = 2.5;
    const double ly = 1.0;

    for (int y = 0; y < num; ++y) {
        for (int x = 0; x < num; ++x) {
            std::ostringstream wkt;
            std::ostringstream id;

            id<< "G:" << (y*num+x);
            wkt << "POLYGON((" ;
            wkt << (x*dx)<< " " << (y*dy);
            wkt << "," << ((x+1)*dx) <<" " << (y*dy);
            wkt << "," << ((x+1)*dx) <<" " << ((y+1)*dy);
            wkt << "," << (x  *dx) <<" " << ((y+1)*dy);
            wkt << "," << (x*dx) <<" " << (y*dy);
            wkt << "))";

            std::cout << wkt.str() << "\n";

            auto geom = new Geom(wkt.str().c_str());
            layer->registerFeature(id.str().c_str(), geom, lx, ly);
        }
    }

    pal::PalStat *stats;

    auto xmin = num*dx / 4;
    auto xmax = 3*xmin;
    auto ymin = num*dx / 4;
    auto ymax = 3*ymin;

    double bbox[4] = {xmin, ymin, xmax, ymax};

    std::list<pal::Label*> * labels = pal->labeller (1, bbox, &stats, false);

    while (labels->size()>0){
        delete labels->front();
        labels->pop_front();
    }

    delete stats;

    delete labels;
}

TEST_CASE("Geos Labelling", "Geos labelling")
{
    pal::Pal pal;

    pal::Layer * layer = pal.addLayer ("main", -1, -1, pal::P_FREE,
                                       pal::PIXEL, 1, false, true, true);


    testSquares(&pal, layer, 3);
    /*
    BENCHMARK("Test 2x2 labelling"){
        return testSquares(&pal, layer, 2);
    };
    BENCHMARK("Test 5x5 labelling"){
        return testSquares(&pal, layer, 5);
    };
    BENCHMARK("Test 10x10 labelling"){
        return testSquares(&pal, layer, 10);
    };
*/
}
