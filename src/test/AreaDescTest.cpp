// Copyright (c) 2005 - 2015 Settlers Freaks (sf-team at siedler25.org)
//
// This file is part of Return To The Roots.
//
// Return To The Roots is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// Return To The Roots is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Return To The Roots. If not, see <http://www.gnu.org/licenses/>.

#include "mapGenerator/AreaDesc.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(MapTest)

/**
 * Tests the AreaDesc::IsInArea method for a point inside of the area independent of 
 * the player positions.
 */
BOOST_FIXTURE_TEST_CASE(IsInArea_PointInsideIgnorePlayer, AreaDesc)
{
    const int width = 16;
    const int height = 32;
    const int minPlayerDist = 0;
    const double playerDistance = 0.0; // distance of check point to player
    
    AreaDesc area(Point<double>(0.5,0.5), // area center (= map center)
                  0.0, // min distance to area center
                  0.2, // max distance to area center
                  100.0, 0, 0, 10, 10, // test-independent values
                  minPlayerDist);
    
    BOOST_REQUIRE(area.IsInArea(Point<int>(width/2,height/2), // check map center
                                playerDistance, width, height));
}

/**
 * Tests the AreaDesc::IsInArea method for a point outside of the area independent of
 * the player positions.
 */
BOOST_FIXTURE_TEST_CASE(IsInArea_PointOutsideIgnorePlayer, AreaDesc)
{
    const int width = 16;
    const int height = 32;
    const int minPlayerDist = 0;
    const double playerDistance = 0.0; // distance of check point to player
    
    AreaDesc area(Point<double>(0.5,0.5), // area center (= map center)
                  0.0, // min distance to area center
                  0.2, // max distance to area center
                  100.0, 0, 0, 10, 10, // test-independent values
                  minPlayerDist);
    
    BOOST_REQUIRE(!area.IsInArea(Point<int>(width, height), // bottom-right corner
                                playerDistance, width, height));
}

/**
 * Tests the AreaDesc::IsInArea method for a point outside of the area because it is too
 * close to a player.
 */
BOOST_FIXTURE_TEST_CASE(IsInArea_PointTooCloseToPlayer, AreaDesc)
{
    const int width = 16;
    const int height = 32;
    const int minPlayerDist = 8;
    const double playerDistance = 0.2; // distance of check point to player

    AreaDesc area(Point<double>(0.5,0.5), // area center (= map center)
                  0.0, // min distance to area center
                  0.2, // max distance to area center
                  100.0, 0, 0, 10, 10, // test-independent values
                  minPlayerDist);
    
    BOOST_REQUIRE(!area.IsInArea(Point<int>(width/2, height/2),
                                 playerDistance, width, height));
}

/**
 * Tests the AreaDesc::IsInArea method for a point inside of the area with a given minimum
 * player distance.
 */
BOOST_FIXTURE_TEST_CASE(IsInArea_PointFarFromPlayer, AreaDesc)
{
    const int width = 16;
    const int height = 32;
    const int minPlayerDist = 8;
    const double playerDistance = 8.2; // distance of check point to player
    
    AreaDesc area(Point<double>(0.5,0.5), // area center (= map center)
                  0.0, // min distance to area center
                  0.2, // max distance to area center
                  100.0, 0, 0, 10, 10, // test-independent values
                  minPlayerDist);
    
    BOOST_REQUIRE(area.IsInArea(Point<int>(width/2, height/2),
                                 playerDistance, width, height));
}

/**
 * Tests the AreaDesc::IsInArea method for a point inside of the area with a given maximum
 * player distance.
 */
BOOST_FIXTURE_TEST_CASE(IsInArea_PointNearPlayer, AreaDesc)
{
    const int width = 16;
    const int height = 32;
    const int minPlayerDist = 0;
    const int maxPlayerDist = 8;
    const double playerDistance = 6.2; // distance of check point to player
    
    AreaDesc area(Point<double>(0.5,0.5), // area center (= map center)
                  0.0, // min distance to area center
                  0.2, // max distance to area center
                  100.0, 0, 0, 10, 10, // test-independent values
                  minPlayerDist, maxPlayerDist);
    
    BOOST_REQUIRE(area.IsInArea(Point<int>(width/2, height/2),
                                playerDistance, width, height));
}

BOOST_AUTO_TEST_SUITE_END()

