//
// Created by happycactus on 30/12/20.
//

#include "catch2/catch.hpp"

#include "Geom.h"
#include "pal/palgeometry.h"
#include "pal/pal.h"

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
